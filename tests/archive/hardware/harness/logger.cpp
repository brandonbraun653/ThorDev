/********************************************************************************
 *  File Name:
 *    logger.cpp
 *
 *  Description:
 *    Logger interface for Thor testing
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Logging Includes */
#include <Aurora/logging>

/* Testing Includes */
#include <tests/hardware/harness/logger.hpp>
#include <tests/hardware/harness/serial_output.hpp>

namespace Thor::Testing
{
  /*-------------------------------------------------------------------------------
  Static Data
  -------------------------------------------------------------------------------*/
  static Aurora::Logging::SerialSink sink;
  static uLog::SinkHandle hSink;

  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  void initializeLogger( const uLog::Level lvl )
  {
    /*-------------------------------------------------
    Initialize the serial hardware
    -------------------------------------------------*/
    initializeSerial();

    /*-------------------------------------------------
    Initialize the logger sink
    -------------------------------------------------*/
    sink.assignChannel( serialChannel );
    sink.setLogLevel( lvl );
    sink.enable();
    sink.setName( "HWLogger" );

    if ( !hSink )
    {
      hSink = uLog::SinkHandle( &sink );
    }

    /*-------------------------------------------------
    Register the sink with the log framework
    -------------------------------------------------*/
    uLog::initialize();
    uLog::setGlobalLogLevel( lvl );
    uLog::registerSink( hSink );
    uLog::setRootSink( hSink );
  }

}    // namespace Thor::Testing
