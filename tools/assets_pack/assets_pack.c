/**
 * File:   assets_pack.c
 * Author: AWTK Develop Team
 * Brief:  pack res to a binary file
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
 * 2023-10-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/path.h"
#include "tkc/utils.h"
#include "tkc/darray.h"
#include "tkc/buffer.h"
#include "tkc/asset_info.h"

#include "asset_loader_types_def.h"
#include "../../src/asset_loader_custom.c"

typedef struct asset_info_ex_t {
  asset_info_t* info;
  char theme[TK_NAME_LEN + 1];
} asset_info_ex_t;

static const char* asset_type_to_str(asset_type_t type) {
  switch (type) {
    case ASSET_TYPE_IMAGE:
      return "image";
    case ASSET_TYPE_FONT:
      return "font";
    case ASSET_TYPE_STRINGS:
      return "strings";
    case ASSET_TYPE_UI:
      return "ui";
    case ASSET_TYPE_STYLE:
      return "style";
    case ASSET_TYPE_DATA:
      return "data";
    case ASSET_TYPE_SCRIPT:
      return "script";
    case ASSET_TYPE_XML:
      return "xml";
    default:
      return "unknown";
  }
}

static asset_info_ex_t* asset_info_ex_create(asset_info_t* info, const char* theme) {
  asset_info_ex_t* info_ex = NULL;
  return_value_if_fail(info != NULL && theme != NULL, NULL);

  info_ex = TKMEM_ALLOC(sizeof(asset_info_ex_t));
  return_value_if_fail(info_ex != NULL, NULL);

  memset(info_ex, 0x00, sizeof(asset_info_ex_t));
  info_ex->info = info;
  tk_strncpy(info_ex->theme, theme, TK_NAME_LEN);

  return info_ex;
}

static ret_t asset_info_ex_destroy(asset_info_ex_t* info_ex) {
  return_value_if_fail(info_ex != NULL, RET_BAD_PARAMS);

  asset_info_destroy(info_ex->info);
  TKMEM_FREE(info_ex);

  return RET_OK;
}

static ret_t load_asset_data(const char* theme, const char* path, darray_t* assets,
                           asset_type_t type) {
  fs_item_t item;
  fs_t* fs = os_fs();
  fs_dir_t* dir = NULL;
  char filename[MAX_PATH + 1];
  asset_info_t* info = NULL;
  asset_info_ex_t* info_ex = NULL;
  uint16_t subtype = 0;
  char name[MAX_PATH + 1] = {0};
  uint32_t size = 0;
  char* extname = NULL;
  return_value_if_fail(path != NULL, RET_BAD_PARAMS);

  dir = fs_open_dir(fs, path);
  return_value_if_fail(dir != NULL, RET_BAD_PARAMS);

  while (fs_dir_read(dir, &item) == RET_OK) {
    if (tk_str_eq(item.name, ".") || tk_str_eq(item.name, "..")) {
      continue;
    }

    path_build(filename, MAX_PATH, path, item.name, NULL);
    if (item.is_reg_file) {
      extname = strrchr(item.name, '.');
      if (extname == NULL) {
        continue;
      } else {
        tk_strncpy(name, item.name, extname - item.name);
        extname++;

        if (type == ASSET_TYPE_IMAGE) {
          if (tk_str_ieq(extname, "png")) {
            subtype = ASSET_TYPE_IMAGE_PNG;
          } else if (tk_str_ieq(extname, "jpg")) {
            subtype = ASSET_TYPE_IMAGE_JPG;
          } else if (tk_str_ieq(extname, "webp")) {
            subtype = ASSET_TYPE_IMAGE_WEBP;
          } else if (tk_str_ieq(extname, "gif")) {
            subtype = ASSET_TYPE_IMAGE_GIF;
          } else if (tk_str_ieq(extname, "bmp")) {
            subtype = ASSET_TYPE_IMAGE_BMP;
          } else if (tk_str_ieq(extname, "bsvg")) {
            subtype = ASSET_TYPE_IMAGE_BSVG;
          } else {
            continue;
          }
        } else if (type == ASSET_TYPE_FONT) {
          if (tk_str_ieq(extname, "ttf")) {
            subtype = ASSET_TYPE_FONT_TTF;
          }
        } else if (type == ASSET_TYPE_UI) {
          if (tk_str_ieq(extname, "bin")) {
            subtype = ASSET_TYPE_UI_BIN;
          } else if (tk_str_ieq(extname, "xml")) {
            subtype = ASSET_TYPE_UI_XML;
          } else {
            continue;
          }
        } else if (type == ASSET_TYPE_SCRIPT) {
          if (tk_str_ieq(extname, "js")) {
            subtype = ASSET_TYPE_SCRIPT_JS;
          } else if (tk_str_ieq(extname, "lua")) {
            subtype = ASSET_TYPE_SCRIPT_LUA;
          } else {
            continue;
          }
        } else if (type == ASSET_TYPE_DATA) {
          if (tk_str_ieq(extname, "json")) {
            subtype = ASSET_TYPE_DATA_JSON;
          } else if (tk_str_ieq(extname, "bin")) {
            subtype = ASSET_TYPE_DATA_BIN;
          } else if (tk_str_ieq(extname, "txt")) {
            subtype = ASSET_TYPE_DATA_TEXT;
          } else {
            subtype = ASSET_TYPE_DATA_DAT;
          }
        } else if (type == ASSET_TYPE_STRINGS) {
          subtype = ASSET_TYPE_STRINGS;
        } else if (type == ASSET_TYPE_STYLE) {
          subtype = ASSET_TYPE_STYLE;
        } else if (type == ASSET_TYPE_XML) {
          subtype = ASSET_TYPE_XML;
        } else {
          continue;
        }
      }

      size = file_get_size(filename);
      info = asset_info_create(type, subtype, name, size);
      assert(info != NULL);
      info->flags = ASSET_INFO_FLAG_IN_ROM;
      info_ex = asset_info_ex_create(info, theme);
      assert(info_ex != NULL);
      file_read_part(filename, info->data, size, 0);
      darray_push(assets, info_ex);
    } else if (item.is_dir) {
      log_debug("skip dir:%s\n", filename);
    }
  }
  fs_dir_close(dir);

  return RET_OK;
}

static ret_t assets_pack(darray_t* assets, const char* filename) {
  uint32_t i = 0;
  uint32_t offset = 0;
  asset_entry_t entry;
  uint32_t n = assets->size;
  uint32_t header_size = sizeof(assets_header_t) + n * sizeof(asset_entry_t);
  assets_header_t header = {0};
  FILE* fp = fopen(filename, "wb");
  return_value_if_fail(fp != NULL, RET_BAD_PARAMS);

  header.count = n;
  header.magic = ASSETS_DATA_MAGIC;
  header.version = ASSETS_DATA_VERSION;
  fwrite(&header, sizeof(header), 1, fp);

  offset = header_size;
  for (i = 0; i < n; i++) {
    asset_info_ex_t* iter = (asset_info_ex_t*)darray_get(assets, i);

    memset(&entry, 0x00, sizeof(entry));
    entry.type = iter->info->type;
    entry.subtype = iter->info->subtype;
    entry.flags = iter->info->flags;
    entry.offset = offset;
    entry.size = iter->info->size;
    tk_strncpy(entry.name, asset_info_get_name(iter->info), TK_NAME_LEN);
    tk_snprintf(entry.theme, sizeof(entry.theme), "%s", iter->theme);
    fwrite(&entry, sizeof(entry), 1, fp);

    log_debug("[%u] name=%s type=%d(%s), subtype=%d, flags=%d, offset=%d, size=%d, theme=%s\n", i,
              entry.name, entry.type, asset_type_to_str(entry.type), entry.subtype, entry.flags,
              entry.offset, entry.size, entry.theme);

    offset += sizeof(asset_info_t) + iter->info->size;
  }

  for (i = 0; i < n; i++) {
    asset_info_ex_t* iter = (asset_info_ex_t*)darray_get(assets, i);
    fwrite(iter->info, sizeof(asset_info_t) + iter->info->size, 1, fp);
  }

  fclose(fp);

  return RET_OK;
}

static ret_t load_asset_data_of_theme(const char* theme, const char* image_scale, darray_t* assets) {
  char filename[MAX_PATH + 1] = {0};
  tk_snprintf(filename, sizeof(filename), "res/assets/%s/raw/images/%s", theme, image_scale);
  load_asset_data(theme, filename, assets, ASSET_TYPE_IMAGE);

  tk_snprintf(filename, sizeof(filename), "res/assets/%s/raw/images/%s", theme, "xx");
  if (file_exist(filename)) {
    load_asset_data(theme, filename, assets, ASSET_TYPE_IMAGE);
  }

  tk_snprintf(filename, sizeof(filename), "res/assets/%s/raw/images/%s", theme, "svg");
  if (file_exist(filename)) {
    load_asset_data(theme, filename, assets, ASSET_TYPE_IMAGE);
  }

  tk_snprintf(filename, sizeof(filename), "res/assets/%s/raw/fonts", theme);
  load_asset_data(theme, filename, assets, ASSET_TYPE_FONT);

  tk_snprintf(filename, sizeof(filename), "res/assets/%s/raw/strings", theme);
  load_asset_data(theme, filename, assets, ASSET_TYPE_STRINGS);

  tk_snprintf(filename, sizeof(filename), "res/assets/%s/raw/ui", theme);
  load_asset_data(theme, filename, assets, ASSET_TYPE_UI);

  tk_snprintf(filename, sizeof(filename), "res/assets/%s/raw/styles", theme);
  load_asset_data(theme, filename, assets, ASSET_TYPE_STYLE);

  tk_snprintf(filename, sizeof(filename), "res/assets/%s/raw/data", theme);
  load_asset_data(theme, filename, assets, ASSET_TYPE_DATA);

  tk_snprintf(filename, sizeof(filename), "res/assets/%s/raw/scripts", theme);
  load_asset_data(theme, filename, assets, ASSET_TYPE_SCRIPT);

  tk_snprintf(filename, sizeof(filename), "res/assets/%s/raw/xml", theme);
  load_asset_data(theme, filename, assets, ASSET_TYPE_XML);

  return RET_OK;
}

static ret_t load_all_assets(darray_t* assets, const char* image_scale) {
  fs_t* fs = os_fs();
  fs_dir_t* dir = NULL;
  fs_item_t item;
  return_value_if_fail(fs != NULL && assets != NULL, RET_BAD_PARAMS);

  dir = fs_open_dir(fs, "res/assets");
  return_value_if_fail(dir != NULL, RET_BAD_PARAMS);

  while (fs_dir_read(dir, &item) == RET_OK) {
    if (tk_str_eq(item.name, ".") || tk_str_eq(item.name, "..")) {
      continue;
    }

    if (item.is_dir) {
      load_asset_data_of_theme(item.name, image_scale, assets);
    }
  }
  fs_dir_close(dir);

  return RET_OK;
}

#include "asset_loader_custom.h"

static ret_t verify_assets_data(const char* filename, darray_t* assets) {
  uint32_t i = 0;
  uint32_t n = assets->size;
  asset_loader_t* loader = asset_loader_custom_create_with_file(filename);

  for (i = 0; i < n; i++) {
    asset_info_ex_t* iter = (asset_info_ex_t*)darray_get(assets, i);
    asset_info_t* info = asset_loader_load(loader, iter->info->type, iter->info->subtype, "",
                                           asset_info_get_name(iter->info));
    assert(info != NULL);
    assert(info->size == iter->info->size);
    assert(memcmp(info->data, iter->info->data, info->size) == 0);
    assert(info->flags == iter->info->flags);
    assert(tk_str_eq(asset_info_get_name(info), asset_info_get_name(iter->info)));
  }

  asset_loader_destroy(loader);

  return RET_OK;
}

MAIN() {
  darray_t assets;
  char data_path[MAX_PATH + 1] = {0};
  char filename[MAX_PATH + 1] = {0};
  const char* image_scale = argc > 1 ? argv[1] : "x1";

  darray_init(&assets, 100, (tk_destroy_t)asset_info_ex_destroy, NULL);

  //load_all_assets(&assets, image_scale);

  load_asset_data_of_theme("default", image_scale, &assets);
  path_prepend_app_root(filename,  ASSETS_BIN_FILENAME);
  path_dirname(filename, data_path, sizeof(data_path));
  fs_create_dir_r(os_fs(), data_path);

  assets_pack(&assets, ASSETS_BIN_FILENAME);
  verify_assets_data(ASSETS_BIN_FILENAME, &assets);
  log_debug("output result to %s\n", ASSETS_BIN_FILENAME);
  
  darray_deinit(&assets);
}

END_MAIN(0)
