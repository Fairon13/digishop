#ifndef SHOPSTATE_H
#define SHOPSTATE_H

#include <QString>
#include <QDateTime>
#include <QList>
#include <QHash>
#include <QByteArray>
#include <QDataStream>
#include <QUuid>

#define ss_sex_male    0
#define ss_sex_female  1


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
    quint8      sex;

    ShopSeller(){
        hash = 0;
        sex = ss_sex_male;
    }

    void save(QDataStream &strm){
        strm << uid;
        strm << name;
        strm << surname;
        strm << patronymic;
        strm << nickname;
        strm << burthday;
        strm << created;
        strm << hash;
        strm << sex;
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
        strm >> sex;
    }
};

#define si_flag_cant_pay_bonus  1
#define si_flag_free_price      2

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
    float       bonus;
    int         count;
    int         flags;

    ShopItem() {
        original = 0;
        buying_price = 0.f;
        selling_price = 0.f;
        discont = 0.f;
        bonus = 0.f;
        count = 0;
        flags = 0;
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
        strm << bonus;
        strm << flags;
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
        strm >> bonus;
        strm >> flags;
    }
};

struct ShopUnit {
    QString             name;
    QList<ShopItem*>    items;
    QList<int>          counts;
    float               selling_price;
};

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
        sex = ss_sex_female;
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

struct ShopBasket {
    QUuid            uid;
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
    QList<ShopBasket*>      _buskets;

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

    static  QString sexToString(const quint8 sex){
        if(sex == ss_sex_male)
            return "Male";

        if(sex == ss_sex_female)
            return "Female";

        return "N/A";
    }

    static  QString flagToString(const bool flag){
        if(flag)
            return "Yes";

        return "No";
    }
};

#endif // SHOPSTATE_H
