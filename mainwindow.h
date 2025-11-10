#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QSettings>
#include "user.h"
#include "product.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 登录相关
    void onLoginClicked();
    void onRegisterClicked();
    void onSwitchToRegisterClicked();
    void onSwitchToLoginClicked();

    // 买家功能
    void onSearchProducts();
    void onPurchaseProduct();
    void onViewPurchaseHistory();
    void onAddToFavorites();

    // 卖家功能
    void onPublishProduct();
    void onViewSalesStatus();

private:
    QSettings *settings;
    void saveUsersToSettings();
    void loadUsersFromSettings();
    void saveProductsToSettings();
    void loadProductsFromSettings();
    // 中央部件和布局
    QStackedWidget *stackedWidget;

    // 登录页面部件
    QWidget *loginPage;
    QLineEdit *loginUsernameEdit;
    QLineEdit *loginPasswordEdit;
    QPushButton *loginBtn;
    QPushButton *switchToRegisterBtn;

    // 注册页面部件
    QWidget *registerPage;
    QLineEdit *registerUsernameEdit;
    QLineEdit *registerPasswordEdit;
    QLineEdit *registerConfirmPasswordEdit;
    QLineEdit *registerEmailEdit;
    QPushButton *registerBtn;
    QPushButton *switchToLoginBtn;

    // 主页面部件 - 使用选项卡
    QWidget *mainPage;
    QTabWidget *mainTabWidget;

    // 买家选项卡
    QWidget *buyerTab;
    QLabel *welcomeLabel;
    QLineEdit *buyerSearchEdit;
    QPushButton *buyerSearchBtn;
    QListWidget *buyerProductList;
    QTextEdit *buyerProductInfo;
    QPushButton *buyerPurchaseBtn;
    QPushButton *buyerFavoriteBtn;
    QPushButton *buyerHistoryBtn;

    // 卖家选项卡
    QWidget *sellerTab;
    QLineEdit *productNameEdit;
    QLineEdit *productPriceEdit;
    QLineEdit *productCategoryEdit;
    QTextEdit *productDescEdit;
    QCheckBox *virtualProductCheck;
    QPushButton *sellerPublishBtn;
    QPushButton *sellerSalesBtn;

    // 通用按钮
    QPushButton *logoutBtn;

    // 业务逻辑对象
    User* currentUser;
    QList<Product> allProducts;
    QList<User*> allUsers;

    void setupUI();
    void setupConnections();
    void showLoginScreen();
    void showRegisterScreen();
    void showMainScreen();
    void populateProductList();
};

#endif // MAINWINDOW_H
