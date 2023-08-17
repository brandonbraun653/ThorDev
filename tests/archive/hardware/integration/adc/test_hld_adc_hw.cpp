/********************************************************************************
 *  File Name:
 *    test_hld_adc_hw.cpp
 *
 *  Description:
 *    Sandbox for testing the HLD interface to ADC hardware
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/adc>
#include <Chimera/clock>
#include <Chimera/gpio>
#include <Chimera/serial>
#include <Chimera/thread>

/* Thor Includes */
#include <Thor/adc>

/* Testing Includes */
#include <tests/hardware/harness/blinky.hpp>
#include <tests/hardware/harness/logger.hpp>

/*-------------------------------------------------------------------------------
Static Functions & Data
-------------------------------------------------------------------------------*/
static void test_thread( void *argument );
static void task_msg_thread( void *argument );

static Chimera::Threading::ThreadId blinkyId;
static Chimera::Threading::ThreadId testId;
static Chimera::Threading::ThreadId taskId;

/*-------------------------------------------------------------------------------
Public Functions
-------------------------------------------------------------------------------*/
int main()
{
  using namespace Chimera::Threading;
  ChimeraInit();

  /*-------------------------------------------------
  Create the blinky thread
  -------------------------------------------------*/
  Thread blinky;
  blinky.initialize( Thor::Testing::blinkyThread, nullptr, Priority::LEVEL_1, Thor::Testing::BLINKY_STACK,
                     Thor::Testing::BLINKY_NAME.cbegin() );
  blinkyId = blinky.start();

  /*-------------------------------------------------
  Create the test thread
  -------------------------------------------------*/
  Thread testing;
  testing.initialize( test_thread, nullptr, Priority::LEVEL_3, STACK_KILOBYTES( 10 ), "test" );
  testId = testing.start();

  /*-------------------------------------------------
  Create the test thread
  -------------------------------------------------*/
  Thread task_msg;
  task_msg.initialize( task_msg_thread, nullptr, Priority::LEVEL_3, STACK_BYTES( 512 ), "msg" );
  taskId = task_msg.start();

  startScheduler();
  return 0;
}


static void test_thread( void *argument )
{
  using namespace Chimera::ADC;

  /*-------------------------------------------------
  Initialize the Logger
  -------------------------------------------------*/
  Thor::Testing::initializeLogger( uLog::Level::LVL_DEBUG );

  /*-------------------------------------------------------------------------------
  Initialize the ADC driver
  -------------------------------------------------------------------------------*/
  DriverConfig cfg;
  cfg.clear();

  cfg.periph      = Converter::ADC_0;
  cfg.clockSource = Chimera::Clock::Bus::PCLK2;
  cfg.resolution  = Resolution::BIT_12;


  auto adc = getDriver( cfg.periph );
  adc->open( cfg );

  Chimera::delayMilliseconds( 100 );
  Chimera::Threading::sendTaskMsg( taskId, 0xDEADBEEF, Chimera::Threading::TIMEOUT_DONT_WAIT );

  adc->setSampleTime( Channel::ADC_CH_0, 650 );

  volatile Sample_t adc_result = adc->sampleSensor( Sensor::TEMP );
  volatile float temperature   = adc->sampleToJunctionTemperature( adc_result );

  /*-------------------------------------------------------------------------------
  Idle away into nothing
  -------------------------------------------------------------------------------*/
  while ( 1 )
  {
    Chimera::delayMilliseconds( 100 );
  }
}

static void task_msg_thread( void *argument )
{
  using namespace Chimera::Threading;

  ThreadMsg tmp = 0;


  while ( 1 )
  {
    if ( this_thread::receiveTaskMsg( tmp, TIMEOUT_10MS ) )
    {
      Chimera::insert_debug_breakpoint();
    }
  }
}