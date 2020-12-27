typedef struct {
	void 				  *drv_specific;   ///< Implementation specific;
	uint8_t               sck_pin;         ///< SCK pin number
    uint8_t               mosi_pin;        ///< MOSI pin number
    uint8_t               miso_pin;        ///< MISO pin number (optional).
    uint8_t               nss_pin;         ///< Slave Select pin number (optional).
	uint8_t               ncd_pin;         ///< D/CX pin number (optional).
	uint8_t               nrs_pin;         ///< Reset pin number (optional).
	uint8_t               irq_pin;         ///< IRQ pin number (optional).
    uint8_t               mode;            ///< SPIM mode.
    uint8_t               bit_order;       ///< SPIM bit order.
	uint32_t              frequency;       ///< SPIM frequency.
	uint8_t				  hw_nr; 	       ///< Hardware number
} bshal_spim_t;


int bshal_spim_init(bshal_spim_t * bshal_spim);

int bshal_spim_transmit(bshal_spim_t * bshal_spim, void *data, size_t amount, bool nostop);
int bshal_spim_recveive(bshal_spim_t * bshal_spim, void *data, size_t amount, bool nostop);
int bshal_spim_transceive(bshal_spim_t * bshal_spim, void *data, size_t amount);
