#include "Controller.h"

Controller::Controller() : vocabTree( NULL ), scheduler( prefs ) {
    applicationDirName = QDir::homePath() + QString( "/.toMOTko" );
    markedFilename = applicationDirName + QString( "/marked.dat.z" );
    prefs.setApplicationDirName( applicationDirName );
    scheduler.setApplicationDirName( applicationDirName );
}

bool Controller::init() {
    QDir applDir( applicationDirName );
    if( !applDir.exists() ) {
        if( !applDir.mkdir( applicationDirName ) )
            return( false );
    }
    getPreferences().load();
    return( true ); 
}

const QString& Controller::getApplicationDirName() const {
    return( applicationDirName );
}

Preferences& Controller::getPreferences() {
    return( prefs );
}

Folder* Controller::getVocabTree() {
    return( vocabTree );
}

void Controller::startQuiz() {
    scheduler.init( getPreferences().getFirstLanguage(), getPreferences().getTestLanguage(), vocabTree );
}

void Controller::restartQuiz() {
    scheduler.reinit();
}

bool Controller::isResumableQuizAvailable() {
    return( scheduler.isResumableQuizAvailable( BilingualKey( getPreferences().getFirstLanguage(), getPreferences().getTestLanguage() ) ) );
}

bool Controller::resumeQuiz() {
    BilingualKey key( getPreferences().getFirstLanguage(), getPreferences().getTestLanguage() );
    return( scheduler.load( key ) );
}

void Controller::prepareQuiz() {
    emit( progressChanged( getProgress() ) );
}

void Controller::concludeQuiz() {
    scheduler.concludeQuiz();
}

Term* Controller::getCurrentTerm() {
    TermKey key = scheduler.getCurrentTerm();
    Vocabulary* vocab = vocabTree->getVocabulary( key.getVocabId() );
    if( vocab && !vocab->isMarkedForDeletion() && vocab->isTermExists( key.getTermId() ) ) {
        Term& term = vocab->getTerm( key.getTermId() );
        return( &term );
    }
    // The term is invalid.  Either the container vocabulary or the term itself have been deleted.
    // Let's try to fetch the next term.
    scheduler.discardCurrentTerm();
    return( getNextTerm() );
}

Term* Controller::getTerm( const TermKey& termKey ) {
    Vocabulary* vocab = vocabTree->getVocabulary( termKey.getVocabId() );
    if( vocab && !vocab->isMarkedForDeletion() && vocab->isTermExists( termKey.getTermId() ) ) {
        Term& term = vocab->getTerm( termKey.getTermId() );
        return( &term );
    }
    return( NULL );
}

Term* Controller::getNextTerm() {
    TermKey nextTerm = scheduler.getNextTerm();
    if( nextTerm.isNull() )
        return( NULL );

    TermKey currTerm = scheduler.getCurrentTerm();
    Term* term = getTerm( currTerm );
    if( term )
        return( term );

    // The term is invalid.  Either the container vocabulary or the term itself have been deleted.
    // Let's try to fetch the next term.
    scheduler.discardCurrentTerm();
    return( scheduler.hasNextTerm() ? getNextTerm() : NULL );
}

bool Controller::hasNextTerm() const {
    return( scheduler.hasNextTerm() );
}

QString Controller::getQuizFirstLanguage() const {
    return( scheduler.getQuizFirstLanguage() );
}

QString Controller::getQuizTestLanguage() const {
    return( scheduler.getQuizTestLanguage() );
}

bool Controller::isQuizInProgress() const {
    return( scheduler.isQuizInProgress() );
}

int Controller::getProgress() const {
    return( scheduler.getProgress() );
}

int Controller::getInitialTermCount() const {
    return( scheduler.getInitialTermCount() );
}

Sequence& Controller::getRevealingSequence() {
    return( currRevealingSeq );
}

int Controller::getRevealingSequenceStep() const {
    return( currRevealingSeqStep );
}

void Controller::incrementRevealingSequenceStep() {
    currRevealingSeqStep++;
}

Folder* Controller::addFolder( Folder* parentFolder, Folder* folder /*= NULL*/ ) {
    int newId = parentFolder->getRoot()->getMaxId() + 1;

    QString folderLocation = applicationDirName + "/" + parentFolder->getPath() /*+ "/" + QString::number( newId )*/;
    QDir folderDir( folderLocation );
    if( !folderDir.exists() ) {
        if( !folderDir.mkpath( folderDir.path() ) ) {
            cerr << "Cannot create directory " << qPrintable( folderLocation ) << endl;
            return( NULL );
        }
    }

    QString newTitle = ( folder ? folder->getTitle() : QObject::tr( "NewFolder" ) );
    Folder* newFolder = new Folder( newId, newTitle );
    newFolder->setAuthor( folder ? folder->getAuthor() : parentFolder->getAuthor() );
    if( folder ) {
        newFolder->setDescription( folder->getDescription() );
        newFolder->setCreationDate( folder->getCreationDate() );
        newFolder->setModificationDate( folder->getModificationDate() );
    }
    newFolder->setDirty( true ); // Force saving data.
    if( !saveFolder( newFolder, folderLocation ) ) {
        cerr << "Could not save the new folder." << endl;
        return( NULL );
    }
    parentFolder->add( newFolder );
    return( newFolder );
}

