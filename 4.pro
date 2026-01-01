QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    # main.cpp \
    mainwindow.cpp \
    product.cpp \
    # test_integration.cpp \
    # test_integration_main.cpp \
    test_main.cpp \
    test_product.cpp \
    test_user.cpp \
    user.cpp

HEADERS += \
    mainwindow.h \
    product.h \
    # test_integration.h \
    test_product.h \
    test_user.h \
    user.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
QMAKE_CXXFLAGS += -Wall -Wextra
QMAKE_CXX_CLANG_TIDY = clang-tidy -header-filter=.*

QT += testlib
CONFIG += testcase

# integration_test {
#     # 集成测试配置
#     TARGET = OnlineMallIntegrationTests

#     # 添加集成测试源文件
#     SOURCES += tests/test_integration.cpp

#     # 排除主程序的main.cpp
#     SOURCES -= main.cpp

#     # 如果需要，可以添加专门的测试main.cpp
#     # SOURCES += tests/test_integration_main.cpp

#     # 设置独立的构建目录
#     DESTDIR = $$OUT_PWD/integration_tests
#     OBJECTS_DIR = $$DESTDIR/.obj

#     # 集成测试需要testlib
#     QT += testlib
#     CONFIG += testcase console
# }

# # 默认构建（非测试）
# else {
#     TARGET = OnlineMall
#     CONFIG -= testcase
# }

