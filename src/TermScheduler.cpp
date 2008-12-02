#include "TermScheduler.h"

const qint32 TermScheduler::magicNumber = qint32( 0x22446677 );

TermScheduler::TermScheduler( const Preferences& prefs ) : prefs( prefs ) {
    srand( time( NULL ) );
}

TermScheduler::TermScheduler( const TermScheduler& scheduler ) 
    : quizFirstLang( scheduler.quizFirstLang ), quizTestLang( scheduler.quizTestLang ), 
        standbyPool( scheduler.standbyPool ), allTerms( scheduler.allTerms ), 
            initQuizLength( scheduler.initQuizLength ), initTermCount( scheduler.initTermCount ),
                currTermPool( scheduler.currTermPool ), currTerm( scheduler.currTerm ), prefs( scheduler.prefs ) {
    for( int i = 0; i < poolCount; i++ )
        termPool[ i ] = scheduler.termPool[ i ];
}

TermScheduler::~TermScheduler() {
}

bool TermScheduler::isResumableQuizAvailable( const BilingualKey& key ) const {
    QFile dataFile( getFilename( key ) );
    return( dataFile.exists() );
}

bool TermScheduler::load( const BilingualKey& key ) {
    QFile dataFile( getFilename( key ) );
    if( !dataFile.open( QIODevice::ReadOnly ) )
        return( false );
    
    QByteArray compressedData( dataFile.readAll() );
    QByteArray data( qUncompress( compressedData ) );

    QDataStream in( data );

    qint32 tempMagicNumber;
    qint16 tempVersion;

    in >> tempMagicNumber >> tempVersion;

    if( tempMagicNumber != magicNumber ) {
        cerr << "Wrong magic number: Incompatible data file." << endl;
        return( false );
    }
    if( tempVersion > 0x0009 ) {
        cerr << "Data file is from a more recent version.  Upgrade toMOTko." << endl;
        return( false );
    }

    in.setVersion( QDataStream::Qt_2_1 );

    TermScheduler tempScheduler( prefs );
    in >> tempScheduler;

    quizFirstLang = tempScheduler.quizFirstLang;
    quizTestLang = tempScheduler.quizTestLang;
    for( int i = 0; i < poolCount; i++ )
        termPool[ i ] = tempScheduler.termPool[ i ];
    standbyPool = tempScheduler.standbyPool;
    allTerms = tempScheduler.allTerms;
    initQuizLength = tempScheduler.initQuizLength;
    initTermCount = tempScheduler.initTermCount;
    currTermPool = tempScheduler.currTermPool;
    currTerm = tempScheduler.currTerm;

    return( true );
}

