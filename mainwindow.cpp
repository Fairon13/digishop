#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "logindialog.h"
#include "shopstate.h"

#include <QFile>
#include <QDataStream>

MainWindow* MainWindow::pWin = 0;

#define file_id_shop_state  quint8(1)

void MainWindow::save()
{
    QFile   file("druff.shop");
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QDataStream     strm(&file);

    //ShopState
    QByteArray  arr = ShopState::pShop->save();
    if(arr.size()){
        strm << file_id_shop_state << arr.size();
        strm.writeRawData(arr.constData(), arr.size());
    }

    file.close();
}

void MainWindow::restore()
{
    QFile   file("druff.shop");
    if(!file.open(QIODevice::ReadOnly))
        return;

    QDataStream     strm(&file);

    quint8      fileId = 0;
    int         size = 0;

    forever{

        if(strm.atEnd())
            break;

        strm >> fileId >> size;
        switch(fileId){
            case file_id_shop_state:
            {
                char*   pdata = new char[size];
                strm.readRawData(pdata, size);
                ShopState::pShop->load(QByteArray::fromRawData(pdata, size));
                delete[] pdata;
            }
            break;

            default:
                file.seek(file.pos() + size);
            break;
        }


    }

    file.close();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),  ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pWin = this;

    restore();
    onTabChange(0);

    pLoginDialog = new LoginDialog(this);
    QMetaObject::invokeMethod(this, "onLogout", Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLogout()
{
   ShopState::pShop->logout();

   if(pLoginDialog->waitLogin() == false){
       close();
       return;
   }

   ShopSeller* pCust = ShopState::pShop->curCustomer();
   QString  name = "Druff Shop: " + pCust->name + " " + pCust->surname + " " + pCust->patronymic;
   setWindowTitle(name);
}

void MainWindow::onBlock()
{
    if(pLoginDialog->waitLogin() == false)
        close();
}

void MainWindow::onTabChange(int toIndex)
{
    static QWidget*  prevView = 0;

    //deactivate
    SkladView* pTakeIn = dynamic_cast<SkladView*>(prevView);
    if(pTakeIn) pTakeIn->deactivate();

    SellingView* pSellingIn = dynamic_cast<SellingView*>(prevView);
    if(pSellingIn) pSellingIn->deactivate();

    BonusCardsView* pCards = dynamic_cast<BonusCardsView*>(prevView);
    if(pCards) pCards->deactivate();

    SellersView* pSellers = dynamic_cast<SellersView*>(prevView);
    if(pSellers) pSellers->deactivate();

    //activate
    QWidget*    pNewView = ui->TabView->widget(toIndex);

    pTakeIn = dynamic_cast<SkladView*>(pNewView);
    if(pTakeIn) pTakeIn->activate();

    pSellingIn = dynamic_cast<SellingView*>(pNewView);
    if(pSellingIn) pSellingIn->activate();

    pCards = dynamic_cast<BonusCardsView*>(pNewView);
    if(pCards) pCards->activate();

    pSellers = dynamic_cast<SellersView*>(pNewView);
    if(pSellers) pSellers->activate();

    prevView = pNewView;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    save();

    QMainWindow::closeEvent(event);
}
