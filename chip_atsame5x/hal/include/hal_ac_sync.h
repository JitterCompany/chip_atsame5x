/**
 * \file
 *
 * \brief AC functionality declaration.
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

#ifndef HAL_AC_SYNC_H_INCLUDED
#define HAL_AC_SYNC_H_INCLUDED

#include <hpl_ac_sync.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup doc_driver_hal_ac_sync
 *
 *@{
 */

/**
 * \brief AC Synchronous descriptor
 *
 * The AC descriptor forward declaration.
 */
struct ac_sync_descriptor;

/**
 * \brief AC descriptor
 */
struct ac_sync_descriptor {
	struct _ac_sync_device device; /*!< AC HPL device descriptor */
};

/**
 * \brief Initialize AC.
 *
 * This function initializes the given AC descriptor.
 * It checks if the given hardware is not initialized and if the given hardware
 * is permitted to be initialized.
 *
 * \param[out] descr An AC descriptor to initialize
 * \param[in] hw The pointer to hardware instance
 *
 * \return Initialization status.
 */
int32_t ac_sync_init(struct ac_sync_descriptor *const descr, void *const hw);

/**
 * \brief Deinitialize AC.
 *
 * This function deinitializes the given AC descriptor.
 * It checks if the given hardware is initialized and if the given hardware is
 * permitted to be deinitialized.
 *
 * \param[in] descr An AC descriptor to deinitialize
 *
 * \return De-initialization status.
 */
int32_t ac_sync_deinit(struct ac_sync_descriptor *const descr);

/**
 * \brief Enable AC
 *
 * This function enables the AC by the given AC descriptor.
 *
 * \param[in] descr An AC descriptor to enable
 *
 * \return Enabling status.
 */
int32_t ac_sync_enable(struct ac_sync_descriptor *const descr);

/**
 * \brief Disable AC
 *
 * This function disables the AC by the given AC descriptor.
 *
 * \param[in] descr An AC descriptor to disable
 *
 * \return Disabling status.
 */
int32_t ac_sync_disable(struct ac_sync_descriptor *const descr);

/**
 * \brief Read Comparison result.
 *
 * \param[in] descr The pointer to AC descriptor
 * \param[in] comp Comparator number
 *
 * \return The comparison result.
 *
 * \retval 0 The comparison result is 0
 * \retval 1 The comparison result is 1
 * \retval ERR_NOT_READY  The comparison result is not ready or input parameters are not correct.
 */
int32_t ac_sync_get_result(struct ac_sync_descriptor *const descr, const uint8_t comp);

/**
 * \brief Start conversion
 *
 * This function starts single-short comparison if signal-shot mode is enabled.
 *
 * \param[in] descr The pointer to AC descriptor
 * \param[in] comp Comparator number
 *
 * \return Start Comparator n start Comparison.
 */
int32_t ac_sync_start_comparison(struct ac_sync_descriptor *const descr, uint8_t comp);

/**
 * \brief Stop conversion.
 *
 * This function stops single-short comparison if signal-shot mode is enabled.
 *
 * \param[in] descr The pointer to AC descriptor
 * \param[in] comp Comparator number
 *
 * \return Start Comparator n start Comparison.
 */
int32_t ac_sync_stop_comparison(struct ac_sync_descriptor *const descr, uint8_t comp);

/**
 * \brief Retrieve the current driver version
 *
 * \return Current driver version.
 */
uint32_t ac_sync_get_version(void);
/**@}*/

#ifdef __cplusplus
}
#endif

#endif
