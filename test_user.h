#ifndef TEST_USER_H
#define TEST_USER_H

#include <QtTest/QtTest>
#include <QObject>
#include "./user.h"

class TestUser : public QObject
{
    Q_OBJECT

public:
    TestUser();

private slots:
    // 基础功能测试
    void testConstructor();
    void testLoginSuccess();
    void testLoginFailure();
    void testLogout();
    void testResetPasswordValid();
    void testResetPasswordInvalid();

    // 买家功能测试
    void testPurchaseProduct();
    void testPurchaseAlreadySold();
    void testAddToFavorites();
    void testViewPurchaseHistory();

    // 卖家功能测试
    void testPublishProduct();
    void testViewSalesStatus();
    void testViewSoldProducts();

    // 边界条件测试
    void testEmptyInput();
    void testNegativePrice();

    // 序列化测试
    void testToJson();
    void testFromJson();

private:
    User* createTestUser();
    QList<Product> createTestProducts();
};

#endif // TEST_USER_H
