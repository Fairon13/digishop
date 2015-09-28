#include "sellersview.h"
#include "ui_sellersview.h"

#include "shopstate.h"
#include "sellerdialog.h"
#include "mainwindow.h"

void SellersView::addToView(ShopSeller *seller)
{
    QTreeWidgetItem* pItem = new QTreeWidgetItem;
    updateItem(pItem, seller);

    pItem->setData(s_view_name, Qt::UserRole, QVariant::fromValue((void*)seller));

    ui->view->addTopLevelItem(pItem);
}

void SellersView::updateItem(QTreeWidgetItem *pItem, ShopSeller *seller)
{
    pItem->setText(s_view_nickname, seller->nickname);
    pItem->setText(s_view_name, seller->name);
    pItem->setText(s_view_surname, seller->surname);
    pItem->setText(s_view_patronym, seller->patronymic);
    pItem->setText(s_view_burthday, seller->burthday.toString());
    pItem->setText(s_view_sex, ShopState::sexToString(seller->sex));
    pItem->setText(s_view_added, seller->created.toString());
}

SellersView::SellersView(QWidget *parent) : QWidget(parent), ui(new Ui::SellersView)
{
    ui->setupUi(this);

    _dialog = new SellerDialog(this);
}

SellersView::~SellersView()
{
    delete ui;
}

void SellersView::activate()
{
    QList<ShopSeller*>   sellers = ShopState::pShop->sellers();
    foreach(ShopSeller* pSeller, sellers){
        addToView(pSeller);
    }
}

void SellersView::deactivate()
{
    ui->view->clear();
}

void SellersView::onAdd()
{
    ShopSeller*  seller = ShopState::pShop->addSeller();

    _dialog->setSeller(seller);
    if(_dialog->exec() == QDialog::Rejected){
        ShopState::pShop->delSeller(seller);
        return;
    }

    seller->created = QDateTime::currentDateTime();
    MainWindow::pWin->save();

    addToView(seller);
}

void SellersView::onDel()
{
    QList<QTreeWidgetItem*>     items = ui->view->selectedItems();

    foreach(QTreeWidgetItem* pItem, items){
        ShopSeller*  seller = (ShopSeller*)pItem->data(s_view_name, Qt::UserRole).value<void*>();
        ShopState::pShop->delSeller(seller);
        delete pItem;
    }
    MainWindow::pWin->save();
}

void SellersView::onChange(QTreeWidgetItem *item, int col)
{
    ShopSeller*  seller = (ShopSeller*)item->data(s_view_name, Qt::UserRole).value<void*>();

    _dialog->setSeller(seller);
    if(_dialog->exec() == QDialog::Rejected)
        return;

    MainWindow::pWin->save();
    updateItem(item, seller);
}

void SellersView::onFilter(QString text)
{
    int numItems = ui->view->topLevelItemCount();
    QTreeWidgetItem* pItem;

    if(text.isEmpty()){
        for(int n = 0; n<numItems; ++n){
            pItem = ui->view->topLevelItem(n);
            pItem->setHidden(false);
        }
        return;
    }

    for(int n = 0; n<numItems; ++n){
        pItem = ui->view->topLevelItem(n);

        bool isShowItem = pItem->text(s_view_name).contains(text) || pItem->text(s_view_surname).contains(text) || pItem->text(s_view_patronym).contains(text);
        pItem->setHidden(!isShowItem);
    }
}
