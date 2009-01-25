#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <QDataStream>
#include <QDateTime>
#include <QMap>
#include <QString>
#include <QStringList>
#include "Base.h"
#include "Term.h"
#include "Util.h"

class Folder;

class Vocabulary : public Base {

public:

    static const qint32 magicNumber;

    typedef QMap<int, Term> TermMap;

    Vocabulary( int id = -1, const QString& title = QString::null );
    Vocabulary( const Vocabulary& voc );
    virtual ~Vocabulary();
   
    const char* className() const { return "Vocabulary"; }

    int getId() const;
    int getMaxId() const;
    bool isMarkedForStudy() const;
    void setMarkedForStudy( bool isMarkedForStudy );
    bool isMarkedForDeletion() const;
    void setMarkedForDeletion( bool isMarkedForDeletion );
    const QString getTitle() const;
    void setTitle( const QString& title );
    const QString getDescription() const;
    void setDescription( const QString& desc );
    const QString getAuthor() const;
    void setAuthor( const QString& author );
    const QDateTime getCreationDate() const;
    void setCreationDate( const QDateTime& date );
    const QDateTime getModificationDate() const;
    void setModificationDate( const QDateTime& date );

    void addTerm( const Term& term );
    void removeTerm( const int& id );
    bool isTermExists( const int& id ) const;
    Term& getTerm( const int& id );
    uint getSize() const;
    bool isEmpty() const;
    void getItemsCount( uint* termCount, uint* checkedTermCount, uint* selectedTermCount, bool isReachableFromRoot, const QString& firstLang = QString::null, const QString& testLang = QString::null ) const;
    int getMaxTermId() const;
    bool containsTermWithTranslations( const QString& lang1, const QString& lang2 ) const;
    QStringList getTranslationLanguages() const;
    void removeTranslations( const QStringList& languages );

    TermMap::ConstIterator begin() const;
    TermMap::ConstIterator end() const;
    TermMap::Iterator begin();
    TermMap::Iterator end();

    bool isDirty() const;
    void setDirty( bool isDirty );

    Folder* getParent() const;
    void setParent( Folder* parent );

    bool load( const QString& filename );
    bool save( const QString& filename ) const;

    friend QDataStream& operator<<( QDataStream& out, const Vocabulary& vocab );
    friend QDataStream& operator>>( QDataStream& in, Vocabulary& vocab );

private:

    int         id;
    bool        markedForStudy;
    bool        markedForDeletion;
    QString     title;
    QString     description;
    QString     author;
    QDateTime   creationDate;
    QDateTime   modificationDate;
    TermMap     terms;

    bool        dirty;

    Folder*     parent;

};

QDataStream& operator<<( QDataStream& out, const Vocabulary& vocab );
QDataStream& operator>>( QDataStream& in, Vocabulary& vocab );

#endif
