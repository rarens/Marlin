#pragma once

#ifdef NUM_DIGITAL_PINS             // Only in ST's Arduino core (STM32duino, STM32Core)
  #include "../STM32/pinsDebug_STM32duino.h"
#elif defined(BOARD_NR_GPIO_PINS)   // Only in STM32GENERIC (Maple)
  #include "../STM32/pinsDebug_STM32GENERIC.h"
#else
  #error "M43 Pins Debugging not supported for this board."
#endif

#ifndef M43_NEVER_TOUCH
  #define _M43_NEVER_TOUCH(Index) (Index >= 0)