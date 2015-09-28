#ifndef SELLERSVIEW_H
#define SELLERSVIEW_H

#include <QWidget>

namespace Ui {
class SellersView;
}

#define     s_view_nickname    0
#define     s_view_name        1
#define     s_view_surname     2
#define     s_view_patronym    3
#define     s_view_burthday    4
#define     s_view_sex         5
#define     s_view_added       6
#define     s_view_total       7

class SellerDialog;
class QTreeWidgetItem;
struct ShopSeller;
class SellersView : public QWidget
{
    Q_OBJECT

    SellerDialog*    _dialog;

    void    addToView(ShopSeller* seller);
    void    updateItem(QTreeWidgetItem* pItem, ShopSeller* seller);

public:
    explicit SellersView(QWidget *parent = 0);
    ~SellersView();

    void    activate();
    void    deactivate();

public slots:
    void    onAdd();
    void    onDel();
    void    onChange(QTreeWidgetItem* item, int col);
    void    onFilter(QString text);

private:
    Ui::SellersView *ui;
};

#endif // SELLERSVIEW_H
