# 硬件综合实践 - 8051 单片机项目

本仓库用于存放“硬件综合实践”课程的相关代码。

## 项目说明

本项目将根据课程进度渐进式地补充和修改代码。
- **历史版本与功能**：请通过 Git 的 `commit history` 和 `tag` 查看不同阶段的功能实现和历史版本。

## 项目结构

- [main.c](main.c) - 主程序文件
- [delay.c](delay.c) / [delay.h](delay.h) - 延时函数实现
- [utils.h](utils.h) - 工具宏定义
- `Objects/` - 编译输出文件
- `Listings/` - 编译列表文件

## 开发环境要求

### 必需软件

1. **Keil uVision 5**
   - 用于编译和调试 8051 单片机程序
   - 下载地址：https://www.keil.com/demo/eval/c51.htm

2. **Visual Studio Code**
   - 代码编辑器

3. **Keil Assistant 插件**
   - 在 VS Code 中集成 Keil 开发环境
   - 安装方法：
     - 打开 VS Code
     - 按 `Ctrl+Shift+X` 打开扩展面板
     - 搜索 "Keil Assistant" 或访问：https://marketplace.visualstudio.com/items?itemName=candycium.keil-assistant-new
     - 点击安装

## 使用方法

1. 安装上述开发环境
2. 使用 VS Code 打开项目文件夹
3. 使用 Keil Assistant 插件关联 Keil 路径并编译项目
4. 将生成的 HEX 文件烧录到 8051 单片机