Vocabulary* Controller::addVocabulary( Folder* parentFolder, Vocabulary* vocab /*= NULL*/ ) {
    int newId = parentFolder->getRoot()->getMaxVocabId() + 1;

    QString vocabLocation = applicationDirName + "/" + parentFolder->getPath() + "/v-" + QString::number( newId );
    QDir vocabDir( vocabLocation );
    if( !vocabDir.exists() ) {
        if( !vocabDir.mkpath( vocabDir.path() ) ) {
            cerr << "Cannot create directory " << qPrintable( vocabLocation ) << endl;
            return( NULL );
        }
    }

    QString newTitle = ( vocab ? vocab->getTitle() : QObject::tr( "NewGlossary" ) );
    Vocabulary* newVocab = new Vocabulary( newId, newTitle );
    newVocab->setAuthor( vocab ? vocab->getAuthor() : parentFolder->getAuthor() );
    if( vocab ) {
        newVocab->setDescription( vocab->getDescription() );
        newVocab->setCreationDate( vocab->getCreationDate() );
        newVocab->setModificationDate( vocab->getModificationDate() );
        for( Vocabulary::TermMap::ConstIterator it = vocab->begin(); it != vocab->end(); it++ ) {
            const Term& term = *it;
            Term newTerm( newVocab->getMaxTermId() + 1, newVocab->getId() );
            for( Term::TranslationMap::ConstIterator it2 = term.translationsBegin(); it2 != term.translationsEnd(); it2++ ) {
                const Translation& trans = *it2;
                newTerm.addTranslation( trans );
            }
            for( Term::CommentMap::ConstIterator it3 = term.commentsBegin(); it3 != term.commentsEnd(); it3++ ) {
                const BilingualKey& key = it3.key();
                const QString& comment = *it3;
                newTerm.addComment( key, comment );
            }
            if( !term.getImagePath().isNull() ) {
                // If the path refers to a file outside toMOTko's directory, we just copy the path.
                // Otherwise, we copy the image as well as the container vocabulary may be moved or deleted ulteriorly.
                if( term.getImagePath().left( getApplicationDirName().length() ) == getApplicationDirName() ) {
                    QFileInfo imageToCopyInfo( term.getImagePath() );
                    QString imageFilename = imageToCopyInfo.fileName();
                    if( QFile::copy( term.getImagePath(), vocabLocation + "/" + imageFilename ) )
                        newTerm.setImagePath( imageFilename ); 
                    else
                        cerr << "Could not copy " << qPrintable( term.getImagePath() ) << " to " << qPrintable( vocabLocation + "/" + imageFilename ) << endl;
                }
                else
                    newTerm.setImagePath( term.getImagePath() ); 
            }
            newVocab->addTerm( newTerm );
        }
        newVocab->setDirty( true ); // Force saving data.
    }
    if( !saveVocabulary( newVocab, vocabLocation ) ) {
        cerr << "Could not save the new vocabulary." << endl;
        return( NULL );
    }
    parentFolder->add( newVocab );
    return( newVocab );
}

void Controller::copy( Vocabulary* vocab ) {
    QString firstLang( getPreferences().getFirstLanguage() );
    QString testLang( getPreferences().getTestLanguage() );
    Vocabulary* vocabCopy = makeCopy( vocab, firstLang, testLang );

    QByteArray data;
    QDataStream out( &data, QIODevice::WriteOnly );
    out << *vocabCopy;
    setClipboardData( QString( "vocabulary" ), qCompress( data ) );
}

void Controller::copy( Folder* folder ) {
    QString firstLang( getPreferences().getFirstLanguage() );
    QString testLang( getPreferences().getTestLanguage() );
    Folder* folderCopy = makeCopy( folder, firstLang, testLang );

    QByteArray data;
    QDataStream out( &data, QIODevice::WriteOnly );
    out << *folderCopy;

    // The Folder::operator<< will only write references for vocabularies.
    // We build a map containing copies of the vocabularies.
    
    QMap<int,Vocabulary> vocabularies;
    folderCopy->buildVocabCopiesMap( vocabularies );
    out << vocabularies;

    setClipboardData( QString( "folder" ), qCompress( data ) );
}

Vocabulary* Controller::makeCopy( Vocabulary* vocab, const QString& firstLang, const QString& testLang ) const {
    QStringList languages;
    languages << firstLang << testLang;

    Vocabulary* vocabCopy = new Vocabulary( vocab->getId(), vocab->getTitle() );
    vocabCopy->setDescription( vocab->getDescription() );
    vocabCopy->setAuthor( vocab->getAuthor() );
    vocabCopy->setCreationDate( vocab->getCreationDate() );
    vocabCopy->setModificationDate( vocab->getModificationDate() );
    for( Vocabulary::TermMap::ConstIterator it = vocab->begin(); it != vocab->end(); it++ ) {
        const Term& term = *it;
        Term* termCopy = new Term( vocabCopy->getMaxTermId() + 1, vocabCopy->getId() );

        for( QStringList::ConstIterator it = languages.begin(); it != languages.end(); it++ ) {
            const QString& lang = *it;
            if( term.isTranslationExists( lang ) ) {
                Translation transCopy( term.getTranslation( lang ) );
                termCopy->addTranslation( transCopy );
            }
        }
        BilingualKey commentKey( firstLang, testLang );
        if( term.isCommentExists( commentKey ) )
            termCopy->addComment( commentKey, term.getComment( commentKey ) );

        termCopy->setImagePath( getResolvedImagePath( term.getImagePath(), *vocab ) );

        vocabCopy->addTerm( *termCopy );
    }
    return( vocabCopy );
}

Folder* Controller::makeCopy( Folder* folder, const QString& firstLang, const QString& testLang ) const {
    QStringList languages;
    languages << firstLang << testLang;

    Folder* folderCopy = new Folder( folder->getId(), folder->getTitle() );
    folderCopy->setDescription( folder->getDescription() );
    folderCopy->setAuthor( folder->getAuthor() );
    folderCopy->setCreationDate( folder->getCreationDate() );
    folderCopy->setModificationDate( folder->getModificationDate() );
    for( int i = 0; i < folder->getChildrenCount(); i++ ) {
        Base* folderChild = folder->childAt( i );
        if( strcmp( folderChild->className(), "Folder" ) == 0 ) {
            Folder* childFolder = (Folder*)folderChild;
            if( childFolder->containsTermWithTranslations( firstLang, testLang ) ) {
                Folder* childFolderCopy = makeCopy( childFolder, firstLang, testLang );
                folderCopy->add( childFolderCopy );
            }
        }
        else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 ) {
            Vocabulary* childVocab = (Vocabulary*)folderChild;
            if( childVocab->containsTermWithTranslations( firstLang, testLang ) ) {
                Vocabulary* childVocabCopy = makeCopy( childVocab, firstLang, testLang );
                folderCopy->add( childVocabCopy );
            }
        }
    }
    return( folderCopy );
}

Vocabulary* Controller::loadVocabulary( const QString& parentDir ) {
    Vocabulary* vocab = NULL;
    QDir dir( parentDir );
    QString vocabDirName = dir.dirName();
    int indexOfDash = vocabDirName.indexOf( "-" );
    if( indexOfDash > 0 ) {
        bool isOk;
        QString strVocabId = vocabDirName.right( vocabDirName.length() - indexOfDash - 1 );
        int vocabId = strVocabId.toInt( &isOk );
        QString filename( parentDir + QString( "/vocab-" ) + QString::number( vocabId ) + QString( ".gz" ) );
        QFile compressedBinFile( filename );
        if( compressedBinFile.exists() ) {
            vocab = new Vocabulary();
            if( !vocab->load( filename ) ) {
                delete( vocab );
                vocab = NULL;
                cerr << "Could not load vocab file in directory " << qPrintable( parentDir ) << endl;
            }
        }
    }
    return( vocab );
}

