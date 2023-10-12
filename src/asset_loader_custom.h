/**
 * File:   asset_loader_custom.h
 * Author: AWTK Develop Team
 * Brief:  asset_loader_custom
 *
 * Copyright (c) 2019 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-10-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ASSET_LOADER_CUSTOM_H
#define TK_ASSET_LOADER_CUSTOM_H

#include "base/asset_loader.h"
#include "asset_loader_types_def.h"

BEGIN_C_DECLS

/**
 * @class asset_loader_custom_t
 * @parent asset_loader_t
 * 定制的资源加载器。
 * 把全部资源打包在一个文件中。
 */

/**
 * @method asset_loader_custom_create
 * 创建custom资源加载器。
 * @param {const void*} data custom数据。
 * @param {uint32_t} size custom数据大小。
 *
 * @return {asset_loader_t*} 返回loader对象。
 */
asset_loader_t* asset_loader_custom_create(const void* data, uint32_t size);

/**
 * @method asset_loader_custom_create_with_file
 * 创建custom资源加载器。
 * @param {const char*} filename 文件名。
 *
 * @return {asset_loader_t*} 返回loader对象。
 */
asset_loader_t* asset_loader_custom_create_with_file(const char* filename);

END_C_DECLS

#endif /*TK_ASSET_LOADER_CUSTOM_H*/
