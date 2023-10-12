/**
 * File:   res_data_types_def.h
 * Author: AWTK Develop Team
 * Brief:  res_data_types_def
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2023-07-27 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#ifndef TK_ASSET_DATA_TYPES_DEF_H
#define TK_ASSET_DATA_TYPES_DEF_H

#include "tkc/value.h"
#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class asset_entry_t
 * @annotation ["scriptable"]
 * 资源数据条目。
 */
typedef struct _asset_entry_t {
  /** 
   * @property {uint16_t} type
   * @annotation ["readable","scriptable"]
   * 类型。
   */
  uint16_t type;
  /** 
   * @property {uint8_t} subtype
   * @annotation ["readable", "scriptable"]
   * 子类型。
   */
  uint8_t subtype;
  /** 
   * @property {uint8_t} flags
   * @annotation ["readable", "scriptable"]
   * 资源标志。
   */
  uint8_t flags;
  /** 
   * @property {uint32_t} offset
   * @annotation ["readable", "scriptable"]
   * 偏移。
   */
  uint32_t offset;
  /** 
   * @property {uint32_t} size
   * @annotation ["readable", "scriptable"]
   * 大小。
   */
  uint32_t size;
  /** 
   * @property {char*} theme
   * @annotation ["readable", "scriptable"]
   * 主题。
   */
  char theme[20];
  /** 
     * @property {char*} name
     * @annotation ["readable", "scriptable"]
     * 名称。
     */
  char name[TK_NAME_LEN + 1];
} asset_entry_t;

/**
 * @class asset_data_t
 * @annotation ["scriptable"]
 * 资源数据。
 */
typedef struct _assets_header_t {
  /** 
   * @property {uint32_t} magic
   * @annotation ["readable", "scriptable"]
   * 魔术字。
   */
  uint32_t magic;
  /** 
   * @property {uint16_t} version
   * @annotation ["readable", "scriptable"]
   * 版本。
   */
  uint16_t version;
  /** 
   * @property {uint16_t} count
   * @annotation ["readable", "scriptable"]
   * 条目数。
   */
  uint16_t count;
} assets_header_t;

#define ASSETS_DATA_MAGIC 0x12345678
#define ASSETS_DATA_VERSION 1

#define ASSETS_BIN_FILENAME "data/assets.bin"

END_C_DECLS

#endif /*TK_ASSET_DATA_TYPES_DEF_H*/