Base* Controller::importData( Folder* folder, const QString& filename, const QStringList& languages ) {
    Folder* rootFolder = folder->getRoot();

    int status;
    Base* newItem = NULL;

    zipFile inputFile = unzOpen( filename.toLatin1() );
    if( inputFile == NULL )
        return( NULL );

    unz_global_info gi;
    status = unzGetGlobalInfo( inputFile, &gi );

    if( status == UNZ_OK ) {
        QMap<int,Vocabulary*> newVocabs;
        QMap<int,Folder*> newFolders;
        Folder* newFolder = NULL;
        Vocabulary* newVocab = NULL;
        int newVocabId = -1;
        int newFolderId = -1;
            
        // Handle all entries.
        for( uLong i = 0; i < gi.number_entry; i++ ) {
            char filename_inzip[ 256 ];
            unz_file_info file_info;
            status = unzGetCurrentFileInfo( inputFile, &file_info, filename_inzip, sizeof( filename_inzip ), NULL, 0, NULL, 0 );
            if( status != UNZ_OK ) 
                break;

            QString filenameInZip( filename_inzip );
            QFileInfo fileInfo( filenameInZip );
            //cout << "f=" << filename_inzip << " dp=" << qPrintable( fileInfo.filePath() ) << " p=" << qPrintable( fileInfo.path() ) << 
            //    " dpa=" << qPrintable( fileInfo.absoluteFilePath() ) << " fn=" << qPrintable( fileInfo.fileName() ) << endl;
            if( fileInfo.completeSuffix() == "gif" || fileInfo.completeSuffix() == "png" ) {
                int importedVocabId = findVocabId( fileInfo.path() );
                if( newVocabs.contains( importedVocabId ) )
                    newVocab = newVocabs[ importedVocabId ];
                else{
                    newVocabId = ( newVocabId == -1 ? rootFolder->getMaxVocabId() + 1 : newVocabId + 1 );
                    newVocab = new Vocabulary( newVocabId );
                    newVocabs.insert( importedVocabId, newVocab );
                    if( !newItem )
                        newItem = newVocab;
                    else {
                        int parentFolderId = findParentFolderId( fileInfo.path() );
                        if( parentFolderId != -1 && newFolders.contains( parentFolderId ) )
                            newFolders[ parentFolderId ]->add( newVocab ); 
                    }
                }

                QString imageLocation = applicationDirName + "/" + folder->getPath() + convertPath( fileInfo.path(), newFolders ) +
                    "/v-" + QString::number( newVocab->getId() ) + "/" + fileInfo.fileName();
                importImageFromZip( imageLocation, inputFile );
            }
            else if( fileInfo.completeSuffix() == "xml" ) {
                if( fileInfo.fileName().left( 7 ) == "folder-" ) {
                    int importedFolderId = findFolderId( fileInfo.fileName() );
                   
                    if( newFolders.contains( importedFolderId ) ) 
                        newFolder = newFolders[ importedFolderId ];
                    else {
                        newFolderId = ( newFolderId == -1 ? rootFolder->getMaxId() + 1 : newFolderId + 1 );
                        newFolder = new Folder( newFolderId );
                        newFolders.insert( importedFolderId, newFolder );
                        if( !newItem )
                            newItem = newFolder;
                        else {
                            int parentFolderId = findParentFolderId( fileInfo.path() );
                            if( parentFolderId != -1 && newFolders.contains( parentFolderId ) )
                                newFolders[ parentFolderId ]->add( newFolder ); 
                        }
                    }
                
                    QString folderLocation = applicationDirName + "/" + folder->getPath() + convertPath( fileInfo.path(), newFolders );
                    importFolderFromZip( newFolder, folderLocation, inputFile ); // Should we handle error here?
                }
                else if( fileInfo.fileName().left( 6 ) == "vocab-" ) {
                    int importedVocabId = findVocabId( fileInfo.path() );
                    if( newVocabs.contains( importedVocabId ) ) 
                        newVocab = newVocabs[ importedVocabId ];
                    else {
                        newVocabId = ( newVocabId == -1 ? rootFolder->getMaxVocabId() + 1 : newVocabId + 1 );
                        newVocab = new Vocabulary( newVocabId );
                        newVocabs.insert( importedVocabId, newVocab );
                        if( !newItem )
                            newItem = newVocab;
                        else {
                            int parentFolderId = findParentFolderId( fileInfo.path() );
                            if( parentFolderId != -1 && newFolders.contains( parentFolderId ) )
                                newFolders[ parentFolderId ]->add( newVocab ); 
                        }
                    }

                    QString vocabLocation = applicationDirName + "/" + folder->getPath() + convertPath( fileInfo.path(), newFolders ) +
                        "/v-" + QString::number( newVocab->getId() );
                    importVocabularyFromZip( newVocab, vocabLocation, languages, inputFile ); // Should we handle error here?
                }
            }

            status = unzGoToNextFile( inputFile );
            if( status == UNZ_END_OF_LIST_OF_FILE ) {
                status = UNZ_OK;
                break;
            }
            else if( status != UNZ_OK ) {
                // A problem occurred so exit loop.
                break;
            }
        }
    }

    // If an error occurred, we recover memory if necessary.
    if( status != UNZ_OK ) {
        if( newItem ) {
            delete( newItem );
            newItem = NULL;
        }
    }

    if( unzClose( inputFile ) != UNZ_OK )
        return( NULL );

    return( newItem );
}

QStringList Controller::getTranslationLanguagesFromFile( const QString& filename ) const {
    QStringList languages;

    zipFile inputFile = unzOpen( filename.toLatin1() );
    if( inputFile ) {
        unz_global_info gi;
        int status = unzGetGlobalInfo( inputFile, &gi );

        if( status == UNZ_OK ) {
            for( uLong i = 0; i < gi.number_entry; i++ ) {
                char filename_inzip[ 256 ];
                unz_file_info file_info;
                status = unzGetCurrentFileInfo( inputFile, &file_info, filename_inzip, sizeof( filename_inzip ), NULL, 0, NULL, 0 );
                if( status != UNZ_OK ) 
                    break;

                QString filenameInZip( filename_inzip );
                QFileInfo fileInfo( filenameInZip );
                if( fileInfo.fileName().left( 6 ) == "vocab-" && fileInfo.completeSuffix() == "xml" ) {
                    QStringList vocabLanguages = getVocabularyTranslationLanguagesFromZip( inputFile );
                    for( QStringList::Iterator it = vocabLanguages.begin(); it != vocabLanguages.end(); it++ ) {
                        const QString& lang = *it;
                        if( !languages.contains( lang ) )
                            languages.append( lang );
                    }
                }

                status = unzGoToNextFile( inputFile );
                if( status == UNZ_END_OF_LIST_OF_FILE ) {
                    status = UNZ_OK;
                    break;
                }
                else if( status != UNZ_OK ) {
                    cerr << "Cannot get next entry in Zip file: " << qPrintable( filename ) << endl;
                    break; // An error has occured so exit the loop.
                }
            }
        }

        if( unzClose( inputFile ) != UNZ_OK )
            cerr << "Cannot close file: " << qPrintable( filename ) << endl;
    }
    else
        cerr << "Cannot open file: " << qPrintable( filename ) << endl;
    
    return( languages );
}

