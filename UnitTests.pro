# UnitTests.pro - 专用于单元测试的项目文件
QT += core testlib
CONFIG += console warn_on c++17 testcase

TEMPLATE = app
TARGET = UnitTests

# 需要测试的核心类
SOURCES += \
    user.cpp \
    product.cpp

# 单元测试文件
SOURCES += \
    test_user.cpp \
    test_product.cpp \
    test_main.cpp

# 头文件
HEADERS += \
    user.h \
    product.h \
    test_user.h \
    test_product.h

# 构建目录配置
OBJECTS_DIR = $$OUT_PWD/obj
MOC_DIR = $$OUT_PWD/moc

# 包含路径
INCLUDEPATH += .

# 编译选项
QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O1

# 链接选项
LIBS += -lQt5Test

# 平台特定配置
win32 {
    # Windows 配置
    LIBS += -luser32
}

unix:!macx {
    # Linux 配置
    LIBS += -lpthread
}

# 输出信息
message("构建单元测试项目")
message("目标文件: $$TARGET")
message("构建目录: $$OUT_PWD")