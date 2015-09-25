#ifndef SELLINGVIEW_H
#define SELLINGVIEW_H

#include <QWidget>

#define sell_view_code          0
#define sell_view_name          1
#define sell_view_count         2
#define sell_view_sell_price    3
#define sell_view_discont       4
#define sell_view_final_price   5
#define sklad_view_total        6

#define bask_view_num           0
#define bask_view_code          1
#define bask_view_name          2
#define bask_view_count         3
#define bask_view_sell_price    4
#define bask_view_discont       5
#define bask_view_final_price   6
#define bask_view_bonuse        7
#define bask_view_total         8

namespace Ui {
class SellingView;
}

class QTreeWidgetItem;

class SellingView : public QWidget
{
    Q_OBJECT

    void    enumBasket();
    void    clearBasket();

public:
    explicit SellingView(QWidget *parent = 0);
    ~SellingView();

    void    activate();
    void    deactivate();

public slots:
    void    ItemToBasket(QTreeWidgetItem* item, int column);
    void    onClear();
    void    onProcess();

private:
    Ui::SellingView *ui;
};

#endif // SELLINGVIEW_H
