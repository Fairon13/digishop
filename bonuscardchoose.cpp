#include "bonuscardchoose.h"
#include "ui_bonuscardchoose.h"

BonusCardChoose::BonusCardChoose(QWidget *parent) : QDialog(parent), ui(new Ui::BonusCardChoose)
{
    ui->setupUi(this);
    ui->view->setChooseMode();
    ui->view->activate();
}

BonusCardChoose::~BonusCardChoose()
{
    delete ui;
}

void BonusCardChoose::onChoose(ShopBonusCard *card)
{
    _card = card;
    accept();
}
