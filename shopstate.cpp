#include "shopstate.h"
#include <QHash>
#include <QDataStream>

ShopState*  ShopState::pShop = 0;

ShopState::ShopState()
{
    _curCustomer = 0;

    ShopCustomer*   pCustomer = new ShopCustomer;
    pCustomer->nickname = "Fairon";
    pCustomer->hash = qHash(QString::fromLatin1("1"));
    pCustomer->name = "Anton";
    pCustomer->surname = "Ilyich";
    pCustomer->family = "Fedoseev";
    _customers.append(pCustomer);
}

bool ShopState::login(const QString nikname, const unsigned hash)
{
    foreach(ShopCustomer* pCustomer, _customers){
        if(pCustomer->hash == hash && pCustomer->nickname == nikname){
            _curCustomer = pCustomer;
            return true;
        }
    }

    return false;
}

void ShopState::logout()
{
    _curCustomer = 0;
}

void ShopState::apply(QList<ShopItem *> add, QList<ShopItem *> remove, QList<ShopItem *> update)
{
    //remove
    foreach(ShopItem* item, remove){
        int idx = _items.indexOf(item);
        if(idx != -1){
            _items.removeAt(idx);
            delete item;
        }
    }

    QDateTime   curDate = QDateTime::currentDateTime();

    //add
    foreach(ShopItem* pItem, add){
        pItem->added = curDate;
    }
    _items.append(add);

    //Patches
    for(int n = 0; n<update.count(); ++n){
        ShopItem* patch = update.at(n);
        ShopItem* pItem = patch->original;

        patch->original = 0;
        patch->added = pItem->added;
        *pItem = *patch;

        pItem->changed = curDate;
        delete patch;
    }
}

QByteArray ShopState::save()
{
    QByteArray  arr;
    QDataStream strm(&arr, QIODevice::WriteOnly);

    int num = _items.count();
    strm << num;

    for(int n = 0; n<num; ++n){
        ShopItem* pItem = _items.at(n);

        strm << pItem->code;
        strm << pItem->name;
        strm << pItem->count;
        strm << pItem->discont;
        strm << pItem->buying_price;
        strm << pItem->selling_price;
        strm << pItem->added;
        strm << pItem->changed;
    }

    return arr;
}

void ShopState::load(QByteArray arr)
{
    _items.clear();

    QDataStream strm(arr);

    int num = 0;
    strm >> num;

    for(int n = 0; n<num; ++n){
        ShopItem* pItem = new ShopItem;

        strm >> pItem->code;
        strm >> pItem->name;
        strm >> pItem->count;
        strm >> pItem->discont;
        strm >> pItem->buying_price;
        strm >> pItem->selling_price;
        strm >> pItem->added;
        strm >> pItem->changed;

        _items.append(pItem);
    }
}

