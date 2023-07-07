/********************************************************************************
 *  File Name:
 *    orbit_esc_v3.hpp
 *
 *  Description:
 *    Maps system hardware resources for the OrbitESC PCB Version 3.0
 *
 *  2023 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef ORBIT_ESC_BOARD_MAP_V3_HPP
#define ORBIT_ESC_BOARD_MAP_V3_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/gpio>
#include <Chimera/serial>

/*-----------------------------------------------------------------------------
Configuration
-----------------------------------------------------------------------------*/
namespace Thor::Testing::BSP::IO
{
  namespace USART
  {
    /*-------------------------------------------------------------------------
    GPIO
    -------------------------------------------------------------------------*/
    static constexpr Chimera::GPIO::Pin  rxPin  = 7;
    static constexpr Chimera::GPIO::Port rxPort = Chimera::GPIO::Port::PORTC;
    static constexpr Chimera::GPIO::Pin  txPin  = 6;
    static constexpr Chimera::GPIO::Port txPort = Chimera::GPIO::Port::PORTC;

    /*-------------------------------------------------------------------------
    Configuration Data
    -------------------------------------------------------------------------*/
    static constexpr Chimera::GPIO::PinInit txPinInit = { .alternate = Chimera::GPIO::Alternate::USART6_TX,
                                                          .drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL,
                                                          .pin       = txPin,
                                                          .port      = txPort,
                                                          .pull      = Chimera::GPIO::Pull::NO_PULL,
                                                          .state     = Chimera::GPIO::State::HIGH,
                                                          .threaded  = false,
                                                          .validity  = true };

    static constexpr Chimera::GPIO::PinInit rxPinInit = { .alternate = Chimera::GPIO::Alternate::USART6_RX,
                                                          .drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL,
                                                          .pin       = rxPin,
                                                          .port      = rxPort,
                                                          .pull      = Chimera::GPIO::Pull::NO_PULL,
                                                          .state     = Chimera::GPIO::State::HIGH,
                                                          .threaded  = false,
                                                          .validity  = true };
  }    // namespace USART
}    // namespace Thor::Testing::BSP::IO

#endif /* !ORBIT_ESC_BOARD_MAP_V3_HPP */
