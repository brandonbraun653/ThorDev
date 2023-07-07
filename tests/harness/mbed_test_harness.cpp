/******************************************************************************
 *  File Name:
 *    mbed_test_harness.cpp
 *
 *  Description:
 *    Common testing drivers for Thor
 *
 *  2023 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Aurora/logging>
#include <Chimera/common>
#include <Chimera/gpio>
#include <Chimera/thread>
#include <Chimera/usart>
#include <tests/harness/mbed_test_harness_intf.hpp>
#include <tests/harness/mbed_test_harness_bsp.hpp>
#include <tests/harness/mbed_test_harness_cfg.hpp>

/* Include this last to prevent compiling issues */
#include <CppUTest/CommandLineTestRunner.h>

namespace Thor::Testing
{
  /*---------------------------------------------------------------------------
  Default implementation of the test harness driver interface
  ---------------------------------------------------------------------------*/
  namespace Driver
  {
    Chimera::Status_t __attribute__( ( weak ) ) setUp()
    {
      return Chimera::Status::OK;
    }


    Chimera::Status_t __attribute__( ( weak ) ) tearDown()
    {
      return Chimera::Status::OK;
    }


    const char *__attribute__( ( weak ) ) getTestName()
    {
      return "Unknown Test";
    }

  }    // namespace Driver

  /*---------------------------------------------------------------------------
  Static Data
  ---------------------------------------------------------------------------*/
  static uint32_t                         s_test_thread_stack[ STACK_BYTES( THOR_TEST_RUNNER_STACK_SIZE ) ];
  static Aurora::Logging::SerialSink      s_serial_sink;
  static Aurora::Logging::SinkHandle_rPtr s_serial_hndl;
  static etl::bip_buffer_spsc_atomic<uint8_t, THOR_TEST_SERIAL_TX_BUFFER_SIZE> sTxBuffer;
  static etl::bip_buffer_spsc_atomic<uint8_t, THOR_TEST_SERIAL_RX_BUFFER_SIZE> sRxBuffer;

  /*---------------------------------------------------------------------------
  Static Functions
  ---------------------------------------------------------------------------*/
  static void init_serial_port()
  {
    using namespace Chimera::Serial;
    using namespace Chimera::Hardware;

    Chimera::GPIO::Driver_rPtr pin = nullptr;

    /*-------------------------------------------------------------------------
    Configure the peripheral IO pins
    -------------------------------------------------------------------------*/
    pin = Chimera::GPIO::getDriver( BSP::IO::USART::txPort, BSP::IO::USART::txPin );
    RT_HARD_ASSERT( pin );
    RT_HARD_ASSERT( Chimera::Status::OK == pin->init( BSP::IO::USART::txPinInit ) );

    pin = Chimera::GPIO::getDriver( BSP::IO::USART::rxPort, BSP::IO::USART::rxPin );
    RT_HARD_ASSERT( pin );
    RT_HARD_ASSERT( Chimera::Status::OK == pin->init( BSP::IO::USART::rxPinInit ) );

    /*-------------------------------------------------------------------------
    Create the serial object and initialize it
    -------------------------------------------------------------------------*/
    Chimera::Serial::Config comConfig;
    comConfig.baud     = THOR_TEST_SERIAL_BAUD;
    comConfig.channel  = THOR_TEST_SERIAL_CHANNEL;
    comConfig.width    = Chimera::Serial::CharWid::CW_8BIT;
    comConfig.parity   = Chimera::Serial::Parity::PAR_NONE;
    comConfig.stopBits = Chimera::Serial::StopBits::SBITS_ONE;
    comConfig.flow     = Chimera::Serial::FlowControl::FCTRL_NONE;
    comConfig.txfrMode = Chimera::Serial::TxfrMode::DMA;
    comConfig.txBuffer = dynamic_cast<Chimera::Serial::BipBuffer *>( &sTxBuffer );
    comConfig.rxBuffer = dynamic_cast<Chimera::Serial::BipBuffer *>( &sRxBuffer );

    auto SerialDriver = Chimera::Serial::getDriver( THOR_TEST_SERIAL_CHANNEL );
    RT_HARD_ASSERT( SerialDriver );
    RT_HARD_ASSERT( Chimera::Status::OK == SerialDriver->open( comConfig ) );

    /*-------------------------------------------------------------------------
    Start the logging framework
    -------------------------------------------------------------------------*/
    Aurora::Logging::initialize();
    Aurora::Logging::setGlobalLogLevel( Aurora::Logging::Level::LVL_TRACE );

    s_serial_sink.assignChannel( THOR_TEST_SERIAL_CHANNEL );
    s_serial_sink.logLevel = Aurora::Logging::Level::LVL_TRACE;
    s_serial_sink.enabled  = true;
    s_serial_sink.name     = "SerialLog";

    s_serial_hndl = Aurora::Logging::SinkHandle_rPtr( &s_serial_sink );
    registerSink( s_serial_hndl );

    RT_HARD_ASSERT( Aurora::Logging::Result::RESULT_SUCCESS == Aurora::Logging::setRootSink( s_serial_hndl ) );
  }


