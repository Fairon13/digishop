#ifndef TAKINVIEW_H
#define TAKINVIEW_H

#include <QWidget>

namespace Ui {
class TakeInView;
}

struct ShopItem;

typedef    ShopItem     ShopItemPatch;

class TakeInView : public QWidget
{
    Q_OBJECT

    QList<ShopItem*>        _toAdd;
    QList<ShopItem*>        _toDel;
    QList<ShopItemPatch*>   _toChange;

public:
    explicit TakeInView(QWidget *parent = 0);
    ~TakeInView();

    void    activate();
    void    deactivate();

public slots:
    void    onAdd();
    void    onDel();
    void    onImp();
    void    onExp();
    void    onSav();

private:
    Ui::TakeInView *ui;
};

#endif // TAKINVIEW_H
