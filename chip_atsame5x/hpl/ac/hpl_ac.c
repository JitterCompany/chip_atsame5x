

/**
 * \file
 *
 * \brief SAM Analog Comparator
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

#include <hpl_ac_async.h>
#include <hpl_ac_sync.h>
#include <hpl_irq.h>
#include <utils_assert.h>
#include <hpl_ac_config.h>

/**
 * \brief AC configuration type
 */
struct ac_configuration {
	hri_ac_compctrl_reg_t comp_ctrl0;   /*!< Comparator0 Control */
	hri_ac_scaler_reg_t   comp_scaler0; /*!< Comparator 0 Vddana Scaler */
	hri_ac_compctrl_reg_t comp_ctrl1;   /*!< Comparator1 Control */
	hri_ac_scaler_reg_t   comp_scaler1; /*!< Comparator 1 Vddana Scaler */
	hri_ac_evctrl_reg_t   ev_ctrl;      /*!< Event Control Register */
};

/**
 * \brief Array of AC configurations
 */
static struct ac_configuration _ac
    = {(CONF_AC_COMPCTRL0_FLEN << AC_COMPCTRL_FLEN_Pos) | (CONF_AC_COMPCTRL0_HYST << AC_COMPCTRL_HYSTEN_Pos)
           | (CONF_AC_COMPCTRL0_OUT << AC_COMPCTRL_OUT_Pos) | (CONF_AC_COMPCTRL0_SWAP << AC_COMPCTRL_SWAP_Pos)
           | (CONF_AC_COMPCTRL0_MUXPOS << AC_COMPCTRL_MUXPOS_Pos) | (CONF_AC_COMPCTRL0_MUXNEG << AC_COMPCTRL_MUXNEG_Pos)
           | (CONF_AC_COMPCTRL0_INTSEL << AC_COMPCTRL_INTSEL_Pos) | (CONF_AC_COMPCTRL0_SPEED << AC_COMPCTRL_SPEED_Pos)
           | (CONF_AC_COMPCTRL0_SINGLE << AC_COMPCTRL_SINGLE_Pos)
           | (CONF_AC_COMPCTRL0_RUNSTDBY << AC_COMPCTRL_RUNSTDBY_Pos),
       (CONF_AC_SCALER0 << AC_SCALER_VALUE_Pos),
       (CONF_AC_COMPCTRL1_FLEN << AC_COMPCTRL_FLEN_Pos) | (CONF_AC_COMPCTRL1_HYST << AC_COMPCTRL_HYSTEN_Pos)
           | (CONF_AC_COMPCTRL1_OUT << AC_COMPCTRL_OUT_Pos) | (CONF_AC_COMPCTRL1_SWAP << AC_COMPCTRL_SWAP_Pos)
           | (CONF_AC_COMPCTRL1_MUXPOS << AC_COMPCTRL_MUXPOS_Pos) | (CONF_AC_COMPCTRL1_MUXNEG << AC_COMPCTRL_MUXNEG_Pos)
           | (CONF_AC_COMPCTRL1_INTSEL << AC_COMPCTRL_INTSEL_Pos) | (CONF_AC_COMPCTRL1_SPEED << AC_COMPCTRL_SPEED_Pos)
           | (CONF_AC_COMPCTRL1_SINGLE << AC_COMPCTRL_SINGLE_Pos)
           | (CONF_AC_COMPCTRL1_RUNSTDBY << AC_COMPCTRL_RUNSTDBY_Pos),
       (CONF_AC_SCALER1 << AC_SCALER_VALUE_Pos),
       (CONF_AC_COMPEI0 << AC_EVCTRL_COMPEI0_Pos) | (CONF_AC_COMPEO0 << AC_EVCTRL_COMPEO0_Pos)
           | (CONF_AC_INVEI0 << AC_EVCTRL_INVEI0_Pos) | (CONF_AC_COMPEI1 << AC_EVCTRL_COMPEI1_Pos)
           | (CONF_AC_COMPEO1 << AC_EVCTRL_COMPEO1_Pos) | (CONF_AC_INVEI1 << AC_EVCTRL_INVEI1_Pos)
           | (CONF_AC_WINEO0 << AC_EVCTRL_WINEO0)};

/*!< Pointer to hpl device */
static struct _ac_async_device *_ac_dev = NULL;

/** \cond INTERNAL */
static int32_t     _ac_init(void *const hw);
static inline void _ac_deinit(void *const hw);
static inline void _ac_start_comparison(void *const hw, const uint8_t comp);
/** \endcond */

/**
 * \brief Initialize synchronous AC
 */
int32_t _ac_sync_init(struct _ac_sync_device *const device, void *const hw)
{
	ASSERT(device);

	device->hw = hw;

	return _ac_init(hw);
}

/**
 * \brief Initialize Asynchronous AC
 */
int32_t _ac_async_init(struct _ac_async_device *const device, void *const hw)
{
	int32_t init_status;

	ASSERT(device);

	init_status = _ac_init(hw);
	if (init_status) {
		return init_status;
	}
	device->hw = hw;
	_ac_dev    = device;
	hri_ac_set_INTEN_COMP0_bit(hw);

	hri_ac_set_INTEN_COMP1_bit(hw);

	hri_ac_set_INTEN_WIN0_bit(hw);

	NVIC_DisableIRQ(AC_IRQn);
	NVIC_ClearPendingIRQ(AC_IRQn);
	NVIC_EnableIRQ(AC_IRQn);

	return ERR_NONE;
}