  /**
   * @brief Core thread that handles execution of the test suite
   *
   * @param argument  Unused
   * @return void
   */
  static void test_thread( void *argument )
  {
    /*-------------------------------------------------------------------------
    Configure the output style:
      - Colorized
      - Verbose
    -------------------------------------------------------------------------*/
    const char *av_override[] = { "-c", "-v" };

    /*-------------------------------------------------------------------------
    Test driver setup
    -------------------------------------------------------------------------*/
    init_serial_port();

    /*-------------------------------------------------------------------------
    Perform project specific setup
    -------------------------------------------------------------------------*/
    RT_HARD_ASSERT( Driver::setUp() == Chimera::Status::OK );

    /*-------------------------------------------------------------------------
    Execute the tests
    -------------------------------------------------------------------------*/
    LOG_INFO( "Starting test: %s", Driver::getTestName() );
    int rcode = CommandLineTestRunner::RunAllTests( ARRAY_COUNT( av_override ), av_override );
    LOG_INFO( "Test exit with code: %d", rcode );

    /*-------------------------------------------------------------------------
    Perform project specific teardown
    -------------------------------------------------------------------------*/
    RT_HARD_ASSERT( Driver::tearDown() == Chimera::Status::OK );

    /*-------------------------------------------------------------------------
    Idle away to infinity
    -------------------------------------------------------------------------*/
    while ( 1 )
    {
      Chimera::delayMilliseconds( 25 );
    }
  }


  /*---------------------------------------------------------------------------
  Embedded Entry Point
  ---------------------------------------------------------------------------*/
  int ExecuteMainTestThread()
  {
    using namespace Chimera::Thread;

    /*-------------------------------------------------------------------------
    Initialize the hardware drivers
    -------------------------------------------------------------------------*/
    ChimeraInit();

    /*-------------------------------------------------------------------------
    Allocate the test thread
    -------------------------------------------------------------------------*/
    Task       userThread;
    TaskConfig cfg;

    cfg.name                                  = "Tester";
    cfg.arg                                   = nullptr;
    cfg.function                              = test_thread;
    cfg.priority                              = Priority::MINIMUM;
    cfg.stackWords                            = STACK_BYTES( sizeof( s_test_thread_stack ) );
    cfg.type                                  = TaskInitType::STATIC;
    cfg.specialization.staticTask.stackBuffer = s_test_thread_stack;
    cfg.specialization.staticTask.stackSize   = sizeof( s_test_thread_stack );

    userThread.create( cfg );
    userThread.start();

    /*-------------------------------------------------------------------------
    Run the test suite
    -------------------------------------------------------------------------*/
    startScheduler();
    return 0;
  }
}    // namespace Thor::Testing
