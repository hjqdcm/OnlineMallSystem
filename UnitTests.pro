# UnitTests.pro - 单元测试专用项目文件
QT += core testlib
CONFIG += console warn_on c++17 testcase

TEMPLATE = app
TARGET = UnitTests

# 单元测试源文件
SOURCES += \
    test_user.cpp \
    test_product.cpp \
    test_main.cpp

# 测试头文件
HEADERS += \
    test_user.h \
    test_product.h

# 需要测试的类文件
SOURCES += \
    user.cpp \
    product.cpp

HEADERS += \
    user.h \
    product.h

# 不包含的文件（明确排除）
# main.cpp - 主程序文件
# mainwindow.cpp - GUI文件
# test_integration.cpp - 集成测试文件
# test.cpp - 故意包含漏洞的测试文件

# 构建目录
OBJECTS_DIR = $$OUT_PWD/obj
MOC_DIR = $$OUT_PWD/moc

# 包含路径
INCLUDEPATH += .

# 编译标志
QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic -Wno-unused-parameter

# 链接标志
LIBS += -lQt5Test

# 调试版本配置
CONFIG(debug, debug|release) {
    DESTDIR = $$OUT_PWD/debug
    DEFINES += QT_DEBUG
}

# 发布版本配置
CONFIG(release, debug|release) {
    DESTDIR = $$OUT_PWD/release
    DEFINES += QT_NO_DEBUG_OUTPUT
}

# 为单元测试启用异常
QMAKE_CXXFLAGS += -fexceptions

# 启用RTTI
QMAKE_CXXFLAGS += -frtti

# 排除警告（如果需要）
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O1  # 对于测试，使用O1优化级别更安全

# Windows特定配置
win32 {
    # Windows下可能需要额外的库
    LIBS += -luser32
}

# Unix/Linux特定配置
unix:!macx {
    LIBS += -lpthread
}

# 输出信息
message("Building unit tests...")
message("Target: $$TARGET")
message("Sources: $$SOURCES")