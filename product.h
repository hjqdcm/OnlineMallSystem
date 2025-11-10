#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonDocument>

class Product {
private:
    QString productId;
    QString productName;
    QString description;
    double price;
    QList<QString> images;
    QString category;
    bool isPromotional;
    bool isVirtual;
    QString sellerId; // 卖家ID
    QString sellerName;
    QString buyerId;  // 买家ID，如果为空则表示未售出
    bool isSold;      // 是否已售出

public:
    Product(const QString& name, double price, const QString& desc,
            const QString& category, const QString& sellerId, const QString& sellerName, bool isVirtual = false);

    // Getter方法
    QString getProductId() const { return productId; }
    QString getProductName() const { return productName; }
    QString getDescription() const { return description; }
    double getPrice() const { return price; }
    QList<QString> getImages() const { return images; }
    QString getCategory() const { return category; }
    bool getIsPromotional() const { return isPromotional; }
    bool getIsVirtual() const { return isVirtual; }
    QString getSellerId() const { return sellerId; }
    QString getSellerName() const { return sellerName; }
    QString getBuyerId() const { return buyerId; }
    bool getIsSold() const { return isSold; }

    // Setter方法
    void setBuyerId(const QString& buyer) { buyerId = buyer; }
    void setIsSold(bool sold) { isSold = sold; }

    // 显示商品信息
    QString displayInfo() const;

    // 设置促销状态
    void setPromotional(bool promotional) { isPromotional = promotional; }

    // 添加图片
    void addImage(const QString& imagePath) { images.append(imagePath); }

    // 序列化和反序列化方法
    QJsonObject toJson() const;
    static Product fromJson(const QJsonObject& json);
};

class ProductCategory {
private:
    QString categoryId;
    QString categoryName;
    QString parentCategory;

public:
    ProductCategory(const QString& name, const QString& parent = "");

    // 商品筛选
    static QList<Product> filterProducts(const QList<Product>& products, const QString& category);

    QString getCategoryName() const { return categoryName; }
    QString getParentCategory() const { return parentCategory; }
};

#endif // PRODUCT_H
