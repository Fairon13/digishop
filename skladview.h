#ifndef SKLADVIEW_H
#define SKLADVIEW_H

#include <QWidget>
#include <QStyledItemDelegate>

namespace Ui {
class SkladView;
}

class ShopState;
class ShopItem;

#define sklad_view_num          0
#define sklad_view_code         1
#define sklad_view_name         2
#define sklad_view_count        3
#define sklad_view_sell_price   4
#define sklad_view_discont      5
#define sklad_view_final_price  6
#define sklad_view_buy_price    7
#define sklad_view_bonus        8
#define sklad_view_bonus_pay    9
#define sklad_view_free_price   10
#define sklad_view_last_change  11
#define sklad_view_added        12
#define sklad_view_total        13

#define sklad_view_role_item    Qt::UserRole
#define sklad_view_role_state   Qt::UserRole + 1

#define sklad_view_exs_item     0
#define sklad_view_new_item     1
#define sklad_view_del_item     2
#define sklad_view_chg_item     3

class SkladView : public QWidget
{
    Q_OBJECT

    void    enumerate();

public:
    explicit SkladView(QWidget *parent = 0);
    ~SkladView();

    void    activate();
    void    deactivate();

    void                add(ShopItem* pItem, const bool isNew);
    QList<ShopItem*>    del();

    void    getItems(QList<ShopItem*> &added, QList<ShopItem*> &deleted, QList<ShopItem*> &changed);

public slots:
    void    onChange(QString name);
    void    onAdd();
    void    onDel();
    void    onImp();
    void    onExp();
    void    onSav();
    void    onCan();

private:
    Ui::SkladView *ui;
};

class   SkladViewEditor : public QStyledItemDelegate
{
public:
    QWidget *	createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    void	setEditorData ( QWidget * editor, const QModelIndex & index ) const;
    void	setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
};

#endif // SKLADVIEW_H