QStringList Controller::getVocabularyTranslationLanguagesFromZip( zipFile inputFile ) const {
    QStringList languages;

    int status = unzOpenCurrentFile( inputFile );
    if( status == UNZ_OK ) {
        bool isOk = true;

        char* buf = NULL;
        uInt size_buf = 8192; // Arbitrary size for now.
        buf = (char*)malloc( size_buf );
        if( buf == NULL ) {
            cerr << "Cannot allocate memory for unzip buffer." << endl;
            status = UNZ_INTERNALERROR;
        }

        if( status == UNZ_OK ) {
            QByteArray ba;

            int readStatus;
            for( ;; ) {
                readStatus = unzReadCurrentFile( inputFile, buf, size_buf );
                if( readStatus > 0 )
                    ba.append( QByteArray( buf, readStatus ) );
                else
                    break;
            }

            if( readStatus == 0 ) {
                Vocabulary* vocab = new Vocabulary();
                QTextStream ts2( ba, QIODevice::ReadOnly );
                ts2.setCodec( "UTF-8" );
                VocabParser parser( *vocab, languages );
                QXmlInputSource source;
                source.setData( ts2.readAll() );

                QXmlSimpleReader reader;
                reader.setContentHandler( &parser );
                isOk = reader.parse( source );

                languages = vocab->getTranslationLanguages();

                delete( vocab );
                vocab = NULL;
            }
        }

        free( buf );

        if( unzCloseCurrentFile( inputFile ) != UNZ_OK )
            cerr << "Cannot close zip entry." << endl; 
    }
    else
        cerr << "Cannot open zip entry." << endl;

    return( languages );
}

bool Controller::importFolderFromZip( Folder* folder, const QString& folderLocation, zipFile inputFile ) {
    int status = unzOpenCurrentFile( inputFile );
    if( status != UNZ_OK )
        return( false );

    bool isOk = true;

    char* buf = NULL;
    uInt size_buf = 8192; // Arbitrary size for now.
    buf = (char*)malloc( size_buf );
    if( buf == NULL ) {
        cerr << "Cannot allocate memory for unzip buffer." << endl;
        status = UNZ_INTERNALERROR;
    }

    if( status == UNZ_OK ) {
        QByteArray ba;

        int readStatus;
        int totalBytes = 0;
        for( ;; ) {
            readStatus = unzReadCurrentFile( inputFile, buf, size_buf );
            if( readStatus > 0 ) {
                ba.append( QByteArray( buf, readStatus ) );
                totalBytes += readStatus;
            }
            else
                break;
        }
        if( readStatus == 0 ) {
            QTextStream ts2( ba, QIODevice::ReadOnly );
            ts2.setCodec( "UTF-8" );
            FolderParser parser( *folder );
            QXmlInputSource source;
            source.setData( ts2.readAll() );

            QXmlSimpleReader reader;
            reader.setContentHandler( &parser );
            isOk = reader.parse( source );
            if( isOk ) {
                // Create the containing folder if needed.
                QDir folderDir( folderLocation );
                isOk = folderDir.mkpath( folderLocation );

                if( isOk ) {
                    const QString& folderDataFilename( folderLocation + "/folder-" + QString::number( folder->getId() ) + ".gz" );
                    isOk = folder->saveMetadata( folderDataFilename );
                    if( isOk )
                        folder->setDirty( false );
                }
                else {
                    cerr << "Could not create directory " << qPrintable( folderLocation ) << endl;
                    isOk = false;
                }
            }

            //if( !isOk || !parser.isVocabularyFile() ) {
            //    delete( vocab );
            //    vocab = NULL;
            //}
        }
    }

    free( buf );

    /*status =*/ unzCloseCurrentFile( inputFile );
    //if( readStatus != UNZ_OK || status != UNZ_OK ) 
    //    return( NULL );
    return( isOk );
}

bool Controller::importVocabularyFromZip( Vocabulary* vocab, const QString& vocabLocation, const QStringList& languages, zipFile inputFile ) {
    int status = unzOpenCurrentFile( inputFile );
    if( status != UNZ_OK )
        return( false );

    bool isOk = true;

    char* buf = NULL;
    uInt size_buf = 8192; // Arbitrary size for now.
    buf = (char*)malloc( size_buf );
    if( buf == NULL ) {
        cerr << "Cannot allocate memory for unzip buffer." << endl;
        status = UNZ_INTERNALERROR;
    }

    if( status == UNZ_OK ) {
        QByteArray ba;

        int readStatus;
        for( ;; ) {
            readStatus = unzReadCurrentFile( inputFile, buf, size_buf );
            if( readStatus > 0 )
                ba.append( QByteArray( buf, readStatus ) );
            else
                break;
        }

        if( readStatus == 0 ) {
            QTextStream ts2( ba, QIODevice::ReadOnly );
            ts2.setCodec( "UTF-8" );
            VocabParser parser( *vocab, languages );
            QXmlInputSource source;
            source.setData( ts2.readAll() );

            QXmlSimpleReader reader;
            reader.setContentHandler( &parser );
            isOk = reader.parse( source );
            if( isOk ) {
                vocab->setDirty( true ); // Force saving data.
                isOk = saveVocabulary( vocab, vocabLocation );
            }
        }
    }

    free( buf );

    /*status =*/ unzCloseCurrentFile( inputFile );
    //if( readStatus != UNZ_OK || status != UNZ_OK ) 
    //    return( NULL );
    return( isOk );
}

