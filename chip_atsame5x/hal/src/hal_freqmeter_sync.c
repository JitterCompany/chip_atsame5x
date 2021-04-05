/**
 * \file
 *
 * \brief Frequency meter  related functionality implementation.
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

#include <hal_freqmeter_sync.h>
#include <utils_assert.h>

/**
 * \brief Driver version
 */
#define DRIVER_VERSION 0x00000001u

/**
 * \brief Initialize frequency meter
 */
int32_t freqmeter_sync_init(struct freqmeter_sync_descriptor *const descr, void *const hw)
{
	int32_t status;

	ASSERT(descr && hw);

	status = _freqmeter_sync_init(&descr->device, hw);
	if (status) {
		return status;
	}

	descr->period = _freqmeter_sync_set_measurement_period(&descr->device, 0xFFFFFFFF);

	return ERR_NONE;
}

/**
 * \brief Deinitialize frequency meter
 */
int32_t freqmeter_sync_deinit(struct freqmeter_sync_descriptor *const descr)
{
	ASSERT(descr);

	_freqmeter_sync_deinit(&descr->device);

	return ERR_NONE;
}

/**
 * \brief Enable frequency meter
 */
int32_t freqmeter_sync_enable(struct freqmeter_sync_descriptor *const descr)
{
	ASSERT(descr);

	_freqmeter_sync_enable(&descr->device);

	return ERR_NONE;
}

/**
 * \brief Disable frequency meter
 */
int32_t freqmeter_sync_disable(struct freqmeter_sync_descriptor *const descr)
{
	ASSERT(descr);

	_freqmeter_sync_disable(&descr->device);

	return ERR_NONE;
}

/**
 * \brief Set period of measurement
 */
int32_t freqmeter_sync_set_measurement_period(struct freqmeter_sync_descriptor *const descr, const uint32_t period)
{
	ASSERT(descr && period);

	descr->period = _freqmeter_sync_set_measurement_period(&descr->device, period);

	return ERR_NONE;
}

/**
 * \brief Set parameter to measure
 */
int32_t freqmeter_sync_set_measurement_parameter(struct freqmeter_sync_descriptor *const descr,
                                                 const enum freqmeter_parameter          parameter)
{
	ASSERT(descr);

	descr->param = parameter;

	return ERR_NONE;
}

/**
 * \brief Read values from frequency meter
 */
int32_t freqmeter_sync_read(struct freqmeter_sync_descriptor *const descr, uint32_t *const data, const uint16_t length)
{
	uint32_t raw_result;
	uint16_t offset = 0;

	ASSERT(descr && data && length);

	do {
		_freqmeter_sync_start(&descr->device);
		while (!_freqmeter_sync_is_measurement_done(&descr->device))
			;

		raw_result = _freqmeter_sync_read(&descr->device);
		if (FREQMETER_FREQUENCY == descr->param) {
			data[offset] = (uint32_t)((uint64_t)raw_result * 1000000 / descr->period);
		} else {
			data[offset] = (uint32_t)((uint64_t)descr->period * 1000 / raw_result);
		}
		offset++;
	} while (offset < length);

	return length;
}

/**
 * \brief Retrieve the current driver version
 */
uint32_t freqmeter_sync_get_version(void)
{
	return DRIVER_VERSION;
}
