/**
 * \file
 *
 * \brief AC functionality implementation.
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

#include "hal_ac_sync.h"
#include <utils_assert.h>
#include <utils.h>
#include <hal_atomic.h>

/**
 * \brief Driver version
 */
#define DRIVER_VERSION 0x00000001u

/**
 * \brief Initialize the AC HAL instance and hardware.
 */
int32_t ac_sync_init(struct ac_sync_descriptor *const descr, void *const hw)
{
	ASSERT(descr && hw);

	return _ac_sync_init(&descr->device, hw);
}

/**
 * \brief Deinitialize the AC HAL instance and hardware.
 */
int32_t ac_sync_deinit(struct ac_sync_descriptor *const descr)
{
	ASSERT(descr);

	_ac_sync_deinit(&descr->device);

	return ERR_NONE;
}

/**
 * \brief Enable AC
 *
 */
int32_t ac_sync_enable(struct ac_sync_descriptor *const descr)
{
	ASSERT(descr);

	_ac_sync_enable(&descr->device);

	return ERR_NONE;
}

/**
 * \brief Disable AC
 *
 */
int32_t ac_sync_disable(struct ac_sync_descriptor *const descr)
{
	ASSERT(descr);

	_ac_sync_disable(&descr->device);

	return ERR_NONE;
}

/*
 * \brief Read Comparison result.
 *
 */
int32_t ac_sync_get_result(struct ac_sync_descriptor *const descr, const uint8_t comp)
{
	ASSERT(descr);

	return _ac_sync_get_result(&descr->device, comp);
}

/**
 * \brief Start comparison
 *
 */
int32_t ac_sync_start_comparison(struct ac_sync_descriptor *const descr, uint8_t comp)
{
	ASSERT(descr);

	_ac_sync_start_comparison(&descr->device, comp);

	return ERR_NONE;
}

/**
 * \brief Stop comparison
 *
 */
int32_t ac_sync_stop_comparison(struct ac_sync_descriptor *const descr, uint8_t comp)
{
	ASSERT(descr);

	_ac_sync_stop_comparison(&descr->device, comp);

	return ERR_NONE;
}

/**
 * \brief Get AC driver version
 */
uint32_t ac_sync_get_version(void)
{
	return DRIVER_VERSION;
}

//@}
