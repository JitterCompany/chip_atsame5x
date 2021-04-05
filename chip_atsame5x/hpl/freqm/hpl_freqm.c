/**
 * \file
 *
 * \brief SAM Frequency Meter
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
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

#include <hpl_freqm_config.h>
#include <hpl_freqmeter_sync.h>
#include <hpl_freqmeter_async.h>
#include <peripheral_clk_config.h>
#include <utils_assert.h>

/*!< Pointer to hpl device */
static struct _freqmeter_async_device *_freq_dev = NULL;

static int32_t     _freqmeter_init(void *const hw);
static inline void _freqmeter_deinit(void *const hw);
static uint32_t    _freqmeter_set_measurement_period(void *const hw, const uint32_t period);

/**
 * \brief Initialize synchronous frequency meter
 */
int32_t _freqmeter_sync_init(struct _freqmeter_sync_device *const device, void *const hw)
{
	ASSERT(device);

	device->hw = hw;

	return _freqmeter_init(device->hw);
}

/**
 * \brief Initialize asynchronous frequency meter
 */
int32_t _freqmeter_async_init(struct _freqmeter_async_device *const device, void *const hw)
{
	int32_t init_status;

	ASSERT(device);

	init_status = _freqmeter_init(hw);
	if (init_status) {
		return init_status;
	}
	device->hw = hw;
	_freq_dev  = device;

	NVIC_DisableIRQ(FREQM_IRQn);
	NVIC_ClearPendingIRQ(FREQM_IRQn);
	NVIC_EnableIRQ(FREQM_IRQn);

	return ERR_NONE;
}

/**
 * \brief De-initialize frequency meter
 */
void _freqmeter_sync_deinit(struct _freqmeter_sync_device *const device)
{
	_freqmeter_deinit(device->hw);
}

/**
 * \brief De-initialize frequency meter
 */
void _freqmeter_async_deinit(struct _freqmeter_async_device *const device)
{
	NVIC_DisableIRQ(FREQM_IRQn);
	NVIC_ClearPendingIRQ(FREQM_IRQn);

	_freqmeter_deinit(device->hw);
}

/**
 * \brief Enable frequency meter module
 */
void _freqmeter_sync_enable(struct _freqmeter_sync_device *const device)
{
	hri_freqm_set_CTRLA_ENABLE_bit(device->hw);
	hri_freqm_wait_for_sync(device->hw, FREQM_SYNCBUSY_ENABLE);
}

/**
 * \brief Enable frequency meter module
 */
void _freqmeter_async_enable(struct _freqmeter_async_device *const device)
{
	hri_freqm_set_CTRLA_ENABLE_bit(device->hw);
	hri_freqm_wait_for_sync(device->hw, FREQM_SYNCBUSY_ENABLE);
}

/**
 * \brief Disable frequency meter module
 */
void _freqmeter_sync_disable(struct _freqmeter_sync_device *const device)
{
	hri_freqm_clear_CTRLA_ENABLE_bit(device->hw);
	hri_freqm_wait_for_sync(device->hw, FREQM_SYNCBUSY_ENABLE);
}

/**
 * \brief Disable frequency meter module
 */
void _freqmeter_async_disable(struct _freqmeter_async_device *const device)
{
	hri_freqm_clear_CTRLA_ENABLE_bit(device->hw);
	hri_freqm_wait_for_sync(device->hw, FREQM_SYNCBUSY_ENABLE);
}

/**
 * \brief Start frequency meter module
 */
void _freqmeter_sync_start(struct _freqmeter_sync_device *const device)
{
	hri_freqm_write_CTRLB_reg(device->hw, FREQM_CTRLB_START);
}

/**
 * \brief Start frequency meter module
 */
void _freqmeter_async_start(struct _freqmeter_async_device *const device)
{
	hri_freqm_write_CTRLB_reg(device->hw, FREQM_CTRLB_START);
}

/**
 * \brief Set measurement period
 */
