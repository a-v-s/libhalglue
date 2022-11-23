#ifndef __STM32F1XX_H
#define __STM32F1XX_H

#ifndef STM32F103xB
#define STM32F103xB
#endif 
#include "stm32f103xb.h"
#include "rv_common.h"

#include "core_cm3.h"
#include "stm32f1xx_hal.h"



#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL))) 



#define ATOMIC_SET_BIT(REG, BIT) __atomic_fetch_or(&REG, BIT, __ATOMIC_RELAXED)

#define ATOMIC_CLEAR_BIT(REG, BIT) __atomic_fetch_and(&REG, ~BIT, __ATOMIC_RELAXED)


#endif // __STM32F1XX_H__
