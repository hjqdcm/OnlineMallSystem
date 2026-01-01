#include "test_product.h"
#include <QtTest/QtTest>

TestProduct::TestProduct()
{
}

Product TestProduct::createTestProduct()
{
    return Product("Test Product", 99.99, "Test Description",
                   "Electronics", "seller123", "Test Seller");
}

void TestProduct::testConstructor()
{
    Product product("Laptop", 1500.99, "High-end gaming laptop",
                    "Electronics", "seller1", "John Doe");

    QVERIFY(product.getProductName() == "Laptop");
    QVERIFY(product.getPrice() == 1500.99);
    QVERIFY(product.getDescription() == "High-end gaming laptop");
    QVERIFY(product.getCategory() == "Electronics");
    QVERIFY(product.getSellerId() == "seller1");
    QVERIFY(product.getSellerName() == "John Doe");
    QVERIFY(product.getIsSold() == false);
    QVERIFY(!product.getProductId().isEmpty());
}

void TestProduct::testDisplayInfo()
{
    Product product("Book", 29.99, "A great novel", "Books", "seller2", "Jane Smith");

    QString info = product.displayInfo();

    QVERIFY(info.contains("Book"));
    QVERIFY(info.contains("29.99"));
    QVERIFY(info.contains("Jane Smith"));  // 应该包含卖家用户名
    QVERIFY(info.contains("待售"));
}

void TestProduct::testSetPromotional()
{
    Product product = createTestProduct();

    QVERIFY(product.getIsPromotional() == false);

    product.setPromotional(true);
    QVERIFY(product.getIsPromotional() == true);

    QString info = product.displayInfo();
    QVERIFY(info.contains("促销商品"));
}

void TestProduct::testAddImage()
{
    Product product = createTestProduct();

    QVERIFY(product.getImages().size() == 1);  // 默认有一张图片

    product.addImage(":/images/product1.jpg");
    product.addImage(":/images/product2.jpg");

    QVERIFY(product.getImages().size() == 3);
    QVERIFY(product.getImages()[0] == ":/images/default_product.png");
    QVERIFY(product.getImages()[1] == ":/images/product1.jpg");
}

void TestProduct::testSetBuyer()
{
    Product product = createTestProduct();

    QVERIFY(product.getBuyerId().isEmpty());
    QVERIFY(product.getIsSold() == false);

    product.setBuyerId("buyer456");
    product.setIsSold(true);

    QVERIFY(product.getBuyerId() == "buyer456");
    QVERIFY(product.getIsSold() == true);

    QString info = product.displayInfo();
    QVERIFY(info.contains("已售出"));
    QVERIFY(info.contains("buyer456"));
}

void TestProduct::testZeroPrice()
{
    Product product("Free Item", 0, "Free giveaway", "Freebies", "seller", "Free Seller");

    QVERIFY(product.getPrice() == 0);

    QString info = product.displayInfo();
    QVERIFY(info.contains("¥0"));
}

void TestProduct::testLargePrice()
{
    Product product("Luxury Car", 1000000.0, "Expensive car", "Vehicles", "seller", "Car Dealer");

    QVERIFY(product.getPrice() == 1000000.0);
}

void TestProduct::testEmptyStrings()
{
    Product product("", 0.0, "", "", "", "");

    QVERIFY(product.getProductName().isEmpty());
    QVERIFY(product.getDescription().isEmpty());
    QVERIFY(product.getCategory().isEmpty());
    QVERIFY(product.getSellerId().isEmpty());
    QVERIFY(product.getSellerName().isEmpty());
}

