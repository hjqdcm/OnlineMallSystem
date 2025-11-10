#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>
#include <QJsonObject>

class User {
private:
    QString userId;
    QString username;
    QString password;
    QString email;
    QString nickname;
    bool isAuthenticated;

    // 买家相关属性
    QList<QString> favorites; // 收藏的商品ID列表
    QList<QString> purchaseHistory; // 购买历史

    // 卖家相关属性
    QString authenticationStatus;
    QList<QString> publishedProducts; // 发布的商品

public:
    User(const QString& username, const QString& password, const QString& email);

    // 基本功能
    bool login(const QString& username, const QString& password);
    void logout();
    bool resetPassword(const QString& newPassword);
    void forgetPassword(const QString& email);

    // Getter方法
    QString getUserId() const { return userId; }
    QString getUsername() const { return username; }
    QString getPassword() const { return password; }
    QString getEmail() const { return email; }
    QString getNickname() const { return nickname; }
    bool getIsAuthenticated() const { return isAuthenticated; }
    QList<QString> getPurchaseHistory() const { return purchaseHistory; }
    QList<QString> getPublishedProducts() const { return publishedProducts; }
    QList<QString> getFavorites() const { return favorites; }

    // 设置昵称
    void setNickname(const QString& newNickname) { nickname = newNickname; }

    // 买家功能
    QList<QString> searchProducts(const QString& keyword);
    bool purchaseProduct(const QString& productId, QList<class Product>& allProducts);
    QList<QString> viewPurchaseHistory();
    void addToFavorites(const QString& productId);
    QList<QString> viewFavorites();

    // 卖家功能
    bool publishProduct(const QString& productName, double price, const QString& description,
                        const QString& category, QList<class Product>& allProducts, bool isVirtual = false);
    bool publishVirtualProduct(const QString& productName, double price, const QString& description, QList<class Product>& allProducts);
    QList<QString> viewSoldProducts(const QList<class Product>& allProducts);
    QList<QString> viewSalesStatus(const QList<class Product>& allProducts);
    QString getAuthStatus() const { return authenticationStatus; }

    // 序列化和反序列化方法
    QJsonObject toJson() const;
    static User* fromJson(const QJsonObject& json);
};

#endif // USER_H
