#ifndef SELLINGVIEW_H
#define SELLINGVIEW_H

#include <QWidget>
#include <QStyledItemDelegate>

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
struct ShopBasket;
class PayDialog;
class SellingView : public QWidget
{
    Q_OBJECT

    PayDialog*      _payDialog;
    ShopBasket*     _basket;

    void    enumBasket();
    void    clearBasket();

public:
    explicit SellingView(QWidget *parent = 0);
    ~SellingView();

    void    activate();
    void    deactivate();

public slots:
    void    ItemToBasket(QTreeWidgetItem* item, int column);
    void    ItemFromBasket(QTreeWidgetItem* item, int column);
    void    ItemBasketChange(QTreeWidgetItem* item, int column);
    void    onClear();
    void    onProcess();
    void    onChange(QString name);
    void    onClearCard();
    void    onSelCard();

private:
    Ui::SellingView *ui;
};

class   BasketViewEditor : public QStyledItemDelegate
{
public:
    QWidget *	createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    void	setEditorData ( QWidget * editor, const QModelIndex & index ) const;
    void	setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
};

#endif // SELLINGVIEW_H