bool Controller::importImageFromZip( const QString& imagePath, zipFile inputFile ) {
    int status = unzOpenCurrentFile( inputFile );
    if( status != UNZ_OK )
        return( false );

    bool isOk = true;

    char* buf = NULL;
    uInt size_buf = 8192; // Arbitrary size for now.
    buf = (char*)malloc( size_buf );
    if( buf == NULL ) {
        cerr << "Cannot allocate memory for unzip buffer." << endl;
        status = UNZ_INTERNALERROR;
    }

    if( status == UNZ_OK ) {
        QFile imageFile( imagePath );

        const QString imageDir = QFileInfo( imageFile ).absolutePath();
        QDir imgDir( imageDir );
        isOk = imgDir.mkpath( imageDir );
        if( isOk ) {
            isOk = imageFile.open( QIODevice::WriteOnly );
            if( isOk ) {
                QDataStream ts( &imageFile );
                int readStatus;
                for( ;; ) {
                    readStatus = unzReadCurrentFile( inputFile, buf, size_buf );
                    if( readStatus > 0 )
                        ts.writeRawData( buf, readStatus );
                    else
                        break;
                }
                imageFile.close();
            }
            else 
                cerr << "Could not create file " << qPrintable( imagePath ) << endl;
        }
        else
            cerr << "Could not create directory " << qPrintable( imageDir ) << endl;
    }

    free( buf );

    /*status =*/ unzCloseCurrentFile( inputFile );
    //if( readStatus != UNZ_OK || status != UNZ_OK ) 
    //    return( NULL );
    return( isOk );
}

void Controller::loadData() {
    vocabTree = loadFolder( applicationDirName );
    if( !vocabTree ) {
        Folder* folder = new Folder( 1, QObject::tr( "MyGlossaries" ) );
        folder->setMarkedForStudy( true );
        folder->setDirty( true );
        vocabTree = folder;
    }
    loadMarkedItems( vocabTree );
}

void Controller::rightAnswer() {
    scheduler.rightAnswer();
    emit( progressChanged( getProgress() ) );
}

void Controller::wrongAnswer() {
    scheduler.wrongAnswer();
    emit( progressChanged( getProgress() ) );
}

void Controller::reveal() {
}

Folder* Controller::loadFolder( const QString& parentDir ) {
    Folder* newFolder = NULL;

    QDir dir( parentDir );
    bool isOk;
    dir.dirName().toInt( &isOk );

    if( isOk ) {
        newFolder = new Folder();
        QString folderMetadataFile = QString( parentDir + "/folder-" + dir.dirName() + ".gz" );
        if( !newFolder->loadMetadata( folderMetadataFile ) ) {
            cerr << "Could not load metadata file " << qPrintable( folderMetadataFile ) << endl;
            delete( newFolder );
            return( NULL );
        }
    }

    QStringList entries = dir.entryList();
    for( QStringList::Iterator it = entries.begin(); it != entries.end(); it++ ) {
        QString entry = *it;
        QString entryPath = parentDir + "/" + entry;
        QFileInfo info( entryPath );
        if( info.isDir() && entry != "." && entry != ".." ) {
            if( !isOk ) 
                newFolder = loadFolder( entryPath );
            else { 
                if( entry.left( 2 ) == QString( "v-" ) ) {
                    Vocabulary* childVocab = loadVocabulary( entryPath );
                    if( childVocab )
                        newFolder->add( childVocab );
                }
                else {
                    Folder* childFolder = loadFolder( entryPath );
                    if( childFolder )
                        newFolder->add( childFolder ); 
                }
            }
        }
    }

    return( newFolder );
}

bool Controller::saveFolder( Folder* folder, const QString& parentDir ) const {
    //cerr << "saveFolder folder=" << folder << " location=" << parentDir << " dirty? " << folder->isDirty() << endl;
    // Create the folder.
    QString folderPath( parentDir + QString( "/" ) + QString::number( folder->getId() ) );
    QDir folderDir( folderPath );
    if( folder->isDirty() && !folderDir.exists() ) {
        if( !folderDir.mkdir( folderDir.path() ) ) {
            cerr << "Could not make directory " << qPrintable( folderPath ) << endl;
            return( false );
        }
    }

    // Write the folder data.
    if( folder->isDirty() ) {
        QString folderDataFilename( QString( folderDir.path() + QString( "/folder-" ) + QString::number( folder->getId() ) + QString( ".gz" ) ) );
        if( !folder->saveMetadata( folderDataFilename ) ) {
            cerr << "Could not write folder metadata " << qPrintable( folderDataFilename ) << endl;
            return( false );
        }
        folder->setDirty( false );
    }

    // Handle children recursively.
    if( !folder->isEmpty() ) {
        for( int i = 0; i < folder->getChildrenCount(); i++ ) {
            Base* folderChild = folder->childAt( i );
            if( strcmp( folderChild->className(), "Folder" ) == 0 ) {
                Folder* childFolder = (Folder*)folderChild;
                if( !childFolder->isMarkedForDeletion() ) 
                    saveFolder( childFolder, folderDir.path() );
            }
            else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 ) {
                Vocabulary* childVocab = (Vocabulary*)folderChild;
                if( !childVocab->isMarkedForDeletion() )
                    saveVocabulary( childVocab, folderDir.path() );
            }
        }
    }

    return( true );
}

bool Controller::deleteItemsMarkedForDeletion( Folder* folder ) {
    for( int i = 0; i < folder->getChildrenCount(); i++ ) {
        Base* childItem = folder->childAt( i );
        if( strcmp( childItem->className(), "Folder" ) == 0 ) {
            Folder* childFolder = new Folder( *((Folder*)childItem) );
            deleteItemsMarkedForDeletion( childFolder );
        }
        else if( strcmp( childItem->className(), "Vocabulary" ) == 0 ) {
            Vocabulary* childVocab = new Vocabulary( *((Vocabulary*)childItem) ); 
            if( childVocab->isMarkedForDeletion() ) {
                QString vocabDir( applicationDirName + "/" + folder->getPath() + "/v-" + QString::number( childVocab->getId() ) );
                if( !Util::deleteDirectory( vocabDir ) ) {
                    cerr << "Cannot delete glossary directory " << qPrintable( vocabDir ) << endl;
                    return( false );
                }
            }
        }
    }

    if( folder->isMarkedForDeletion() ) {
        // To remove the reference to the deleted folder in preferences, we set it as opened.
        getPreferences().setFolderOpen( folder->getId(), true );
        QString folderDir( applicationDirName + "/" + folder->getPath() );
        if( !Util::deleteDirectory( folderDir ) ) {
            cerr << "Cannot delete folder directory " << qPrintable( folderDir ) << endl;
            return( false );
        }
    }

    return( true );
}

int Controller::findFolderId( const QString& filename ) const {
    int indexOfDash = filename.indexOf( "-" );
    int indexOfDot = filename.indexOf( "." );
    const QString& strFolderId = filename.mid( indexOfDash + 1, indexOfDot - indexOfDash - 1 );
    bool isOk;
    int folderId = strFolderId.toInt( &isOk );
    return( isOk ? folderId : -1 );
}

