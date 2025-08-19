
# 🎯 GGToolBox Script Market Launcher

> 🛠 为 OIer 服务的脚本市场平台！

欢迎来到 **GGToolBox Script Market Launcher**，这是一个为 OIer（信息学爱好者）打造的脚本插件系统。
你可以在这里上传、共享、启动自己的 OI 工具插件，用脚本的方式快速集成到 GGToolBox 中！

如果你不会写脚本，你可以把此文档发送给 LLM（大模型），它会教你上传编写。

---

## 📦 插件结构规范

每一个插件请使用 **独立文件夹** 并包含以下内容：

```
插件名/
├── start.cmd     // 启动脚本（必须）
├── id.txt        // 插件元信息（必须）
└── 其他资源文件   // 可选资源文件
```

> ⚠️ **注意**：请尽量避免将体积较大的二进制文件上传到本仓库，建议使用 `curl` 等工具在 `start.cmd` 中下载你的程序资源，仅在此仓库中保留最小核心脚本。

> ⚠️ **注意**：如果你的代码中包含中文，请使用 `chcp 65001` 来启用 UTF8 模式。



---

## 🧪 示例插件：PCL2 启动器

以启动 **PCL2** 为例：

```
PCL2/
├── start.cmd
├── PCL2.exe              // 建议外部下载
└── id.txt
```

### 📜 start.cmd

```bat
@echo off
start PCL2.exe
exit
```

> ✅ 你也可以替换为自动下载并启动：

```bat
@echo off
curl -L -o PCL2.exe https://example.com/path/to/PCL2.exe
start PCL2.exe
exit
```

### 🆔 id.txt

```txt
lyf 1.0
```

---

## ✅ 插件提交指南

1. **创建新文件夹**，命名为插件名。
2. **添加 `start.cmd` 和 `id.txt` 文件**，并确保能在 GGToolBox 中正常启动。
3. **可选**：上传图标、配置、说明文档等附加资源。
4. **避免上传大型可执行文件**，改用脚本下载。

---

## 🧑‍💻 插件启动原理

GGToolBox 会自动识别每个插件目录，并执行其中的 `start.cmd` 来启动对应脚本。通过这种方式，可以灵活集成各种工具如评测器、可视化调试器、在线题库客户端等。

---

## 📮 联系与贡献

欢迎 PR 新插件或改进 launcher 机制！
如有疑问请联系维护者：**liyifan202201**

---

**GGToolBox Script Market —— 让 OI 工具分发更简单！**

---
