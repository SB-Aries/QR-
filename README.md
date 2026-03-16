# QRS 隐写术工具

## 项目简介

QRS 是一个基于 QR 码的文件隐写术工具，用于将shellcode编码为一系列 QR 码图像，然后通过解码这些 QR 码图像来恢复原始shellcode。

## 核心功能

### 编码功能
- 读取输入文件并分块
- 封装数据为二进制数据包
- 生成优化的 QR 码
- 保存为 8bpp 灰度 PNG 图像

### 解码功能
- 扫描目录中的 QR 码文件
- 解码 QR 码获取数据包
- 重组数据块恢复原始文件

## 技术实现

- **开发语言**：纯 C++17
- **依赖库**：
  - ZXing C++（QR 编解码）
  - OpenCV（图像处理）
  - Windows WIC API（PNG 渲染）
- **输出格式**：8bpp 灰度 PNG，每个模块 8x8 像素，带 4 个模块静区

## 命令行使用

### 编码
```bash
QRGenerate.exe encode <input_file> <output_dir>
```

### 解码
```bash
QRGenerate.exe decode <input_dir>
```

## 项目结构

- `src/`：源代码目录
  - `main.cpp`：程序入口
  - `encoder.cpp`：编码功能实现
  - `decoder.cpp`：解码功能实现
  - `qrutils.cpp`：QR 码工具函数
  - `fileutils.cpp`：文件操作工具函数
  - `imageutils.cpp`：图像处理工具函数

- `include/`：头文件目录
  - `encoder.h`：编码类声明
  - `decoder.h`：解码类声明
  - `qrutils.h`：QR 码工具函数声明
  - `fileutils.h`：文件操作工具函数声明
  - `imageutils.h`：图像处理工具函数声明

- `COMPILATION.md`：编译说明文档

## 注意事项

- 确保输入文件大小不超过 100MB
- 生成的 QR 码文件命名为 Qr0000.png、Qr0001.png 等
- 解码时会自动识别目录中的 Qr*.png 文件
- 解码后会在当前目录生成恢复的原始文件
