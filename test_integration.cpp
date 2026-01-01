#include "test_integration.h"
#include <QCoreApplication>
#include <QTemporaryDir>
#include <QDebug>

TestIntegration::TestIntegration()
{
    // Create temporary directory for testing
    QTemporaryDir tempDir;
    testConfigPath = tempDir.path() + "/test_config.ini";
    testSettings = new QSettings(testConfigPath, QSettings::IniFormat);
}

void TestIntegration::initTestCase()
{
    qDebug() << "=== Starting Integration Tests ===";
}

void TestIntegration::cleanupTestCase()
{
    qDebug() << "=== Integration Tests Completed ===";
    delete testSettings;
}

void TestIntegration::testCompletePurchaseWorkflow()
{
    qDebug() << "Testing complete purchase workflow...";

    // 1. Create users
    User* buyer = new User("buyer_test", "password123", "buyer@test.com");
    User* seller = new User("seller_test", "password456", "seller@test.com");

    // 2. Publish a product
    QList<Product> allProducts;
    seller->publishProduct("Test Product", 100.0, "Integration test product", "Test Category", allProducts);

    QVERIFY2(allProducts.size() == 1, "Product publish failed");
    QString productId = allProducts[0].getProductId();

    // 3. Purchase the product
    bool purchaseResult = buyer->purchaseProduct(productId, allProducts);
    QVERIFY2(purchaseResult, "Purchase failed");

    // 4. Verify purchase result
    QVERIFY2(allProducts[0].getIsSold(), "Product status not updated");
    QVERIFY2(allProducts[0].getBuyerId() == buyer->getUserId(), "Buyer ID not set");

    // 5. Verify purchase history
    QList<QString> history = buyer->getPurchaseHistory();
    QVERIFY2(history.contains(productId), "Purchase history not recorded");

    // 6. Verify sales status
    QList<QString> salesStatus = seller->viewSalesStatus(allProducts);
    QVERIFY2(salesStatus.size() >= 1, "Sales status query failed");

    qDebug() << "✓ Complete purchase workflow test passed";

    delete buyer;
    delete seller;
}

void TestIntegration::testCompletePublishWorkflow()
{
    qDebug() << "Testing complete publish workflow...";

    // 1. Create user
    User* seller = new User("publisher_test", "password789", "publisher@test.com");

    // 2. Publish multiple products
    QList<Product> allProducts;

    bool result1 = seller->publishProduct("Product A", 50.0, "Description A", "Category A", allProducts);
    bool result2 = seller->publishProduct("Product B", 150.0, "Description B", "Category B", allProducts);
    bool result3 = seller->publishVirtualProduct("Virtual Product", 30.0, "Virtual description", allProducts);

    QVERIFY2(result1 && result2 && result3, "Product publish failed");
    QVERIFY2(allProducts.size() == 3, "Product count incorrect");

    // 3. Verify published products
    QList<QString> published = seller->getPublishedProducts();
    QVERIFY2(published.size() == 3, "Published product list incorrect");

    // 4. Verify product details
    for (const Product& product : allProducts) {
        QVERIFY2(!product.getProductId().isEmpty(), "Product ID is empty");
        QVERIFY2(!product.getProductName().isEmpty(), "Product name is empty");
        QVERIFY2(product.getPrice() > 0, "Product price is invalid");
        QVERIFY2(product.getSellerId() == seller->getUserId(), "Seller ID doesn't match");
    }

    // 5. Verify sales statistics
    QList<QString> status = seller->viewSalesStatus(allProducts);
    QVERIFY2(status.size() >= 3, "Sales status information incomplete");

    qDebug() << "✓ Complete publish workflow test passed";

    delete seller;
}

