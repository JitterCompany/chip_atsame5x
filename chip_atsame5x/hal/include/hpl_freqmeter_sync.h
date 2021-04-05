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

#ifndef _HPL_FREQMETER_SYNC_H_INCLUDED
#define _HPL_FREQMETER_SYNC_H_INCLUDED

/**
 * \addtogroup HPL Frequency meter
 *
 * \section hpl_freqmeter_sync_rev Revision History
 * - v1.0.0 Initial Release
 *
 *@{
 */

#include <hpl_freqmeter.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Frequency meter descriptor device structure
 */
struct _freqmeter_sync_device {
	void *hw;
};

/**
 * \name HPL functions
 */
//@{
/**
 * \brief Initialize synchronous frequency meter
 *
 * This function does low level frequency meter configuration.
 *
 * \param[in] device The pointer to frequency meter device instance
 * \param[in] hw The pointer to hardware instance
 *
 * \return Initialization status
 */
int32_t _freqmeter_sync_init(struct _freqmeter_sync_device *const device, void *const hw);

/**
 * \brief Deinitialize frequency meter
 *
 * This function closes the given frequency meter by disabling its clock.
 *
 * \param[in] device The pointer to frequency meter device instance
 */
void _freqmeter_sync_deinit(struct _freqmeter_sync_device *const device);

/**
 * \brief Enable frequency meter module
 *
 * This function will enable the frequency meter module
 *
 * \param[in] device The pointer to frequency meter device instance
 */
void _freqmeter_sync_enable(struct _freqmeter_sync_device *const device);

/**
 * \brief Disable frequency meter module
 *
 * This function will disable the frequency meter module
 *
 * \param[in] device The pointer to frequency meter device instance
 */
void _freqmeter_sync_disable(struct _freqmeter_sync_device *const device);

/**
 * \brief Start frequency meter module
 *
 * \param[in] device The pointer to frequency meter device instance
 */
void _freqmeter_sync_start(struct _freqmeter_sync_device *const device);

/**
 * \brief Set measurement period
 *
 * \param[in] device The pointer to frequency meter device instance
 * \param[in] period The period to set, in ms
 *
 * \return The period value which is set
 */
uint32_t _freqmeter_sync_set_measurement_period(struct _freqmeter_sync_device *const device, const uint32_t period);

/**
 * \brief Check if measurement is complete
 *
 * \param[in] device The pointer to frequency meter device instance
 *
 * \return True if it is complete, false otherwise
 */
bool _freqmeter_sync_is_measurement_done(const struct _freqmeter_sync_device *const device);

/**
 * \brief Read values from frequency meter
 *
 * \param[in] device The pointer to frequency meter device instance
 *
 * \return Read value
 */
uint32_t _freqmeter_sync_read(const struct _freqmeter_sync_device *const device);
//@}

#ifdef __cplusplus
}
#endif
/**@}*/
#endif /* _HPL_FREQMETER_SYNC_H_INCLUDED */
