# awtk-custom-asset-loader 

演示自定义资源加载器（本项目主要是演示目的，实际使用时，请自己多测试，有问题请反馈）。

本项目中的资源加载器，可以在没有文件系统的情况下，从 norflash 加载资源。

基本工作原理：

* 通过资源打包工具 (assets_pack) 将资源打包成二进制文件。

* 将打包二进制文件烧录到 norflash 中。

* 在代码中定义宏 ASSETS\_CUSTOM\_INIT()，并实现 my\_assets\_init 函数。在该函数中，将资源地址指向 norflash 中资源所在地址。

## 准备

1. 获取 awtk 并编译

```
git clone https://github.com/zlgopen/awtk.git
cd awtk; scons; cd -
```

* 编译 资源打包工具 和 demo

```
scons
```

* 生成资源

```
python3 ./scripts/update_res.py all
```

## 资源打包工具

assets_pack 负责将资源打包成二进制文件。

* 用法

```
./bin/assets_pack <项目根目录> <生成的资源包文件名>
```

**项目根目录** 中必须存在事先生成好的 res 目录。

**生成的资源包文件名** 为二进制文件，用来烧录到 norflash 中。

* 示例（打包当前项目）

```
./bin/assets_pack ./ data/assets.bin
```

* 示例（打包其它项目）

```
./bin/assets_pack ../awtk-ftpd ../awtk-ftpd/assets.bin
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

## 注意

在嵌入式项目中，加入下面的源文件即可：

* src/asset_loader_custom.h
* src/asset_loader_types_def.h
* src/asset_loader_custom.c      

> src 目录中其它文件只是用于演示目的，可根据需要参考。 tools/assets_pack 目录下的文件为资源打包工具，只是在 PC 上使用。