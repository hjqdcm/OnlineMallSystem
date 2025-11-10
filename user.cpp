#include "user.h"
#include <QUuid>
#include <QDebug>

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

void User::forgetPassword(const QString& inputEmail) {
    if (inputEmail == email) {
        qDebug() << "密码重置链接已发送到邮箱:" << email;
        // 这里应该发送密码重置邮件
    } else {
        qDebug() << "邮箱地址不匹配";
    }
}

// 买家功能实现
QList<QString> User::searchProducts(const QString& keyword) {
    qDebug() << "用户" << username << "搜索商品:" << keyword;
    QList<QString> results;
    // 这里应该实现实际的搜索逻辑
    // 暂时返回空列表
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

    // 创建新商品，传入卖家用户名
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
