/********************************************************************************
 *  File Name:
 *    test_driver_implementation.cpp
 *
 *  Description:
 *    Tests implementation details of the STM32L4 LLD CAN driver
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <limits>

/* Chimera Includes */
#include <Chimera/can>

/* Thor Includes */
#include <Thor/hld/interrupt/hld_interrupt_definitions.hpp>

#include <Thor/lld/common/cortex-m4/interrupts.hpp>
#include <Thor/lld/common/types.hpp>

#include <Thor/lld/interface/can/can_detail.hpp>
#include <Thor/lld/interface/can/can_intf.hpp>
#include <Thor/lld/interface/can/can_prv_data.hpp>
#include <Thor/lld/interface/can/can_types.hpp>

#include <Thor/lld/stm32l4x/can/hw_can_prj.hpp>
#include <Thor/lld/stm32l4x/can/hw_can_prv_driver.hpp>
#include <Thor/lld/stm32l4x/can/hw_can_types.hpp>

/* CppUTest Includes */
#include <CppUTest/TestHarness.h>

/*-------------------------------------------------------------------------------
Aliases
-------------------------------------------------------------------------------*/
namespace LLD = Thor::LLD;
namespace CAN = LLD::CAN;
namespace IT  = LLD::IT;

/*-------------------------------------------------------------------------------
Static Functions & Data
-------------------------------------------------------------------------------*/
static std::array<Chimera::CAN::BasicFrame, 5> s_tx_buffer;
static std::array<Chimera::CAN::BasicFrame, 5> s_rx_buffer;

static void reset_test()
{
  CAN::prv_reset( CAN::CAN1_PERIPH );
}

static Chimera::CAN::DriverConfig getValidConfig()
{
  Chimera::CAN::DriverConfig cfg;
  cfg.clear();
  cfg.validity = true;

  /*-------------------------------------------------
  GPIO Configuration
  -------------------------------------------------*/
  cfg.TXInit.alternate = Chimera::GPIO::Alternate::CAN1_TX;
  cfg.TXInit.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  cfg.TXInit.pin       = 12;
  cfg.TXInit.port      = Chimera::GPIO::Port::PORTA;
  cfg.TXInit.pull      = Chimera::GPIO::Pull::NO_PULL;
  cfg.TXInit.threaded  = true;
  cfg.TXInit.validity  = true;

  cfg.RXInit.alternate = Chimera::GPIO::Alternate::CAN1_RX;
  cfg.RXInit.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  cfg.RXInit.pin       = 11;
  cfg.RXInit.port      = Chimera::GPIO::Port::PORTA;
  cfg.RXInit.pull      = Chimera::GPIO::Pull::NO_PULL;
  cfg.RXInit.threaded  = true;
  cfg.RXInit.validity  = true;

  /*-------------------------------------------------
  CAN Parameter Configuration
  -------------------------------------------------*/
  cfg.HWInit.baudRate           = 100000;
  cfg.HWInit.channel            = Chimera::CAN::Channel::CAN0;
  cfg.HWInit.filterMode         = Chimera::CAN::FilterMode::ID_LIST;
  cfg.HWInit.filterWidth        = Chimera::CAN::FilterWidth::WIDTH_32BIT;
  cfg.HWInit.maxBaudError       = 0.05f;
  cfg.HWInit.resyncJumpWidth    = 1;
  cfg.HWInit.samplePointPercent = 0.85f;
  cfg.HWInit.timeQuanta         = 16;
  cfg.HWInit.rxBuffer           = s_rx_buffer.data();
  cfg.HWInit.rxElements         = s_rx_buffer.size();
  cfg.HWInit.txBuffer           = s_tx_buffer.data();
  cfg.HWInit.txElements         = s_tx_buffer.size();

  return cfg;
}

/*-------------------------------------------------------------------------------
Testing Functions
-------------------------------------------------------------------------------*/
TEST_GROUP( STM32L4_LLD_CAN_DRIVER ){};

