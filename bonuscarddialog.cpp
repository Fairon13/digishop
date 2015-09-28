#include "bonuscarddialog.h"
#include "ui_bonuscarddialog.h"

#include "shopstate.h"

BonusCardDialog::BonusCardDialog(QWidget *parent) : QDialog(parent), ui(new Ui::BonusCardDialog)
{
    ui->setupUi(this);
}

BonusCardDialog::~BonusCardDialog()
{
    delete ui;
}

void BonusCardDialog::setBonusCard(ShopBonusCard *pCard)
{
    _card = pCard;

    ui->codeLineEdit->setText(pCard->code);
    ui->nameLineEdit->setText(pCard->name);
    ui->surnameLineEdit->setText(pCard->surname);
    ui->patronumicLineEdit->setText(pCard->patronymic);
    ui->sexComboBox->setCurrentIndex(pCard->sex);
    ui->burthdayDateEdit->setDate(pCard->burthday);
    ui->bonusesEdit->setText(QString::number(pCard->value));
    ui->expiredDateTimeEdit->setDateTime(pCard->expired);
}

void BonusCardDialog::accept()
{
    _card->code = ui->codeLineEdit->text();
    _card->name = ui->nameLineEdit->text();
    _card->surname = ui->surnameLineEdit->text();
    _card->patronymic = ui->patronumicLineEdit->text();
    _card->sex = ui->sexComboBox->currentIndex();
    _card->burthday = ui->burthdayDateEdit->date();
    _card->value = ui->bonusesEdit->text().toFloat();
    _card->expired = ui->expiredDateTimeEdit->dateTime();

    QDialog::accept();
}
