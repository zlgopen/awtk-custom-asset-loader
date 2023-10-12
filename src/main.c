/**
 * File:   main.c
 * Author: AWTK Develop Team
 * Brief:  main window sources
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-08-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "window1.h"
#include "window2.h"
#include "settings.h"

#include "../res/assets.inc"

static ret_t on_open_window(void* ctx, event_t* e) {
  const char* name = (const char*)ctx;
  if (tk_str_eq(name, "settings")) {
    settings_open();
  } else if (tk_str_eq(name, "window1")) {
    window1_open();
  } else if (tk_str_eq(name, "window2")) {
    window2_open();
  }

  return RET_OK;
}

#include "asset_loader_custom.h"
#define ASSETS_CUSTOM_INIT() my_assets_init()

static ret_t my_assets_init(void) {
  asset_loader_t* loader = NULL;
  assets_manager_set_res_root(assets_manager(), "data");
#if defined(WIN32) || defined(MACOS) || defined(LINUX)
  loader = asset_loader_custom_create_with_file(ASSETS_BIN_FILENAME);
#else
/*对于嵌入式系统：请将下面的宏改成实际的地址和大小*/
#define ASSETS_BIN_ADDR 0x0000
#define ASSETS_BIN_SIZE 0x0000

  loader = asset_loader_custom_create(ASSETS_BIN_ADDR, ASSETS_BIN_SIZE);
#endif
  assets_manager_set_loader(assets_manager(), loader);
  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_open("main");

  widget_child_on(win, "settings", EVT_CLICK, on_open_window, "settings");
  widget_child_on(win, "vpn", EVT_CLICK, on_open_window, "window1");
  widget_child_on(win, "dns", EVT_CLICK, on_open_window, "window2");
  widget_child_on(win, "ip", EVT_CLICK, on_open_window, "window1");

  return RET_OK;
}

ret_t application_exit() {
  return RET_OK;
}

#include "awtk_main.inc"
