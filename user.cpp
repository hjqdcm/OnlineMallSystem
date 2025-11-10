#include "user.h"
#include <QUuid>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "product.h"

// User 类实现
User::User(const QString& username, const QString& password, const QString& email)
    : userId(QUuid::createUuid().toString()), username(username), password(password),
    email(email), nickname(username), isAuthenticated(false), authenticationStatus("Approved")
{
}

bool User::login(const QString& inputUsername, const QString& inputPassword) {
    if (inputUsername == username && inputPassword == password) {
        isAuthenticated = true;
        qDebug() << "用户" << username << "登录成功";
        return true;
    }
    isAuthenticated = false;
    return false;
}

void User::logout() {
    isAuthenticated = false;
    qDebug() << "用户" << username << "已登出";
}

bool User::resetPassword(const QString& newPassword) {
    if (newPassword.length() < 6) {
        qDebug() << "密码长度不足6位";
        return false;
    }

    password = newPassword;
    qDebug() << "用户" << username << "密码已重置";
    return true;
}

void User::forgetPassword(const QString& inputEmail) {
    if (inputEmail == email) {
        qDebug() << "密码重置链接已发送到邮箱:" << email;
    } else {
        qDebug() << "邮箱地址不匹配";
    }
}

// 买家功能实现
QList<QString> User::searchProducts(const QString& keyword) {
    qDebug() << "用户" << username << "搜索商品:" << keyword;
    QList<QString> results;
    return results;
}

bool User::purchaseProduct(const QString& productId, QList<Product>& allProducts) {
    qDebug() << "用户" << username << "尝试购买商品:" << productId;

    // 在商品列表中查找商品
    for (Product& product : allProducts) {
        if (product.getProductId() == productId) {
            // 检查商品是否已售出
            if (product.getIsSold()) {
                qDebug() << "商品" << productId << "已被购买";
                return false;
            }

            // 购买商品
            product.setBuyerId(userId);
            product.setIsSold(true);
            purchaseHistory.append(productId);

            qDebug() << "用户" << username << "成功购买商品:" << productId;
            return true;
        }
    }

    qDebug() << "商品" << productId << "不存在";
    return false;
}

QList<QString> User::viewPurchaseHistory() {
    qDebug() << "用户" << username << "查看购买历史";
    return purchaseHistory;
}

void User::addToFavorites(const QString& productId) {
    if (!favorites.contains(productId)) {
        favorites.append(productId);
        qDebug() << "商品" << productId << "已添加到收藏夹";
    } else {
        qDebug() << "商品已在收藏夹中";
    }
}

QList<QString> User::viewFavorites() {
    qDebug() << "用户" << username << "查看收藏夹";
    return favorites;
}

// 卖家功能实现
bool User::publishProduct(const QString& productName, double price, const QString& description,
                          const QString& category, QList<Product>& allProducts, bool isVirtual) {
    if (authenticationStatus != "Approved") {
        qDebug() << "用户未通过认证，无法发布商品";
        return false;
    }

    // 创建新商品
    Product newProduct(productName, price, description, category, userId, username, isVirtual);
    QString productId = newProduct.getProductId();
    publishedProducts.append(productId);

    // 添加到商品列表
    allProducts.append(newProduct);

    qDebug() << "用户" << username << "发布商品:" << productName
             << "价格:" << price << "类别:" << category;

    qDebug() << "商品发布成功";
    return true;
}

bool User::publishVirtualProduct(const QString& productName, double price, const QString& description, QList<Product>& allProducts) {
    qDebug() << "用户" << username << "发布虚拟商品:" << productName;
    return publishProduct(productName, price, description, "Virtual", allProducts, true);
}

QList<QString> User::viewSoldProducts(const QList<Product>& allProducts) {
    qDebug() << "用户" << username << "查看已售商品";
    QList<QString> soldProducts;

    for (const Product& product : allProducts) {
        if (product.getSellerId() == userId && product.getIsSold()) {
            soldProducts.append(product.getProductName() + " - 买家: " + product.getBuyerId());
        }
    }

    return soldProducts;
}

QList<QString> User::viewSalesStatus(const QList<Product>& allProducts) {
    qDebug() << "用户" << username << "查看销售状态";

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
    salesStatus.append(QString("总销售额: ¥%1").arg(totalRevenue));
    salesStatus.append(QString("已售商品: %1件").arg(totalSold));
    salesStatus.append(QString("待售商品: %1件").arg(totalForSale));

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