/*-------------------------------------------------------------------------------
Verify that the peripheral clock can be enabled/disabled
-------------------------------------------------------------------------------*/
TEST( STM32L4_LLD_CAN_DRIVER, PeripheralClock )
{
  /*-------------------------------------------------
  Initialize
  -------------------------------------------------*/
  reset_test();

  auto periph          = CAN::CAN1_PERIPH;
  CAN::Driver_rPtr can = CAN::getDriver( Chimera::CAN::Channel::CAN0 );
  can->attach( periph );

  /*-------------------------------------------------
  No one knows what state DBF starts out in, so force
  it to some value. This is half a test because it
  has value only when the initial state of DBF == 0.
  -------------------------------------------------*/
  can->enableClock();
  CAN::DBF::clear( periph, CAN::MCR_DBF );
  CHECK( CAN::DBF::get( periph ) == 0 );

  /*-------------------------------------------------
  Disable the clock and make sure we can't write
  -------------------------------------------------*/
  can->disableClock();
  CAN::DBF::set( periph, CAN::MCR_DBF );
  CHECK( CAN::DBF::get( periph ) == 0 );

  /*-------------------------------------------------
  Enable the clock and validate the write occurs
  -------------------------------------------------*/
  can->enableClock();
  CAN::DBF::set( periph, CAN::MCR_DBF );
  CHECK( CAN::DBF::get( periph ) == CAN::MCR_DBF );

  /*-------------------------------------------------
  Disable the clock one more time and make sure the
  value doesn't change on trying to clear the bit.
  -------------------------------------------------*/
  can->disableClock();
  CAN::DBF::clear( periph, CAN::MCR_DBF );
  CHECK( CAN::DBF::get( periph ) == CAN::MCR_DBF );
}


