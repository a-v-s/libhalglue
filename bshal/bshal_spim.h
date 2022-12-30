#ifndef __BSHAL_SPIM__
#define __BSHAL_SPIM__


#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    void                  *drv_specific;   ///< Implementation specific;
    uint8_t               hw_nr;            ///< Hardware number
    uint8_t               sck_pin;         ///< SCK pin number
    uint8_t               mosi_pin;        ///< MOSI pin number
    uint8_t               miso_pin;        ///< MISO pin number (optional).
    uint8_t               cs_pin;          ///< Chip Select pin number (optional).
    uint8_t               cs_pol;           ///< Chip Select Polarity
    uint8_t               rs_pin;        ///< Reset pin number (optional).
    uint8_t               rs_pol;        ///< Reset Polarity
    uint8_t               mode;            ///< SPIM mode.
    uint8_t               bit_order;       ///< SPIM bit order.
    uint32_t              frequency;       ///< SPIM frequency.

    //uint8_t            irq_pin;        ///< IRQ pin number (optional).
} bshal_spim_instance_t;



int bshal_spim_init(bshal_spim_instance_t * bshal_spim);
int bshal_spim_config(bshal_spim_instance_t *config);

int bshal_spim_transmit(bshal_spim_instance_t * bshal_spim, void *data, size_t amount, bool nostop);
int bshal_spim_recveive(bshal_spim_instance_t * bshal_spim, void *data, size_t amount, bool nostop);
int bshal_spim_transceive(bshal_spim_instance_t * bshal_spim, void *data, size_t amount, bool nostop);
int bshal_spim_transceive_split(bshal_spim_instance_t * bshal_spim, void *txdata, void *rxdata,size_t amount, bool nostop);

#endif //  __BSHAL_SPIM__
