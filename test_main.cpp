#include <QCoreApplication>
#include <QtTest/QtTest>

#include "test_user.h"
#include "test_product.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    int result = 0;

    // 运行User测试
    TestUser testUser;
    result |= QTest::qExec(&testUser, argc, argv);

    // 运行Product测试
    TestProduct testProduct;
    result |= QTest::qExec(&testProduct, argc, argv);

    return result;
}
