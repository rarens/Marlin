/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#if defined(__IMXRT1062__)

#include "../../inc/MarlinConfig.h"
//#include "TeensyTimerTool.h"
#include "imxrt.h"
////using namespace TeensyTimerTool;
//PeriodicTimer t1(GPT1);
//PeriodicTimer t2(GPT2);
/** \brief Instruction Synchronization Barrier
  Instruction Synchronization Barrier flushes the pipeline in the processor,
  so that all instructions following the ISB are fetched from cache or
  memory, after the instruction has been completed.
*/
FORCE_INLINE static void __ISB() {
  __asm__ __volatile__("isb 0xF":::"memory");
}

/** \brief Data Synchronization Barrier
  This function acts as a special kind of Data Memory Barrier.
  It completes when all explicit memory accesses before this instruction complete.
*/
FORCE_INLINE static void __DSB() {
  __asm__ __volatile__("dsb 0xF":::"memory");
}

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  switch (timer_num) {
    case 0:
    /*  regs->CTRL = 0x0000;
        regs->LOAD = 0x0000;
        regs->COMP1 = reload;
        regs->CMPLD1 = reload;
        regs->CNTR = 0x0000;*/
        TMR1_CTRL0=0x0000;
        TMR1_LOAD0=0x0000;
        TMR1_COMP10=0xFFFE;
        TMR1_CMPLD10=0xFFFE;
        TMR1_CNTR0 = 0x0000;          
    TMR1_CTRL0 = TMR_CTRL_CM(1) | TMR_CTRL_PCS(TMR0_TIMER_PRESCALE_BITS) | TMR_CTRL_LENGTH;
    TMR1_CSCTRL0 &= ~TMR_CSCTRL_TCF1;
    TMR1_CSCTRL0 |= TMR_CSCTRL_TCF1EN;
    
   
      break;
    case 1:
     TMR2_CTRL0=0x0000;
        TMR2_LOAD0=0x0000;
        TMR2_COMP10=0xFFFE;
        TMR2_CMPLD10=0xFFFE;
        TMR2_CNTR0 = 0x0000;          
    TMR2_CTRL0 = TMR_CTRL_CM(1) | TMR_CTRL_PCS(TMR1_TIMER_PRESCALE_BITS) | TMR_CTRL_LENGTH;
    TMR2_CSCTRL0 &= ~TMR_CSCTRL_TCF1;
    TMR2_CSCTRL0 |= TMR_CSCTRL_TCF1EN;
      break;
  }
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: NVIC_ENABLE_IRQ(timer_num); break;
    case 1: NVIC_ENABLE_IRQ(timer_num); break;
  }
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: NVIC_DISABLE_IRQ(timer_num); break;
    case 1: NVIC_DISABLE_IRQ(timer_num); break;
  }

  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  __DSB();
  __ISB();
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: return NVIC_IS_ENABLED(0);
    case 1: return NVIC_IS_ENABLED(1);
  }
  return false;
}

void HAL_timer_isr_prologue(const uint8_t timer_num) {
  switch (timer_num) {
    case 0:
    TMR1_CNTR0 = 0x0000;
      //FTM0_CNT = 0x0000;
     TMR1_SCTRL0 &= ~TMR_SCTRL_EEOF;
     
      break;
    case 1:
    TMR2_CNTR0 = 0x0000;
      //FTM0_CNT = 0x0000;
     TMR2_SCTRL0 &= ~TMR_SCTRL_EEOF;
    
      break;
  }
}
#endif // IMX1062
