#include "Vocabulary.h"

const qint32 Vocabulary::magicNumber = qint32( 0x22446688 );

Vocabulary::Vocabulary( int id, const QString& title /*= QString::null*/ ) 
    : Base(), id( id ), markedForStudy( false ), markedForDeletion( false ), title( title ), description( QString::null ), author( QString::null ), 
        dirty( false ) {
    QDateTime now( QDateTime::currentDateTime() );
    creationDate = modificationDate = now;
}

Vocabulary::Vocabulary( const Vocabulary& voc ) 
    : Base(), id( voc.id ), markedForStudy( voc.markedForStudy ), markedForDeletion( voc.markedForDeletion ), 
        title( voc.title ), description( voc.description ), author( voc.author ), 
            creationDate( voc.creationDate ), modificationDate( voc.modificationDate ), dirty( voc.dirty ), parent( voc.parent ) {
    for( TermMap::ConstIterator it = voc.terms.begin(); it != voc.terms.end(); it++ ) {
        const Term& term = *it;
        addTerm( term );
    }
}

Vocabulary::~Vocabulary() {
}

int Vocabulary::getId() const {
    return( id );
}

bool Vocabulary::isMarkedForStudy() const {
    return( markedForStudy );
}

void Vocabulary::setMarkedForStudy( bool isMarkedForStudy ) {
    markedForStudy = isMarkedForStudy;
}

bool Vocabulary::isMarkedForDeletion() const {
    return( markedForDeletion );
}

void Vocabulary::setMarkedForDeletion( bool isMarkedForDeletion ) {
    markedForDeletion = isMarkedForDeletion;
}

const QString Vocabulary::getTitle() const {
    return( title );
    //return( title + QString( "(" ) + QString::number( getId() ) + QString( ")" ) ); // For debugging.
}

void Vocabulary::setTitle( const QString& title ) {
    this->title = title;
}

const QString Vocabulary::getDescription() const {
    return( description );
}

void Vocabulary::setDescription( const QString& desc ) {
    this->description = desc;
}

const QString Vocabulary::getAuthor() const {
    return( author );
}

void Vocabulary::setAuthor( const QString& author ) {
    this->author = author;
}

const QDateTime Vocabulary::getCreationDate() const {
    return( creationDate );
}

void Vocabulary::setCreationDate( const QDateTime& creationDate ) {
    this->creationDate = creationDate;
}

const QDateTime Vocabulary::getModificationDate() const {
    return( modificationDate );
}

void Vocabulary::setModificationDate( const QDateTime& modificationDate ) {
    this->modificationDate = modificationDate;
}

void Vocabulary::addTerm( const Term& term ) {
    terms.insert( term.getId(), term );
}

void Vocabulary::removeTerm( const int& id ) {
    terms.remove( id );
}

bool Vocabulary::isTermExists( const int& id ) const {
    return( terms.contains( id ) );
}

Term& Vocabulary::getTerm( const int& id ) {
    return( terms[ id ] );
}

bool Vocabulary::isEmpty() const {
    return( terms.isEmpty() );
}

uint Vocabulary::getSize() const {
    return( terms.count() );
}

void Vocabulary::getItemsCount( uint* termCount, uint* checkedTermCount, uint* selectedTermCount, bool isReachableFromRoot, const QString& firstLang /*= QString::null*/, const QString& testLang /*= QString::null*/ ) const {
    if( !isMarkedForDeletion() ) {
        for( TermMap::ConstIterator it = terms.begin(); it != terms.end(); it++ ) {
            const Term& term = *it;
            if( !firstLang.isNull() && !testLang.isNull() ) {
                if( term.isTranslationExists( firstLang ) && term.isTranslationExists( testLang ) ) {
                    *termCount += 1;
                    if( term.isMarkedForStudy() ) {
                        *checkedTermCount += 1;
                        if( isReachableFromRoot )
                            *selectedTermCount += 1;
                    }
                }
            }
            else {
                *termCount += 1;
                if( term.isMarkedForStudy() ) {
                    *checkedTermCount += 1;
                    if( isReachableFromRoot )
                        *selectedTermCount += 1;
                }
            }
        }
    }
}

int Vocabulary::getMaxTermId() const {
    int maxId = 0;
    for( TermMap::ConstIterator it = terms.begin(); it != terms.end(); it++ ) {
        const Term& term = *it;
        if( term.getId() > maxId )
            maxId = term.getId();
    }
    return( maxId );
}

bool Vocabulary::containsTermWithTranslations( const QString& lang1, const QString& lang2 ) const {
    for( Vocabulary::TermMap::ConstIterator it = terms.begin(); it != terms.end(); it++ ) {
        const Term& term = *it;
        if( term.isTranslationExists( lang1 ) && term.isTranslationExists( lang2 ) )
            return( true );
    }
    return( false );
}

