/**
 * \file
 *
 * \brief Position Decoder functionality declaration.
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
#include <hpl_irq.h>
#include <hal_pdec_async.h>

/**
 * \brief Driver version
 */
#define DRIVER_VERSION 0x00000001u

static void pdec_async_pos_changed(const struct _pdec_async_device *const device, uint8_t ch);
static void pdec_async_irq_handler(const struct _pdec_async_device *const device, enum pdec_async_interrupt_type type,
                                   uint8_t ch);

/**
 * \brief Initialize Position Decoder
 */
int32_t pdec_async_init(struct pdec_async_descriptor *const descr, void *const hw)
{
	int32_t                    init_status;
	struct _pdec_async_device *device;

	ASSERT(descr && hw);

	device      = &descr->device;
	init_status = _pdec_async_init(&descr->device, hw);
	if (init_status) {
		return init_status;
	}
	device->pdec_async_cb.pos_changed = pdec_async_pos_changed;
	device->pdec_async_cb.irq_handler = pdec_async_irq_handler;

	return ERR_NONE;
}

/**
 * \brief Deinitialize Position Decoder
 */
int32_t pdec_async_deinit(struct pdec_async_descriptor *const descr)
{
	ASSERT(descr);

	_pdec_async_deinit(&descr->device);

	return ERR_NONE;
}

/**
 * \brief Enable Position Decoder
 */
int32_t pdec_async_enable(struct pdec_async_descriptor *const descr)
{
	ASSERT(descr);

	_pdec_async_enable(&descr->device);

	return ERR_NONE;
}

/**
 * \brief Disable Position Decoder
 */
int32_t pdec_async_disable(struct pdec_async_descriptor *const descr)
{
	ASSERT(descr);

	_pdec_async_disable(&descr->device);

	return ERR_NONE;
}

/**
 * \brief Write motor axis position count
 */
int32_t pdec_async_write_position(struct pdec_async_descriptor *const descr, uint32_t value, uint8_t axis)
{
	ASSERT(descr);

	_pdec_async_write_position(&descr->device, value, axis);

	return ERR_NONE;
}

/**
 * \brief Read motor axis position count
 */
uint32_t pdec_async_read_position(struct pdec_async_descriptor *const descr, uint8_t axis)
{
	ASSERT(descr);

	return _pdec_async_read_position(&descr->device, axis);
}

/**
 * \brief Set Position Decoder upper threshold
 */
int32_t pdec_async_set_up_threshold(struct pdec_async_descriptor *const descr, const uint32_t up_threshold,
                                    uint8_t axis)
{
	ASSERT(descr);

	return _pdec_async_set_up_threshold(&descr->device, up_threshold, axis);
}

/**
 * \brief Set Position Decoder lower threshold
 */
int32_t pdec_async_set_low_threshold(struct pdec_async_descriptor *const descr, const uint32_t low_threshold,
                                     uint8_t axis)
{
	ASSERT(descr);

	return _pdec_async_set_low_threshold(&descr->device, low_threshold, axis);
}

/**
 * \brief Register Position Decoder callback
 */
int32_t pdec_async_register_callback(struct pdec_async_descriptor *const descr,
                                     const enum pdec_async_callback_type type, FUNC_PTR cb)
{
	ASSERT(descr);

	switch (type) {
	case PDEC_ASYNC_POS_CHANGED_CB:
		descr->pdec_async_cb.pos_changed = (pdec_async_position_cb_t)cb;
		break;
	case PDEC_ASYNC_IRQ_CB:
		descr->pdec_async_cb.irq_handler = (pdec_async_irq_cb_t)cb;
		break;
	default:
		return ERR_INVALID_ARG;
	}

	_pdec_async_set_irq_state(&descr->device, type, cb != NULL);

	return ERR_NONE;
}

/**
 * \brief Retrieve the current driver version
 */
uint32_t pdec_async_get_version(void)
{
	return DRIVER_VERSION;
}

/**
 * \internal Position Decoder position changed handler
 *
 * \param[in] device The pointer to Position Decoder device structure
 */
static void pdec_async_pos_changed(const struct _pdec_async_device *const device, uint8_t ch)
{
	struct pdec_async_descriptor *const descr = CONTAINER_OF(device, struct pdec_async_descriptor, device);

	if (descr->pdec_async_cb.pos_changed) {
		descr->pdec_async_cb.pos_changed(descr, ch);
	}
}

/**
 * \internal Position Decoder interrupt handler
 *
 * \param[in] device The pointer to Position Decoder device structure
 * \param[in] type The type of Position Decoder interrupt
 */
static void pdec_async_irq_handler(const struct _pdec_async_device *const device, enum pdec_async_interrupt_type type,
                                   uint8_t ch)
{
	struct pdec_async_descriptor *const descr = CONTAINER_OF(device, struct pdec_async_descriptor, device);

	if (descr->pdec_async_cb.irq_handler) {
		descr->pdec_async_cb.irq_handler(descr, type, ch);
	}
}