int Controller::findParentFolderId( const QString& dirPath ) const {
    int lastSlashPos = dirPath.lastIndexOf( "/" );
    if( lastSlashPos > 0 ) {
        int beforeLastSlashPos = dirPath.lastIndexOf( "/", lastSlashPos - 1 );
        if( beforeLastSlashPos > 0 ) {
            QString strParentFolderId = dirPath.mid( beforeLastSlashPos + 1, lastSlashPos - beforeLastSlashPos - 1 );
            bool isOk;
            int parentFolderId = strParentFolderId.toInt( &isOk );
            return( isOk ? parentFolderId : -1 );
        }
        else {
            QString strParentFolderId = dirPath.left( lastSlashPos );
            bool isOk;
            int parentFolderId = strParentFolderId.toInt( &isOk );
            return( isOk ? parentFolderId : -1 );
        }
    }
    return( -1 );
}

int Controller::findVocabId( const QString& dirPath ) const {
    int posOfLeftDelim = dirPath.indexOf( "v-" );
    int posOfRightDelim = dirPath.lastIndexOf( "/" );
    QString strImportedVocabId = dirPath.mid( posOfLeftDelim + 2, posOfRightDelim - posOfLeftDelim - 2 );
    bool isOk;
    int importedVocabId = strImportedVocabId.toInt( &isOk );
    return( isOk ? importedVocabId : -1 );
}

QString Controller::convertPath( const QString& path, QMap<int,Folder*>& newFolders ) const {
    QString realPath;
    QStringList subDirs = path.split( "/" );
    for( QStringList::Iterator it = subDirs.begin(); it != subDirs.end(); it++ ) {
        const QString& strFolderId = (*it);
        bool isOk;
        int subDirFolderId = strFolderId.toInt( &isOk );
        if( isOk && newFolders.contains( subDirFolderId ) ) {
            Folder* assocFolder = newFolders[ subDirFolderId ];
            realPath += "/" + QString::number( assocFolder->getId() );
        }
    }
    return( realPath );
}

bool Controller::saveData() {
    if( !deleteItemsMarkedForDeletion( vocabTree ) ) {
        // Just write a warning message.  We don't return( false ) here
        // because we want to try to save the new data, at least.
        cerr << "Could not delete all items marked for deletion." << endl;
    }
    if( !saveFolder( vocabTree, applicationDirName ) )
        return( false );
    if( !saveMarkedItems( vocabTree ) )
        return( false );
    return( prefs.save() );
}

bool Controller::saveMarkedItems( Folder* folder ) {
    QByteArray data;

    QDataStream out( &data, QIODevice::WriteOnly );
    out.setVersion( QDataStream::Qt_2_1 );

    // 0x0011 means 0.11.x version.
    out << qint32( Preferences::magicNumber ) << qint16( 0x0011 );

    IdList folderIds;
    IdList vocabIds;
    IdListMap termIds;

    saveMarkedItemsRec( folder, folderIds, vocabIds, termIds );

    out << folderIds << vocabIds << termIds;

    QByteArray compressedData( qCompress( data ) ); 

    QFile dataFile( markedFilename );
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

void Controller::saveMarkedItemsRec( Folder* folder, IdList& folderIds, IdList& vocabIds, IdListMap& termIds ) {
    if( folder->isMarkedForStudy() )
        folderIds.append( folder->getId() );
    if( !folder->isEmpty() ) {
        for( int i = 0; i < folder->getChildrenCount(); i++ ) {
            Base* folderChild = folder->childAt( i );
            if( strcmp( folderChild->className(), "Folder" ) == 0 )
                saveMarkedItemsRec( (Folder*)folderChild, folderIds, vocabIds, termIds );
            else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 )
                saveMarkedItemsRec( (Vocabulary*)folderChild, vocabIds, termIds );
        }
    }
}

void Controller::saveMarkedItemsRec( Vocabulary* vocab, IdList& vocabIds, IdListMap& termIds ) {
    if( vocab->isMarkedForStudy() )
        vocabIds.append( vocab->getId() );
    IdList termIdList;
    for( Vocabulary::TermMap::ConstIterator it = vocab->begin(); it != vocab->end(); it++ ) {
        const Term& term = *it;
        if( term.isMarkedForStudy() )
            termIdList.append( term.getId() );
    }
    termIds.insert( vocab->getId(), termIdList );
}

void Controller::loadMarkedItems( Folder* folder ) {
    QFile markedFile( markedFilename );
    if( markedFile.exists() ) {
        if( !markedFile.open( QIODevice::ReadOnly ) )
            return;

        QByteArray compressedData( markedFile.readAll() );
        QByteArray data( qUncompress( compressedData ) );

        QDataStream in( data );

        qint32 tempMagicNumber;
        qint16 tempVersion;

        IdList tempFolderIds;
        IdList tempVocabIds;
        IdListMap tempTermIds;

        in >> tempMagicNumber >> tempVersion;

        if( tempMagicNumber != Preferences::magicNumber )
            cerr << "Wrong magic number: Incompatible data file for marked file." << endl;
        if( tempVersion > 0x0011 )
            cerr << "Marked data file is from a more recent version.  Upgrade toMOTko." << endl;

        in.setVersion( QDataStream::Qt_2_1 );
        in >> tempFolderIds >> tempVocabIds >> tempTermIds; 

        markedFile.close();
        
        initMarkedForStudyRec( folder, tempFolderIds, tempVocabIds, tempTermIds );
    }
}

void Controller::initMarkedForStudyRec( Folder* folder, IdList& folderIds, IdList& vocabIds, IdListMap& termIds ) {
    if( folderIds.contains( folder->getId() ) )
        folder->setMarkedForStudy( true );
    if( !folder->isEmpty() ) {
        for( int i = 0; i < folder->getChildrenCount(); i++ ) {
            Base* folderChild = folder->childAt( i );
            if( strcmp( folderChild->className(), "Folder" ) == 0 )
                initMarkedForStudyRec( (Folder*)folderChild, folderIds, vocabIds, termIds );
            else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 )
                initMarkedForStudyRec( (Vocabulary*)folderChild, vocabIds, termIds );
        }
    }
}

void Controller::initMarkedForStudyRec( Vocabulary* vocab, IdList& vocabIds, IdListMap& termIds ) {
    if( vocabIds.contains( vocab->getId() ) )
        vocab->setMarkedForStudy( true );
    IdList termIdList = termIds[ vocab->getId() ];
    for( IdList::ConstIterator it = termIdList.begin(); it != termIdList.end(); it++ ) {
        int termId = *it;
        if( vocab->isTermExists( termId ) ) {
            Term& term = vocab->getTerm( termId );
            term.setMarkedForStudy( true );
        }
    }
}

