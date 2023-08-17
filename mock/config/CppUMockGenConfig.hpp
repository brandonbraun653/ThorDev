/******************************************************************************
 *  File Name:
 *    CppUMockGenConfig.hpp
 *
 *  Description:
 *    Preprocessor definitions for CppUMockGen to support STM32F4 driver tests
 *
 *  2023 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef THOR_CPPUMOCKGEN_CONFIG_HPP
#define THOR_CPPUMOCKGEN_CONFIG_HPP

/*-----------------------------------------------------------------------------
Literals
-----------------------------------------------------------------------------*/
#ifndef TARGET_STM32F4 
#define TARGET_STM32F4
#endif

#ifndef STM32F446xx
#define STM32F446xx
#endif

#ifndef USING_NATIVE_THREADS
#define USING_NATIVE_THREADS
#endif

#endif  /* !THOR_CPPUMOCKGEN_CONFIG_HPP */
