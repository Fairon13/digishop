#ifndef SHOPSTATE_H
#define SHOPSTATE_H

#include <QString>
#include <QDateTime>
#include <QList>
#include <QHash>
#include <QByteArray>
#include <QDataStream>
#include <QUuid>

#define ss_file_id_none     quint8(0)
#define ss_file_id_sellers  quint8(1)
#define ss_file_id_items    quint8(2)
#define ss_file_id_cards    quint8(3)

struct ShopSeller {
    QUuid       uid;
    QString     name;
    QString     surname;
    QString     patronymic;
    QString     nickname;
    QDate       burthday;
    QDateTime   created;
    unsigned    hash;

    void save(QDataStream &strm){
        strm << uid;
        strm << name;
        strm << surname;
        strm << patronymic;
        strm << nickname;
        strm << burthday;
        strm << created;
        strm << hash;
    }

    void load(QDataStream &strm){
        strm >> uid;
        strm >> name;
        strm >> surname;
        strm >> patronymic;
        strm >> nickname;
        strm >> burthday;
        strm >> created;
        strm >> hash;
    }
};

struct ShopItem {
    QUuid       uid;
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
        original = 0;
        buying_price = 0.;
        selling_price = 0.;
        discont = 0.;
        count = 0;
    }

    void save(QDataStream &strm){
        strm << uid;
        strm << code;
        strm << name;
        strm << added;
        strm << changed;
        strm << buying_price;
        strm << selling_price;
        strm << discont;
        strm << count;
    }

    void load(QDataStream &strm){
        strm >> uid;
        strm >> code;
        strm >> name;
        strm >> added;
        strm >> changed;
        strm >> buying_price;
        strm >> selling_price;
        strm >> discont;
        strm >> count;
    }
};

struct ShopUnit {
    QString             name;
    QList<ShopItem*>    items;
    QList<int>          counts;
    float               selling_price;
};

#define sbc_sex_male    0
#define sbc_sex_female  1

struct  ShopBonusCard {
    QUuid       uid;
    QUuid       seller;
    QString     code;
    QDateTime   emmited;
    QDateTime   expired;
    QDate       burthday;
    QString     name;
    QString     surname;
    QString     patronymic;
    float       value;
    quint8      sex;

    ShopBonusCard(){
        value = 0.;
        sex = sbc_sex_male;
    }

    QString   sexString(){
        if(sex == sbc_sex_male)
            return "Male";

        if(sex == sbc_sex_female)
            return "Female";

        return "N/A";
    }

    void    save(QDataStream &strm){
        strm << uid;
        strm << code;
        strm << emmited;
        strm << expired;
        strm << burthday;
        strm << name;
        strm << surname;
        strm << patronymic;
        strm << value;
        strm << sex;
        strm << seller;
    }

    void    load(QDataStream &strm){
        strm >> uid;
        strm >> code;
        strm >> emmited;
        strm >> expired;
        strm >> burthday;
        strm >> name;
        strm >> surname;
        strm >> patronymic;
        strm >> value;
        strm >> sex;
        strm >> seller;
    }
};

struct ShopBusket {
    QList<ShopItem*> items;
    QDateTime        created;
    QDateTime        closed;
    ShopBonusCard*   bonus;
    ShopSeller*      seller;
};

class ShopState
{
    QList<ShopItem*>        _items;
    QList<ShopUnit*>        _units;
    QList<ShopSeller*>      _sellers;
    QList<ShopBonusCard*>   _cards;
    QList<ShopBusket*>      _buskets;

    ShopSeller*         _curSeller;

public:
    static      ShopState*  pShop;

public:
    ShopState();

    bool    login(const QString nikname, const unsigned hash);
    void    logout();

    ShopSeller*       curCustomer() { return _curSeller; }
    QList<ShopSeller*>  sellers() { return _sellers; }
    ShopSeller*         addSeller();
    void                delSeller(ShopSeller* seller);

    void                apply(QList<ShopItem*> add, QList<ShopItem*> remove, QList<ShopItem*> update );
    QList<ShopItem*>    items() { return _items; }
    ShopItem*           addItem();
    void                delItem(ShopItem* item);

    ShopBonusCard*          addBonusCard();
    void                    delBonusCard(ShopBonusCard* card);
    QList<ShopBonusCard*>   cards() { return _cards; }

    QByteArray      save();
    void            load(QByteArray arr);
};

#endif // SHOPSTATE_H
