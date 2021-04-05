/**
 * \file
 *
 * \brief Frequency meter related functionality declaration.
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

#ifndef _HAL_FREQ_METER_SYNC_H_INCLUDED
#define _HAL_FREQ_METER_SYNC_H_INCLUDED

#include <hpl_freqmeter_sync.h>

/**
 * \addtogroup doc_driver_hal_freqmeter_sync
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Synchronous frequency meter descriptor
 */
struct freqmeter_sync_descriptor {
	uint32_t                      period;
	enum freqmeter_parameter      param;
	struct _freqmeter_sync_device device;
};

/**
 * \brief Initialize the frequency meter
 *
 * \param[out] descr The pointer to the frequency meter descriptor
 * \param[in] hw The pointer to the hardware instance
 *
 * \return Initialization status.
 */
int32_t freqmeter_sync_init(struct freqmeter_sync_descriptor *const descr, void *const hw);

/**
 * \brief Deinitialize the frequency meter
 *
 * \param[in] descr The pointer to the frequency meter descriptor
 *
 * \return De-initialization status.
 */
int32_t freqmeter_sync_deinit(struct freqmeter_sync_descriptor *const descr);

/**
 * \brief Enable the frequency meter
 *
 * \param[in] descr The pointer to the frequency meter descriptor
 *
 * \return Enabling status.
 */
int32_t freqmeter_sync_enable(struct freqmeter_sync_descriptor *const descr);

/**
 * \brief Disable the frequency meter
 *
 * \param[in] descr The pointer to the frequency meter descriptor
 *
 * \return Disabling status.
 */
int32_t freqmeter_sync_disable(struct freqmeter_sync_descriptor *const descr);

/**
 * \brief Set period of measurement
 *
 * \param[in] descr The pointer to the frequency meter descriptor
 * \param[in] period Period in microseconds
 *
 * \return Status for period setting
 */
int32_t freqmeter_sync_set_measurement_period(struct freqmeter_sync_descriptor *const descr, const uint32_t period);

/**
 * \brief Set the parameter to measure
 *
 * \param[in] descr The pointer to the frequency meter descriptor
 * \param[in] parameter Signal parameter to measure
 *
 * \return Status for the parameter setting
 */
int32_t freqmeter_sync_set_measurement_parameter(struct freqmeter_sync_descriptor *const descr,
                                                 const enum freqmeter_parameter          parameter);

/**
 * \brief Read values from the frequency meter
 *
 * \param[in] descr  The pointer to the frequency meter descriptor
 * \param[in] data   The point to the data buffer to write data to
 * \param[in] length The amount of measurements to read
 *
 * \return Amount of bytes to read
 */
int32_t freqmeter_sync_read(struct freqmeter_sync_descriptor *const descr, uint32_t *const data, const uint16_t length);

/**
 * \brief Retrieve the current driver version
 *
 * \return Current driver version.
 */
uint32_t freqmeter_sync_get_version(void);

#ifdef __cplusplus
}
#endif
/**@}*/
#endif /* _HAL_FREQ_METER_SYNC_H_INCLUDED */
