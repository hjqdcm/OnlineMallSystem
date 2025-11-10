#include "mainwindow.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , currentUser(nullptr)
    , settings(new QSettings("MyCompany", "OnlineMall", this))
{
    setupUI();
    setupConnections();
    loadUsersFromSettings();
    loadProductsFromSettings();
    showLoginScreen();

    // 如果没有加载到商品，添加一些示例商品
    if (allProducts.isEmpty()) {
        // 修改示例商品，使用有意义的卖家用户名
        allProducts.append(Product("笔记本电脑", 4999.99, "高性能游戏笔记本，适合编程和游戏", "电子产品", "system", "官方商城"));
        allProducts.append(Product("智能手机", 2999.99, "最新款智能手机，拍照清晰", "电子产品", "system", "官方商城"));
        allProducts.append(Product("编程书籍", 59.99, "C++编程指南，适合初学者", "图书", "system", "官方商城"));
        allProducts.append(Product("无线耳机", 399.99, "蓝牙无线耳机，音质出色", "电子产品", "system", "官方商城"));
        allProducts.append(Product("办公椅", 899.99, "人体工学办公椅，舒适耐用", "家具", "system", "官方商城"));

        // 保存示例商品
        saveProductsToSettings();
    }

    populateProductList();
}

MainWindow::~MainWindow()
{
    saveUsersToSettings(); // 程序关闭前保存用户数据
    saveProductsToSettings(); // 保存商品数据
    // 清理内存
    for(User* user : allUsers) {
        delete user;
    }
}

void MainWindow::saveUsersToSettings()
{
    // 清除之前的用户数据
    settings->remove("users");

    // 保存用户数量
    settings->setValue("userCount", allUsers.size());

    // 保存每个用户的信息
    for (int i = 0; i < allUsers.size(); ++i) {
        QString prefix = "users/user" + QString::number(i);
        settings->setValue(prefix + "/username", allUsers[i]->getUsername());
        settings->setValue(prefix + "/password", allUsers[i]->getPassword());
        settings->setValue(prefix + "/email", allUsers[i]->getEmail());
    }

    qDebug() << "用户数据已保存";
}

void MainWindow::loadUsersFromSettings()
{
    // 获取保存的用户数量
    int userCount = settings->value("userCount", 0).toInt();

    if (userCount == 0) {
        // 如果没有保存的用户，创建一个默认测试用户
        User* defaultUser = new User("test", "123456", "test@example.com");
        allUsers.append(defaultUser);
        qDebug() << "创建默认测试用户";
        return;
    }

    // 加载每个用户
    for (int i = 0; i < userCount; ++i) {
        QString prefix = "users/user" + QString::number(i);
        QString username = settings->value(prefix + "/username").toString();
        QString password = settings->value(prefix + "/password").toString();
        QString email = settings->value(prefix + "/email").toString();

        if (!username.isEmpty() && !password.isEmpty()) {
            User* user = new User(username, password, email);
            allUsers.append(user);
            qDebug() << "加载用户:" << username;
        }
    }

    qDebug() << "共加载" << allUsers.size() << "个用户";
}

void MainWindow::saveProductsToSettings()
{
    // 清除之前的商品数据
    settings->remove("products");

    // 保存商品数量
    settings->setValue("productCount", allProducts.size());

    // 保存每个商品的信息
    for (int i = 0; i < allProducts.size(); ++i) {
        QString prefix = "products/product" + QString::number(i);
        QJsonObject productJson = allProducts[i].toJson();
        QString jsonString = QString::fromUtf8(QJsonDocument(productJson).toJson(QJsonDocument::Compact));
        settings->setValue(prefix, jsonString);
    }

    qDebug() << "商品数据已保存，共" << allProducts.size() << "个商品";
}

// 从 QSettings 加载商品数据
void MainWindow::loadProductsFromSettings()
{
    // 获取保存的商品数量
    int productCount = settings->value("productCount", 0).toInt();

    if (productCount == 0) {
        qDebug() << "没有保存的商品数据";
        return;
    }

    // 加载每个商品
    for (int i = 0; i < productCount; ++i) {
        QString prefix = "products/product" + QString::number(i);
        QString jsonString = settings->value(prefix).toString();

        if (!jsonString.isEmpty()) {
            QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!doc.isNull() && doc.isObject()) {
                Product product = Product::fromJson(doc.object());
                allProducts.append(product);
                qDebug() << "加载商品:" << product.getProductName();
            }
        }
    }

    qDebug() << "共加载" << allProducts.size() << "个商品";
}

