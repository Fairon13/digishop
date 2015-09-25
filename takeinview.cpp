#include "takeinview.h"
#include "ui_takeinview.h"

#include "shopstate.h"
#include "mainwindow.h"

TakeInView::TakeInView(QWidget *parent) : QWidget(parent), ui(new Ui::TakeInView)
{
    ui->setupUi(this);
}

TakeInView::~TakeInView()
{
    delete ui;
}

void TakeInView::activate()
{
    ui->skladView->update(ShopState::pShop);
}

void TakeInView::deactivate()
{
    _toAdd.clear();
    _toDel.clear();
    _toChange.clear();

    ui->skladView->clear();
}

void TakeInView::onAdd()
{
    ui->skladView->onNew(new ShopItem);
}

void TakeInView::onDel()
{
    ui->skladView->onDel();
}

void TakeInView::onImp()
{

}

void TakeInView::onExp()
{

}

void TakeInView::onSav()
{
    ui->skladView->getItems(_toAdd, _toDel, _toChange);
    ShopState::pShop->apply(_toAdd, _toDel, _toChange);

    MainWindow::pWin->save();

    deactivate();
    activate();
}
