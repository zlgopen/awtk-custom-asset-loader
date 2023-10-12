/**
 * File:   asset_loader_custom.c
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
 * 2023-11-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/fs.h"
#include "tkc/mem.h"

#include "asset_loader_custom.h"
#include "base/assets_manager.h"
#include "asset_loader_types_def.h"

typedef struct _asset_loader_custom_t {
  asset_loader_t asset_loader;
  const void* data;
  uint32_t size;

  bool_t own_the_data;
} asset_loader_custom_t;

static bool_t asset_loader_custom_exist(asset_loader_t* loader, const char* path) {
  return TRUE;
}

static asset_info_t* asset_loader_custom_load(asset_loader_t* loader, uint16_t type,
                                              uint16_t subtype, const char* path,
                                              const char* name) {
  uint32_t i = 0;
  asset_entry_t* entries = NULL;
  assets_header_t* header = NULL;
  asset_loader_custom_t* custom = (asset_loader_custom_t*)loader;
  return_value_if_fail(custom != NULL && custom->data != NULL, NULL);
  header = (assets_header_t*)custom->data;
  return_value_if_fail(header->magic == ASSETS_DATA_MAGIC, NULL);

  entries = (asset_entry_t*)(header + 1);

  for (i = 0; i < header->count; i++) {
    asset_entry_t* iter = entries + i;
    if (iter->type == type && iter->subtype == subtype && tk_str_eq(iter->name, name)) {
      asset_info_t* info = (asset_info_t*)((uint8_t*)custom->data + iter->offset);
      return info;
    }
  }

  for (i = 0; i < header->count; i++) {
    asset_entry_t* iter = entries + i;
    if (iter->type == type && tk_str_eq(iter->name, name)) {
      asset_info_t* info = (asset_info_t*)((uint8_t*)custom->data + iter->offset);
      return info;
    }
  }

  log_debug("not found assets %s\n", name);
  return NULL;
}

static ret_t asset_loader_custom_destroy(asset_loader_t* loader) {
  asset_loader_custom_t* custom = (asset_loader_custom_t*)loader;

  if (custom->own_the_data) {
    TKMEM_FREE(custom->data);
  }

  TKMEM_FREE(loader);

  return RET_OK;
}

static const asset_loader_vtable_t s_asset_loader_custom_vtable = {
    .load = asset_loader_custom_load,
    .exist = asset_loader_custom_exist,
    .destroy = asset_loader_custom_destroy};

static asset_loader_custom_t* asset_loader_custom_create_default(void) {
  asset_loader_custom_t* custom = TKMEM_ZALLOC(asset_loader_custom_t);
  return_value_if_fail(custom != NULL, NULL);

  custom->asset_loader.vt = &s_asset_loader_custom_vtable;

  return custom;
}

asset_loader_t* asset_loader_custom_create_with_file(const char* filename) {
  uint32_t size = 0;
  void* data = NULL;
  asset_loader_custom_t* custom = NULL;
  return_value_if_fail(filename != NULL, NULL);

  data = file_read(filename, &size);
  if (data != NULL) {
    custom = (asset_loader_custom_t*)asset_loader_custom_create(data, size);
    if (custom != NULL) {
      custom->own_the_data = TRUE;
    } else {
      TKMEM_FREE(data);
    }
  }

  return (asset_loader_t*)custom;
}

asset_loader_t* asset_loader_custom_create(const void* data, uint32_t size) {
  asset_loader_custom_t* custom = NULL;
  return_value_if_fail(data != NULL && size > 0, NULL);

  custom = asset_loader_custom_create_default();
  return_value_if_fail(custom != NULL, NULL);

  custom->data = data;
  custom->size = size;

  return (asset_loader_t*)custom;
}
