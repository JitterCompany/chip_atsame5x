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

#ifndef HAL_PDEC_ASYNC_H_INCLUDED
#define HAL_PDEC_ASYNC_H_INCLUDED

#include <utils.h>
#include <hpl_pdec_async.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup doc_driver_hal_pdec_async
 *
 *@{
 */

/**
 * \brief Position Decoder descriptor
 *
 * The Position Decoder descriptor forward declaration.
 */
struct pdec_async_descriptor;

typedef void (*pdec_async_position_cb_t)(const struct pdec_async_descriptor *const descr, uint8_t ch);
typedef void (*pdec_async_irq_cb_t)(const struct pdec_async_descriptor *const descr,
                                    enum pdec_async_interrupt_type type, uint8_t ch);

/**
 * \brief Position Decoder callbacks
 */
struct pdec_async_callbacks {
	/* Callback type for position changed */
	pdec_async_position_cb_t pos_changed;
	/* Callback type for other interrupts */
	pdec_async_irq_cb_t irq_handler;
};

/**
 * \brief Position Decoder descriptor
 */
struct pdec_async_descriptor {
	/* Position Decoder device */
	struct _pdec_async_device device;
	/* Position Decoder callbacks type */
	struct pdec_async_callbacks pdec_async_cb;
};

/**
 * \brief Initialize Position Decoder
 *
 * This function initializes the given Position Decoder descriptor.
 * It checks if the given hardware is not initialized and if the given hardware
 * is permitted to be initialized.
 *
 * \param[out] descr         A Position Decoder descriptor to be initialized
 * \param[in] hw             The pointer to hardware instance
 *
 * \return Initialization status.
 * \retval -1 Passed parameters were invalid
 * \retval 0 The initialization is completed successfully
 */
int32_t pdec_async_init(struct pdec_async_descriptor *const descr, void *const hw);

/**
 * \brief Deinitialize Position Decoder
 *
 * This function deinitializes the given Position Decoder descriptor.
 * It checks if the given hardware is initialized and if the given hardware is
 * permitted to be deinitialized.
 *
 * \param[in] descr A Position Decoder descriptor to deinitialize
 *
 * \return De-initialization status.
 */
int32_t pdec_async_deinit(struct pdec_async_descriptor *const descr);

/**
 * \brief Eanble Position Decoder
 *
 * This function starts the Position Decoder
 *
 * \param[in] descr The pointer to the Position Decoder descriptor
 *
 * \return Enabling status.
 */
int32_t pdec_async_enable(struct pdec_async_descriptor *const descr);

/**
 * \brief Disable Position Decoder
 *
 * This function stops of Position Decoder
 *
 * \param[in] descr The pointer to the Position Decoder descriptor
 *
 * \return Disabling status.
 */
int32_t pdec_async_disable(struct pdec_async_descriptor *const descr);

/**
 * \brief Write motor axis position
 *
 * \param[in] descr     The pointer to the Position Decoder descriptor
 * \param[in] value     The position count to write
 * \param[in] axis      The axis number to read
 *
 * \return Write operation status
 */
int32_t pdec_async_write_position(struct pdec_async_descriptor *const descr, uint32_t value, uint8_t axis);

/**
 * \brief Read motor axis position
 *
 * This function reads the position count of the motor axis. With this count,
 * the user will know if the motor axis movement is clockwise (increase) or
 * re-clockwise (decrease),  and the position of the movement.
 *
 * \param[in] descr     The pointer to the Position Decoder descriptor
 * \param[in] axis      The axis number to read
 *
 * \return The position count of the motor axis.
 */
uint32_t pdec_async_read_position(struct pdec_async_descriptor *const descr, uint8_t axis);

/**
 * \brief Set Position Decoder upper threshold
 *
 * This function sets Position Decoder upper threshold.
 *
 * \param[in] descr         The pointer to the Position Decoder descriptor
 * \param[in] up_threshold  An upper threshold to set
 *
 * \return Status of the Position Decoder upper threshold setting.
 */
int32_t pdec_async_set_up_threshold(struct pdec_async_descriptor *const descr, const uint32_t up_threshold,
                                    uint8_t axis);

/**
 * \brief Set Position Decoder lower threshold
 *
 * This function sets Position Decoder lower threshold.
 *
 * \param[in] descr          The pointer to the Position Decoder descriptor
 * \param[in] low_threshold  A lower threshold to set
 * \param[in] axis           The axis number to set
 *
 * \return Status of the Position Decoder lower threshold setting.
 */
int32_t pdec_async_set_low_threshold(struct pdec_async_descriptor *const descr, const uint32_t low_threshold,
                                     uint8_t axis);

/**
 * \brief Register Position Decoder callback
 *
 * \param[in] io_descr  A Position Decoder descriptor
 * \param[in] type      Callback type
 * \param[in] cb        A callback function, passing NULL de-registers callback
 *
 * \return The status of callback assignment.
 * \retval -1 Passed parameters were invalid
 * \retval 0 A callback is registered successfully
 */
int32_t pdec_async_register_callback(struct pdec_async_descriptor *const descr,
                                     const enum pdec_async_callback_type type, FUNC_PTR cb);

/**
 * \brief Retrieve the current driver version
 *
 * \return Current driver version.
 */
uint32_t pdec_async_get_version(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* HAL_PDEC_ASYNC_H_INCLUDED */
