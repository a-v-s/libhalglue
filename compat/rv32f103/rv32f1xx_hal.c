#include <stdint.h>

#include "stm32f1xx_hal.h"

//uint32_t SystemCoreClock = 16000000;
uint32_t SystemCoreClock = 8000000;
const uint8_t AHBPrescTable[16U] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8U] =  {0, 0, 0, 0, 1, 2, 3, 4};
uint32_t uwTickPrio = 0;







#define CH32V1_TIMER_START  	((0xE000F000))
#define CH32V1_MTIMEL 	((0xE000F004))
#define CH32V1_MTIMEH 	((0xE000F008))
//#define CH32V1_TFREQ    ((uint32_t)SystemCoreClock/8)  //units HZ
#define CH32V1_TDIVMS   (8000)  
#define CH32V1_TDIVUS   (8)

#define CH32V2_TIMER_START  	((0xE000F000))
#define CH32V2_MTIMEL 	((0xE000F008))
#define CH32V2_MTIMEH 	((0xE000F00C))
#define CH32V2_TDIVMS   (8000)
#define CH32V2_TDIVUS   (8)


//#define GD32VF_MSTOP	((0xd1000FF8))
#define GD32VF_MSIP 	((0xd1000FFC))
#define GD32VF_MTIMEL	((0xd1000000))
#define GD32VF_MTIMEH	((0xd1000004))
//#define GD32VF_TFREQ    ((uint32_t)SystemCoreClock/4)  //units HZ
#define GD32VF_TDIVMS   (4000)
#define GD32VF_TDIVUS   (4)


static volatile uint32_t*	mrun;
static volatile uint32_t*	mtimel;
static volatile uint32_t*	mtimeh;
//static uint32_t		tfreq;
static uint32_t		tdivms=0;




#define GD32_MARCHID    0x80000022		// Identifiers for GD32VF103
#define GD32_MVENDORID  0x0000031E		// Identifiers for GD32VF103

#define CH32V0_MARCHID    0xdc68d841		// Identifiers for CH32V003
#define CH32V0_MVENDORID  0x00000000		// Identifiers for CH32V003

#define CH32V1_MARCHID    0x00000000		// Identifiers for CH32V103
#define CH32V1_MVENDORID  0x01020304		// Identifiers for CH32V103

#define CH32V2_MARCHID    0xdc68d882
#define CH32V2_MVENDORID  0x00000000

#define  CH32V3_MARCHID    0xdc68d881
#define  CH32V3_MVENDORID  0x00000000

#define read_csr(reg) ({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })



void timer_init(void) {

	uint32_t marchid = read_csr(marchid);
	switch (marchid) {
		case GD32_MARCHID:
			mtimel = GD32VF_MTIMEL;
			mtimeh = GD32VF_MTIMEH;
			tdivms = GD32VF_TDIVMS;
		break;
		case CH32V1_MARCHID:
			mtimel = CH32V1_MTIMEL;
			mtimeh = CH32V1_MTIMEH;
			tdivms = CH32V1_TDIVMS;
			mrun = CH32V1_TIMER_START;
			*mrun = 1;
			break;
		case CH32V0_MARCHID:
		case CH32V2_MARCHID:
		case CH32V3_MARCHID:
			mtimel = CH32V2_MTIMEL;
			mtimeh = CH32V2_MTIMEH;
			tdivms = CH32V2_TDIVMS;
			mrun = CH32V2_TIMER_START;
			*mrun = 1;
		break;
	}


}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority){
	(void)TickPriority;
	timer_init();
	return 0;
}

HAL_StatusTypeDef HAL_Init(void){
	HAL_InitTick(0);
}



uint64_t get_timer_value() {
  while (1) {
    uint32_t hi = (*mtimeh);
    uint32_t lo = (*mtimel);
    if (hi == (*mtimeh))
      return ((uint64_t)hi << 32) | lo;
  }
}

uint32_t get_time_ms(void) {
	return get_timer_value() / tdivms;
}


#if  (defined __riscv)

// Note... this should go elsewhere but for now this will do

void bshal_delay_ms(uint32_t ms){
	uint64_t begin = get_timer_value() ;
	uint64_t delay_for = ms * tdivms;
	uint64_t delay_until = begin + delay_for;
	while (get_timer_value() < delay_until);
}

void bshal_delay_us(uint32_t us){
	uint64_t begin = get_timer_value() ;
	uint64_t delay_for = us * tdivms / 1000;
	uint64_t delay_until = begin + delay_for;
	while (get_timer_value() < delay_until);
}
#endif


uint32_t HAL_GetTick(void){
	return get_timer_value()/tdivms;
}

