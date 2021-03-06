#include "skladview.h"
#include "ui_skladview.h"

#include <float.h>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QComboBox>

#include "shopstate.h"
#include "mainwindow.h"

void SkladView::activate()
{
    ShopItem* pShopItem;
    int numItems = ShopState::pShop->items().count();
    for(int n = 0; n<numItems; ++n){
        pShopItem = ShopState::pShop->items().at(n);
        add(pShopItem, false);
    }

    enumerate();
}

void SkladView::deactivate()
{
    ui->view->clear();
}

SkladView::SkladView(QWidget *parent) :  QWidget(parent), ui(new Ui::SkladView)
{
    ui->setupUi(this);

    ui->view->setItemDelegate(new SkladViewEditor());
}

SkladView::~SkladView()
{
    delete ui;
}

void SkladView::enumerate()
{
    ui->view->setSortingEnabled(false);

    int num = ui->view->topLevelItemCount();
    for(int n = 0; n < num; ++n){
        ui->view->topLevelItem(n)->setText(sklad_view_num, QString("%0").arg(n, 6));
    }

    ui->view->setSortingEnabled(true);
}

void SkladView::add(ShopItem *pItem, const bool isNew)
{
    QTreeWidgetItem*    pTreeItem = new QTreeWidgetItem();
    pTreeItem->setText(sklad_view_num, "---");
    pTreeItem->setText(sklad_view_code,  pItem->code);
    pTreeItem->setText(sklad_view_name,  pItem->name);
    pTreeItem->setText(sklad_view_count, QString::number(pItem->count));
    pTreeItem->setText(sklad_view_sell_price,  QString::number(pItem->selling_price));
    pTreeItem->setText(sklad_view_buy_price,  QString::number(pItem->buying_price));
    pTreeItem->setText(sklad_view_discont,  QString::number(pItem->discont));
    pTreeItem->setText(sklad_view_bonus,  QString::number(pItem->bonus));
    pTreeItem->setText(sklad_view_bonus_pay,  ShopState::flagToString(pItem->flags & si_flag_cant_pay_bonus));
    pTreeItem->setText(sklad_view_free_price,  ShopState::flagToString(pItem->flags & si_flag_free_price));

    float   finalPrice = pItem->selling_price*(1. - 0.01*pItem->discont);
    pTreeItem->setText(sklad_view_final_price,  QString::number(finalPrice));

    pTreeItem->setData(sklad_view_num, sklad_view_role_item, QVariant::fromValue((void*)pItem));
    pTreeItem->setData(sklad_view_num, sklad_view_role_state, isNew ? sklad_view_new_item : sklad_view_exs_item);

    pTreeItem->setFlags(pTreeItem->flags() | Qt::ItemIsEditable);

    if(isNew){
        for(int n = 0; n < sklad_view_total; ++n)
            pTreeItem->setBackgroundColor(n, QColor(200, 255, 200, 255));
    } else{
        pTreeItem->setText(sklad_view_added,        pItem->added.toString());
        pTreeItem->setText(sklad_view_last_change,  pItem->changed.toString());
    }

    ui->view->addTopLevelItem(pTreeItem);
}

void SkladView::onDel()
{
    QList<QTreeWidgetItem*>     items = ui->view->selectedItems();

    foreach(QTreeWidgetItem* pItem, items){

        int state = pItem->data(sklad_view_num, sklad_view_role_state).toInt();
        if(state == sklad_view_new_item)
            delete pItem;
        else {
            pItem->setData(sklad_view_num, sklad_view_role_state, sklad_view_del_item);
            for(int n = 0; n < sklad_view_total; ++n)
                pItem->setBackgroundColor(n, QColor(255, 200, 200, 255));
        }
    }
}

void SkladView::onImp()
{

}

void SkladView::onExp()
{

}

void SkladView::onSav()
{
    QList<ShopItem*>    _toAdd, _toDel, _toChange;

    getItems(_toAdd, _toDel, _toChange);
    ShopState::pShop->apply(_toAdd, _toDel, _toChange);

    MainWindow::pWin->save();

    deactivate();
    activate();
}

void SkladView::onCan()
{
    deactivate();
    activate();
}

