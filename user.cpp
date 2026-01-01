#include "user.h"
#include <QUuid>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "product.h"

// User class implementation
User::User(const QString& username, const QString& password, const QString& email)
    : userId(QUuid::createUuid().toString()), username(username), password(password),
    email(email), nickname(username), isAuthenticated(false), authenticationStatus("Approved")
{
}

bool User::login(const QString& inputUsername, const QString& inputPassword) {
    if (inputUsername == username && inputPassword == password) {
        isAuthenticated = true;
        qDebug() << "User" << username << "login successfully";
        return true;
    }
    isAuthenticated = false;
    return false;
}

void User::logout() {
    isAuthenticated = false;
    qDebug() << "User" << username << "logged out";
}

bool User::resetPassword(const QString& newPassword) {
    if (newPassword.length() < 6) {
        qDebug() << "Password length must be at least 6 characters";
        return false;
    }

    password = newPassword;
    qDebug() << "User" << username << "password has been reset";
    return true;
}

void User::forgetPassword(const QString& inputEmail) {
    if (inputEmail == email) {
        qDebug() << "Password reset link has been sent to email:" << email;
    } else {
        qDebug() << "Email address does not match";
    }
}

// Buyer functions implementation
QList<QString> User::searchProducts(const QString& keyword) {
    qDebug() << "User" << username << "search for products:" << keyword;
    QList<QString> results;
    return results;
}

bool User::purchaseProduct(const QString& productId, QList<Product>& allProducts) {
    qDebug() << "User" << username << "attempts to purchase product:" << productId;

    // Find the product in the product list
    for (Product& product : allProducts) {
        if (product.getProductId() == productId) {
            // Check if the product is already sold
            if (product.getIsSold()) {
                qDebug() << "Product" << productId << "has already been sold";
                return false;
            }

            // Purchase the product
            product.setBuyerId(userId);
            product.setIsSold(true);
            purchaseHistory.append(productId);

            qDebug() << "User" << username << "successfully purchased product:" << productId;
            return true;
        }
    }

    qDebug() << "Product" << productId << "does not exist";
    return false;
}

QList<QString> User::viewPurchaseHistory() {
    qDebug() << "User" << username << "views purchase history";
    return purchaseHistory;
}

void User::addToFavorites(const QString& productId) {
    if (!favorites.contains(productId)) {
        favorites.append(productId);
        qDebug() << "Product" << productId << "has been added to favorites";
    } else {
        qDebug() << "Product is already in favorites";
    }
}

QList<QString> User::viewFavorites() {
    qDebug() << "User" << username << "views favorites";
    return favorites;
}

// Seller functions implementation
bool User::publishProduct(const QString& productName, double price, const QString& description,
                          const QString& category, QList<Product>& allProducts, bool isVirtual) {
    if (authenticationStatus != "Approved") {
        qDebug() << "User not authenticated, cannot publish product";
        return false;
    }

    // Create new product
    Product newProduct(productName, price, description, category, userId, username, isVirtual);
    QString productId = newProduct.getProductId();
    publishedProducts.append(productId);

    // Add to product list
    allProducts.append(newProduct);

    qDebug() << "User" << username << "publishes product:" << productName
             << "price:" << price << "category:" << category;

    qDebug() << "Product published successfully";
    return true;
}

bool User::publishVirtualProduct(const QString& productName, double price, const QString& description, QList<Product>& allProducts) {
    qDebug() << "User" << username << "publishes virtual product:" << productName;
    return publishProduct(productName, price, description, "Virtual", allProducts, true);
}

QList<QString> User::viewSoldProducts(const QList<Product>& allProducts) {
    qDebug() << "User" << username << "views sold products";
    QList<QString> soldProducts;

    for (const Product& product : allProducts) {
        if (product.getSellerId() == userId && product.getIsSold()) {
            soldProducts.append(product.getProductName() + " - Buyer: " + product.getBuyerId());
        }
    }

    return soldProducts;
}

QList<QString> User::viewSalesStatus(const QList<Product>& allProducts) {
    qDebug() << "User" << username << "views sales status";

    int totalSold = 0;
    int totalForSale = 0;
    double totalRevenue = 0.0;

    for (const Product& product : allProducts) {
        if (product.getSellerId() == userId) {
            if (product.getIsSold()) {
                totalSold++;
                totalRevenue += product.getPrice();
            } else {
                totalForSale++;
            }
        }
    }

    QList<QString> salesStatus;
    salesStatus.append(QString("Total revenue: ¥%1").arg(totalRevenue));
    salesStatus.append(QString("Sold products: %1 items").arg(totalSold));
    salesStatus.append(QString("Products for sale: %1 items").arg(totalForSale));

    return salesStatus;
}

// 序列化方法
QJsonObject User::toJson() const {
    QJsonObject json;
    json["userId"] = userId;
    json["username"] = username;
    json["password"] = password;
    json["email"] = email;
    json["nickname"] = nickname;
    json["isAuthenticated"] = isAuthenticated;
    json["authenticationStatus"] = authenticationStatus;

    // 序列化收藏列表
    QJsonArray favoritesArray;
    for (const QString& fav : favorites) {
        favoritesArray.append(fav);
    }
    json["favorites"] = favoritesArray;

    // 序列化购买历史
    QJsonArray purchaseHistoryArray;
    for (const QString& purchase : purchaseHistory) {
        purchaseHistoryArray.append(purchase);
    }
    json["purchaseHistory"] = purchaseHistoryArray;

    // 序列化发布的商品
    QJsonArray publishedProductsArray;
    for (const QString& product : publishedProducts) {
        publishedProductsArray.append(product);
    }
    json["publishedProducts"] = publishedProductsArray;

    return json;
}

// 反序列化方法
User* User::fromJson(const QJsonObject& json) {
    QString username = json["username"].toString();
    QString password = json["password"].toString();
    QString email = json["email"].toString();

    User* user = new User(username, password, email);
    user->userId = json["userId"].toString();
    user->nickname = json["nickname"].toString();
    user->isAuthenticated = json["isAuthenticated"].toBool();
    user->authenticationStatus = json["authenticationStatus"].toString();

    // 反序列化收藏列表
    if (json.contains("favorites")) {
        QJsonArray favoritesArray = json["favorites"].toArray();
        for (const QJsonValue& fav : favoritesArray) {
            user->favorites.append(fav.toString());
        }
    }

    // 反序列化购买历史
    if (json.contains("purchaseHistory")) {
        QJsonArray purchaseHistoryArray = json["purchaseHistory"].toArray();
        for (const QJsonValue& purchase : purchaseHistoryArray) {
            user->purchaseHistory.append(purchase.toString());
        }
    }

    // 反序列化发布的商品
    if (json.contains("publishedProducts")) {
        QJsonArray publishedProductsArray = json["publishedProducts"].toArray();
        for (const QJsonValue& product : publishedProductsArray) {
            user->publishedProducts.append(product.toString());
        }
    }

    return user;
}
