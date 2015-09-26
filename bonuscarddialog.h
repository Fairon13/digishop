#ifndef BONUSCARDDIALOG_H
#define BONUSCARDDIALOG_H

#include <QDialog>

namespace Ui {
class BonusCardDialog;
}

struct  ShopBonusCard;

class BonusCardDialog : public QDialog
{
    Q_OBJECT

    ShopBonusCard*      _card;

public:
    explicit BonusCardDialog(QWidget *parent = 0);
    ~BonusCardDialog();

    void    setBonusCard(ShopBonusCard* pCard);

public slots:
    void    accept();

private:
    Ui::BonusCardDialog *ui;
};

#endif // BONUSCARDDIALOG_H