void HAL_Delay(uint32_t ms){
	uint64_t begin = get_timer_value() ;
	uint64_t delay_for = ms * tdivms;
	uint64_t delay_until = begin + delay_for;
	while (get_timer_value() < delay_until);
}



///-- can we get away with this
/**
  * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *           
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.         
  *     
  * @note   - The system frequency computed by this function is not the real 
  *           frequency in the chip. It is calculated based on the predefined 
  *           constant and the selected clock source:
  *             
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(*)
  *                                              
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(**)
  *                          
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(**) 
  *             or HSI_VALUE(*) multiplied by the PLL factors.
  *         
  *         (*) HSI_VALUE is a constant defined in stm32f1xx.h file (default value
  *             8 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.   
  *    
  *         (**) HSE_VALUE is a constant defined in stm32f1xx.h file (default value
  *              8 MHz or 25 MHz, depending on the product used), user has to ensure
  *              that HSE_VALUE is same as the real frequency of the crystal used.
  *              Otherwise, this function may have wrong result.
  *                
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate (void)
{
  uint32_t tmp = 0U, pllmull = 0U, pllsource = 0U;

#if defined(STM32F105xC) || defined(STM32F107xC)
  uint32_t prediv1source = 0U, prediv1factor = 0U, prediv2factor = 0U, pll2mull = 0U;
#endif /* STM32F105xC */

#if defined(STM32F100xB) || defined(STM32F100xE)
  uint32_t prediv1factor = 0U;
#endif /* STM32F100xB or STM32F100xE */
    
  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;
  
  switch (tmp)
  {
    case 0x00U:  /* HSI used as system clock */
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x04U:  /* HSE used as system clock */
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x08U:  /* PLL used as system clock */

      /* Get PLL clock source and multiplication factor ----------------------*/
      pllmull = RCC->CFGR & RCC_CFGR_PLLMULL;
      pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;
      
#if !defined(STM32F105xC) && !defined(STM32F107xC)      
      pllmull = ( pllmull >> 18U) + 2U;
      
      if (pllsource == 0x00U)
      {
        /* HSI oscillator clock divided by 2 selected as PLL clock entry */
        SystemCoreClock = (HSI_VALUE >> 1U) * pllmull;
      }
      else
      {
 #if defined(STM32F100xB) || defined(STM32F100xE)
       prediv1factor = (RCC->CFGR2 & RCC_CFGR2_PREDIV1) + 1U;
       /* HSE oscillator clock selected as PREDIV1 clock entry */
       SystemCoreClock = (HSE_VALUE / prediv1factor) * pllmull; 
 #else
        /* HSE selected as PLL clock entry */
        if ((RCC->CFGR & RCC_CFGR_PLLXTPRE) != (uint32_t)RESET)
        {/* HSE oscillator clock divided by 2 */
          SystemCoreClock = (HSE_VALUE >> 1U) * pllmull;
        }
        else
        {
          SystemCoreClock = HSE_VALUE * pllmull;
        }
 #endif
      }
#else
      pllmull = pllmull >> 18U;
      
      if (pllmull != 0x0DU)
      {
         pllmull += 2U;
      }
      else
      { /* PLL multiplication factor = PLL input clock * 6.5 */
        pllmull = 13U / 2U; 
      }
            
      if (pllsource == 0x00U)
      {
        /* HSI oscillator clock divided by 2 selected as PLL clock entry */
        SystemCoreClock = (HSI_VALUE >> 1U) * pllmull;
      }
      else
      {/* PREDIV1 selected as PLL clock entry */
        
        /* Get PREDIV1 clock source and division factor */
        prediv1source = RCC->CFGR2 & RCC_CFGR2_PREDIV1SRC;
        prediv1factor = (RCC->CFGR2 & RCC_CFGR2_PREDIV1) + 1U;
        
        if (prediv1source == 0U)
        { 
          /* HSE oscillator clock selected as PREDIV1 clock entry */
          SystemCoreClock = (HSE_VALUE / prediv1factor) * pllmull;          
        }
        else
        {/* PLL2 clock selected as PREDIV1 clock entry */
          
          /* Get PREDIV2 division factor and PLL2 multiplication factor */
          prediv2factor = ((RCC->CFGR2 & RCC_CFGR2_PREDIV2) >> 4U) + 1U;
          pll2mull = ((RCC->CFGR2 & RCC_CFGR2_PLL2MUL) >> 8U) + 2U; 
          SystemCoreClock = (((HSE_VALUE / prediv2factor) * pll2mull) / prediv1factor) * pllmull;                         
        }
      }
#endif /* STM32F105xC */ 
      break;

    default:
      SystemCoreClock = HSI_VALUE;
      break;
  }
  
  /* Compute HCLK clock frequency ----------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4U)];
  /* HCLK clock frequency */
  SystemCoreClock >>= tmp;  
}