/*-------------------------------------------------------------------------------
Verifies that each ISR signal can properly be enabled/disabled
-------------------------------------------------------------------------------*/
TEST( STM32L4_LLD_CAN_DRIVER, ISRSignalEnableDisable )
{
  /*-------------------------------------------------
  Local variables to help exercise the test
  -------------------------------------------------*/
  /* clang-format off */
  std::array<IRQn_Type, CAN::NUM_CAN_IRQ_HANDLERS> irqSignals = {
    CAN1_TX_IRQn,
    CAN1_RX0_IRQn,
    CAN1_RX1_IRQn,
    CAN1_SCE_IRQn
  };

  std::array<Chimera::CAN::InterruptType, CAN::NUM_CAN_IRQ_HANDLERS> isrWakeupSignals = {
    Chimera::CAN::InterruptType::TX_ISR,
    Chimera::CAN::InterruptType::RX_ISR,
    Chimera::CAN::InterruptType::ERR_ISR,
    Chimera::CAN::InterruptType::STS_ISR
  };
  /* clang-format on */

  /*-------------------------------------------------
  Initialize
  -------------------------------------------------*/
  reset_test();

  auto periph          = CAN::CAN1_PERIPH;
  CAN::Driver_rPtr can = CAN::getDriver( Chimera::CAN::Channel::CAN0 );
  can->attach( periph );
  can->enableClock();

  /*-------------------------------------------------
  Verify upon attaching that:
  1. System level CAN interrupts are disabled
  2. System level CAN interrupt priority is set ok
  3. There are no pending system level CAN interrupts
  4. Peripheral level CAN interrupts are disabled
  5. ISR signal semaphores are reset to default state
  -------------------------------------------------*/
  for ( auto idx = 0; idx < irqSignals.size(); idx++ )
  {
    // Pull the interrupt preempt and priority config
    uint32_t preempt = 0xaa;
    uint32_t subprio = 0xbb;
    IT::getPriority( irqSignals[ idx ], IT::getPriorityGrouping(), &preempt, &subprio );

    // Pull the ISR wakeup signal
    auto isrSignal = can->getISRSignal( isrWakeupSignals[ idx ] );

    CHECK( IT::getActive( irqSignals[ idx ] ) == 0 );
    CHECK( IT::getPendingIRQ( irqSignals[ idx ] ) == 0 );
    CHECK( preempt == Thor::Interrupt::CAN_IT_PREEMPT_PRIORITY );
    CHECK( subprio == 0 );
    CHECK( isrSignal->try_acquire() == false );
  }

  CHECK( CAN::TMEIE::get( periph ) == 0 );
  CHECK( CAN::FMPIE0::get( periph ) == 0 );
  CHECK( CAN::FMPIE1::get( periph ) == 0 );
  CHECK( CAN::FFIE0::get( periph ) == 0 );
  CHECK( CAN::FFIE1::get( periph ) == 0 );
  CHECK( CAN::FOVIE0::get( periph ) == 0 );
  CHECK( CAN::FOVIE1::get( periph ) == 0 );
  CHECK( CAN::SLKIE::get( periph ) == 0 );
  CHECK( CAN::WKUIE::get( periph ) == 0 );
  CHECK( CAN::LECIE::get( periph ) == 0 );
  CHECK( CAN::BOFIE::get( periph ) == 0 );
  CHECK( CAN::EPVIE::get( periph ) == 0 );
  CHECK( CAN::EWGIE::get( periph ) == 0 );

  /*-------------------------------------------------
  Transmit Mailbox Empty
  -------------------------------------------------*/
  // Enable
  CHECK( can->enableISRSignal( Chimera::CAN::InterruptType::TRANSMIT_MAILBOX_EMPTY ) == Chimera::Status::OK );
  CHECK( CAN::TMEIE::get( periph ) == CAN::IER_TMEIE );

  // Disable
  can->disableISRSignal( Chimera::CAN::InterruptType::TRANSMIT_MAILBOX_EMPTY );
  CHECK( CAN::TMEIE::get( periph ) == 0 );

  /*-------------------------------------------------
  FIFO Message Pending
  -------------------------------------------------*/
  // Enable
  CHECK( can->enableISRSignal( Chimera::CAN::InterruptType::RECEIVE_FIFO_NEW_MESSAGE ) == Chimera::Status::OK );
  CHECK( CAN::FMPIE0::get( periph ) == CAN::IER_FMPIE0 );
  CHECK( CAN::FMPIE1::get( periph ) == CAN::IER_FMPIE1 );

  // Disable
  can->disableISRSignal( Chimera::CAN::InterruptType::RECEIVE_FIFO_NEW_MESSAGE );
  CHECK( CAN::FMPIE0::get( periph ) == 0 );
  CHECK( CAN::FMPIE1::get( periph ) == 0 );

  /*-------------------------------------------------
  FIFO Full
  -------------------------------------------------*/
  // Enable
  CHECK( can->enableISRSignal( Chimera::CAN::InterruptType::RECEIVE_FIFO_FULL ) == Chimera::Status::OK );
  CHECK( CAN::FFIE0::get( periph ) == CAN::IER_FFIE0 );
  CHECK( CAN::FFIE1::get( periph ) == CAN::IER_FFIE1 );

  // Disable
  can->disableISRSignal( Chimera::CAN::InterruptType::RECEIVE_FIFO_FULL );
  CHECK( CAN::FFIE0::get( periph ) == 0 );
  CHECK( CAN::FFIE1::get( periph ) == 0 );

  /*-------------------------------------------------
  FIFO Overrun
  -------------------------------------------------*/
  // Enable
  CHECK( can->enableISRSignal( Chimera::CAN::InterruptType::RECEIVE_FIFO_OVERRUN ) == Chimera::Status::OK );
  CHECK( CAN::FOVIE0::get( periph ) == CAN::IER_FOVIE0 );
  CHECK( CAN::FOVIE1::get( periph ) == CAN::IER_FOVIE1 );

  // Disable
  can->disableISRSignal( Chimera::CAN::InterruptType::RECEIVE_FIFO_OVERRUN );
  CHECK( CAN::FOVIE0::get( periph ) == 0 );
  CHECK( CAN::FOVIE1::get( periph ) == 0 );

  /*-------------------------------------------------
  Sleep Event
  -------------------------------------------------*/
  // Enable
  CHECK( can->enableISRSignal( Chimera::CAN::InterruptType::SLEEP_EVENT ) == Chimera::Status::OK );
  CHECK( CAN::SLKIE::get( periph ) == CAN::IER_SLKIE );

  // Disable
  can->disableISRSignal( Chimera::CAN::InterruptType::SLEEP_EVENT );
  CHECK( CAN::SLKIE::get( periph ) == 0 );

  /*-------------------------------------------------
  Wakeup Event
  -------------------------------------------------*/
  // Enable
  CHECK( can->enableISRSignal( Chimera::CAN::InterruptType::WAKEUP_EVENT ) == Chimera::Status::OK );
  CHECK( CAN::WKUIE::get( periph ) == CAN::IER_WKUIE );

  // Disable
  can->disableISRSignal( Chimera::CAN::InterruptType::WAKEUP_EVENT );
  CHECK( CAN::WKUIE::get( periph ) == 0 );

  /*-------------------------------------------------
  Error Code Event
  -------------------------------------------------*/
  // Enable
  CHECK( can->enableISRSignal( Chimera::CAN::InterruptType::ERROR_CODE_EVENT ) == Chimera::Status::OK );
  CHECK( CAN::ERRIE::get( periph ) == CAN::IER_ERRIE );
  CHECK( CAN::LECIE::get( periph ) == CAN::IER_LECIE );

  // Disable
  can->disableISRSignal( Chimera::CAN::InterruptType::ERROR_CODE_EVENT );
  CHECK( CAN::ERRIE::get( periph ) == CAN::IER_ERRIE ); // Should not be able to disable
  CHECK( CAN::LECIE::get( periph ) == 0 );

  /*-------------------------------------------------
  Error Bus Off Event
  -------------------------------------------------*/
  // Enable
  CHECK( can->enableISRSignal( Chimera::CAN::InterruptType::ERROR_BUS_OFF_EVENT ) == Chimera::Status::OK );
  CHECK( CAN::ERRIE::get( periph ) == CAN::IER_ERRIE );
  CHECK( CAN::BOFIE::get( periph ) == CAN::IER_BOFIE );

  // Disable
  can->disableISRSignal( Chimera::CAN::InterruptType::ERROR_BUS_OFF_EVENT );
  CHECK( CAN::ERRIE::get( periph ) == CAN::IER_ERRIE ); // Should not be able to disable
  CHECK( CAN::BOFIE::get( periph ) == 0 );

  /*-------------------------------------------------
  Error Passive Event
  -------------------------------------------------*/
  // Enable
  CHECK( can->enableISRSignal( Chimera::CAN::InterruptType::ERROR_PASSIVE_EVENT ) == Chimera::Status::OK );
  CHECK( CAN::ERRIE::get( periph ) == CAN::IER_ERRIE );
  CHECK( CAN::EPVIE::get( periph ) == CAN::IER_EPVIE );

  // Disable
  can->disableISRSignal( Chimera::CAN::InterruptType::ERROR_PASSIVE_EVENT );
  CHECK( CAN::ERRIE::get( periph ) == CAN::IER_ERRIE ); // Should not be able to disable
  CHECK( CAN::EPVIE::get( periph ) == 0 );

  /*-------------------------------------------------
  Error Warning Event
  -------------------------------------------------*/
  // Enable
  CHECK( can->enableISRSignal( Chimera::CAN::InterruptType::ERROR_WARNING_EVENT ) == Chimera::Status::OK );
  CHECK( CAN::ERRIE::get( periph ) == CAN::IER_ERRIE );
  CHECK( CAN::EWGIE::get( periph ) == CAN::IER_EWGIE );

  // Disable
  can->disableISRSignal( Chimera::CAN::InterruptType::ERROR_WARNING_EVENT );
  CHECK( CAN::ERRIE::get( periph ) == CAN::IER_ERRIE ); // Should not be able to disable
  CHECK( CAN::EWGIE::get( periph ) == 0 );
}