void MainWindow::setupUI()
{
    // 创建中央部件
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // ====== 登录页面 ======
    loginPage = new QWidget();
    QVBoxLayout *loginLayout = new QVBoxLayout(loginPage);

    loginLayout->setContentsMargins(50, 100, 50, 130);
    loginLayout->setSpacing(15);

    QLabel *loginTitle = new QLabel("网络商场系统");
    loginTitle->setAlignment(Qt::AlignCenter);
    loginTitle->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color: #333; }");

    loginUsernameEdit = new QLineEdit();
    loginUsernameEdit->setPlaceholderText("用户名");

    loginPasswordEdit = new QLineEdit();
    loginPasswordEdit->setPlaceholderText("密码");
    loginPasswordEdit->setEchoMode(QLineEdit::Password);

    loginBtn = new QPushButton("登录");
    switchToRegisterBtn = new QPushButton("注册新账户");

    loginLayout->addWidget(loginTitle);
    loginLayout->addWidget(loginUsernameEdit);
    loginLayout->addWidget(loginPasswordEdit);
    loginLayout->addWidget(loginBtn);
    loginLayout->addWidget(switchToRegisterBtn);

    // ====== 注册页面 ======
    registerPage = new QWidget();
    QVBoxLayout *registerLayout = new QVBoxLayout(registerPage);

    registerLayout->setContentsMargins(50, 100, 50, 130);
    registerLayout->setSpacing(15);

    QLabel *registerTitle = new QLabel("注册新账户");
    registerTitle->setAlignment(Qt::AlignCenter);
    registerTitle->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color: #333; }");

    registerUsernameEdit = new QLineEdit();
    registerUsernameEdit->setPlaceholderText("用户名");

    registerPasswordEdit = new QLineEdit();
    registerPasswordEdit->setPlaceholderText("密码");
    registerPasswordEdit->setEchoMode(QLineEdit::Password);

    registerConfirmPasswordEdit = new QLineEdit();
    registerConfirmPasswordEdit->setPlaceholderText("确认密码");
    registerConfirmPasswordEdit->setEchoMode(QLineEdit::Password);

    registerEmailEdit = new QLineEdit();
    registerEmailEdit->setPlaceholderText("邮箱");

    registerBtn = new QPushButton("注册");
    switchToLoginBtn = new QPushButton("返回登录");

    registerLayout->addWidget(registerTitle);
    registerLayout->addWidget(registerUsernameEdit);
    registerLayout->addWidget(registerPasswordEdit);
    registerLayout->addWidget(registerConfirmPasswordEdit);
    registerLayout->addWidget(registerEmailEdit);
    registerLayout->addWidget(registerBtn);
    registerLayout->addWidget(switchToLoginBtn);

    // ====== 主页面 ======
    mainPage = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(mainPage);

    welcomeLabel = new QLabel("欢迎!");

    // 创建选项卡
    mainTabWidget = new QTabWidget();

    // ====== 买家选项卡 ======
    buyerTab = new QWidget();
    QVBoxLayout *buyerLayout = new QVBoxLayout(buyerTab);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    buyerSearchEdit = new QLineEdit();
    buyerSearchEdit->setPlaceholderText("搜索商品...");
    buyerSearchBtn = new QPushButton("搜索");
    searchLayout->addWidget(buyerSearchEdit);
    searchLayout->addWidget(buyerSearchBtn);

    buyerProductList = new QListWidget();
    buyerProductInfo = new QTextEdit();
    buyerProductInfo->setReadOnly(true);

    QHBoxLayout *buyerButtonsLayout = new QHBoxLayout();
    buyerPurchaseBtn = new QPushButton("购买");
    buyerFavoriteBtn = new QPushButton("收藏");
    buyerHistoryBtn = new QPushButton("购买历史");

    buyerButtonsLayout->addWidget(buyerPurchaseBtn);
    buyerButtonsLayout->addWidget(buyerFavoriteBtn);
    buyerButtonsLayout->addWidget(buyerHistoryBtn);

    buyerLayout->addLayout(searchLayout);
    buyerLayout->addWidget(buyerProductList);
    buyerLayout->addWidget(buyerProductInfo);
    buyerLayout->addLayout(buyerButtonsLayout);

    // ====== 卖家选项卡 ======
    sellerTab = new QWidget();
    QVBoxLayout *sellerLayout = new QVBoxLayout(sellerTab);

    QLabel *publishTitle = new QLabel("发布新商品");

    productNameEdit = new QLineEdit();
    productNameEdit->setPlaceholderText("商品名称");

    productPriceEdit = new QLineEdit();
    productPriceEdit->setPlaceholderText("价格");

    productCategoryEdit = new QLineEdit();
    productCategoryEdit->setPlaceholderText("类别");

    productDescEdit = new QTextEdit();
    productDescEdit->setPlaceholderText("商品描述");

    virtualProductCheck = new QCheckBox("虚拟商品");

    QHBoxLayout *sellerButtonsLayout = new QHBoxLayout();
    sellerPublishBtn = new QPushButton("发布商品");
    sellerSalesBtn = new QPushButton("销售状态");

    sellerButtonsLayout->addWidget(sellerPublishBtn);
    sellerButtonsLayout->addWidget(sellerSalesBtn);

    sellerLayout->addWidget(publishTitle);
    sellerLayout->addWidget(productNameEdit);
    sellerLayout->addWidget(productPriceEdit);
    sellerLayout->addWidget(productCategoryEdit);
    sellerLayout->addWidget(productDescEdit);
    sellerLayout->addWidget(virtualProductCheck);
    sellerLayout->addLayout(sellerButtonsLayout);

    // 添加选项卡
    mainTabWidget->addTab(buyerTab, "购买商品");
    mainTabWidget->addTab(sellerTab, "发布商品");

    // 退出登录按钮
    logoutBtn = new QPushButton("退出登录");

    mainLayout->addWidget(welcomeLabel);
    mainLayout->addWidget(mainTabWidget);
    mainLayout->addWidget(logoutBtn);

    // 将所有页面添加到stackedWidget
    stackedWidget->addWidget(loginPage);
    stackedWidget->addWidget(registerPage);
    stackedWidget->addWidget(mainPage);

    // 设置窗口大小
    setWindowTitle("网络商场系统");
    resize(700, 600);
}

