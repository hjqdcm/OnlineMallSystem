#include <QApplication>
#include <QApplication>
#include <QtTest/QtTest>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "test_integration.h"

int main(int argc, char *argv[])
{
// Fix encoding for Windows console
#ifdef Q_OS_WIN
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif


    QApplication app(argc, argv);

    qDebug() << "========================================";
    qDebug() << "    Online Mall System - Integration Tests";
    qDebug() << "========================================";

    TestIntegration testIntegration;
    int result = QTest::qExec(&testIntegration, argc, argv);

    qDebug() << "========================================";
    qDebug() << "Integration tests" << (result == 0 ? "ALL PASSED" : "HAVE FAILURES");
    qDebug() << "========================================";

    return result;
}
