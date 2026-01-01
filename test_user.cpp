// #include "test_user.h"
// #include "product.h"
// #include <QtTest/QtTest>
// #include <QUuid>

// TestUser::TestUser()
// {
// }

// User* TestUser::createTestUser()
// {
//     return new User("testuser", "password123", "test@example.com");
// }

// QList<Product> TestUser::createTestProducts()
// {
//     QList<Product> products;
//     products.append(Product("Test Product 1", 100.0, "Description 1", "Category 1", "seller1", "Seller One"));
//     products.append(Product("Test Product 2", 200.0, "Description 2", "Category 2", "seller2", "Seller Two"));
//     return products;
// }

// void TestUser::testConstructor()
// {
//     User user("john", "password", "john@example.com");

//     QVERIFY(user.getUsername() == "john");
//     QVERIFY(user.getPassword() == "password");
//     QVERIFY(user.getEmail() == "john@example.com");
//     QVERIFY(!user.getUserId().isEmpty());
//     QVERIFY(!user.getIsAuthenticated());
// }

// void TestUser::testLoginSuccess()
// {
//     User user("test", "123456", "test@example.com");

//     bool result = user.login("test", "123456");

//     QVERIFY(result == true);
//     QVERIFY(user.getIsAuthenticated() == true);
// }

// void TestUser::testLoginFailure()
// {
//     User user("test", "123456", "test@example.com");

//     bool result = user.login("wrong", "password");

//     QVERIFY(result == false);
//     QVERIFY(user.getIsAuthenticated() == false);
// }

// void TestUser::testLogout()
// {
//     User user("test", "123456", "test@example.com");

//     user.login("test", "123456");
//     QVERIFY(user.getIsAuthenticated() == true);

//     user.logout();
//     QVERIFY(user.getIsAuthenticated() == false);
// }

// void TestUser::testResetPasswordValid()
// {
//     User user("test", "oldpassword", "test@example.com");

//     bool result = user.resetPassword("newpassword123");

//     QVERIFY(result == true);
//     QVERIFY(user.getPassword() == "newpassword123");
// }

// void TestUser::testResetPasswordInvalid()
// {
//     User user("test", "oldpassword", "test@example.com");

//     bool result = user.resetPassword("123");  // 密码太短

//     QVERIFY(result == false);
//     QVERIFY(user.getPassword() == "oldpassword");  // 密码未改变
// }

// void TestUser::testPurchaseProduct()
// {
//     User* user = createTestUser();
//     QList<Product> products = createTestProducts();

//     QString productId = products[0].getProductId();
//     bool result = user->purchaseProduct(productId, products);

//     QVERIFY(result == true);

//     // 检查商品状态
//     for (Product& product : products) {
//         if (product.getProductId() == productId) {
//             QVERIFY(product.getIsSold() == true);
//             QVERIFY(product.getBuyerId() == user->getUserId());
//             break;
//         }
//     }

//     // 检查购买历史
//     QList<QString> history = user->getPurchaseHistory();
//     QVERIFY(history.contains(productId));

//     delete user;
// }

// void TestUser::testPurchaseAlreadySold()
// {
//     User* user = createTestUser();
//     QList<Product> products = createTestProducts();

//     QString productId = products[0].getProductId();

//     // 第一次购买
//     bool result1 = user->purchaseProduct(productId, products);
//     QVERIFY(result1 == true);

//     // 尝试再次购买同一商品
//     User* anotherUser = new User("user2", "pass", "user2@example.com");
//     bool result2 = anotherUser->purchaseProduct(productId, products);

//     QVERIFY(result2 == false);  // 应该失败，因为商品已售出

//     delete user;
//     delete anotherUser;
// }

// void TestUser::testAddToFavorites()
// {
//     User* user = createTestUser();
//     QString productId = "test-product-123";

//     user->addToFavorites(productId);
//     QList<QString> favorites = user->getFavorites();

//     QVERIFY(favorites.contains(productId));

//     delete user;
// }

// void TestUser::testViewPurchaseHistory()
// {
//     User* user = createTestUser();
//     QList<Product> products = createTestProducts();

//     // 购买两个商品
//     user->purchaseProduct(products[0].getProductId(), products);
//     user->purchaseProduct(products[1].getProductId(), products);

//     QList<QString> history = user->viewPurchaseHistory();

//     QVERIFY(history.size() == 2);

//     delete user;
// }

// void TestUser::testPublishProduct()
// {
//     User* user = createTestUser();
//     QList<Product> products;

//     bool result = user->publishProduct(
//         "New Product",
//         99.99,
//         "Product Description",
//         "Electronics",
//         products
//         );

//     QVERIFY(result == true);
//     QVERIFY(products.size() == 1);
//     QVERIFY(products[0].getProductName() == "New Product");
//     QVERIFY(products[0].getPrice() == 99.99);
//     QVERIFY(products[0].getSellerId() == user->getUserId());

//     delete user;
// }

// void TestUser::testViewSalesStatus()
// {
//     User* user = createTestUser();
//     QList<Product> products = createTestProducts();

