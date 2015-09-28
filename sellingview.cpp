#include "sellingview.h"
#include "ui_sellingview.h"

#include "shopstate.h"
#include "bonuscardchoose.h"
#include "paydialog.h"

void SellingView::enumBasket()
{
    int num = ui->basket->topLevelItemCount();
    for(int n = 0; n < num; ++n){
        ui->basket->topLevelItem(n)->setText(bask_view_num, QString("%0").arg(n, 6));
    }

    ui->totalPos->setText(QString::number(num));
}

void SellingView::clearBasket()
{
    int numItems = ui->basket->topLevelItemCount();
    for(int n = 0; n<numItems; ++n)
        delete (ShopItem*)ui->basket->topLevelItem(n)->data(bask_view_num, Qt::UserRole).value<void*>();

    ui->basket->clear();
    if(_basket){
        delete _basket;
        _basket = 0;
    }

    ui->totalPos->setText("0");
    ui->totalBonuses->setText("0");
    ui->totalPrice->setText("0");
    ui->totalDisount->setText("0");
}

SellingView::SellingView(QWidget *parent) :  QWidget(parent),  ui(new Ui::SellingView)
{
    ui->setupUi(this);
    ui->basket->setItemDelegate(new BasketViewEditor());
    _basket = 0;

    _payDialog = new PayDialog(this);
}

SellingView::~SellingView()
{
    delete ui;
}

void SellingView::activate()
{
    foreach(ShopItem* pItem, ShopState::pShop->items()){

        QTreeWidgetItem*    pTreeItem = new QTreeWidgetItem(ui->view);
        pTreeItem->setText(sell_view_code,  pItem->code);
        pTreeItem->setText(sell_view_name,  pItem->name);
        pTreeItem->setText(sell_view_count, QString::number(pItem->count));
        pTreeItem->setText(sell_view_sell_price,  QString::number(pItem->selling_price));
        pTreeItem->setText(sell_view_discont,  QString::number(pItem->discont));

        float   finalPrice = pItem->selling_price*(1. - 0.01*pItem->discont);
        pTreeItem->setText(sell_view_final_price,  QString::number(finalPrice));

        pTreeItem->setData(sell_view_code, Qt::UserRole, QVariant::fromValue((void*)pItem));
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
    if(_basket == 0){
        _basket = new ShopBasket;
        _basket->created = QDateTime::currentDateTime();
        _basket->seller = ShopState::pShop->curCustomer();
        _basket->uid = QUuid::createUuid();
    }

    ShopItem*   pShopItem = (ShopItem*)item->data(sell_view_code, Qt::UserRole).value<void*>();

    //Seach this item in basket
    ShopItem*           pBasketItem = 0;
    QTreeWidgetItem*    pTreeItem = 0;

    int num = ui->basket->topLevelItemCount();
    for(int idx = 0; idx<num; ++idx){
        pTreeItem = ui->basket->topLevelItem(idx);
        ShopItem*   basketItem = (ShopItem*)pTreeItem->data(bask_view_num, Qt::UserRole).value<void*>();
        if(basketItem->original == pShopItem){
            pBasketItem = basketItem;
            break;
        }
    }

    if(pBasketItem){
        ++pBasketItem->count;
        pTreeItem->setText(bask_view_count, QString::number(pBasketItem->count));
    } else {
        pBasketItem = new ShopItem;
        *pBasketItem = *pShopItem;
        pBasketItem->original = pShopItem;
        pBasketItem->count = 1;

        pTreeItem = new QTreeWidgetItem(ui->basket);
        pTreeItem->setData(bask_view_num, Qt::UserRole, QVariant::fromValue((void*)pBasketItem));

        pTreeItem->setText(bask_view_code,  pShopItem->code);
        pTreeItem->setText(bask_view_name,  pShopItem->name);
        pTreeItem->setText(bask_view_count, QString::number(pBasketItem->count));
        pTreeItem->setText(bask_view_sell_price,  QString::number(pShopItem->selling_price));
        pTreeItem->setText(bask_view_discont,  QString::number(pShopItem->discont));
        pTreeItem->setFlags(pTreeItem->flags() | Qt::ItemIsEditable);

        _basket->items.append(pBasketItem);
    }

    enumBasket();
}

void SellingView::ItemFromBasket(QTreeWidgetItem *item, int column)
{
    if(column == bask_view_count)
        return;

    ShopItem*   basketItem = (ShopItem*)item->data(bask_view_num, Qt::UserRole).value<void*>();
    _basket->items.removeOne(basketItem);
    delete basketItem;
    delete item;

    enumBasket();
}

void SellingView::ItemBasketChange(QTreeWidgetItem *item, int column)
{
    if(column == bask_view_count){
        ShopItem*   basketItem = (ShopItem*)item->data(bask_view_num, Qt::UserRole).value<void*>();
        basketItem->count = qMin(item->text(column).toInt(), basketItem->original->count);

        float   finalPrice = basketItem->selling_price*(1.f - 0.01f*basketItem->discont)*basketItem->count;
        float   bonuses = 0.15f*finalPrice;
        item->setText(bask_view_count,  QString::number(basketItem->count));
        item->setText(bask_view_final_price,  QString::number(finalPrice));
        item->setText(bask_view_bonuse, QString::number(bonuses));
    }

    float   totalPrice = 0.f;
    float   totalDiscont = 0.f;
    float   totalBonuses = 0.f;

    int num = ui->basket->topLevelItemCount();
    for(int n = 0; n < num; ++n){
        ShopItem*   basketItem = (ShopItem*)ui->basket->topLevelItem(n)->data(bask_view_num, Qt::UserRole).value<void*>();

        float   discont = basketItem->selling_price*0.01*basketItem->discont;
        float   finalPrice = basketItem->selling_price - discont;
        float   bonuses = 0.15f*finalPrice;

        totalPrice += finalPrice*basketItem->count;
        totalBonuses += bonuses*basketItem->count;
        totalDiscont += discont*basketItem->count;
    }

    ui->totalBonuses->setText(QString::number(totalBonuses));
    ui->totalPrice->setText(QString::number(totalPrice));
    ui->totalDisount->setText(QString::number(totalDiscont));
}

void SellingView::onClear()
{
    clearBasket();
}

void SellingView::onProcess()
{
    if(_payDialog->exec() == QDialog::Accepted){

    }
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

QWidget *BasketViewEditor::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == bask_view_count){
        return new QLineEdit(parent);
    }

    return 0;
}

void BasketViewEditor::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == bask_view_count){
        QLineEdit*  pEdit = static_cast<QLineEdit*>(editor);
        pEdit->setText(index.data().toString());
    }
}

void BasketViewEditor::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == bask_view_count){
        QLineEdit*  pEdit = static_cast<QLineEdit*>(editor);
        QString str = pEdit->text();
        bool ok;
        int val, idx;

        if(idx = str.indexOf('+') != -1){
            QString sub = str.mid(idx);
            val = sub.toInt(&ok);
            val += index.data().toInt();
        } else if(idx = str.indexOf('-') != -1){
            QString sub = str.mid(idx);
            val = sub.toInt(&ok);
            val = qMax(index.data().toInt() - val, 0);
        } else {
            val = str.toInt(&ok);
        }

        if(ok)
            model->setData(index, val);
    }
}