/**
 * \brief De-initialize AC
 */
void _ac_async_deinit(struct _ac_async_device *const device)
{
	NVIC_DisableIRQ(AC_IRQn);

	_ac_deinit(&device->hw);
}

/**
 * \brief De-initialize AC
 */
void _ac_sync_deinit(struct _ac_sync_device *const device)
{
	_ac_deinit(&device->hw);
}

/**
 * \brief Enable AC
 *
 */
void _ac_sync_enable(struct _ac_sync_device *const device)
{
	hri_ac_set_CTRLA_ENABLE_bit(device->hw);
}

/**
 * \brief Enable AC
 *
 */
void _ac_async_enable(struct _ac_async_device *const device)
{
	hri_ac_set_CTRLA_ENABLE_bit(device->hw);
}

/**
 * \brief Disable AC
 *
 */
void _ac_sync_disable(struct _ac_sync_device *const device)
{
	hri_ac_clear_CTRLA_ENABLE_bit(device->hw);
}

/**
 * \brief Disable AC
 *
 */
void _ac_async_disable(struct _ac_async_device *const device)
{
	hri_ac_clear_CTRLA_ENABLE_bit(device->hw);
}

/**
 * \brief Start AC comparison
 */
void _ac_sync_start_comparison(struct _ac_sync_device *const device, const uint8_t comp)
{
	_ac_start_comparison(device->hw, comp);
}

/**
 * \brief Start AC comparison
 */
void _ac_async_start_comparison(struct _ac_async_device *const device, const uint8_t comp)
{
	_ac_start_comparison(device->hw, comp);
}

/**
 * \brief Stop AC Comparison
 */
void _ac_sync_stop_comparison(struct _ac_sync_device *const device, const uint8_t comp)
{
	hri_ac_clear_COMPCTRL_ENABLE_bit(device->hw, comp);
}

/**
 * \brief Stop AC Comparison
 */
void _ac_async_stop_comparison(struct _ac_async_device *const device, const uint8_t comp)
{
	hri_ac_clear_COMPCTRL_ENABLE_bit(device->hw, comp);
}

/**
 * \brief write synchronous AC data for output
 */
int32_t _ac_sync_get_result(struct _ac_sync_device *const device, const uint8_t comp)
{
	switch (comp) {
	case 0:
		if (!hri_ac_get_STATUSB_READY0_bit(device->hw)) {
			return ERR_NOT_READY;
		}
		return hri_ac_get_STATUSA_STATE0_bit(device->hw);
	case 1:
		if (!hri_ac_get_STATUSB_READY1_bit(device->hw)) {
			return ERR_NOT_READY;
		}
		return hri_ac_get_STATUSA_STATE1_bit(device->hw);
	default:
		break;
	}
	return ERR_NOT_READY;
}

/**
 * \brief Initialize AC
 *
 * \param[in] hw The pointer to hardware instance
 */
static int32_t _ac_init(void *const hw)
{
	ASSERT(hw == AC);

	if (!hri_ac_is_syncing(hw, AC_SYNCBUSY_SWRST)) {
		if (hri_ac_get_CTRLA_reg(hw, AC_CTRLA_ENABLE)) {
			hri_ac_clear_CTRLA_ENABLE_bit(hw);
			hri_ac_wait_for_sync(hw, AC_SYNCBUSY_ENABLE);
		}
		hri_ac_write_CTRLA_reg(hw, AC_CTRLA_SWRST);
	}
	hri_ac_wait_for_sync(hw, AC_SYNCBUSY_SWRST);
	hri_ac_write_COMPCTRL_reg(hw, 0, _ac.comp_ctrl0);
	hri_ac_write_SCALER_reg(hw, 0, _ac.comp_scaler0);
	hri_ac_write_COMPCTRL_reg(hw, 1, _ac.comp_ctrl1);
	hri_ac_write_SCALER_reg(hw, 1, _ac.comp_scaler1);
	hri_ac_write_EVCTRL_reg(hw, _ac.ev_ctrl);

	return ERR_NONE;
}

/**
 * \brief De-initialize AC
 *
 * \param[in] hw The pointer to hardware instance
 */
static inline void _ac_deinit(void *const hw)
{
	hri_ac_clear_CTRLA_ENABLE_bit(hw);
	hri_ac_set_CTRLA_SWRST_bit(hw);
}

/**
 * \brief Start AC comparison
 *
 * \param[in] hw The pointer to hardware instance
 */
static inline void _ac_start_comparison(void *const hw, const uint8_t comp)
{
	hri_ac_set_COMPCTRL_ENABLE_bit(hw, comp);
	if (hri_ac_get_COMPCTRL_SINGLE_bit(hw, comp) == false) {
		return;
	}
	switch (comp) {
	case 0:
		hri_ac_write_CTRLB_reg(hw, AC_CTRLB_START0);
		break;
	case 1:
		hri_ac_write_CTRLB_reg(hw, AC_CTRLB_START1);
		break;
	default:
		break;
	}
}