//     // 发布一个商品
//     user->publishProduct("My Product", 100.0, "My description", "Books", products);

//     // 销售状态应该包含统计数据
//     QList<QString> status = user->viewSalesStatus(products);

//     QVERIFY(status.size() > 0);

//     delete user;
// }

// void TestUser::testViewSoldProducts()
// {
//     User* user = createTestUser();
//     QList<Product> products = createTestProducts();

//     // 发布商品
//     user->publishProduct("Product 1", 100.0, "Desc", "Cat", products);
//     QString productId = products.last().getProductId();

//     // 购买这个商品
//     User* buyer = new User("buyer", "pass", "buyer@example.com");
//     buyer->purchaseProduct(productId, products);

//     // 查看已售商品
//     QList<QString> sold = user->viewSoldProducts(products);

//     QVERIFY(sold.size() == 1);

//     delete user;
//     delete buyer;
// }

// void TestUser::testEmptyInput()
// {
//     // 测试各种空输入情况
//     User user("", "", "");

//     // 尝试重置为空密码
//     bool resetResult = user.resetPassword("");
//     QVERIFY(resetResult == false);
// }

// void TestUser::testNegativePrice()
// {
//     User* user = createTestUser();
//     QList<Product> products;

//     // 尝试发布价格为负数的商品
//     bool result = user->publishProduct(
//         "Negative Price Product",
//         -10.0,  // 负数价格
//         "Test",
//         "Test",
//         products
//         );

//     // 根据实现，可能会失败或接受负数
//     // 这里我们只是测试不会崩溃

//     delete user;
// }

// void TestUser::testToJson()
// {
//     User user("testuser", "password", "test@example.com");
//     user.setNickname("Test Nickname");

//     QJsonObject json = user.toJson();

//     QVERIFY(json["username"].toString() == "testuser");
//     QVERIFY(json["email"].toString() == "test@example.com");
//     QVERIFY(json["nickname"].toString() == "Test Nickname");
//     QVERIFY(json.contains("userId"));
// }

// void TestUser::testFromJson()
// {
//     // 创建原始用户
//     User originalUser("original", "pass123", "original@example.com");
//     originalUser.setNickname("Original Nick");
//     originalUser.addToFavorites("fav1");
//     originalUser.addToFavorites("fav2");

//     // 转换为JSON
//     QJsonObject json = originalUser.toJson();

//     // 从JSON重建
//     User* restoredUser = User::fromJson(json);

//     // 验证属性
//     QVERIFY(restoredUser->getUsername() == "original");
//     QVERIFY(restoredUser->getEmail() == "original@example.com");
//     QVERIFY(restoredUser->getNickname() == "Original Nick");
//     QVERIFY(restoredUser->getFavorites().size() == 2);

//     delete restoredUser;
// }



#include "test_user.h"
#include "product.h"
#include <QtTest/QtTest>
#include <QUuid>

TestUser::TestUser()
{
}

User* TestUser::createTestUser()
{
    return new User("testuser", "password123", "test@example.com");
}

QList<Product> TestUser::createTestProducts()
{
    QList<Product> products;
    products.append(Product("Test Product 1", 100.0, "Description 1", "Category 1", "seller1", "Seller One", false));
    products.append(Product("Test Product 2", 200.0, "Description 2", "Category 2", "seller2", "Seller Two", false));
    return products;
}

void TestUser::testConstructor()
{
    User user("john", "password", "john@example.com");

    QVERIFY(user.getUsername() == "john");
    QVERIFY(user.getPassword() == "password");
    QVERIFY(user.getEmail() == "john@example.com");
    QVERIFY(!user.getUserId().isEmpty());
    QVERIFY(!user.getIsAuthenticated());
}

void TestUser::testLoginSuccess()
{
    User user("test", "123456", "test@example.com");

    bool result = user.login("test", "123456");

    QVERIFY(result == true);
    QVERIFY(user.getIsAuthenticated() == true);
}

void TestUser::testLoginFailure()
{
    User user("test", "123456", "test@example.com");

    bool result = user.login("wrong", "password");

    QVERIFY(result == false);
    QVERIFY(user.getIsAuthenticated() == false);
}

void TestUser::testLogout()
{
    User user("test", "123456", "test@example.com");

    user.login("test", "123456");
    QVERIFY(user.getIsAuthenticated() == true);

    user.logout();
    QVERIFY(user.getIsAuthenticated() == false);
}

void TestUser::testResetPasswordValid()
{
    User user("test", "oldpassword", "test@example.com");

    bool result = user.resetPassword("newpassword123");

    QVERIFY(result == true);
    QVERIFY(user.getPassword() == "newpassword123");
}

void TestUser::testResetPasswordInvalid()
{
    User user("test", "oldpassword", "test@example.com");

    bool result = user.resetPassword("123");  // Password too short

    QVERIFY(result == false);
    QVERIFY(user.getPassword() == "oldpassword");  // Password unchanged
}

