#include "logindialog.h"
#include "ui_logindialog.h"

#include "shopstate.h"

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

bool LoginDialog::waitLogin()
{
    ShopCustomer*   pCust = ShopState::pShop->curCustomer();
    if(pCust)
        ui->lineLogin->setText(pCust->nickname);
    else
        ui->lineLogin->clear();

    ui->linePassword->clear();
    if(exec() == QDialog::Rejected)
        return false;

    return true;
}

void LoginDialog::login()
{
    QString psw = ui->linePassword->text();

    if(ShopState::pShop->login(ui->lineLogin->text(), qHash(psw)))
        accept();
}
