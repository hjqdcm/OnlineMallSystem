# OnlineMallSystem
一个基于 Qt C++ 的网络商场系统
# 网络商场系统

一个基于 Qt C++ 开发的网络商场系统，支持用户注册、登录、商品浏览、购买和发布等功能。

## 功能特性

- ✅ 用户注册和登录
- ✅ 密码重置功能
- ✅ 商品浏览和搜索
- ✅ 商品购买（不能购买自己发布的商品）
- ✅ 商品发布和管理
- ✅ 数据持久化存储
- ✅ 购买历史记录

## 技术栈

- **框架**: Qt 6
- **语言**: C++
- **数据存储**: QSettings
- **UI**: 纯代码创建（无 .ui 文件）
- 
- ## 项目结构
OnlineMallSystem/
├── main.cpp # 程序入口
├── mainwindow.h/cpp # 主窗口类
├── user.h/cpp # 用户管理类
├── product.h/cpp # 商品管理类
├── CMakeLists.txt # 构建配置
└── README.md # 项目说明
