#ifndef SEQUENCE_H
#define SEQUENCE_H 

#include <QList>
#include <QString>
#include <QStringList>
#include <QtAlgorithms>

class Sequence {

public:

    static const int MAX_ITEM;
    static const QString itemString[];
    enum Item { INVALID = -1, FIRST_LANG_TERM, TEST_LANG_ALT, TEST_LANG_TERM, COMMENT, IMAGE };

    typedef QList<Item> ItemList;

    Sequence( const QString& seqStr = QString::null );
    Sequence( const Sequence& seq );
    ~Sequence(); 

    bool isEnabled() const;
    void setEnabled( bool isEnabled );

    bool contains( const Item& item ) const;
    bool isEmpty() const;

    void addGroup( ItemList group );
    void removeLastGroup();
    ItemList getGroupAt( int index ) const;
    int getGroupCount() const;

    static Item stringToItem( const QString& itemStr );

    QString toString() const;
    QString toHumanReadableString() const;

    friend QDataStream& operator<<( QDataStream& out, const Sequence& sequence );
    friend QDataStream& operator>>( QDataStream& in, Sequence& sequence );

private:

    bool                    enabled;
    QList<ItemList>         groups;

};

QDataStream& operator<<( QDataStream& out, const Sequence& sequence );
QDataStream& operator>>( QDataStream& in, Sequence& sequence );

#endif