void MainWindow::setupConnections()
{
    // 登录注册按钮
    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(registerBtn, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    connect(switchToRegisterBtn, &QPushButton::clicked, this, &MainWindow::onSwitchToRegisterClicked);
    connect(switchToLoginBtn, &QPushButton::clicked, this, &MainWindow::onSwitchToLoginClicked);

    // 买家功能按钮
    connect(buyerSearchBtn, &QPushButton::clicked, this, &MainWindow::onSearchProducts);
    connect(buyerPurchaseBtn, &QPushButton::clicked, this, &MainWindow::onPurchaseProduct);
    connect(buyerHistoryBtn, &QPushButton::clicked, this, &MainWindow::onViewPurchaseHistory);
    connect(buyerFavoriteBtn, &QPushButton::clicked, this, &MainWindow::onAddToFavorites);

    // 卖家功能按钮
    connect(sellerPublishBtn, &QPushButton::clicked, this, &MainWindow::onPublishProduct);
    connect(sellerSalesBtn, &QPushButton::clicked, this, &MainWindow::onViewSalesStatus);

    // 退出登录
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::showLoginScreen);

    // 商品列表选择
    connect(buyerProductList, &QListWidget::itemSelectionChanged, this, [this]() {
        QListWidgetItem* item = buyerProductList->currentItem();
        if (item) {
            QString productId = item->data(Qt::UserRole).toString();
            for (const Product& product : allProducts) {
                if (product.getProductId() == productId) {
                    buyerProductInfo->setText(product.displayInfo());
                    break;
                }
            }
        }
    });
}

void MainWindow::showLoginScreen()
{
    stackedWidget->setCurrentIndex(0);
    loginUsernameEdit->clear();
    loginPasswordEdit->clear();

    // 清理当前用户
    currentUser = nullptr;
}

void MainWindow::showRegisterScreen()
{
    stackedWidget->setCurrentIndex(1);
    registerUsernameEdit->clear();
    registerPasswordEdit->clear();
    registerConfirmPasswordEdit->clear();
    registerEmailEdit->clear();
}