/*-------------------------------------------------------------------------------
Verifies that supported mode transistions occur correctly
-------------------------------------------------------------------------------*/
TEST( STM32L4_LLD_CAN_DRIVER, DebugModeConfig )
{
  /*-------------------------------------------------
  Initialize Peripheral Hardware
  -------------------------------------------------*/
  reset_test();

  auto periph          = CAN::CAN1_PERIPH;
  CAN::Driver_rPtr can = CAN::getDriver( Chimera::CAN::Channel::CAN0 );
  can->attach( periph );
  can->enableClock();
  CHECK( Chimera::Status::OK == can->configure( getValidConfig() ) );

  /*-------------------------------------------------
  Clear modes so the test is in a known state
  -------------------------------------------------*/
  can->exitDebugMode();
  CHECK( CAN::SLIM::get( periph ) == 0 );
  CHECK( CAN::LBKM::get( periph ) == 0 );

  /*-------------------------------------------------
  Silent Mode
  -------------------------------------------------*/
  can->enterDebugMode( Chimera::CAN::DebugMode::SILENT );
  CHECK( CAN::SLIM::get( periph ) == CAN::BTR_SILM );
  CHECK( CAN::LBKM::get( periph ) == 0 );

  /*-------------------------------------------------
  Loopback Mode
  -------------------------------------------------*/
  can->enterDebugMode( Chimera::CAN::DebugMode::LOOPBACK );
  CHECK( CAN::SLIM::get( periph ) == 0 );
  CHECK( CAN::LBKM::get( periph ) == CAN::BTR_LBKM );

  /*-------------------------------------------------
  Self-Test Mode
  -------------------------------------------------*/
  can->enterDebugMode( Chimera::CAN::DebugMode::LOOPBACK_AND_SILENT );
  CHECK( CAN::SLIM::get( periph ) == CAN::BTR_SILM );
  CHECK( CAN::LBKM::get( periph ) == CAN::BTR_LBKM );

  /*-------------------------------------------------
  Unknown Mode
  -------------------------------------------------*/
  can->enterDebugMode( Chimera::CAN::DebugMode::UNKNOWN );
  CHECK( CAN::SLIM::get( periph ) == 0 );
  CHECK( CAN::LBKM::get( periph ) == 0 );
}


