/**
 * \file
 *
 * \brief SAM Position Decoder (PDEC)
 *
 * Copyright (c) 2016-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include <utils_assert.h>
#include <hpl_pdec_async.h>
#include <hpl_pdec_config.h>

/**
 * \brief PDEC configuration type
 */
struct pdec_configuration {
	hri_pdec_ctrla_reg_t    ctrl_a;
	hri_pdec_filter_reg_t   filter;
	hri_pdec_dbgctrl_reg_t  dbgctrl;
	hri_pdec_presc_reg_t    presc;
	hri_pdec_intenset_reg_t interrupt;
	hri_pdec_evctrl_reg_t   evctrl;
};

/**
 * \brief Array of PDEC configurations
 */
static struct pdec_configuration _pdecs
    = {PDEC_CTRLA_MODE_QDEC | (CONF_PDEC_RUNSTDBY << PDEC_CTRLA_RUNSTDBY_Pos) | PDEC_CTRLA_CONF(CONF_PDEC_CONFIG)
           | (CONF_PDEC_PHASE_SWAP << PDEC_CTRLA_SWAP_Pos) | PDEC_CTRLA_PEREN | PDEC_CTRLA_PINEN_Msk
           | (CONF_PDEC_PHASEA_INVERT << PDEC_CTRLA_PINVEN0_Pos) | (CONF_PDEC_PHASEB_INVERT << PDEC_CTRLA_PINVEN1_Pos)
           | (CONF_PDEC_INDEX_INVERT << PDEC_CTRLA_PINVEN2_Pos) | PDEC_CTRLA_ANGULAR(0x7)
           | PDEC_CTRLA_MAXCMP(CONF_PDEC_FILTER),
       PDEC_FILTER_FILTER(CONF_PDEC_DURATION),
       (CONF_PDEC_DBGRUN << PDEC_DBGCTRL_DBGRUN_Pos),
       PDEC_PRESC_PRESC(CONF_PDEC_PRESCALER),
       (CONF_PDEC_INT_OVERFLOW << PDEC_INTENSET_OVF_Pos) | (CONF_PDEC_INT_ERROR << PDEC_INTENSET_ERR_Pos)
           | (CONF_PDEC_INT_DIR << PDEC_INTENSET_DIR_Pos) | (CONF_PDEC_INT_VEL << PDEC_INTENSET_VLC_Pos),
       (CONF_PDEC_OVERFLOWEO << PDEC_EVCTRL_OVFEO_Pos) | (CONF_PDEC_ERROREO << PDEC_EVCTRL_ERREO_Pos)
           | (CONF_PDEC_DIREO << PDEC_EVCTRL_DIREO_Pos) | (CONF_PDEC_VELOCITYEO << PDEC_EVCTRL_VLCEO_Pos)};

/*!< Pointer to hpl device */
static struct _pdec_async_device *_pdecs_dev = NULL;

/**
 * \brief Initialize PDEC
 *
 * \param[in] hw The pointer to hardware instance
 *
 * \return Initialization status
 */
static int32_t _pdec_init(void *const hw)
{
	ASSERT(hw);

	if (!hri_pdec_is_syncing(hw, PDEC_SYNCBUSY_SWRST)) {
		if (hri_pdec_get_CTRLA_reg(hw, PDEC_CTRLA_ENABLE)) {
			hri_pdec_clear_CTRLA_ENABLE_bit(hw);
			hri_pdec_wait_for_sync(hw, PDEC_SYNCBUSY_ENABLE);
		}
		hri_pdec_write_CTRLA_reg(hw, PDEC_CTRLA_SWRST);
	}
	hri_pdec_wait_for_sync(hw, PDEC_SYNCBUSY_SWRST);

	hri_pdec_write_FILTER_reg(hw, _pdecs.filter);
	hri_pdec_write_DBGCTRL_reg(hw, _pdecs.dbgctrl);
	hri_pdec_write_PRESC_reg(hw, _pdecs.presc);
	hri_pdec_write_EVCTRL_reg(hw, _pdecs.evctrl);
	hri_pdec_write_CC_reg(hw, 0, 0xffff);
	hri_pdec_write_CTRLA_reg(hw, _pdecs.ctrl_a);

	return ERR_NONE;
}

/**
 * \brief Initialize PDEC
 */
int32_t _pdec_async_init(struct _pdec_async_device *const device, void *const hw)
{
	int32_t init_status;

	ASSERT(device);

	init_status = _pdec_init(hw);
	if (init_status) {
		return init_status;
	}
	device->hw = hw;
	_pdecs_dev = device;

	NVIC_DisableIRQ(PDEC_0_IRQn);
	NVIC_ClearPendingIRQ(PDEC_0_IRQn);
	NVIC_EnableIRQ(PDEC_0_IRQn);

	return ERR_NONE;
}

/**
 * \brief De-initialize PDEC
 */
