# awtk-custom-asset-loader 

演示自定义资源加载器。

本项目中的资源加载器，可以在没有文件系统的情况下，从 norflash 加载资源。

## 准备

1. 获取 awtk 并编译

```
git clone https://github.com/zlgopen/awtk.git
cd awtk; scons; cd -
```

2. 获取 awtk-hello 并编译

```
git clone https://github.com/zlgopen/awtk-hello.git
cd awtk-hello
```

* 生成资源

```
python3 ./scripts/update_res.py all
./bin/assets_pack 
```

> 生成的资源包为：data/assets.bin

* 编译 PC 版本

```
scons
```

* 编译 LINUX FB 版本

```
scons LINUX_FB=true
```

> 完整编译选项请参考 [编译选项](https://github.com/zlgopen/awtk-widget-generator/blob/master/docs/build_options.md)

## 运行

```
./bin/demo
```

## 使用方法

在 main.c 中定义宏 ASSETS\_CUSTOM\_INIT()，并实现 my\_assets\_init 函数，如下所示：

> ASSETS_BIN_ADDR 为资源包的起始地址，ASSETS_BIN_SIZE 为资源包的大小。

```c
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
```