/*-------------------------------------------------------------------------------
Verifies that the hardware configuration works correctly
-------------------------------------------------------------------------------*/
TEST( STM32L4_LLD_CAN_DRIVER, CoreConfiguration )
{
  CHECK( 1 == 1 );
  // Eventually verify that a clock frequency range can be configured. This is pretty
  // dependent upon the peripheral clock, so the tester needs to be able to
  // set that dynamically first rather than relying on whatever the current
  // configuration is. Otherwise that will lead to inconsistent/non-portable tests.
}


/*-------------------------------------------------------------------------------
Verifies transmit functionality
-------------------------------------------------------------------------------*/
TEST( STM32L4_LLD_CAN_DRIVER, TransmitParameterChecks )
{
  /*-------------------------------------------------
  Initialize Peripheral Hardware
  -------------------------------------------------*/
  reset_test();

  auto periph          = CAN::CAN1_PERIPH;
  CAN::Driver_rPtr can = CAN::getDriver( Chimera::CAN::Channel::CAN0 );
  can->attach( periph );
  can->enableClock();

  /*-------------------------------------------------
  Verify that bad frames are rejected
  -------------------------------------------------*/
  Chimera::CAN::BasicFrame badFrame;

  // Invalid data length
  badFrame.clear();
  badFrame.dataLength = 0;
  badFrame.idMode     = Chimera::CAN::IdentifierMode::STANDARD;
  badFrame.frameType  = Chimera::CAN::FrameType::DATA;

  CHECK( Chimera::Status::INVAL_FUNC_PARAM == can->send( CAN::Mailbox::TX_MAILBOX_1, badFrame ) );


  // Invalid ID mode
  badFrame.clear();
  badFrame.dataLength = 3;
  badFrame.idMode     = Chimera::CAN::IdentifierMode::NUM_OPTIONS;
  badFrame.frameType  = Chimera::CAN::FrameType::DATA;

  CHECK( Chimera::Status::INVAL_FUNC_PARAM == can->send( CAN::Mailbox::TX_MAILBOX_1, badFrame ) );


  // Invalid frame type
  badFrame.clear();
  badFrame.dataLength = 3;
  badFrame.idMode     = Chimera::CAN::IdentifierMode::STANDARD;
  badFrame.frameType  = Chimera::CAN::FrameType::NUM_OPTIONS;

  CHECK( Chimera::Status::INVAL_FUNC_PARAM == can->send( CAN::Mailbox::TX_MAILBOX_1, badFrame ) );

  /*-------------------------------------------------
  Verify that bad mailboxes are rejected
  -------------------------------------------------*/
  Chimera::CAN::BasicFrame goodFrame;
  goodFrame.clear();
  goodFrame.dataLength = 1;
  goodFrame.idMode     = Chimera::CAN::IdentifierMode::STANDARD;
  goodFrame.frameType  = Chimera::CAN::FrameType::DATA;

  CHECK( Chimera::Status::INVAL_FUNC_PARAM == can->send( CAN::Mailbox::RX_MAILBOX_1, goodFrame ) );
  CHECK( Chimera::Status::INVAL_FUNC_PARAM == can->send( CAN::Mailbox::RX_MAILBOX_2, goodFrame ) );
  CHECK( Chimera::Status::INVAL_FUNC_PARAM == can->send( CAN::Mailbox::NUM_OPTIONS, goodFrame ) );
  CHECK( Chimera::Status::INVAL_FUNC_PARAM == can->send( CAN::Mailbox::UNKNOWN, goodFrame ) );
}