bool Controller::exportData( Vocabulary* vocab, const QString& file, QStringList* languages ) const {
    zipFile outputFile = zipOpen( file.toLatin1(), APPEND_STATUS_CREATE );
    if( outputFile == NULL )
        return( false );

    bool isOk = exportVocabularyIntoZip( vocab, outputFile, QString::null, languages );

    if( zipClose( outputFile, "Closing comment" ) != 0 )
        return( false );

    return( isOk );
}

bool Controller::exportVocabularyIntoZip( Vocabulary* vocab, zipFile outputFile, QString path, QStringList* languages ) const {
    QString vocabPath = ( path == QString::null ? QString( "v-" + QString::number( vocab->getId() ) ) : 
        path + "/v-" + QString::number( vocab->getId() ) );

    // Copy the referred images first.
    for( Vocabulary::TermMap::ConstIterator it = vocab->begin(); it != vocab->end(); it++ ) {
        const Term& term = *it;
        if( term.getImagePath() != QString::null ) {
            QString fileExtension = term.getImagePath().right( 4 );
            QByteArray imageDataFilename = QString( vocabPath + "/" + QString::number( term.getId() ) + fileExtension ).toLatin1();
            const char* filenameInZip = (const char*)imageDataFilename.data();
            QString absPath = getResolvedImagePath( term.getImagePath(), *vocab );
            QFile imageFile( absPath );
            if( imageFile.exists() ) {
                if( !imageFile.open( QIODevice::ReadOnly ) )
                    return( false );
                QByteArray buffer = imageFile.readAll();
                imageFile.close();

                int err = writeFileIntoZipFile( outputFile, filenameInZip, buffer.data(), buffer.size() );
                if( err != ZIP_OK )
                    return( false );
            }
            else {
                cerr << "Image " << qPrintable( imageFile.fileName() ) << " referenced by glossary " << qPrintable( vocab->getTitle() ) << 
                    " was not found, and therefore, was discarded during export." << endl;
            }
        }
    }

    // Copy the vocabulary itself in XML.
    QByteArray dataFilename = QString( vocabPath + "/vocab-" + QString::number( vocab->getId() ) + ".xml" ).toLatin1();
    const char* filenameInZip = (const char*)dataFilename.data();

    QByteArray buffer;
    QTextStream ts( &buffer, QIODevice::WriteOnly );
    ts.setCodec( "UTF-8" );
    writeVocabularyInXml( ts, *vocab, languages );
    int err = writeFileIntoZipFile( outputFile, filenameInZip, buffer.data(), buffer.size() );

    return( err == ZIP_OK );
}

bool Controller::exportData( Folder* folder, const QString& file, QStringList* languages ) const {
    zipFile outputFile = zipOpen( file.toLatin1(), APPEND_STATUS_CREATE );
    if( outputFile == NULL )
        return( false );

    bool isOk = exportFolderRecIntoZip( folder, outputFile, QString::null, languages );

    if( zipClose( outputFile, "Closing comment" ) != 0 )
        return( false );

    return( isOk );
}

bool Controller::exportFolderRecIntoZip( Folder* folder, zipFile outputFile, QString path, QStringList* languages ) const {
    QString folderPath = ( path == QString::null ? QString::number( folder->getId() ) : 
        path + QString( "/" ) + QString::number( folder->getId() ) );
    if( !folder->isEmpty() ) {
        QByteArray folderDataFilename = QString( folderPath + "/folder-" + QString::number( folder->getId() ) + ".xml" ).toLatin1();
        const char* filenameInZip = (const char*) ( folderDataFilename.data() );

        QByteArray buffer;
        QTextStream ts( &buffer, QIODevice::WriteOnly );
        ts.setCodec( "UTF-8" );
        writeFolderDataInXml( ts, *folder );

        int err = writeFileIntoZipFile( outputFile, filenameInZip, buffer.data(), buffer.size() );
        if( err != ZIP_OK )
            return( false );
    }

    for( int i = 0; i < folder->getChildrenCount(); i++ ) {
        Base* child = folder->childAt( i );
        if( strcmp( child->className(), "Vocabulary" ) == 0 ) {
            Vocabulary* childVocab = (Vocabulary*)child;
            if( !exportVocabularyIntoZip( childVocab, outputFile, folderPath, languages ) )
                return( false );
        }
        else if( strcmp( child->className(), "Folder" ) == 0 ) {
            Folder* childFolder = (Folder*)child;
            if( !exportFolderRecIntoZip( childFolder, outputFile, folderPath, languages ) )
                return( false );
        }
    }

    return( true );
}

int Controller::writeFileIntoZipFile( zipFile outputFile, const char* filename, const char* data, int length ) const {
    zip_fileinfo zipFileInfo;

    zipFileInfo.tmz_date.tm_sec = zipFileInfo.tmz_date.tm_min = zipFileInfo.tmz_date.tm_hour =
        zipFileInfo.tmz_date.tm_mday = zipFileInfo.tmz_date.tm_mon = zipFileInfo.tmz_date.tm_year = 0;
    zipFileInfo.dosDate = 0;
    zipFileInfo.internal_fa = 0;
    zipFileInfo.external_fa = 0;
    //filetime( filenameInZip, &zipFileInfo.tmz_date, &zipFileInfo.dosDate );

    int err = zipOpenNewFileInZip3( outputFile, filename, &zipFileInfo,
         NULL, 0, NULL, 0, NULL /* comment*/,
         Z_DEFLATED /* method */,
         5 /* level */, 0,
         /* -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, */
         -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
         NULL, 0 );
    if( err )
        return( err );

    int writeErr = zipWriteInFileInZip( outputFile, data, length );

    err = zipCloseFileInZip( outputFile );

    if( writeErr || err )
        return( writeErr ? writeErr : err );

    return( ZIP_OK );
}

void Controller::writeFolderDataInXml( QTextStream& ts, const Folder& folder ) const {
    ts << QString( "<?xml version=\"1.0\"?>" ) << endl;
    ts << QString( "<folder id=\"" ) << folder.getId() << QString( "\" name=\"" ) << Qt::escape( folder.getTitle() ) << QString( "\"" );

    if( !folder.getAuthor().isNull() )
        ts << QString( " author=\"" ) << Qt::escape( folder.getAuthor() ) << QString( "\"" );
    ts << QString( ">" ) << endl;

    if( !folder.getDescription().isNull() )
        ts << QString( "\t<desc>" ) << Qt::escape( folder.getDescription() ) << QString( "</desc>" ) << endl;
    ts << QString( "</folder>" ) << endl;
}

