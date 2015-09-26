#ifndef BONUSCARDCHOOSE_H
#define BONUSCARDCHOOSE_H

#include <QDialog>

namespace Ui {
class BonusCardChoose;
}

struct ShopBonusCard;
class BonusCardChoose : public QDialog
{
    Q_OBJECT

    ShopBonusCard*  _card;

public:
    explicit BonusCardChoose(QWidget *parent = 0);
    ~BonusCardChoose();

    ShopBonusCard*  card() { return _card; }

public slots:
    void    onChoose(ShopBonusCard*  card);

private:
    Ui::BonusCardChoose *ui;
};

#endif // BONUSCARDCHOOSE_H
