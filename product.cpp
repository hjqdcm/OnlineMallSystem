#include "product.h"
#include <QUuid>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>

Product::Product(const QString& name, double price, const QString& desc,
                 const QString& cat, const QString& sellerId, const QString& sellerName,
                 bool virtualProduct)
    : productId(QUuid::createUuid().toString()), productName(name), description(desc),
    price(price), category(cat), isPromotional(false), isVirtual(virtualProduct),
    sellerId(sellerId), sellerName(sellerName), buyerId(""), isSold(false)
{
    // 添加默认图片
    images.append(":/images/default_product.png");
}

QString Product::displayInfo() const {
    // 修改显示信息，使用卖家用户名而不是卖家ID
    QString info = QString("商品名称: %1\n价格: ¥%2\n描述: %3\n类别: %4\n卖家: %5\n状态: %6")
                       .arg(productName)
                       .arg(price)
                       .arg(description)
                       .arg(category)
                       .arg(sellerName)  // 使用卖家用户名
                       .arg(isSold ? QString("已售出(买家: %1)").arg(buyerId) : "待售");

    if (isPromotional) {
        info += "\n🔥 促销商品";
    }

    if (isVirtual) {
        info += "\n💻 虚拟商品";
    }

    return info;
}

// 序列化方法
QJsonObject Product::toJson() const {
    QJsonObject json;
    json["productId"] = productId;
    json["productName"] = productName;
    json["description"] = description;
    json["price"] = price;
    json["category"] = category;
    json["isPromotional"] = isPromotional;
    json["isVirtual"] = isVirtual;
    json["sellerId"] = sellerId;
    json["sellerName"] = sellerName;  // 新增
    json["buyerId"] = buyerId;
    json["isSold"] = isSold;

    // 序列化图片列表
    QJsonArray imagesArray;
    for (const QString& image : images) {
        imagesArray.append(image);
    }
    json["images"] = imagesArray;

    return json;
}

// 反序列化方法
Product Product::fromJson(const QJsonObject& json) {
    QString name = json["productName"].toString();
    double price = json["price"].toDouble();
    QString desc = json["description"].toString();
    QString category = json["category"].toString();
    QString sellerId = json["sellerId"].toString();

    // 处理旧的商品数据可能没有 sellerName 字段的情况
    QString sellerName = json.contains("sellerName") ?
                             json["sellerName"].toString() :
                             "未知卖家";  // 默认值

    bool isVirtual = json["isVirtual"].toBool();

    Product product(name, price, desc, category, sellerId, sellerName, isVirtual);

    // 设置其他属性
    product.productId = json["productId"].toString();
    product.isPromotional = json["isPromotional"].toBool();
    product.buyerId = json["buyerId"].toString();
    product.isSold = json["isSold"].toBool();

    // 反序列化图片列表
    QJsonArray imagesArray = json["images"].toArray();
    for (const QJsonValue& image : imagesArray) {
        product.images.append(image.toString());
    }

    return product;
}

// ProductCategory 类实现
ProductCategory::ProductCategory(const QString& name, const QString& parent)
    : categoryId(QUuid::createUuid().toString()), categoryName(name), parentCategory(parent)
{
}

QList<Product> ProductCategory::filterProducts(const QList<Product>& products, const QString& category) {
    QList<Product> filteredProducts;

    for (const Product& product : products) {
        if (product.getCategory() == category) {
            filteredProducts.append(product);
        }
    }

    return filteredProducts;
}
