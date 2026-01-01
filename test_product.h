#ifndef TEST_PRODUCT_H
#define TEST_PRODUCT_H

#include <QtTest/QtTest>
#include <QObject>
#include "product.h"

class TestProduct : public QObject
{
    Q_OBJECT

public:
    TestProduct();

private slots:
    // 基础功能测试
    void testConstructor();
    void testDisplayInfo();
    void testSetPromotional();
    void testAddImage();
    void testSetBuyer();

    // 边界条件测试
    void testZeroPrice();
    void testLargePrice();
    void testEmptyStrings();

    // 序列化测试
    void testToJson();
    void testFromJson();
    void testFromJsonOldFormat();

    // ProductCategory测试
    void testProductCategory();
    void testFilterProducts();

    // 特殊商品测试
    void testVirtualProduct();
    void testSoldProduct();

private:
    Product createTestProduct();
};

#endif // TEST_PRODUCT_H
