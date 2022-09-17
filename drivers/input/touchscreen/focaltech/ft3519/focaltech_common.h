/*
 *
 * FocalTech fts TouchScreen driver.
 *
 * Copyright (c) 2012-2020, Focaltech Ltd. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
/*****************************************************************************
*
* File Name: focaltech_common.h
*
* Author: Focaltech Driver Team
*
* Created: 2016-08-16
*
* Abstract:
*
* Reference:
*
*****************************************************************************/

#ifndef __LINUX_FOCALTECH_COMMON_H__
#define __LINUX_FOCALTECH_COMMON_H__

#include "focaltech_config.h"
#if IS_ENABLED(CONFIG_INPUT_SEC_INPUT)
#include "../../../sec_input/sec_input.h"
#endif

#if IS_ENABLED(CONFIG_SPU_VERIFY)
#include <linux/spu-verify.h>
#define SUPPORT_SIGNED_FW
#endif


/*****************************************************************************
* Macro definitions using #define
*****************************************************************************/
#define FTS_DRIVER_VERSION                  "Focaltech V3.3.5 20210514"

#define BYTE_OFF_0(x)           (u8)((x) & 0xFF)
#define BYTE_OFF_8(x)           (u8)(((x) >> 8) & 0xFF)
#define BYTE_OFF_16(x)          (u8)(((x) >> 16) & 0xFF)
#define BYTE_OFF_24(x)          (u8)(((x) >> 24) & 0xFF)
#define FLAGBIT(x)              (0x00000001 << (x))
#define FLAGBITS(x, y)          ((0xFFFFFFFF >> (32 - (y) - 1)) & (0xFFFFFFFF << (x)))

#define FLAG_ICSERIALS_LEN      8
#define FLAG_HID_BIT            10
#define FLAG_IDC_BIT            11

#define IC_SERIALS              (FTS_CHIP_TYPE & FLAGBITS(0, FLAG_ICSERIALS_LEN-1))
#define IC_TO_SERIALS(x)        ((x) & FLAGBITS(0, FLAG_ICSERIALS_LEN-1))
#define FTS_CHIP_IDC            ((FTS_CHIP_TYPE & FLAGBIT(FLAG_IDC_BIT)) == FLAGBIT(FLAG_IDC_BIT))
#define FTS_HID_SUPPORTTED      ((FTS_CHIP_TYPE & FLAGBIT(FLAG_HID_BIT)) == FLAGBIT(FLAG_HID_BIT))

#define FTS_MAX_CHIP_IDS        8

#define FTS_CHIP_TYPE_MAPPING {{0x89, 0x54, 0x52, 0x54, 0x52, 0x54, 0x5B, 0x54, 0x5E}}

#define BYTES_PER_TIME						(32)

#define FILE_NAME_LENGTH                    128
#define ENABLE                              1
#define DISABLE                             0
#define VALID                               1
#define INVALID                             0
#define FTS_CMD_START1                      0x55
#define FTS_CMD_START2                      0xAA
#define FTS_CMD_START_DELAY                 12
#define FTS_CMD_READ_ID                     0x90
#define FTS_CMD_READ_ID_LEN                 4
#define FTS_CMD_READ_ID_LEN_INCELL          1
#define FTS_CMD_READ_FW_CONF                0xA8

#define FTS_SYSFS_ECHO_ON(buf)      (buf[0] == '1')
#define FTS_SYSFS_ECHO_OFF(buf)     (buf[0] == '0')

#define kfree_safe(pbuf) do {\
    if (pbuf) {\
        kfree(pbuf);\
        pbuf = NULL;\
    }\
} while(0)

/*****************************************************************************
*  Alternative mode (When something goes wrong, the modules may be able to solve the problem.)
*****************************************************************************/
/*
 * point report check
 * default: disable
 */
#define FTS_POINT_REPORT_CHECK_EN               0