uint32_t _freqmeter_sync_set_measurement_period(struct _freqmeter_sync_device *const device, const uint32_t period)
{
	return _freqmeter_set_measurement_period(device->hw, period);
}

/**
 * \brief Set measurement period
 */
uint32_t _freqmeter_async_set_measurement_period(struct _freqmeter_async_device *const device, const uint32_t period)
{
	return _freqmeter_set_measurement_period(device->hw, period);
}

/**
 * \brief Check if measurement is complete
 *
 * \param[in] device The pointer to frequency meter device instance
 *
 * \return True if it is complete, false otherwise
 */
bool _freqmeter_sync_is_measurement_done(const struct _freqmeter_sync_device *const device)
{
	return !hri_freqm_get_STATUS_BUSY_bit(device->hw);
}

/**
 * \brief Read values from frequency meter
 */
uint32_t _freqmeter_sync_read(const struct _freqmeter_sync_device *const device)
{
	return hri_freqm_read_VALUE_reg(device->hw);
}

/**
 * \brief Enable/disable frequency meter interrupt
 */
void _freqmeter_async_set_irq_state(struct _freqmeter_async_device *const         device,
                                    const enum _freqmeter_dev_async_callback_type type, const bool state)
{
	void *const hw = device->hw;

	if (FREQMETER_DEV_ASYNC_MEASUREMENET_DONE_CB == type) {
		hri_freqm_write_INTEN_DONE_bit(hw, state);
	}
}

/**
 * \brief Initialize frequency meter
 *
 * \param[in] hw The pointer to hardware instance
 *
 * \return Initialization status
 */
static int32_t _freqmeter_init(void *const hw)
{
	if (!hri_freqm_is_syncing(hw, FREQM_SYNCBUSY_SWRST)) {
		if (hri_freqm_get_CTRLA_reg(hw, FREQM_CTRLA_ENABLE)) {
			hri_freqm_clear_CTRLA_ENABLE_bit(hw);
			hri_freqm_wait_for_sync(hw, FREQM_SYNCBUSY_ENABLE);
		}
		hri_freqm_write_CTRLA_reg(hw, FREQM_CTRLA_SWRST);
	}
	hri_freqm_wait_for_sync(hw, FREQM_SYNCBUSY_SWRST);

	hri_freqm_write_CFGA_reg(hw, CONF_FREQM_REFNUM);

	return ERR_NONE;
}

/**
 * \brief De-initialize frequency meter
 *
 * \param[in] hw The pointer to hardware instance
 */
static inline void _freqmeter_deinit(void *const hw)
{
	hri_freqm_clear_CTRLA_ENABLE_bit(hw);
	hri_freqm_wait_for_sync(hw, FREQM_SYNCBUSY_ENABLE);
	hri_freqm_set_CTRLA_SWRST_bit(hw);
}

/**
 * \brief Set measurement period
 *
 * \param[in] hw The pointer to hardware instance
 * \param[in] period The period to set
 *
 * \return The period value which is set
 */
static uint32_t _freqmeter_set_measurement_period(void *const hw, const uint32_t period)
{
	bool     enabled = hri_freqm_get_CTRLA_ENABLE_bit(hw);
	uint32_t value   = (uint32_t)((uint64_t)period * CONF_GCLK_FREQM_REF_FREQUENCY / 1000000);

	hri_freqm_write_CTRLA_ENABLE_bit(hw, 0);
	hri_freqm_wait_for_sync(hw, FREQM_SYNCBUSY_ENABLE);

	if (value > 0xFF) {
		value = CONF_FREQM_REFNUM;
	}
	hri_freqm_write_CFGA_reg(hw, value);

	if (enabled) {
		hri_freqm_write_CTRLA_ENABLE_bit(hw, enabled);
		hri_freqm_wait_for_sync(hw, FREQM_SYNCBUSY_ENABLE);
	}

	return (value * 1000000) / CONF_GCLK_FREQM_REF_FREQUENCY;
}