void TestProduct::testToJson()
{
    Product product("Test", 50.0, "Desc", "Cat", "sid", "Seller Name");
    product.setPromotional(true);
    product.setBuyerId("bid");
    product.setIsSold(true);
    product.addImage("extra.jpg");

    QJsonObject json = product.toJson();

    QVERIFY(json["productName"].toString() == "Test");
    QVERIFY(json["price"].toDouble() == 50.0);
    QVERIFY(json["sellerName"].toString() == "Seller Name");
    QVERIFY(json["isPromotional"].toBool() == true);
    QVERIFY(json["isSold"].toBool() == true);

    QJsonArray images = json["images"].toArray();
    QVERIFY(images.size() == 2);  // 默认图片 + extra.jpg
}

void TestProduct::testFromJson()
{
    QJsonObject json;
    json["productId"] = "test-id-123";
    json["productName"] = "JSON Product";
    json["price"] = 75.5;
    json["description"] = "From JSON";
    json["category"] = "JSON Category";
    json["sellerId"] = "seller-json";
    json["sellerName"] = "JSON Seller";
    json["isPromotional"] = true;
    json["isVirtual"] = false;
    json["buyerId"] = "buyer-json";
    json["isSold"] = true;

    QJsonArray images;
    images.append("image1.jpg");
    images.append("image2.jpg");
    json["images"] = images;

    Product product = Product::fromJson(json);

    QVERIFY(product.getProductId() == "test-id-123");
    QVERIFY(product.getProductName() == "JSON Product");
    QVERIFY(product.getPrice() == 75.5);
    QVERIFY(product.getSellerName() == "JSON Seller");
    QVERIFY(product.getIsPromotional() == true);
    QVERIFY(product.getIsSold() == true);
}

void TestProduct::testFromJsonOldFormat()
{
    // 测试兼容旧格式（没有sellerName字段）
    QJsonObject json;
    json["productId"] = "old-id";
    json["productName"] = "Old Product";
    json["price"] = 50.0;
    json["description"] = "Old desc";
    json["category"] = "Old";
    json["sellerId"] = "old-seller";
    // 没有 sellerName 字段
    json["isPromotional"] = false;
    json["isVirtual"] = false;
    json["buyerId"] = "";
    json["isSold"] = false;

    QJsonArray images;
    json["images"] = images;

    Product product = Product::fromJson(json);

    // 应该使用默认值
    QVERIFY(product.getSellerName() == "未知卖家");
}

void TestProduct::testProductCategory()
{
    ProductCategory category("Electronics", "");

    QVERIFY(category.getCategoryName() == "Electronics");
    QVERIFY(category.getParentCategory().isEmpty());

    ProductCategory subcategory("Laptops", "Electronics");
    QVERIFY(subcategory.getParentCategory() == "Electronics");
}

void TestProduct::testFilterProducts()
{
    QList<Product> products;
    products.append(Product("Laptop", 1000, "Laptop", "Electronics", "s1", "Seller1"));
    products.append(Product("Book", 20, "Book", "Books", "s2", "Seller2"));
    products.append(Product("Phone", 800, "Phone", "Electronics", "s3", "Seller3"));
    products.append(Product("Chair", 150, "Chair", "Furniture", "s4", "Seller4"));

    QList<Product> filtered = ProductCategory::filterProducts(products, "Electronics");

    QVERIFY(filtered.size() == 2);
    QVERIFY(filtered[0].getProductName() == "Laptop");
    QVERIFY(filtered[1].getProductName() == "Phone");
}

void TestProduct::testVirtualProduct()
{
    Product virtualProduct("E-book", 9.99, "Digital book",
                           "Digital", "seller", "Digital Seller", true);

    QVERIFY(virtualProduct.getIsVirtual() == true);

    QString info = virtualProduct.displayInfo();
    QVERIFY(info.contains("虚拟商品"));
}

void TestProduct::testSoldProduct()
{
    Product product = createTestProduct();

    QVERIFY(product.getIsSold() == false);
    QVERIFY(product.displayInfo().contains("待售"));

    product.setBuyerId("buyer123");
    product.setIsSold(true);

    QVERIFY(product.getIsSold() == true);
    QVERIFY(product.displayInfo().contains("已售出"));
}
