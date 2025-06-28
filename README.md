
# 🧰 GGToolBox

> 一个基于 [**raylib**](https://www.raylib.com/) 的桌面级工具箱，为信息学爱好者（OIer）提供简洁高效的多功能支持平台。

---

## 🌟 项目简介

**GGToolBox** 是一款 C++ 开发的桌面应用，集成多个功能模块，支持导航、签到、Minecraft 启动、脚本市场等实用功能，目标是通过模块化的设计，为 OIer 提供一站式工具服务。

---

## 🔧 功能模块

### 🏠 主页导航

快速访问常用资源网站，覆盖 OI、游戏与技术等领域：

* **常用网站**：
  [BiliBili](https://www.bilibili.com/)｜[博客园](https://www.cnblogs.com/)｜[深度求知](https://chat.deepseek.com/)

* **信息学资源**：
  [洛谷](https://www.luogu.com.cn/)｜[OI Wiki](https://OI.wiki/)｜[Vjudge](https://vjudge.net/)

* **Minecraft 社区**：
  [MCMod 百科](https://www.mcmod.cn/)｜[Modrinth](https://modrinth.com/)｜[Minecraft Wiki](https://zh.minecraft.wiki/)

* **小游戏推荐**：
  [Florr.io](https://florr.io/)｜[Florr Wiki](https://florrio.fandom.com/zh/wiki/)｜[DigDig.io](https://digdig.io/)

---

### 📅 每日签到

* 显示当前日期与星期。
* 一键签到，随机展示今日运势（如：大吉、中吉、凶等）。
* 自动记录签到历史。

---

### 🟩 Minecraft 启动器（RMCL）

* 启动 Minecraft 主程序。
* 查看并管理游戏进程。
* 支持启动日志与状态显示。
* 提供停止功能和自动清理残留进程。

---

### 🧩 脚本市场（GSML）

* 自动下载并加载脚本插件。
* 支持离线缓存与启动。
* 插件采用统一目录结构，支持脚本和自定义资源。

插件结构示例：

```
插件名/
├── start.cmd     # 启动脚本
├── id.txt        # 插件信息
└── ...           # 其他资源
```

---

## 🖥️ 安装与构建指南

### ✅ 环境要求

* Windows 系统
* 支持 C++20 的编译器（推荐 MinGW）
* 必需依赖：

  * [`raylib`](https://www.raylib.com/)
  * OpenGL
  * Windows 系统库（`gdi32`, `winmm` 等）

### 📦 构建步骤

```bash
# 克隆项目
git clone https://github.com/yourusername/ggtoolbox.git
cd ggtoolbox

# 构建
mkdir build
cd build
cmake ..
cmake --build .

# 运行
./ggtoolbox.exe
```

---

## 📁 项目结构

```
.
├── CMakeLists.txt        # CMake 构建配置
├── main.cpp              # 主程序入口
├── nbs.h                 # 自定义绘图组件
├── nbsapi.h              # 工具函数接口
├── src/                  # 图形资源
│   ├── home.png
│   ├── mcv.png
│   ├── logo.png
│   └── app/              # 图标资源
│       ├── qh.png
│       ├── qj.png
│       ├── ...
├── data/
│   └── data.txt          # 签到记录
└── mcf/                  # Minecraft 启动器相关
    ├── install.bat
    ├── cmcl.exe
    └── nbsmc.mrpack
```

---

## ⚙️ 使用说明

### 🔀 快捷键操作

* `Home`：返回主页
* `1`：主页导航
* `2`：Minecraft 启动器
* `3`：脚本市场

### ✅ 签到操作

1. 打开应用后进入主页。
2. 点击“签到”完成操作。
3. 查看当日运势结果。

### 🟢 Minecraft 启动

1. 切换至 RMCL 模块。
2. 点击“开始游戏”以启动。
3. 游戏运行中可选择“停止”进行清理。

### 📦 脚本市场使用

* 启动应用后会自动尝试拉取远程插件仓库。
* 插件均支持使用 `start.cmd` 启动。
* 插件格式统一，便于扩展与集成。

---

## 🤝 贡献方式

我们欢迎任何贡献者参与优化：

```bash
# Fork 项目并创建新分支
git checkout -b feature/my-feature

# 提交更改
git commit -m "新增功能/修复 bug"

# 推送并创建 PR
git push origin feature/my-feature
```

---

## 📄 许可证

本项目遵循 [MIT License](./LICENSE)。

---

## 📬 联系方式

* 💬 洛谷主页：[https://www.luogu.com.cn/user/661094](https://www.luogu.com.cn/user/661094)
* 🐛 提交问题：[GitHub Issues](https://github.com/nxtblock/ggtoolbox/issues)

---

> GGToolBox —— 一站式信息学辅助工具，为热爱代码的你服务 💻✨
