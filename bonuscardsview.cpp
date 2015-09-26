#include "bonuscardsview.h"
#include "ui_bonuscardsview.h"

#include "bonuscarddialog.h"
#include "shopstate.h"
#include "mainwindow.h"

void BonusCardsView::addToView(ShopBonusCard *card)
{
    QTreeWidgetItem* pItem = new QTreeWidgetItem;
    updateItem(pItem, card);

    pItem->setText(bc_view_num, QString::number(ui->view->topLevelItemCount()));
    pItem->setData(bc_view_num, Qt::UserRole, QVariant::fromValue((void*)card));

    ui->view->addTopLevelItem(pItem);
}

void BonusCardsView::updateItem(QTreeWidgetItem *pItem, ShopBonusCard *card)
{
    pItem->setText(bc_view_code, card->code);
    pItem->setText(bc_view_name, card->name);
    pItem->setText(bc_view_surname, card->surname);
    pItem->setText(bc_view_patronym, card->patronymic);
    pItem->setText(bc_view_sex, card->sexString());
    pItem->setText(bc_view_bonus, QString::number(card->value));
    pItem->setText(bc_view_burthday, card->burthday.toString());
    pItem->setText(bc_view_emmition, card->emmited.toString());
    pItem->setText(bc_view_expiration, card->expired.toString());
}

BonusCardsView::BonusCardsView(QWidget *parent) : QWidget(parent), ui(new Ui::BonusCardsView)
{
    ui->setupUi(this);
    _dialog = new BonusCardDialog(this);

    _chooseMode = false;
}

BonusCardsView::~BonusCardsView()
{
    delete ui;
}

void BonusCardsView::setChooseMode()
{
    _chooseMode = true;
    ui->butDel->setDisabled(true);
}

void BonusCardsView::activate()
{
    QList<ShopBonusCard*>   cards = ShopState::pShop->cards();
    foreach(ShopBonusCard* pCard, cards){
        addToView(pCard);
    }
}

void BonusCardsView::deactivate()
{
    ui->view->clear();
}

void BonusCardsView::enumerate()
{
    int num = ui->view->topLevelItemCount();
    for(int n = 0; n < num; ++n){
        ui->view->topLevelItem(n)->setText(bc_view_num, QString("%0").arg(n, 6));
    }
}

void BonusCardsView::onAdd()
{
    ShopBonusCard*  pCard = ShopState::pShop->addBonusCard();

    _dialog->setBonusCard(pCard);
    if(_dialog->exec() == QDialog::Rejected){
        ShopState::pShop->delBonusCard(pCard);
        return;
    }

    pCard->emmited = QDateTime::currentDateTime();
    MainWindow::pWin->save();

    addToView(pCard);
}

void BonusCardsView::onDel()
{
    QList<QTreeWidgetItem*>     items = ui->view->selectedItems();

    foreach(QTreeWidgetItem* pItem, items){
        ShopBonusCard*  pCard = (ShopBonusCard*)pItem->data(bc_view_num, Qt::UserRole).value<void*>();
        ShopState::pShop->delBonusCard(pCard);
        delete pItem;
    }
    MainWindow::pWin->save();

    enumerate();
}

void BonusCardsView::onChange(QString name)
{
    int numItems = ui->view->topLevelItemCount();
    QTreeWidgetItem* pItem;

    if(name.isEmpty()){
        for(int n = 0; n<numItems; ++n){
            pItem = ui->view->topLevelItem(n);
            pItem->setHidden(false);
        }
        return;
    }

    for(int n = 0; n<numItems; ++n){
        pItem = ui->view->topLevelItem(n);

        bool isShowItem = pItem->text(bc_view_code).contains(name) || pItem->text(bc_view_name).contains(name);
        pItem->setHidden(!isShowItem);
    }
}

void BonusCardsView::onEdit(QTreeWidgetItem *item, int col)
{
    ShopBonusCard*  pCard = (ShopBonusCard*)item->data(bc_view_num, Qt::UserRole).value<void*>();

    if(_chooseMode){
        emit  ChoosedCard(pCard);
        return;
    }

    _dialog->setBonusCard(pCard);
    if(_dialog->exec() == QDialog::Rejected)
        return;

    pCard->seller = ShopState::pShop->curCustomer()->uid;
    MainWindow::pWin->save();

    updateItem(item, pCard);
}