QStringList Vocabulary::getTranslationLanguages() const {
    QStringList languages;
    for( TermMap::ConstIterator it = begin(); it != end(); it++ ) {
        const Term& term = *it;
        for( Term::TranslationMap::ConstIterator it2 = term.translationsBegin(); it2 != term.translationsEnd(); it2++ ) {
            const Translation& trans = *it2;
            if( !languages.contains( trans.getLanguage() ) )
                languages.append( trans.getLanguage() );
        }
    }
    return( languages );
}

void Vocabulary::removeTranslations( const QStringList& languages ) {
    for( TermMap::Iterator it = begin(); it != end(); it++ ) {
        Term& term = *it;
        for( QStringList::ConstIterator it2 = languages.begin(); it2 != languages.end(); it2++ ) {
            const QString& lang = *it2;
            term.removeTranslation( lang );
        }
    }
}

Vocabulary::TermMap::ConstIterator Vocabulary::begin() const {
    return( terms.begin() );
}

Vocabulary::TermMap::ConstIterator Vocabulary::end() const {
    return( terms.end() );
}

Vocabulary::TermMap::Iterator Vocabulary::begin() {
    return( terms.begin() );
}

Vocabulary::TermMap::Iterator Vocabulary::end() {
    return( terms.end() );
}

bool Vocabulary::isDirty() const {
    return( dirty );
}

void Vocabulary::setDirty( bool isDirty ) {
    dirty = isDirty;
}

Folder* Vocabulary::getParent() const {
    return( parent );
}

void Vocabulary::setParent( Folder* parent ) {
    this->parent = parent;
}

bool Vocabulary::load( const QString& filename ) {
    QFile dataFile( filename );
    if( !dataFile.open( QIODevice::ReadOnly ) )
        return( false );

    QByteArray compressedData( dataFile.readAll() );
    QByteArray data( qUncompress( compressedData ) );

    QDataStream in( data );

    qint32 tempMagicNumber;
    qint16 tempVersion;
    Vocabulary tempVocab;

    in >> tempMagicNumber >> tempVersion;

    if( tempMagicNumber != Vocabulary::magicNumber ) {
        cerr << "Wrong magic number: Incompatible vocabulary data file." << endl;
        return( false );
    }
    if( tempVersion > 0x0010 ) {
        cerr << "Vocabulary data file is from a more recent version.  Upgrade toMOTko." << endl;
        return( false );
    }

    in.setVersion( QDataStream::Qt_2_1 );
    in >> tempVocab;

    dataFile.close();

    id = tempVocab.getId();
    markedForStudy = tempVocab.isMarkedForStudy();
    title = tempVocab.getTitle();
    description = tempVocab.getDescription();
    author = tempVocab.getAuthor();
    creationDate = tempVocab.getCreationDate();
    modificationDate = tempVocab.getModificationDate();
    dirty = tempVocab.isDirty();
    for( TermMap::ConstIterator it = tempVocab.begin(); it != tempVocab.end(); it++ ) {
        const Term& term = *it;
        addTerm( term );
    }
    return( true );
}

bool Vocabulary::save( const QString& filename ) const {
    QByteArray data;

    QDataStream out( &data, QIODevice::WriteOnly );
    out.setVersion( QDataStream::Qt_2_1 );

    // 0x0010 means 0.10.x version.  
    out << qint32( Vocabulary::magicNumber ) << qint16( 0x0010 ) << *this;

    QByteArray compressedData( qCompress( data ) ); 

    QFile dataFile( filename );
    QFileInfo dataFileInfo( dataFile );
    QDir dataFileDir( dataFileInfo.absoluteDir() );

    if( !dataFileDir.mkpath( dataFileDir.path() ) )
        return( false );

    if( !dataFile.open( QIODevice::WriteOnly ) )
        return( false );

    int ret = dataFile.write( compressedData );
    dataFile.close();

    if( ret == -1 || dataFile.error() != QFile::NoError ) {
        dataFile.unsetError();
        return( false );
    }

    return( true );
}

QDataStream& operator<<( QDataStream& out, const Vocabulary& vocab ) {
    out << vocab.id << vocab.title << vocab.terms;
    out << vocab.description << vocab.author << vocab.creationDate << vocab.modificationDate;

    return( out );
}

QDataStream& operator>>( QDataStream& in, Vocabulary& vocab ) {
    int tempId;
    QString tempTitle;
    Vocabulary::TermMap tempTerms;
    QString tempDescription;
    QString tempAuthor;
    QDateTime tempCreationDate;
    QDateTime tempModificationDate;

    in >> tempId >> tempTitle >> tempTerms;

    in >> tempDescription >> tempAuthor >> tempCreationDate >> tempModificationDate;

    vocab = Vocabulary( tempId, tempTitle );
    for( Vocabulary::TermMap::ConstIterator it = tempTerms.begin(); it != tempTerms.end(); it++ ) {
        const Term& term = *it;
        vocab.addTerm( term );
    }
   
    vocab.setDescription( tempDescription );
    vocab.setAuthor( tempAuthor );
    vocab.setCreationDate( tempCreationDate );
    vocab.setModificationDate( tempModificationDate );

    return( in );
}
