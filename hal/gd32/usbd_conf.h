#ifndef USBD_CONF_H
#define USBD_CONF_H



#include <stdint.h>

#if defined GD32F1
#include "gd32f10x.h"
#elif defined GD32VF1
#include "gd32vf103.h"
#endif


#define LPM_ENABLED

#define EP_COUNT	(8)
#define BUFFER_ADDRESS                     (0x0000U)

#ifndef USB_PULLUP 
#define USB_PULLUP                         GPIOC
#endif
#ifndef USB_PULLUP_PIN
#define USB_PULLUP_PIN                     GPIO_PIN_13
#endif
#ifndef RCC_AHBPeriph_GPIO_PULLUP 
#define RCC_AHBPeriph_GPIO_PULLUP          RCU_GPIOC
#endif

#endif /* USBD_CONF_H */
