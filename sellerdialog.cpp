#include "sellerdialog.h"
#include "ui_sellerdialog.h"

#include "shopstate.h"
#include <QMessageBox>

SellerDialog::SellerDialog(QWidget *parent) : QDialog(parent),  ui(new Ui::SellerDialog)
{
    ui->setupUi(this);
}

SellerDialog::~SellerDialog()
{
    delete ui;
}

void SellerDialog::setSeller(ShopSeller *pSeller)
{
    ui->burthdayDateEdit->setDate(pSeller->burthday);
    ui->nameLineEdit->setText(pSeller->name);
    ui->nicknameLineEdit->setText(pSeller->nickname);
    ui->passwordLineEdit->clear();
    ui->patronymicLineEdit->setText(pSeller->patronymic);
    ui->surnameLineEdit->setText(pSeller->surname);
    _seller = pSeller;
}

void SellerDialog::accept()
{
    if(ui->passwordLineEdit->text().isEmpty()){
        QMessageBox::critical(this, tr("Input error"), tr("Password is empty"));
        return;
    }

    if(ui->nicknameLineEdit->text().isEmpty()){
        QMessageBox::critical(this, tr("Input error"), tr("Nickname is empty"));
        return;
    }

    _seller->name = ui->nameLineEdit->text();
    _seller->surname = ui->surnameLineEdit->text();
    _seller->patronymic = ui->patronymicLineEdit->text();
    _seller->nickname = ui->nicknameLineEdit->text();
    _seller->burthday = ui->burthdayDateEdit->date();
    _seller->hash = qHash(ui->passwordLineEdit->text());

    QDialog::accept();
}