bool TermScheduler::save() {
    QByteArray data;

    QDataStream out( &data, QIODevice::WriteOnly );
    out.setVersion( QDataStream::Qt_2_1 );

    // 0x0009 means 0.9.x version.  
    out << qint32( TermScheduler::magicNumber ) << qint16( 0x0009 ) << *this;
    
    QByteArray compressedData( qCompress( data ) ); 

    QFile dataFile( getFilename( BilingualKey( quizFirstLang, quizTestLang ) ) );
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

void TermScheduler::concludeQuiz() {
    if( isQuizInProgress() )
        save();
    else {
        QString filename( getFilename( BilingualKey( quizFirstLang, quizTestLang ) ) );
        QFile dataFile( filename );
        if( dataFile.exists() ) {
            if( !dataFile.remove() )
                cerr << "Could not remove file " << qPrintable( filename ) << ". Status=" << dataFile.error() << endl;
        }
    }
}

void TermScheduler::setApplicationDirName( const QString& applDir ) {
    this->applDir = applDir;
}

QString TermScheduler::getQuizFirstLanguage() const {
    return( quizFirstLang );
}

QString TermScheduler::getQuizTestLanguage() const {
    return( quizTestLang );
}

bool TermScheduler::isQuizInProgress() const {
    for( int i = 0; i < poolCount; i++ ) {
        if( termPool[ i ].count() > 0 )
            return( true );
    }
    return( standbyPool.count() > 0 );
}

void TermScheduler::init( const QString& quizFirstLang, const QString& quizTestLang, Folder* rootFolder ) {
    this->quizFirstLang = quizFirstLang;
    this->quizTestLang = quizTestLang;
    initTermCount = 0;
    initQuizLength = prefs.getQuizLength();
    for( int i = 0; i < poolCount; i++ )
        termPool[ i ].clear();
    allTerms.clear();
    initRec( quizFirstLang, quizTestLang, rootFolder );
    standbyPool.clear();
    currTermPool = 0;
}

void TermScheduler::reinit() {
    for( int i = 0; i < poolCount; i++ )
        termPool[ i ].clear();
    standbyPool.clear();
    for( QList<TermKey>::ConstIterator it = allTerms.begin(); it != allTerms.end(); it++ ) {
        TermKey termKey = *it;
        addTerm( termKey, ( poolCount - 1 ) - initQuizLength );
    }
    currTermPool = 0;
}

void TermScheduler::initRec( const QString& quizFirstLang, const QString& quizTestLang, Folder* folder ) {
    if( folder->isMarkedForStudy() ) {
        for( int i = 0; i < folder->getChildrenCount(); i++ ) {
            Base* child = folder->childAt( i );
            if( strcmp( child->className(), "Folder" ) == 0 )
                initRec( quizFirstLang, quizTestLang, (Folder*)child );
            else if( strcmp( child->className(), "Vocabulary" ) == 0 )
                initRec( quizFirstLang, quizTestLang, (Vocabulary*)child );
        }
    }
}

void TermScheduler::initRec( const QString& quizFirstLang, const QString& quizTestLang, Vocabulary* vocab ) {
    if( vocab->isMarkedForStudy() ) {
        for( Vocabulary::TermMap::ConstIterator it = vocab->begin(); it != vocab->end(); it++ ) {
            const Term& term = *it;
            if( term.isMarkedForStudy() &&
                term.isTranslationExists( quizFirstLang ) && term.isTranslationExists( quizTestLang ) ) {
                TermKey termKey( term.getId(), term.getVocabId() );
                addTerm( termKey, ( poolCount - 1 ) - initQuizLength );
                allTerms.append( termKey );
                initTermCount++;
            }
        }
    }
}

void TermScheduler::addTerm( const TermKey& termKey, const int priority /* = 0 */ ) {
    termPool[ priority ].append( termKey );
}

void TermScheduler::discardCurrentTerm() {
    int index = termPool[ currTermPool ].indexOf( currTerm );
    termPool[ currTermPool ].removeAt( index );
}

const TermKey TermScheduler::getCurrentTerm() const {
    return( currTerm );
}

bool TermScheduler::hasNextTerm() const {
    if( !standbyPool.isEmpty() )
        return( true );
   
    for( int i = 0; i < poolCount; i++ ) {
        if( !termPool[ i ].isEmpty() )
            return( true );
    }

    return( false );
}

const TermKey TermScheduler::getNextTerm() {
    if( standbyPool.count() == TermScheduler::standbyPoolSize )
        reintroduceStandbyTerm();
    
search:
    int startPool = ( currTermPool + 1 ) % TermScheduler::poolCount;
    int nextPool = startPool; 
    do {
        const int poolSize = termPool[ nextPool ].count();
        if( poolSize > 0 ) {
            const int index = rand() % poolSize;
            currTermPool = nextPool;
            currTerm = termPool[ nextPool ][ index ];
            return( currTerm );
        }
        nextPool = ( nextPool + 1 ) % TermScheduler::poolCount; 
    } while( nextPool != startPool );
    if( standbyPool.count() > 0 ) {
        reintroduceStandbyTerm();
        goto search;
    }
    else {
        currTerm = TermKey();
        currTermPool = -1;
        return( currTerm );
    }
}

void TermScheduler::increaseTermPriority() {
    if( currTermPool > 0 ) {
        int index = termPool[ currTermPool ].indexOf( currTerm );
        termPool[ currTermPool ].removeAt( index );
        currTermPool--;
        termPool[ currTermPool ].append( currTerm );
    }
}

void TermScheduler::decreaseTermPriority() {
    if( currTermPool < (int)( TermScheduler::poolCount - 1 ) ) {
        int index = termPool[ currTermPool ].indexOf( currTerm );
        termPool[ currTermPool ].removeAt( index );
        currTermPool++;
        termPool[ currTermPool ].append( currTerm );
    }
}

void TermScheduler::rightAnswer() {
    if( currTermPool == TermScheduler::poolCount - 1 )
        discardCurrentTerm();
    else {
        decreaseTermPriority();
        putCurrentTermOnStandby();
    }
}

void TermScheduler::wrongAnswer() {
    increaseTermPriority();
    putCurrentTermOnStandby();
}

int TermScheduler::getProgress() const {
    if( initTermCount == 0 )
        return( 0 );
    int currTermCount = 0;
    int progress = 0;
    int parts = ( initQuizLength + 1 ) * initTermCount;
    for( int i = 0; i < poolCount; i++ ) {
        currTermCount += termPool[ i ].count();
        if( i >= ( poolCount - initQuizLength ) ) {
            int incr = ( termPool[ i ].count() * ( initQuizLength + 1 - poolCount + i ) ); 
            progress += incr;
        }
    }
    for( int i = 0; i < standbyPool.count(); i++ ) {
        currTermCount += 1;
        if( standbyPool[ i ].getPool() >= ( poolCount - initQuizLength ) ) {
            int incr = ( initQuizLength + 1 - poolCount + standbyPool[ i ].getPool() );
            progress += incr;
        }
    }
    progress += ( initTermCount - currTermCount ) * ( parts / initTermCount );
    int result = (int)( progress * 100 / parts );
    return( result ); 
}

int TermScheduler::getInitialTermCount() const {
    return( initTermCount );
}

void TermScheduler::putCurrentTermOnStandby() {
    StandbyTerm term( currTerm, currTermPool );
    standbyPool.prepend( term ); 
    int index = termPool[ currTermPool ].indexOf( currTerm );
    termPool[ currTermPool ].removeAt( index );
}

void TermScheduler::reintroduceStandbyTerm() {
    const StandbyTerm& reintroducedTerm = standbyPool.last();
    termPool[ reintroducedTerm.getPool() ].append( reintroducedTerm.getKey() ); 
    int index = standbyPool.indexOf( reintroducedTerm );
    standbyPool.removeAt( index );
}

QString TermScheduler::getFilename( const BilingualKey& key ) const {
    return( applDir + QString( "/quiz_" ) + key.toString() + QString( ".dat.z" ) );
}

QDataStream& operator<<( QDataStream& out, const TermScheduler& scheduler ) {
    out << scheduler.quizFirstLang << scheduler.quizTestLang;

    for( int i = 0; i < TermScheduler::poolCount; i++ ) {
        for( QList<TermKey>::ConstIterator it = scheduler.termPool[ i ].begin(); it != scheduler.termPool[ i ].end(); it++ )
            out << *it;
        out << TermKey( 0, 0 ); // To mark the end of the pool.
    }

    for( QList<StandbyTerm>::ConstIterator it = scheduler.standbyPool.begin(); it != scheduler.standbyPool.end(); it++ )
        out << *it;
    out << StandbyTerm( TermKey( 0, 0 ), 0 ); // To mark the end of the standbyPool.
    
    out << scheduler.allTerms; 
    out << scheduler.initQuizLength << scheduler.initTermCount << scheduler.currTermPool << scheduler.currTerm;

    return( out );
}

QDataStream& operator>>( QDataStream& in, TermScheduler& scheduler ) {
    QString tempFirstLanguage;
    QString tempTestLanguage;
    QList<TermKey> tempTermPool[ TermScheduler::poolCount ];
    QList<StandbyTerm> tempStandbyPool;
    QList<TermKey> tempAllTerms;
    int tempInitQuizLength;
    int tempInitTermCount;
    int tempCurrTermPool;
    TermKey tempCurrTerm;

    in >> tempFirstLanguage >> tempTestLanguage;
    for( int i = 0; i < TermScheduler::poolCount; i++ ) {
        for( ;; ) {
            TermKey tempTermKey;
            in >> tempTermKey;
            if( tempTermKey.getTermId() == 0 && tempTermKey.getVocabId() == 0 )
                break;
            tempTermPool[ i ].append( tempTermKey );
        }
    }

    for( ;; ) {
        StandbyTerm tempStandbyTerm;
        in >> tempStandbyTerm;
        if( tempStandbyTerm.getPool() == 0 && tempStandbyTerm.getKey().getTermId() == 0 && tempStandbyTerm.getKey().getVocabId() == 0 )
            break;
        tempStandbyPool.append( tempStandbyTerm );
    }

    in >> tempAllTerms;
    in >> tempInitQuizLength >> tempInitTermCount >> tempCurrTermPool >> tempCurrTerm;

    scheduler.quizFirstLang = tempFirstLanguage;
    scheduler.quizTestLang = tempTestLanguage;
    for( int i = 0; i < TermScheduler::poolCount; i++ )
        scheduler.termPool[ i ] = tempTermPool[ i ]; 
    scheduler.standbyPool = tempStandbyPool;
    scheduler.allTerms = tempAllTerms;
    scheduler.initQuizLength = tempInitQuizLength;
    scheduler.initTermCount = tempInitTermCount;
    scheduler.currTermPool = tempCurrTermPool;
    scheduler.currTerm = tempCurrTerm;

    return( in );
}