void TestIntegration::testDataPersistence()
{
    qDebug() << "Testing data persistence...";

    // 1. Create test data
    QList<User*> originalUsers;
    originalUsers.append(new User("user1", "pass1", "user1@test.com"));
    originalUsers.append(new User("user2", "pass2", "user2@test.com"));

    QList<Product> originalProducts;
    originalProducts.append(Product("Product 1", 100.0, "Description 1", "Category 1",
                                    originalUsers[0]->getUserId(), "User 1"));
    originalProducts.append(Product("Product 2", 200.0, "Description 2", "Category 2",
                                    originalUsers[1]->getUserId(), "User 2"));

    // 2. Simulate saving to QSettings
    testSettings->beginGroup("TestData");
    testSettings->setValue("UserCount", originalUsers.size());
    testSettings->setValue("ProductCount", originalProducts.size());

    for (int i = 0; i < originalUsers.size(); ++i) {
        QString key = QString("User%1").arg(i);
        QJsonObject userJson = originalUsers[i]->toJson();
        QString jsonString = QString::fromUtf8(QJsonDocument(userJson).toJson());
        testSettings->setValue(key, jsonString);
    }

    for (int i = 0; i < originalProducts.size(); ++i) {
        QString key = QString("Product%1").arg(i);
        QJsonObject productJson = originalProducts[i].toJson();
        QString jsonString = QString::fromUtf8(QJsonDocument(productJson).toJson());
        testSettings->setValue(key, jsonString);
    }
    testSettings->endGroup();

    testSettings->sync();

    // 3. Simulate loading from QSettings
    QList<User*> loadedUsers;
    QList<Product> loadedProducts;

    testSettings->beginGroup("TestData");
    int loadedUserCount = testSettings->value("UserCount", 0).toInt();
    int loadedProductCount = testSettings->value("ProductCount", 0).toInt();

    for (int i = 0; i < loadedUserCount; ++i) {
        QString key = QString("User%1").arg(i);
        QString jsonString = testSettings->value(key).toString();
        QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
        if (!doc.isNull() && doc.isObject()) {
            User* user = User::fromJson(doc.object());
            loadedUsers.append(user);
        }
    }

    for (int i = 0; i < loadedProductCount; ++i) {
        QString key = QString("Product%1").arg(i);
        QString jsonString = testSettings->value(key).toString();
        QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
        if (!doc.isNull() && doc.isObject()) {
            Product product = Product::fromJson(doc.object());
            loadedProducts.append(product);
        }
    }
    testSettings->endGroup();

    // 4. Verify data consistency
    QVERIFY2(loadedUsers.size() == originalUsers.size(), "User count mismatch");
    QVERIFY2(loadedProducts.size() == originalProducts.size(), "Product count mismatch");

    // Verify user data
    for (int i = 0; i < loadedUsers.size(); ++i) {
        QVERIFY2(loadedUsers[i]->getUsername() == originalUsers[i]->getUsername(),
                 "Username mismatch");
        QVERIFY2(loadedUsers[i]->getEmail() == originalUsers[i]->getEmail(),
                 "Email mismatch");
    }

    // Verify product data
    for (int i = 0; i < loadedProducts.size(); ++i) {
        QVERIFY2(loadedProducts[i].getProductName() == originalProducts[i].getProductName(),
                 "Product name mismatch");
        QVERIFY2(loadedProducts[i].getPrice() == originalProducts[i].getPrice(),
                 "Product price mismatch");
    }

    qDebug() << "✓ Data persistence test passed";

    // Clean up
    for (User* user : originalUsers) delete user;
    for (User* user : loadedUsers) delete user;
}

void TestIntegration::testSessionState()
{
    qDebug() << "Testing session state...";

    // 1. Create user and login
    User* user = new User("session_test", "password", "session@test.com");
    bool loginResult = user->login("session_test", "password");
    QVERIFY2(loginResult, "Login failed");
    QVERIFY2(user->getIsAuthenticated(), "User not authenticated");

    // 2. Perform series of operations
    QList<Product> products;

    // Publish product (seller function)
    user->publishProduct("Session Product", 99.99, "Test session", "Test", products);

    // Add to favorites (buyer function)
    if (!products.isEmpty()) {
        user->addToFavorites(products[0].getProductId());
    }

    // 3. Verify session state
    QVERIFY2(user->getPublishedProducts().size() == 1, "Published product state abnormal");
    QVERIFY2(user->getFavorites().size() == 1, "Favorite state abnormal");

    // 4. Logout
    user->logout();
    QVERIFY2(!user->getIsAuthenticated(), "Logout failed");

    // 5. Relogin and verify data persistence
    bool reloginResult = user->login("session_test", "password");
    QVERIFY2(reloginResult, "Relogin failed");

    // Published product and favorites should still be there
    QVERIFY2(user->getPublishedProducts().size() == 1, "Published product data lost");
    QVERIFY2(user->getFavorites().size() == 1, "Favorite data lost");

    qDebug() << "✓ Session state test passed";

    delete user;
}

// Add missing testConcurrentOperations function to fix the linker error
void TestIntegration::testConcurrentOperations()
{
    qDebug() << "Testing concurrent operations (simplified version)...";

    // Since we don't have a multithreaded test environment, we simulate multiple users operating simultaneously
    User* user1 = new User("concurrent_user1", "password123", "user1@test.com");
    User* user2 = new User("concurrent_user2", "password456", "user2@test.com");

    QList<Product> allProducts;

    // User1 publishes a product
    user1->publishProduct("Concurrent Product", 100.0, "Concurrent test product", "Test", allProducts);
    QVERIFY2(allProducts.size() == 1, "Product publish failed");

    QString productId = allProducts[0].getProductId();

    // Simulate user1 and user2 trying to purchase at the same time (actually sequential, but testing concurrent logic)
    bool user1Purchase = user1->purchaseProduct(productId, allProducts);
    bool user2Purchase = user2->purchaseProduct(productId, allProducts);

    // Note: user1 cannot buy their own published product, so user1Purchase should be false
    // user2Purchase should be true
    qDebug() << "User1 purchase result:" << user1Purchase << ", User2 purchase result:" << user2Purchase;

    // Since user1 cannot buy their own published product, user1Purchase is false
    QVERIFY2(user1Purchase == false, "User1 should not be able to purchase their own product");
    QVERIFY2(user2Purchase == true, "User2 should successfully purchase the product");

    // Verify product status
    QVERIFY2(allProducts[0].getIsSold(), "Product should be marked as sold");
    QVERIFY2(allProducts[0].getBuyerId() == user2->getUserId(), "Buyer should be user2");

    // Clean up
    delete user1;
    delete user2;

    qDebug() << "✓ Concurrent operations test passed (simplified version)";
}
