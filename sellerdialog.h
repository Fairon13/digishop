#ifndef SELLERDIALOG_H
#define SELLERDIALOG_H

#include <QDialog>

namespace Ui {
class SellerDialog;
}

struct ShopSeller;
class SellerDialog : public QDialog
{
    Q_OBJECT

    ShopSeller* _seller;

public:
    explicit SellerDialog(QWidget *parent = 0);
    ~SellerDialog();

    void    setSeller(ShopSeller*  pSeller);

public slots:
    void    accept();

private:
    Ui::SellerDialog *ui;
};

#endif // SELLERDIALOG_H
