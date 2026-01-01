QT += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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

QMAKE_CXXFLAGS += -Wall -Wextra