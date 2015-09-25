#ifndef SHOPSTATE_H
#define SHOPSTATE_H

#include <QString>
#include <QDateTime>
#include <QList>
#include <QHash>
#include <QByteArray>

struct ShopItem {
    QString     code;
    QString     name;
    QDateTime   added;
    QDateTime   changed;
    ShopItem*   original;
    float       buying_price;
    float       selling_price;
    float       discont;
    int         count;

    ShopItem() {
        name = "new";
        original = 0;
        buying_price = 0.;
        selling_price = 0.;
        discont = 0.;
        count = 0;
    }
};

struct ShopUnit {
    QString             name;
    QList<ShopItem*>    items;
    QList<int>          counts;
    float               selling_price;
};

struct  ShopDiscont {
    QString     code;
    QDateTime   emmited;
    float       value;
};

struct ShopCustomer {
    QString     name;
    QString     family;
    QString     surname;
    QString     nickname;
    unsigned    hash;
};

struct ShopBuyer {
    QString         name;
    QString         family;
    QString         surname;
    quint8          age;
    quint8          sex;
    ShopDiscont*    discont;
};

struct ShopBusket {
    QList<ShopItem> items;
    QList<int>      counts;
};

class ShopState
{
    QList<ShopItem*>      _items;

    QList<ShopUnit*>      _units;
    QList<ShopCustomer*>  _customers;
    QList<ShopBuyer*>     _buyers;
    QList<ShopDiscont*>   _disconts;
    ShopBusket            _busket;

    ShopCustomer*        _curCustomer;

public:
    static      ShopState*  pShop;

public:
    ShopState();

    bool    login(const QString nikname, const unsigned hash);
    void    logout();

    ShopCustomer*       curCustomer() { return _curCustomer; }
    QList<ShopItem*>    items() { return _items; }

    void            apply(QList<ShopItem*> add, QList<ShopItem*> remove, QList<ShopItem*> update );

    QByteArray      save();
    void            load(QByteArray arr);
};

#endif // SHOPSTATE_H
