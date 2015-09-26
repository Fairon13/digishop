#ifndef BONUSCARDSVIEW_H
#define BONUSCARDSVIEW_H

#include <QWidget>

namespace Ui {
class BonusCardsView;
}

#define     bc_view_num         0
#define     bc_view_code        1
#define     bc_view_bonus       2
#define     bc_view_name        3
#define     bc_view_surname     4
#define     bc_view_patronym    5
#define     bc_view_sex         6
#define     bc_view_burthday    7
#define     bc_view_emmition    8
#define     bc_view_expiration  9
#define     bc_view_total       10

class BonusCardDialog;
class QTreeWidgetItem;
struct  ShopBonusCard;
class BonusCardsView : public QWidget
{
    Q_OBJECT

    BonusCardDialog*    _dialog;
    bool    _chooseMode;

    void    addToView(ShopBonusCard* card);
    void    updateItem(QTreeWidgetItem* pItem, ShopBonusCard* card);

public:
    explicit BonusCardsView(QWidget *parent = 0);
    ~BonusCardsView();

    void    setChooseMode();

    void    activate();
    void    deactivate();
    void    enumerate();

signals:
    void    ChoosedCard(ShopBonusCard* card);

public slots:
    void    onAdd();
    void    onDel();
    void    onChange(QString name);
    void    onEdit(QTreeWidgetItem* item, int col);

private:
    Ui::BonusCardsView *ui;
};

#endif // BONUSCARDSVIEW_H
