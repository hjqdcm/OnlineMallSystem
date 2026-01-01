# tests.pro - 专门用于构建和运行测试
QT += core testlib

CONFIG += c++17 testcase console

TARGET = OnlineMallTests

SOURCES += \
    test_main.cpp \
    test_user.cpp \
    test_product.cpp \
    user.cpp \
    product.cpp

HEADERS += \
    test_user.h \
    test_product.h \
    user.h \
    product.h

# 包含当前目录和上级目录
INCLUDEPATH += . ..

# 设置输出
DESTDIR = $$PWD