void _pdec_async_deinit(struct _pdec_async_device *const device)
{
	ASSERT(device);

	NVIC_DisableIRQ(PDEC_0_IRQn);
	NVIC_ClearPendingIRQ(PDEC_0_IRQn);
	hri_pdec_clear_CTRLA_ENABLE_bit(device->hw);
	hri_pdec_set_CTRLA_SWRST_bit(device->hw);
}

/**
 * \brief Enable PDEC
 */
void _pdec_async_enable(struct _pdec_async_device *const device)
{
	ASSERT(device);

	hri_pdec_set_CTRLA_ENABLE_bit(device->hw);
	/* Writing a START command */
	hri_pdec_write_CTRLB_CMD_bf(device->hw, PDEC_CTRLBSET_CMD_START_Val);
	hri_pdec_wait_for_sync(device->hw, PDEC_SYNCBUSY_CTRLB);
}

/**
 * \brief Disable PDEC
 */
void _pdec_async_disable(struct _pdec_async_device *const device)
{
	ASSERT(device);

	/* Writing a STOP command */
	hri_pdec_write_CTRLB_CMD_bf(device->hw, PDEC_CTRLBSET_CMD_STOP_Val);
	hri_pdec_wait_for_sync(device->hw, PDEC_SYNCBUSY_CTRLB);
	hri_pdec_clear_CTRLA_ENABLE_bit(device->hw);
}

/**
 * \brief Write PDEC position value
 */
void _pdec_async_write_position(struct _pdec_async_device *const device, uint32_t value, uint8_t axis)
{
	ASSERT(device);
	(void)axis;

	hri_pdec_write_COUNT_reg(device->hw, (uint16_t)value);
	hri_pdec_wait_for_sync(device->hw, PDEC_SYNCBUSY_COUNT);
}

/**
 * \brief Read PDEC position value
 */
uint32_t _pdec_async_read_position(struct _pdec_async_device *const device, uint8_t axis)
{
	ASSERT(device);
	(void)axis;

	hri_pdec_write_CTRLB_CMD_bf(device->hw, PDEC_CTRLBSET_CMD_READSYNC_Val);
	hri_pdec_wait_for_sync(device->hw, PDEC_SYNCBUSY_COUNT);

	return hri_pdec_read_COUNT_reg(device->hw);
}

/**
 * \brief Set PDEC upper threshold
 */
int32_t _pdec_async_set_up_threshold(struct _pdec_async_device *const device, const uint32_t up_threshold, uint8_t axis)
{
	ASSERT(device);
	(void)axis;

	hri_pdec_write_CC_reg(device->hw, 0, (uint16_t)up_threshold);

	return ERR_NONE;
}

/**
 * \brief Set PDEC lower threshold
 */
int32_t _pdec_async_set_low_threshold(struct _pdec_async_device *const device, const uint32_t low_threshold,
                                      uint8_t axis)
{
	ASSERT(device);
	(void)axis;

	return ERR_UNSUPPORTED_OP;
}

/**
 * \brief Enable/disable PDEC interrupt
 */
void _pdec_async_set_irq_state(struct _pdec_async_device *const device, const enum pdec_async_callback_type type,
                               const bool state)
{
	ASSERT(device);

	if (PDEC_ASYNC_POS_CHANGED_CB == type) {
		hri_pdec_write_INTEN_VLC_bit(device->hw, state);
	} else if (PDEC_ASYNC_IRQ_CB == type) {
		hri_pdec_write_INTEN_reg(device->hw, _pdecs.interrupt);
	}
}

/**
 * \internal PDEC interrupt handler
 */
void PDEC_0_Handler(void)
{
	void *const hw = _pdecs_dev->hw;

	if (hri_pdec_get_interrupt_OVF_bit(hw)) {
		hri_pdec_clear_interrupt_OVF_bit(hw);
		if (hri_pdec_get_STATUS_DIR_bit(hw)) {
			_pdecs_dev->pdec_async_cb.irq_handler(_pdecs_dev, PDEC_IRQ_UNDERFLOW, 0);
		} else {
			_pdecs_dev->pdec_async_cb.irq_handler(_pdecs_dev, PDEC_IRQ_OVERFLOW, 0);
		}
	} else if (hri_pdec_get_interrupt_DIR_bit(hw)) {
		hri_pdec_clear_interrupt_DIR_bit(hw);
		_pdecs_dev->pdec_async_cb.irq_handler(_pdecs_dev, PDEC_IRQ_DIRECTION, 0);
	} else if (hri_pdec_get_interrupt_VLC_bit(hw)) {
		hri_pdec_clear_interrupt_VLC_bit(hw);
		_pdecs_dev->pdec_async_cb.pos_changed(_pdecs_dev, 0);
	} else if (hri_pdec_get_interrupt_ERR_bit(hw)) {
		hri_pdec_clear_interrupt_ERR_bit(hw);
		_pdecs_dev->pdec_async_cb.irq_handler(_pdecs_dev, PDEC_IRQ_ERROR, 0);
	}
}