void Controller::writeVocabularyInXml( QTextStream& ts, const Vocabulary& vocab, QStringList* languages, bool writeXmlDirective /* = true */, int depth /* = 0 */ ) const {
    if( writeXmlDirective ) {
        for( int i = 0; i < depth; i++ )
            ts << "\t";
        ts << QString( "<?xml version=\"1.0\"?>" ) << endl;
    }

    for( int i = 0; i < depth; i++ )
        ts << "\t";
    ts << QString( "<glossary id=\"" ) << vocab.getId() << "\" name=\"" << Qt::escape( vocab.getTitle() ) << "\" ";
    ts << QString( "author=\"" ) << Qt::escape( vocab.getAuthor() ) << "\">" << endl;

    for( int i = 0; i < depth; i++ )
        ts << "\t";
    ts << QString( "\t<desc>" ) << Qt::escape( vocab.getDescription() ) << QString( "</desc>" ) << endl;
    for( Vocabulary::TermMap::ConstIterator it = vocab.begin(); it != vocab.end(); it++ ) {
        const Term& term = *it;
        for( int i = 0; i < depth; i++ )
            ts << "\t";
        ts << Util::term2Xml( term, languages, depth + 1 );
    }
    for( int i = 0; i < depth; i++ )
        ts << "\t";
    ts << QString( "</glossary>" ) << endl;
}

bool Controller::saveVocabulary( Vocabulary* vocab, const QString& location ) const {
    // Create the containing folder if needed.
    QString folderPath( location.contains( "v-" ) ? location : location + QString( "/v-" ) + QString::number( vocab->getId() ) );
    QDir folderDir( folderPath );
    if( vocab->isDirty() && !folderDir.exists() ) {
        if( !folderDir.mkdir( folderDir.path() ) ) {
            cerr << "Cannot create directory " << qPrintable( folderPath ) << endl;
            return( false );
        }
    }

    // Write the vocab data.
    if( vocab->isDirty() ) {
        QString dataFilename( QString( folderDir.path() + QString( "/" ) + QString( "vocab-" ) + QString::number( vocab->getId() ) + QString( ".gz" ) ) );
        if( !vocab->save( dataFilename ) ) {
            cerr << "Could not write vocab data " << qPrintable( dataFilename ) << endl;
            return( false );
        }
        vocab->setDirty( false );
    }

    return( true );
}

void Controller::initRevealingSequence() {
    int index = ( rand() % prefs.getActiveRevealingSequenceCount() );
    int sequenceCount = prefs.getRevealingSequenceCount();
    for( int i = 0, j = 0; i < sequenceCount; i++ ) {
        Sequence seq = prefs.getRevealingSequenceAt( i );
        if( seq.isEnabled() ) {
            if( j == index ) {
                currRevealingSeq = seq;
                break;
            }
            else 
                j++;
        }
    }
    currRevealingSeqStep = 0;
}

void Controller::setClipboardData( const QString& type, const QByteArray& data ) {
    clipboardDataType = type;
    clipboard = data;
}

QByteArray Controller::getClipboardData() const {
    return( clipboard );
}

QString Controller::getClipboardDataType() const {
    return( clipboardDataType );
}

QString Controller::getResolvedImagePath( const QString& path, const Vocabulary& vocab ) const {
    if( path.isNull() )
        return( QString::null );

    QFileInfo pathFileInfo( path );
    if( pathFileInfo.isAbsolute() )
        return( path );
    else {
        QString absPath = getApplicationDirName() + "/" + vocab.getParent()->getPath() + "/v-" + QString::number( vocab.getId() ) + "/" + path;
        return( absPath );
    }
}

void Controller::clearSearch() {
    searchQuery = QString::null;
    searchResults.clear();
}

QList<TermKey> Controller::search( const QString& query, const QString& firstLang /* = QString::null*/, const QString& testLang /* = QString::null */ ) {
    searchQuery = query;
    searchResults.clear();
    QString strippedQuery = query.simplified();
    if( !strippedQuery.isEmpty() )
        searchRec( strippedQuery, firstLang, testLang, vocabTree, searchResults );  
    return( searchResults );
}

void Controller::searchRec( const QString& query, const QString& firstLang, const QString& testLang, Folder* folder, QList<TermKey>& results ) {
    if( !folder || folder->isMarkedForDeletion() )
        return;

    for( int i = 0; i < folder->getChildrenCount(); i++ ) {
        Base* child = folder->childAt( i );
        if( strcmp( child->className(), "Vocabulary" ) == 0 ) {
            Vocabulary* childVocab = (Vocabulary*)child;
            searchRec( query, firstLang, testLang, childVocab, results );
        }
        else if( strcmp( child->className(), "Folder" ) == 0 ) {
            Folder* childFolder = (Folder*)child;
            searchRec( query, firstLang, testLang, childFolder, results );
        }
    }

}

void Controller::searchRec( const QString& query, const QString& firstLang, const QString& testLang, Vocabulary* vocab, QList<TermKey>& results ) {
    if( !vocab || vocab->isMarkedForDeletion() )
        return;

    for( Vocabulary::TermMap::ConstIterator it = vocab->begin(); it != vocab->end(); it++ ) {
        const Term& term = *it;
        bool isStringFound = false;

        if( prefs.isLanguageFilterEnabled() ) {
            if( term.isTranslationExists( firstLang ) && term.isTranslationExists( testLang ) ) {
                const Translation& firstLangTrans = term.getTranslation( firstLang );
                const Translation& testLangTrans = term.getTranslation( testLang );
                if( firstLangTrans.getWord().contains( query ) || firstLangTrans.getAlt().contains( query ) ||
                    testLangTrans.getWord().contains( query ) || testLangTrans.getAlt().contains( query ) ) {
                    isStringFound = true;
                }
            }
        }
        else {
            for( Term::TranslationMap::ConstIterator it2 = term.translationsBegin(); it2 != term.translationsEnd(); it2++ ) {
                const Translation& trans = *it2;
                if( trans.getWord().contains( query ) || trans.getAlt().contains( query ) ) {
                    isStringFound = true;
                    break;
                }
            }
        }

        if( !isStringFound ) {
            if( prefs.isLanguageFilterEnabled() ) {
                BilingualKey key( firstLang, testLang );
                if( term.isCommentExists( key ) && term.getComment( key ).contains( query ) )
                    isStringFound = true;
            }
            else {
                for( Term::CommentMap::ConstIterator it = term.commentsBegin(); it != term.commentsEnd(); it++ ) {
                    const QString& comment = *it;
                    if( comment.contains( query ) ) {
                        isStringFound = true;
                        break;
                    }
                }
            }
        }

        if( isStringFound ) {
            TermKey termKey( term.getId(), vocab->getId() );
            results.append( termKey );
        }
    }
}

QList<TermKey> Controller::getSearchResults() const {
    return( searchResults ); 
}

int Controller::getSearchResultsCount() const {
    return( searchResults.count() );
}
