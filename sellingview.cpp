#include "sellingview.h"
#include "ui_sellingview.h"

#include "shopstate.h"
#include "bonuscardchoose.h"

void SellingView::enumBasket()
{
//    ui->basket->setSortingEnabled(false);

    int num = ui->basket->topLevelItemCount();
    for(int n = 0; n < num; ++n){
        ui->basket->topLevelItem(n)->setText(bask_view_num, QString("%0").arg(n, 6));
    }

    //    ui->basket->setSortingEnabled(true);
}

void SellingView::clearBasket()
{
    int numItems = ui->basket->topLevelItemCount();
    for(int n = 0; n<numItems; ++n)
        delete (ShopItem*)ui->basket->topLevelItem(n)->data(bask_view_num, Qt::UserRole).value<void*>();

    ui->basket->clear();
}

SellingView::SellingView(QWidget *parent) :  QWidget(parent),  ui(new Ui::SellingView)
{
    ui->setupUi(this);
}

SellingView::~SellingView()
{
    delete ui;
}

void SellingView::activate()
{
    foreach(ShopItem* pItem, ShopState::pShop->items()){

        QTreeWidgetItem*    pTreeItem = new QTreeWidgetItem();
        pTreeItem->setText(sell_view_code,  pItem->code);
        pTreeItem->setText(sell_view_name,  pItem->name);
        pTreeItem->setText(sell_view_count, QString::number(pItem->count));
        pTreeItem->setText(sell_view_sell_price,  QString::number(pItem->selling_price));
        pTreeItem->setText(sell_view_discont,  QString::number(pItem->discont));

        float   finalPrice = pItem->selling_price*(1. - 0.01*pItem->discont);
        pTreeItem->setText(sell_view_final_price,  QString::number(finalPrice));

        pTreeItem->setData(sell_view_code, Qt::UserRole, QVariant::fromValue((void*)pItem));

        ui->view->addTopLevelItem(pTreeItem);
    }
}

void SellingView::deactivate()
{
    //clear view
    ui->view->clear();

    //clear basket
    clearBasket();
}

void SellingView::ItemToBasket(QTreeWidgetItem *item, int column)
{
    ShopItem*   pShopItem = (ShopItem*)item->data(sell_view_code, Qt::UserRole).value<void*>();

    ShopItem*   pBasketItem = new ShopItem;
    *pBasketItem = *pShopItem;
    pBasketItem->original = pShopItem;
    pBasketItem->count = 1;


    QTreeWidgetItem*    pTreeItem = new QTreeWidgetItem();
    pTreeItem->setText(bask_view_code,  pShopItem->code);
    pTreeItem->setText(bask_view_name,  pShopItem->name);
    pTreeItem->setText(bask_view_count, QString::number(pBasketItem->count));
    pTreeItem->setText(bask_view_sell_price,  QString::number(pShopItem->selling_price));
    pTreeItem->setText(bask_view_discont,  QString::number(pShopItem->discont));

    float   finalPrice = pShopItem->selling_price*(1. - 0.01*pShopItem->discont);
    pTreeItem->setText(bask_view_final_price,  QString::number(finalPrice));
    pTreeItem->setText(bask_view_bonuse, QString::number(finalPrice*0.1));
    pTreeItem->setData(bask_view_num, Qt::UserRole, QVariant::fromValue((void*)pBasketItem));
    ui->basket->addTopLevelItem(pTreeItem);

    enumBasket();
}

void SellingView::onClear()
{
    clearBasket();
}

void SellingView::onProcess()
{

}

void SellingView::onChange(QString name)
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

        bool isShowItem = pItem->text(sell_view_code).contains(name) || pItem->text(sell_view_name).contains(name);
        pItem->setHidden(!isShowItem);
    }
}

void SellingView::onClearCard()
{
    ui->butSelectCard->setText("none");
    ui->cardValue->clear();
    ui->cardOwner->clear();
}

void SellingView::onSelCard()
{
    BonusCardChoose*    dialog = new BonusCardChoose(this);
    if(dialog->exec() == QDialog::Accepted){

        ShopBonusCard*  card = dialog->card();
        ui->butSelectCard->setText(card->code);
        ui->cardValue->setText(QString::number(card->value));

        QString name = card->name + " " + card->surname + " " + card->patronymic;
        ui->cardOwner->setText(name);

    }

    delete dialog;
}
