
# GGToolBox

**GGToolBox** 是一个基于 `raylib` 的桌面工具箱应用，提供多种功能模块，包括主页导航、Minecraft 启动器（RMCL）、脚本市场（GSML）等。该项目旨在通过简洁的界面和高效的功能，为 OIer 提供便捷的工具体验。

---

## 功能概览

### 1. **主页导航**
- 快速访问常用网站：
  - [BiliBili](https://www.bilibili.com/)
  - [博客园](https://www.cnblogs.com/)
  - [深度求知](https://chat.deepseek.com/)
- 信息学奥赛相关资源：
  - [洛谷](https://www.luogu.com.cn/)
  - [OI Wiki](https://OI.wiki/)
  - [Vjudge](https://vjudge.net/)
- Minecraft 相关资源：
  - [Mod 百科](https://www.mcmod.cn/)
  - [MC 资源](https://modrinth.com/)
  - [MC Wiki](https://zh.minecraft.wiki/)
- Florr.io 游戏相关资源：
  - [Florr.io](https://florr.io/)
  - [Florr.io Wiki](https://florrio.fandom.com/zh/wiki/)
  - [DigDig](https://digdig.io/)

### 2. **每日签到**
- 显示当前日期、星期等信息。
- 提供每日签到功能，记录签到状态并显示随机运势（如“大吉”、“中平”、“大凶”）。

### 3. **Minecraft 启动器（RMCL）**
- 支持启动 Minecraft 游戏。
- 提供配置功能。
- 显示启动状态和错误信息。
- 支持停止游戏并清理相关进程。

### 4. **脚本市场（GSML）**
- 自动从远程仓库下载脚本市场资源并解压。
- 提供脚本市场的基本界面。

---

## 安装与运行

### 环境要求
- **操作系统**：Windows
- **编译器**：支持 C++20 的编译器（如 MinGW）
- **依赖库**：
  - [raylib](https://www.raylib.com/)
  - OpenGL
  - Windows 系统库（如 `gdi32`, `winmm`）

### 构建步骤
1. 克隆项目到本地：
   ```bash
   git clone https://github.com/yourusername/ggtoolbox.git
   cd ggtoolbox
   ```

2. 安装依赖：
   - 确保已安装 `raylib` 并将其包含路径和库路径配置到编译器中。

3. 使用 CMake 构建项目：
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

4. 运行可执行文件：
   ```bash
   ./ggtoolbox.exe
   ```

---

## 文件结构

```
.
├── CMakeLists.txt       # CMake 构建配置文件
├── main.cpp             # 主程序入口
├── nbs.h                # 自定义绘图组件
├── nbsapi.h             # 工具函数和 API
├── src/                 # 资源文件（图片、字体等）
│   ├── DouyinSansBold.otf
│   ├── home.png
│   ├── logo.png
│   ├── mcv.png
│   └── app/             # 应用图标
│       ├── qh.png
│       ├── qj.png
│       ├── qm.png
│       ├── sh.png
│       ├── sj.png
│       └── sm.png
├── data/                # 数据文件
│   └── data.txt         # 签到记录
└── mcf/                 # Minecraft 启动器相关文件
    ├── install.bat
    ├── cmcl.exe
    └── nbsmc.mrpack
```

---

## 使用说明

### 快捷键
- `Home`：返回主页。
- `1`：切换到主页模块。
- `2`：切换到 Minecraft 启动器模块。
- `3`：切换到脚本市场模块。

### 签到功能
1. 打开应用后，进入主页模块。
2. 点击“签到”按钮完成每日签到。
3. 签到后将显示当天的运势。

### Minecraft 启动器
1. 切换到 RMCL 模块。
2. 点击“开始游戏”按钮启动 Minecraft。
3. 若需要停止游戏，可点击“停止游戏”按钮。

### 脚本市场
1. 应用启动时会尝试下载脚本市场资源。
2. 若下载失败，将显示错误提示。

---

## 贡献指南

欢迎对本项目进行贡献！以下是参与方式：
1. Fork 本仓库。
2. 创建新分支：
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. 提交更改并推送到你的分支：
   ```bash
   git commit -m "Add your message"
   git push origin feature/your-feature-name
   ```
4. 提交 Pull Request。

---

## 许可证

本项目采用 [MIT License](LICENSE) 许可证。

---

## 联系方式

如有任何问题或建议，请通过以下方式联系：
- luogu: [https://www.luogu.com.cn/user/661094](https://www.luogu.com.cn/user/661094)
- GitHub Issues: [https://github.com/yourusername/ggtoolbox/issues](https://github.com/yourusername/ggtoolbox/issues)