void SkladView::getItems(QList<ShopItem *> &added, QList<ShopItem *> &deleted, QList<ShopItem *> &changed)
{
    int     numItems = ui->view->topLevelItemCount();
    for(int n = 0; n<numItems; ++n){
        QTreeWidgetItem* pItem = ui->view->topLevelItem(n);

        int state = pItem->data(sklad_view_num, sklad_view_role_state).toInt();
        ShopItem*   pShopItem = (ShopItem*)pItem->data(sklad_view_num, sklad_view_role_item).value<void*>();

        if(state == sklad_view_del_item)
            deleted.append(pShopItem);
        else if(state == sklad_view_new_item){
            pShopItem->code = pItem->text(sklad_view_code);
            pShopItem->name = pItem->text(sklad_view_name);
            pShopItem->count = pItem->text(sklad_view_count).toInt();
            pShopItem->selling_price = pItem->text(sklad_view_sell_price).toFloat();
            pShopItem->buying_price = pItem->text(sklad_view_buy_price).toFloat();
            pShopItem->discont = pItem->text(sklad_view_discont).toFloat();
            added.append(pShopItem);
        } else if(state == sklad_view_chg_item){
            ShopItem*   pChange = new ShopItem;
            pChange->original = pShopItem;
            pChange->code = pItem->text(sklad_view_code);
            pChange->name = pItem->text(sklad_view_name);
            pChange->count = pItem->text(sklad_view_count).toInt();
            pChange->selling_price = pItem->text(sklad_view_sell_price).toFloat();
            pChange->buying_price = pItem->text(sklad_view_buy_price).toFloat();
            pChange->discont = pItem->text(sklad_view_discont).toFloat();
            changed.append(pChange);
        }
    }
}

void SkladView::onChange(QString name)
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

        bool isShowItem = pItem->text(sklad_view_code).contains(name) || pItem->text(sklad_view_name).contains(name);
        pItem->setHidden(!isShowItem);
    }
}

void SkladView::onAdd()
{
    ShopItem    *pItem = new ShopItem;
    add(pItem, true);
}

QWidget *SkladViewEditor::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch(index.column()){
        case    sklad_view_name:
        case    sklad_view_code:
        case    sklad_view_count:
            return  new QLineEdit(parent);
        case    sklad_view_buy_price:
        case    sklad_view_sell_price:
        case    sklad_view_discont:
        {
            QDoubleSpinBox* pEditor = new QDoubleSpinBox(parent);
            pEditor->setMaximum(FLT_MAX);
            return pEditor;
        }
        case    sklad_view_bonus_pay:
        case    sklad_view_free_price:
        {
            QComboBox*  editor = new QComboBox(parent);
            editor->addItem("No", false);
            editor->addItem("Yes", true);
            return  editor;
        }
        default: break;
    }

    return 0;
}

void SkladViewEditor::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch(index.column()){
        case    sklad_view_name:
        case    sklad_view_code:
        case    sklad_view_count:
        {
            QLineEdit*  pEdit = static_cast<QLineEdit*>(editor);
            pEdit->setText(index.data().toString());
        }
        break;
        case    sklad_view_bonus_pay:
        case    sklad_view_free_price:
        {
            QModelIndex idx = index.model()->index(index.row(), sklad_view_num);
            ShopItem* pItem = (ShopItem*)idx.data(sklad_view_role_item).value<void*>();

            QComboBox*  pEdit = static_cast<QComboBox*>(editor);
            if(index.column() == sklad_view_bonus_pay)
                pEdit->setCurrentIndex(pItem->flags & si_flag_cant_pay_bonus ? 1 : 0);
            else if(index.column() == sklad_view_free_price)
                pEdit->setCurrentIndex(pItem->flags & si_flag_free_price ? 1 : 0);
        }
        break;
        case    sklad_view_buy_price:
        case    sklad_view_sell_price:
        case    sklad_view_discont:
        {
            QDoubleSpinBox*  pEdit = static_cast<QDoubleSpinBox*>(editor);
            pEdit->setValue(index.data().toDouble());
        }
        default: break;
    }
}

void SkladViewEditor::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch(index.column()){
        case    sklad_view_name:
        case    sklad_view_code:
        {
            QLineEdit*  pEdit = static_cast<QLineEdit*>(editor);
            model->setData(index, pEdit->text());
        }
        break;
        case    sklad_view_buy_price:
        case    sklad_view_sell_price:
        case    sklad_view_discont:
        {
            QDoubleSpinBox*  pEdit = static_cast<QDoubleSpinBox*>(editor);
            model->setData(index, pEdit->value());
        }
        break;
        case    sklad_view_count:
        {
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
        break;
        default: break;
    }

    QModelIndex pItemIdx = model->index(index.row(), sklad_view_num);
    int state = model->data(pItemIdx, sklad_view_role_state).toInt();
    if(state == sklad_view_exs_item)
    {
        model->setData(pItemIdx, sklad_view_chg_item, sklad_view_role_state);
        model->setData(index, QBrush(QColor(255, 255, 200, 255)) ,Qt::BackgroundRole);
    }
}