void TestUser::testPurchaseProduct()
{
    User* user = createTestUser();
    QList<Product> products = createTestProducts();

    QString productId = products[0].getProductId();
    bool result = user->purchaseProduct(productId, products);

    QVERIFY(result == true);

    // Check product status
    for (Product& product : products) {
        if (product.getProductId() == productId) {
            QVERIFY(product.getIsSold() == true);
            QVERIFY(product.getBuyerId() == user->getUserId());
            break;
        }
    }

    // Check purchase history
    QList<QString> history = user->getPurchaseHistory();
    QVERIFY(history.contains(productId));

    delete user;
}

void TestUser::testPurchaseAlreadySold()
{
    User* user = createTestUser();
    QList<Product> products = createTestProducts();

    QString productId = products[0].getProductId();

    // First purchase
    bool result1 = user->purchaseProduct(productId, products);
    QVERIFY(result1 == true);

    // Try to purchase the same product again
    User* anotherUser = new User("user2", "pass", "user2@example.com");
    bool result2 = anotherUser->purchaseProduct(productId, products);

    QVERIFY(result2 == false);  // Should fail because product already sold

    delete user;
    delete anotherUser;
}

void TestUser::testAddToFavorites()
{
    User* user = createTestUser();
    QString productId = "test-product-123";

    user->addToFavorites(productId);
    QList<QString> favorites = user->getFavorites();

    QVERIFY(favorites.contains(productId));

    delete user;
}

void TestUser::testViewPurchaseHistory()
{
    User* user = createTestUser();
    QList<Product> products = createTestProducts();

    // Purchase two products
    user->purchaseProduct(products[0].getProductId(), products);
    user->purchaseProduct(products[1].getProductId(), products);

    QList<QString> history = user->viewPurchaseHistory();

    QVERIFY(history.size() == 2);

    delete user;
}

void TestUser::testPublishProduct()
{
    User* user = createTestUser();
    QList<Product> products;

    bool result = user->publishProduct(
        "New Product",
        99.99,
        "Product Description",
        "Electronics",
        products
        );

    QVERIFY(result == true);
    QVERIFY(products.size() == 1);
    QVERIFY(products[0].getProductName() == "New Product");
    QVERIFY(products[0].getPrice() == 99.99);
    QVERIFY(products[0].getSellerId() == user->getUserId());

    delete user;
}

void TestUser::testViewSalesStatus()
{
    User* user = createTestUser();
    QList<Product> products = createTestProducts();

    // Publish a product
    user->publishProduct("My Product", 100.0, "My description", "Books", products);

    // Sales status should contain statistics
    QList<QString> status = user->viewSalesStatus(products);

    QVERIFY(status.size() > 0);

    delete user;
}

void TestUser::testViewSoldProducts()
{
    User* user = createTestUser();
    QList<Product> products = createTestProducts();

    // Publish a product
    user->publishProduct("Product 1", 100.0, "Desc", "Cat", products);
    QString productId = products.last().getProductId();

    // Buy this product
    User* buyer = new User("buyer", "pass", "buyer@example.com");
    buyer->purchaseProduct(productId, products);

    // View sold products
    QList<QString> sold = user->viewSoldProducts(products);

    QVERIFY(sold.size() == 1);

    delete user;
    delete buyer;
}

void TestUser::testEmptyInput()
{
    // Test various empty input cases
    User user("", "", "");
    // Try to login with empty username and password
    bool loginResult = user.login("", "");
    QVERIFY(loginResult == false);

    // Try to reset to empty password
    bool resetResult = user.resetPassword("");
    QVERIFY(resetResult == false);
}

void TestUser::testNegativePrice()
{
    User* user = createTestUser();
    QList<Product> products;

    // Try to publish product with negative price
    bool result = user->publishProduct(
        "Negative Price Product",
        -10.0,  // Negative price
        "Test",
        "Test",
        products
        );

    // According to the implementation, might fail or accept negative price
    // Here we just test that it doesn't crash

    delete user;
}

void TestUser::testToJson()
{
    User user("testuser", "password", "test@example.com");
    user.setNickname("Test Nickname");

    QJsonObject json = user.toJson();

    QVERIFY(json["username"].toString() == "testuser");
    QVERIFY(json["email"].toString() == "test@example.com");
    QVERIFY(json["nickname"].toString() == "Test Nickname");
    QVERIFY(json.contains("userId"));
}

void TestUser::testFromJson()
{
    // Create original user
    User originalUser("original", "pass123", "original@example.com");
    originalUser.setNickname("Original Nick");
    originalUser.addToFavorites("fav1");
    originalUser.addToFavorites("fav2");

    // Convert to JSON
    QJsonObject json = originalUser.toJson();

    // Rebuild from JSON
    User* restoredUser = User::fromJson(json);

    // Verify properties
    QVERIFY(restoredUser->getUsername() == "original");
    QVERIFY(restoredUser->getEmail() == "original@example.com");
    QVERIFY(restoredUser->getNickname() == "Original Nick");
    QVERIFY(restoredUser->getFavorites().size() == 2);

    delete restoredUser;
}