TEST( STM32L4_LLD_CAN_DRIVER, TransmitMailboxNotReady )
{
  // This one will be a little tricky. Need to send data while monitoring the status of
  // the mailbox flags. Once it moves to the pending state, I need to try and send more
  // data, then verify I can't and correctly receive the "NOT_READY" flag.
}


TEST( STM32L4_LLD_CAN_DRIVER, TransmitAvailability )
{
  // Queue up a transfer, validate the mailbox reads as unavailable, validate the next
  // mailbox is free, validate the first mailbox becomes free again upon successful TX.

  // Queue up two transfers, validate the last mailbox is free.

  // Queue up three transfers, validate no mailbox is free
}

TEST( STM32L4_LLD_CAN_DRIVER, Transmit)
{
  /*-------------------------------------------------
  Initialize Peripheral Hardware
  -------------------------------------------------*/
  reset_test();

  auto periph          = CAN::CAN1_PERIPH;
  CAN::Driver_rPtr can = CAN::getDriver( Chimera::CAN::Channel::CAN0 );
  can->attach( periph );
  can->enableClock();
  CHECK( Chimera::Status::OK == can->configure( getValidConfig() ) );

  can->enableISRSignal( Chimera::CAN::InterruptType::TRANSMIT_MAILBOX_EMPTY );
  auto boxEmptySignal = can->getISRSignal( Chimera::CAN::InterruptType::TRANSMIT_MAILBOX_EMPTY );
  CHECK( boxEmptySignal != nullptr );

  /*-------------------------------------------------
  Initialize data to be tx'd
  -------------------------------------------------*/
  Chimera::CAN::BasicFrame txData;
  txData.clear();
  txData.id         = 0;
  txData.idMode     = Chimera::CAN::IdentifierMode::STANDARD;
  txData.frameType  = Chimera::CAN::FrameType::DATA;
  txData.dataLength = 8;
  memset( txData.data, 0xAA, txData.dataLength );

  /*-------------------------------------------------
  Transmit the data then block on the wakeup signal
  -------------------------------------------------*/
  can->enterDebugMode( Chimera::CAN::DebugMode::LOOPBACK_AND_SILENT );
  can->send( CAN::Mailbox::TX_MAILBOX_1, txData );
  CHECK( boxEmptySignal->try_acquire_for( Chimera::Threading::TIMEOUT_50MS ) );

  /*-------------------------------------------------
  Verify the transmit event happened
  -------------------------------------------------*/
  auto isrContext = can->getISRContext( Chimera::CAN::InterruptType::TRANSMIT_MAILBOX_EMPTY );

  CHECK( isrContext != nullptr );
  CHECK( isrContext->isrPending == ( 1u << static_cast<size_t>( Chimera::CAN::InterruptType::TRANSMIT_MAILBOX_EMPTY ) ) );
  CHECK( isrContext->details.txEvent.mailbox0.txOk );
}


