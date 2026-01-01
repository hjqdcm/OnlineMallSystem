# tests.pro
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

INCLUDEPATH += . ..

DESTDIR = $$PWD