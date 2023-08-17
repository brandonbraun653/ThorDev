/******************************************************************************
 *  File Name:
 *    chimera_cfg_prj.hpp
 *
 *  Description:
 *    Thor Testing configuration of Chimera
 *
 *  2023 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef THOR_TESTING_CHIMERA_CONFIG_HPP
#define THOR_TESTING_CHIMERA_CONFIG_HPP

/*-----------------------------------------------------------------------------
Literals
-----------------------------------------------------------------------------*/
/*-------------------------------------------------------------------
Disable the default driver registration b/c GCC doesn't like the
weak linking behavior and isn't substituting the Thor implementation
for the Chimera one.
-------------------------------------------------------------------*/
#define CHIMERA_DEFAULT_DRIVER_REGISTRATION  0

#endif  /* !THOR_TESTING_CHIMERA_CONFIG_HPP */