/*-------------------------------------------------------------------------------
Verifies receive functionality
-------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------
Verifies Error Handling Functionality
-------------------------------------------------------------------------------*/
TEST( STM32L4_LLD_CAN_DRIVER, TransmitFault_Warning)
{
  /*-------------------------------------------------
  Test Constants
  -------------------------------------------------*/
  auto warningSignal = Chimera::CAN::InterruptType::ERROR_WARNING_EVENT;
  auto passiveSignal = Chimera::CAN::InterruptType::ERROR_PASSIVE_EVENT;

  uint16_t warningISRBit = ( 1u << static_cast<uint16_t>( warningSignal ) );
  uint16_t passiveISRBit = ( 1u << static_cast<uint16_t>( passiveSignal ) );

  /*-------------------------------------------------
  Initialize Peripheral Hardware
  -------------------------------------------------*/
  reset_test();

  auto periph          = CAN::CAN1_PERIPH;
  CAN::Driver_rPtr can = CAN::getDriver( Chimera::CAN::Channel::CAN0 );
  can->attach( periph );
  can->enableClock();
  CHECK( Chimera::Status::OK == can->configure( getValidConfig() ) );

  /*-------------------------------------------------
  Make sure the only ISR enabled is the Warning Error.
  Technically the Passive Error will fire in this test
  as well and we don't want to test the wrong thing.
  -------------------------------------------------*/
  can->disableISRSignal( passiveSignal );
  can->enableISRSignal( warningSignal );
  auto errorSignal = can->getISRSignal( warningSignal );
  CHECK( errorSignal != nullptr );

  /*-------------------------------------------------
  Initialize data to be tx'd
  -------------------------------------------------*/
  Chimera::CAN::BasicFrame txData;
  txData.clear();
  txData.id         = 0;
  txData.idMode     = Chimera::CAN::IdentifierMode::STANDARD;
  txData.frameType  = Chimera::CAN::FrameType::DATA;
  txData.dataLength = 8;
  memset( txData.data, 0xAA, txData.dataLength );

  /*-------------------------------------------------
  Make sure we aren't in debug mode. This test only
  works if the CAN transceiver isn't connected to
  anything AND isn't in debug mode. The CAN bus is
  looking for an ACK response bit and won't get it in
  this configuration.
  -------------------------------------------------*/
  can->exitDebugMode();

  /*-------------------------------------------------
  Transmit the data then block on the wakeup signal
  -------------------------------------------------*/
  can->send( CAN::Mailbox::TX_MAILBOX_1, txData );
  CHECK( errorSignal->try_acquire_for( Chimera::Threading::TIMEOUT_50MS ) );

  /*-------------------------------------------------
  Verify the transmit event error-ed out
  -------------------------------------------------*/
  auto isrContext = can->getISRContext( warningSignal );

  CHECK( isrContext != nullptr );
  CHECK( isrContext->isrPending & warningISRBit );
  CHECK( ( isrContext->isrPending & passiveISRBit ) == 0 );
  CHECK( isrContext->details.errEvent.warning == true );
}