void MainWindow::showMainScreen()
{
    stackedWidget->setCurrentIndex(2);
    welcomeLabel->setText(QString("欢迎, %1!").arg(currentUser->getUsername()));
    populateProductList();
}

void MainWindow::onLoginClicked()
{
    QString username = loginUsernameEdit->text();
    QString password = loginPasswordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "登录失败", "请输入用户名和密码");
        return;
    }

    // 检查用户列表中是否有匹配的用户
    bool loginSuccess = false;
    for (User* user : allUsers) {
        if (user->getUsername() == username && user->getPassword() == password) {
            currentUser = user;
            currentUser->login(username, password);
            loginSuccess = true;
            break;
        }
    }

    if (loginSuccess) {
        showMainScreen();
        QMessageBox::information(this, "登录成功", "登录成功！");
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误");
    }
}

void MainWindow::onRegisterClicked()
{
    QString username = registerUsernameEdit->text();
    QString password = registerPasswordEdit->text();
    QString confirmPassword = registerConfirmPasswordEdit->text();
    QString email = registerEmailEdit->text();

    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "请填写所有字段");
        return;
    }

    // 检查密码是否一致
    if (password != confirmPassword) {
        QMessageBox::warning(this, "注册失败", "两次输入的密码不一致");
        return;
    }

    // 检查密码长度
    if (password.length() < 6) {
        QMessageBox::warning(this, "注册失败", "密码长度至少为6位");
        return;
    }

    // 检查用户名是否已存在
    for (User* user : allUsers) {
        if (user->getUsername() == username) {
            QMessageBox::warning(this, "注册失败", "用户名已存在");
            return;
        }
    }

    // 创建用户（同时具备买家和卖家功能）
    User* newUser = new User(username, password, email);
    allUsers.append(newUser);

    // 立即保存用户数据
    saveUsersToSettings();

    QMessageBox::information(this, "注册成功", "账户创建成功！请使用新账户登录");

    // 注册成功后回到登录页面
    showLoginScreen();
}

void MainWindow::onSwitchToRegisterClicked()
{
    showRegisterScreen();
}

void MainWindow::onSwitchToLoginClicked()
{
    showLoginScreen();
}

void MainWindow::populateProductList()
{
    buyerProductList->clear();
    for (const Product& product : allProducts) {
        // 只显示未售出的商品
        if (!product.getIsSold()) {
            QString itemText = QString("%1 - ¥%2 - %3 - 卖家: %4")
                                   .arg(product.getProductName())
                                   .arg(product.getPrice())
                                   .arg(product.getCategory())
                                   .arg(product.getSellerName());  // 使用卖家用户名
            QListWidgetItem* item = new QListWidgetItem(itemText);
            item->setData(Qt::UserRole, product.getProductId());
            buyerProductList->addItem(item);
        }
    }
}

void MainWindow::onSearchProducts()
{
    QString keyword = buyerSearchEdit->text();
    if (keyword.isEmpty()) {
        populateProductList(); // 显示所有商品
        return;
    }

    buyerProductList->clear();
    for (const Product& product : allProducts) {
        if (product.getProductName().contains(keyword, Qt::CaseInsensitive) ||
            product.getDescription().contains(keyword, Qt::CaseInsensitive) ||
            product.getCategory().contains(keyword, Qt::CaseInsensitive)) {

            QString itemText = QString("%1 - ¥%2 - %3")
                                   .arg(product.getProductName())
                                   .arg(product.getPrice())
                                   .arg(product.getCategory());
            QListWidgetItem* item = new QListWidgetItem(itemText);
            item->setData(Qt::UserRole, product.getProductId());
            buyerProductList->addItem(item);
        }
    }

    QMessageBox::information(this, "搜索完成",
                             QString("找到 %1 个相关商品").arg(buyerProductList->count()));
}