/*****************************************************************************
* Global variable or extern global variabls/functions
*****************************************************************************/
struct ft_chip_t {
	u16 type;
	u8 chip_idh;
	u8 chip_idl;
	u8 rom_idh;
	u8 rom_idl;
	u8 pb_idh;
	u8 pb_idl;
	u8 bl_idh;
	u8 bl_idl;
};

struct ft_chip_id_t {
	u16 type;
	u16 chip_ids[FTS_MAX_CHIP_IDS];
};

struct ts_ic_info {
	bool is_incell;
	bool hid_supported;
	struct ft_chip_t ids;
	struct ft_chip_id_t cid;
};

enum {
	LCD_EARLY_EVENT = 0,
	LCD_LATE_EVENT
};

enum {
	SERVICE_SHUTDOWN = -1,
	LCD_NONE = 0,
	LCD_OFF,
	LCD_ON,
	LCD_DOZE1,
	LCD_DOZE2,
	LPM_OFF = 20,
	FORCE_OFF,
	FORCE_ON,
};

/*****************************************************************************
* DEBUG function define here
*****************************************************************************/
#if FTS_DEBUG_EN
#if IS_ENABLED(CONFIG_INPUT_SEC_INPUT)
#define FTS_DEBUG(fmt, args...) do { \
	input_info(true, fts_data ? fts_data->dev : NULL, "[FTS_TS]%s: "fmt"\n", __func__, ##args); \
} while (0)

#define FTS_FUNC_ENTER() do { \
	input_info(true, fts_data ? fts_data->dev : NULL, "[FTS_TS]%s: Enter\n", __func__); \
} while (0)

#define FTS_FUNC_EXIT() do { \
	input_info(true, fts_data ? fts_data->dev : NULL, "[FTS_TS]%s: Exit(%d)\n", __func__, __LINE__); \
} while (0)
#else /* IS_ENABLED(CONFIG_INPUT_SEC_INPUT) */
#define FTS_DEBUG(fmt, args...) do { \
	printk("[FTS_TS]%s:"fmt"\n", __func__, ##args); \
} while (0)

#define FTS_FUNC_ENTER() do { \
	printk("[FTS_TS]%s: Enter\n", __func__); \
} while (0)

#define FTS_FUNC_EXIT() do { \
	printk("[FTS_TS]%s: Exit(%d)\n", __func__, __LINE__); \
} while (0)
#endif
#else /* #if FTS_DEBUG_EN*/
#define FTS_DEBUG(fmt, args...)
#define FTS_FUNC_ENTER()
#define FTS_FUNC_EXIT()
#endif

#if IS_ENABLED(CONFIG_INPUT_SEC_INPUT)
#define FTS_INFO(fmt, args...) do { \
	input_info(true, fts_data ? fts_data->dev : NULL, "[FTS_TS/I]%s: "fmt"\n", __func__, ##args); \
} while (0)
#define FTS_RAW_INFO(fmt, args...) do { \
	input_raw_info(true, fts_data ? fts_data->dev : NULL, "[FTS_TS] "fmt"\n", ##args); \
} while (0)
#define FTS_ERROR(fmt, args...) do { \
	input_err(true, fts_data ? fts_data->dev : NULL, "[FTS_TS/E]%s: "fmt"\n", __func__, ##args); \
} while (0)
#else /* IS_ENABLED(CONFIG_INPUT_SEC_INPUT) */
#define FTS_INFO(fmt, args...) do { \
	printk(KERN_INFO "[FTS_TS/I]%s:"fmt"\n", __func__, ##args); \
} while (0)
#define FTS_RAW_INFO(fmt, args...) do { \
	printk(KERN_INFO "[FTS_TS]%s:"fmt"\n", __func__, ##args); \
} while (0)
#define FTS_ERROR(fmt, args...) do { \
	printk(KERN_ERR "[FTS_TS/E]%s:"fmt"\n", __func__, ##args); \
} while (0)
#endif
#endif /* __LINUX_FOCALTECH_COMMON_H__ */
