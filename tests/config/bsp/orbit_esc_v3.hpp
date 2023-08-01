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
#include <Chimera/sdio>
#include <Chimera/serial>
#include <cstdint>

/*-----------------------------------------------------------------------------
Configuration
-----------------------------------------------------------------------------*/
namespace Thor::Testing::BSP::IO
{
  namespace SDIO
  {
    /*-------------------------------------------------------------------------
    SDIO
    -------------------------------------------------------------------------*/
    static constexpr Chimera::SDIO::Channel  Channel    = Chimera::SDIO::Channel::SDIO1;
    static constexpr Chimera::SDIO::BusWidth BusWidth   = Chimera::SDIO::BusWidth::BUS_WIDTH_4BIT;
    static constexpr uint32_t                ClockSpeed = 4000000;    // 4 MHz
    static constexpr uint32_t                BlockSize  = 512;

    /*-------------------------------------------------------------------------
    GPIO
    -------------------------------------------------------------------------*/
    static constexpr Chimera::GPIO::Pin     d0Pin     = 8;
    static constexpr Chimera::GPIO::Port    d0Port    = Chimera::GPIO::Port::PORTC;
    static constexpr Chimera::GPIO::PinInit d0PinInit = { .alternate = Chimera::GPIO::Alternate::SDIO_D0,
                                                          .drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL,
                                                          .pin       = d0Pin,
                                                          .port      = d0Port,
                                                          .pull      = Chimera::GPIO::Pull::NO_PULL,
                                                          .state     = Chimera::GPIO::State::LOW,
                                                          .threaded  = false,
                                                          .validity  = true };

    static constexpr Chimera::GPIO::Pin     d1Pin     = 9;
    static constexpr Chimera::GPIO::Port    d1Port    = Chimera::GPIO::Port::PORTC;
    static constexpr Chimera::GPIO::PinInit d1PinInit = { .alternate = Chimera::GPIO::Alternate::SDIO_D1,
                                                          .drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL,
                                                          .pin       = d1Pin,
                                                          .port      = d1Port,
                                                          .pull      = Chimera::GPIO::Pull::NO_PULL,
                                                          .state     = Chimera::GPIO::State::LOW,
                                                          .threaded  = false,
                                                          .validity  = true };

    static constexpr Chimera::GPIO::Pin     d2Pin     = 10;
    static constexpr Chimera::GPIO::Port    d2Port    = Chimera::GPIO::Port::PORTC;
    static constexpr Chimera::GPIO::PinInit d2PinInit = { .alternate = Chimera::GPIO::Alternate::SDIO_D2,
                                                          .drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL,
                                                          .pin       = d2Pin,
                                                          .port      = d2Port,
                                                          .pull      = Chimera::GPIO::Pull::NO_PULL,
                                                          .state     = Chimera::GPIO::State::LOW,
                                                          .threaded  = false,
                                                          .validity  = true };

    static constexpr Chimera::GPIO::Pin     d3Pin     = 11;
    static constexpr Chimera::GPIO::Port    d3Port    = Chimera::GPIO::Port::PORTC;
    static constexpr Chimera::GPIO::PinInit d3PinInit = { .alternate = Chimera::GPIO::Alternate::SDIO_D3,
                                                          .drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL,
                                                          .pin       = d3Pin,
                                                          .port      = d3Port,
                                                          .pull      = Chimera::GPIO::Pull::NO_PULL,
                                                          .state     = Chimera::GPIO::State::LOW,
                                                          .threaded  = false,
                                                          .validity  = true };

    static constexpr Chimera::GPIO::Pin     clkPin     = 12;
    static constexpr Chimera::GPIO::Port    clkPort    = Chimera::GPIO::Port::PORTC;
    static constexpr Chimera::GPIO::PinInit clkPinInit = { .alternate = Chimera::GPIO::Alternate::SDIO_CK,
                                                           .drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL,
                                                           .pin       = clkPin,
                                                           .port      = clkPort,
                                                           .pull      = Chimera::GPIO::Pull::NO_PULL,
                                                           .state     = Chimera::GPIO::State::LOW,
                                                           .threaded  = false,
                                                           .validity  = true };

    static constexpr Chimera::GPIO::Pin     cmdPin     = 2;
    static constexpr Chimera::GPIO::Port    cmdPort    = Chimera::GPIO::Port::PORTD;
    static constexpr Chimera::GPIO::PinInit cmdPinInit = { .alternate = Chimera::GPIO::Alternate::SDIO_CMD,
                                                           .drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL,
                                                           .pin       = cmdPin,
                                                           .port      = cmdPort,
                                                           .pull      = Chimera::GPIO::Pull::NO_PULL,
                                                           .state     = Chimera::GPIO::State::LOW,
                                                           .threaded  = false,
                                                           .validity  = true };
  }    // namespace SDIO

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
