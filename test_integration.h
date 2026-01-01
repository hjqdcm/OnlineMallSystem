#ifndef TEST_INTEGRATION_H
#define TEST_INTEGRATION_H

#include <QtTest/QtTest>
#include <QObject>
#include <QSettings>
#include "user.h"
#include "product.h"
#include "mainwindow.h"

class TestIntegration : public QObject
{
    Q_OBJECT

public:
    TestIntegration();

private slots:
    // 第一组：用户与商品集成
    void testCompletePurchaseWorkflow();
    void testCompletePublishWorkflow();

    // 第二组：数据持久化集成
    void testDataPersistence();
    void testSessionState();

    // 第三组：异常流程集成
    void testConcurrentOperations();
    void testInvalidDataRecovery();

    // 辅助函数
    void initTestCase();
    void cleanupTestCase();

private:
    QSettings* testSettings;
    QString testConfigPath;
};

#endif // TEST_INTEGRATION_H
