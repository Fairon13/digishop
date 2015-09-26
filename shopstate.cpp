#include "shopstate.h"
#include <QHash>
#include <QDataStream>

ShopState*  ShopState::pShop = 0;

ShopState::ShopState()
{
    _curSeller = 0;
}

bool ShopState::login(const QString nikname, const unsigned hash)
{
    foreach(ShopSeller* pCustomer, _sellers){
        if(pCustomer->hash == hash && pCustomer->nickname == nikname){
            _curSeller = pCustomer;
            return true;
        }
    }

    return false;
}

void ShopState::logout()
{
    _curSeller = 0;
}

ShopSeller *ShopState::addSeller()
{
    ShopSeller* pSeller = new ShopSeller;
    _sellers.append(pSeller);
    return pSeller;
}

void ShopState::delSeller(ShopSeller *seller)
{
    _sellers.removeOne(seller);
    delete seller;
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
        pItem->uid = QUuid::createUuid();
        pItem->added = curDate;
        pItem->changed = curDate;
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

ShopItem *ShopState::addItem()
{
    ShopItem*   item = new ShopItem;
    item->uid = QUuid::createUuid();
    _items.append(item);
    return item;
}

void ShopState::delItem(ShopItem *item)
{
    _items.removeOne(item);
    delete item;
}

ShopBonusCard* ShopState::addBonusCard()
{
    ShopBonusCard* card = new ShopBonusCard;
    card->uid = QUuid::createUuid();
    card->seller = _curSeller->uid;
    _cards.append(card);
    return card;
}

void ShopState::delBonusCard(ShopBonusCard *card)
{
    _cards.removeOne(card);
    delete card;
}

QByteArray ShopState::save()
{
    QByteArray  arr;
    int num;
    QDataStream strm(&arr, QIODevice::WriteOnly);

    //shop sellers
    strm << ss_file_id_sellers;
    num = _sellers.count();
    strm << num;
    for(int n = 0; n<num; ++n){
        ShopSeller* pItem = _sellers.at(n);
        pItem->save(strm);
    }

    //shop items
    strm << ss_file_id_items;
    num = _items.count();
    strm << num;
    for(int n = 0; n<num; ++n){
        ShopItem* pItem = _items.at(n);
        pItem->save(strm);
    }

    //shop cards
    strm << ss_file_id_cards;
    num = _cards.count();
    strm << num;
    for(int n = 0; n<num; ++n){
        ShopBonusCard* pCard = _cards.at(n);
        pCard->save(strm);
    }

    strm << ss_file_id_none;
    return arr;
}

void ShopState::load(QByteArray arr)
{
    _items.clear();
    _sellers.clear();
    _cards.clear();

    int     num;
    quint8  idx;
    QDataStream strm(arr);

    forever{
        if(strm.atEnd())
            break;

        strm >> idx;
        if(idx == ss_file_id_none)
            break;

        switch(idx){
            case ss_file_id_sellers:
            strm >> num;
            for(int n = 0; n<num; ++n){
                ShopSeller* pItem = new ShopSeller;
                pItem->load(strm);
                _sellers.append(pItem);
            }
            break;

            case ss_file_id_items:
            strm >> num;
            for(int n = 0; n<num; ++n){
                ShopItem* pItem = new ShopItem;
                pItem->load(strm);
                _items.append(pItem);
            }
            break;

            case ss_file_id_cards:
            strm >> num;
            for(int n = 0; n<num; ++n){
                ShopBonusCard* pItem = new ShopBonusCard;
                pItem->load(strm);
                _cards.append(pItem);
            }
            break;

            default: break;
        }
    }


}