void MainWindow::onPurchaseProduct()
{
    QListWidgetItem* currentItem = buyerProductList->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "购买失败", "请选择要购买的商品");
        return;
    }

    // 获取选中的商品ID
    QString productId = currentItem->data(Qt::UserRole).toString();

    // 查找商品信息
    Product* selectedProduct = nullptr;
    for (Product& product : allProducts) {
        if (product.getProductId() == productId) {
            selectedProduct = &product;
            break;
        }
    }

    if (!selectedProduct) {
        QMessageBox::warning(this, "购买失败", "商品不存在");
        return;
    }

    // 检查商品是否已售出
    if (selectedProduct->getIsSold()) {
        QMessageBox::warning(this, "购买失败", "该商品已被购买");
        return;
    }

    // 检查当前用户是否是商品的卖家
    if (currentUser && selectedProduct->getSellerId() == currentUser->getUserId()) {
        QMessageBox::warning(this, "购买失败", "不能购买自己发布的商品");
        return;
    }

    if (currentUser && currentUser->purchaseProduct(currentItem->data(Qt::UserRole).toString(), allProducts)) {
        QMessageBox::information(this, "购买成功", "商品购买成功！");

        // 保存商品数据（更新购买状态）
        saveProductsToSettings();

        // 刷新商品列表，已购买的商品将不再显示
        populateProductList();
    } else {
        QMessageBox::warning(this, "购买失败", "购买失败，商品可能已被其他人购买");
    }
}

void MainWindow::onViewPurchaseHistory()
{
    if (currentUser) {
        QList<QString> history = currentUser->viewPurchaseHistory();
        QString historyText = "购买历史:\n";
        if (history.isEmpty()) {
            historyText += "暂无购买记录";
        } else {
            for (const QString& productId : history) {
                // 在商品列表中查找商品信息
                for (const Product& product : allProducts) {
                    if (product.getProductId() == productId) {
                        historyText += QString("%1 - ¥%2 - 卖家: %3\n")
                                           .arg(product.getProductName())
                                           .arg(product.getPrice())
                                           .arg(product.getSellerId());
                        break;
                    }
                }
            }
        }
        QMessageBox::information(this, "购买历史", historyText);
    }
}

void MainWindow::onAddToFavorites()
{
    QListWidgetItem* currentItem = buyerProductList->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "操作失败", "请选择要收藏的商品");
        return;
    }

    if (currentUser) {
        currentUser->addToFavorites(currentItem->data(Qt::UserRole).toString());
        QMessageBox::information(this, "收藏成功", "商品已添加到收藏夹！");
    }
}

void MainWindow::onPublishProduct()
{
    QString name = productNameEdit->text();
    QString priceText = productPriceEdit->text();
    QString description = productDescEdit->toPlainText();
    QString category = productCategoryEdit->text();

    if (name.isEmpty() || priceText.isEmpty() || category.isEmpty()) {
        QMessageBox::warning(this, "发布失败", "请填写商品名称、价格和类别");
        return;
    }

    bool ok;
    double price = priceText.toDouble(&ok);
    if (!ok || price <= 0) {
        QMessageBox::warning(this, "发布失败", "请输入有效的价格");
        return;
    }

    bool isVirtual = virtualProductCheck->isChecked();

    if (currentUser) {
        bool success;
        if (isVirtual) {
            success = currentUser->publishVirtualProduct(name, price, description, allProducts);
        } else {
            success = currentUser->publishProduct(name, price, description, category, allProducts);
        }

        if (success) {
            QMessageBox::information(this, "发布成功", "商品发布成功！");

            // 清空输入框
            productNameEdit->clear();
            productPriceEdit->clear();
            productDescEdit->clear();
            productCategoryEdit->clear();
            virtualProductCheck->setChecked(false);

            // 保存商品数据
            saveProductsToSettings();

            // 刷新商品列表
            populateProductList();
        } else {
            QMessageBox::warning(this, "发布失败", "商品发布失败");
        }
    }
}

void MainWindow::onViewSalesStatus()
{
    if (currentUser) {
        QList<QString> status = currentUser->viewSalesStatus(allProducts);
        QString statusText = "销售状态:\n";
        for (const QString& line : status) {
            statusText += line + "\n";
        }
        QMessageBox::information(this, "销售状态", statusText);

        // 显示已售商品详情
        QList<QString> soldProducts = currentUser->viewSoldProducts(allProducts);
        if (!soldProducts.isEmpty()) {
            statusText += "\n已售商品:\n";
            for (const QString& product : soldProducts) {
                statusText += product + "\n";
            }
            QMessageBox::information(this, "销售详情", statusText);
        }
    }
}
