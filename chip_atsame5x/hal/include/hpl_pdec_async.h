/**
 * \file
 *
 * \brief Position Decoder related functionality declaration.
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

#ifndef _HPL_PDEC_ASYNC_H_INCLUDED
#define _HPL_PDEC_ASYNC_H_INCLUDED

/**
 * \addtogroup HPL Position Decoder
 *
 * \section hpl_async_pdec_rev Revision History
 * - v1.0.0 Initial Release
 *
 *@{
 */

#include <hpl_irq.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Position Decoder device structure
 *
 * The Position Decoder device structure forward declaration.
 */
struct _pdec_async_device;

/**
 * \brief Position Decoder callback types
 */
enum pdec_async_callback_type {
	/** Position changed */
	PDEC_ASYNC_POS_CHANGED_CB,
	/** IRQ events */
	PDEC_ASYNC_IRQ_CB
};

/**
 * \brief Position Decoder interrupt types
 */
enum pdec_async_interrupt_type {
	/** Interrupt type for direction changed */
	PDEC_IRQ_DIRECTION,
	/** Interrupt type for position counter overflow */
	PDEC_IRQ_OVERFLOW,
	/** Interrupt type for position counter underflow */
	PDEC_IRQ_UNDERFLOW,
	/** Interrupt type for errors */
	PDEC_IRQ_ERROR
};

/**
 * \brief Position Decoder interrupt callbacks
 */
struct _pdec_async_callbacks {
	void (*pos_changed)(const struct _pdec_async_device *const device, uint8_t ch);
	void (*irq_handler)(const struct _pdec_async_device *dev, enum pdec_async_interrupt_type type, uint8_t ch);
};

/**
 * \brief Position Decoder descriptor device structure
 */
struct _pdec_async_device {
	struct _pdec_async_callbacks pdec_async_cb;
	struct _irq_descriptor       irq;
	void *                       hw;
};

/**
 * \brief Initialize Position Decoder
 *
 * This function does low level Position Decoder configuration.
 *
 * param[in] device The pointer to Position Decoder device instance
 * param[in] hw     The pointer to hardware instance
 *
 * \return Initialization status
 */
int32_t _pdec_async_init(struct _pdec_async_device *const device, void *const hw);

/**
 * \brief Deinitialize Position Decoder
 *
 * \param[in] device The pointer to Position Decoder device instance
 */
void _pdec_async_deinit(struct _pdec_async_device *const device);

/**
 * \brief Enable Position Decoder
 *
 * \param[in] device The pointer to Position Decoder device instance
 */
void _pdec_async_enable(struct _pdec_async_device *const device);

/**
 * \brief Disable Position Decoder
 *
 * \param[in] device The pointer to Position Decoder device instance
 */
void _pdec_async_disable(struct _pdec_async_device *const device);

/**
 * \brief Write motor axis position
 *
 * \param[in] device     The pointer to Position Decoder device instance
 * \param[in] value      The position count to write
 * \param[in] axis       The axis number to write
 */
void _pdec_async_write_position(struct _pdec_async_device *const device, uint32_t value, uint8_t axis);

/**
 * \brief Read motor axis position
 *
 * \param[in] device     The pointer to Position Decoder device instance
 * \param[in] axis       The axis number to read
 *
 * \return The position count of motor axis
 */
uint32_t _pdec_async_read_position(struct _pdec_async_device *const device, uint8_t axis);

/**
 * \brief Set Position Decoder upper threshold
 *
 * This function sets Position Decoder upper threshold.
 *
 * \param[in] device         The pointer to Position Decoder device instance
 * \param[in] up_threshold   An upper threshold to set
 * \param[in] axis           The axis number to set
 *
 * \return Status of Position Decoder threshold setting.
 */
int32_t _pdec_async_set_up_threshold(struct _pdec_async_device *const device, const uint32_t up_threshold,
                                     uint8_t axis);

/**
 * \brief Set Position Decoder lower threshold
 *
 * This function sets Position Decoder lower threshold.
 *
 * \param[in] device         The pointer to Position Decoder device instance
 * \param[in] low_threshold  An lower threshold to set
 * \param[in] axis           The axis number to set
 *
 * \return Status of Position Decoder threshold setting.
 */
int32_t _pdec_async_set_low_threshold(struct _pdec_async_device *const device, const uint32_t low_threshold,
                                      uint8_t axis);

/**
 * \brief Enable/disable Position Decoder interrupt
 *
 * \param[in] device   The pointer to Position Decoder device instance
 * \param[in] type     The type of interrupt to disable/enable if applicable
 * \param[in] state    Enable or disable
 */
void _pdec_async_set_irq_state(struct _pdec_async_device *const device, const enum pdec_async_callback_type type,
                               const bool state);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* _HPL_PDEC_ASYNC_H_INCLUDED */
