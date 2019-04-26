/********************************************************************************
 *  File Name:
 *    system_flash_test.cpp
 *
 *  Description:
 *    Embedded tests for the Thor internal system flash driver 
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <algorithm>
#include <array>
#include <cstdint>

/* Test Driver Includes */
#include "TinyEmbeddedTest.h"

/* Thor Includes */



/*------------------------------------------------
Local Functions
------------------------------------------------*/

/*------------------------------------------------
Verify the memory helper functions
------------------------------------------------*/
TEST_GROUP( HelperFunctions ){};

/*------------------------------------------------
Verify the actual Flash memory interface behavior
------------------------------------------------*/
/* clang-format off */
TEST_GROUP( InternalFlash )
{
  public:
    void teardown() final override
    {
      //reset_test();
    }
};
/* clang-format on */

