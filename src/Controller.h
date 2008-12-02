#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <QByteArray>
#include <QDir>
#include <QObject>
#include <QList>
#include <QString>
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#include "Folder.h"
#include "FolderParser.h"
#include "Preferences.h"
#include "TermKey.h"
#include "TermScheduler.h"
#include "VocabParser.h"
#include "minizip/unzip.h"
#include "minizip/zip.h"

using namespace std;

class Controller : public QObject {

    Q_OBJECT

public:

    Controller();

    bool init();

    const QString& getApplicationDirName() const;
    Preferences& getPreferences();

    Folder* getVocabTree();

    void startQuiz();
    void restartQuiz();
    bool isResumableQuizAvailable();
    bool resumeQuiz();
    void prepareQuiz();

    Term* getCurrentTerm();
    bool hasNextTerm() const;
    Term* getTerm( const TermKey& termKey );
    Term* getNextTerm();
 
    QString getQuizFirstLanguage() const;
    QString getQuizTestLanguage() const;

    bool isQuizInProgress() const;
    int getProgress() const;
    Sequence& getRevealingSequence();
    int getRevealingSequenceStep() const;
    void incrementRevealingSequenceStep();
        
    void rightAnswer();
    void wrongAnswer();
    void reveal();
   
    bool saveData();
    void loadData();
   
    bool saveMarkedItems( Folder* folder );
    void loadMarkedItems( Folder* folder );

    Vocabulary* addVocabulary( Folder* parentFolder, Vocabulary* vocab = NULL ); 
    Folder* addFolder( Folder* parentFolder, Folder* folder = NULL ); 

    void copy( Vocabulary* vocab );
    void copy( Folder* folder );

    Vocabulary* loadVocabulary( const QString& parentDir );
    Folder* loadFolder( const QString& parentDir );
    bool saveFolder( Folder* folder, const QString& parentDir ) const;

    bool saveVocabulary( Vocabulary* vocab, const QString& location ) const;
    bool exportData( Vocabulary* vocab, const QString& file, QStringList* languages = NULL ) const;
    bool exportData( Folder* folder, const QString& file, QStringList* languages = NULL ) const;
    void writeFolderDataInXml( QTextStream& ts, const Folder& folder ) const;
    void writeVocabularyInXml( QTextStream& ts, const Vocabulary& vocab, QStringList* languages, bool writeXmlDirective = true, int depth = 0 ) const;

    QStringList getTranslationLanguagesFromFile( const QString& filename ) const;
    Base* importData( Folder* rootFolder, const QString& filename, const QStringList& languages );

    int getInitialTermCount() const;

    void initRevealingSequence();

    void setClipboardData( const QString& type, const QByteArray& data );
    QByteArray getClipboardData() const;
    QString getClipboardDataType() const;

    QString getResolvedImagePath( const QString& path, const Vocabulary& vocab ) const;

    void clearSearch();
    QList<TermKey> search( const QString& query, const QString& firstLang = QString::null, const QString& testLang = QString::null );
    QList<TermKey> getSearchResults() const;
    int getSearchResultsCount() const;

public slots:
    
    void concludeQuiz();

signals:

    void progressChanged( int progress );

private:

    int writeFileIntoZipFile( zipFile outputFile, const char* filename, const char* data, int length ) const; 

    void initMarkedForStudyRec( Folder* folder, IdList& folderIds, IdList& vocabIds, IdListMap& termIds );
    void initMarkedForStudyRec( Vocabulary* vocab, IdList& vocabIds, IdListMap& termIds );
    
    void saveMarkedItemsRec( Folder* folder, IdList& folderIds, IdList& vocabIds, IdListMap& termIds );
    void saveMarkedItemsRec( Vocabulary* vocab, IdList& vocabIds, IdListMap& termIds );

    QStringList getVocabularyTranslationLanguagesFromZip( zipFile inputFile ) const;

    bool importFolderFromZip( Folder* folder, const QString& folderLocation, zipFile inputFile );
    bool importVocabularyFromZip( Vocabulary* vocab, const QString& vocabLocation, const QStringList& languages, zipFile inputFile );
    bool importImageFromZip( const QString& imgPath, zipFile inputFile );
    bool exportFolderRecIntoZip( Folder* folder, zipFile outputFile, QString path, QStringList* languages = NULL ) const;
    bool exportVocabularyIntoZip( Vocabulary* vocab, zipFile outputFile, QString path, QStringList* languages = NULL ) const;

    Vocabulary* makeCopy( Vocabulary* vocab, const QString& firstLang, const QString& testLang ) const;
    Folder* makeCopy( Folder* folder, const QString& firstLang, const QString& testLang ) const;

    bool deleteItemsMarkedForDeletion( Folder* folder );

    int findFolderId( const QString& filename ) const;
    int findParentFolderId( const QString& dirPath ) const;
    int findVocabId( const QString& filename ) const;
    QString convertPath( const QString& path, QMap<int,Folder*>& newFolders ) const;

    void searchRec( const QString& query, const QString& firstLang, const QString& testLang, Folder* folder, QList<TermKey>& results );
    void searchRec( const QString& query, const QString& firstLang, const QString& testLang, Vocabulary* folder, QList<TermKey>& results );

    Preferences         prefs;

    Folder*             vocabTree; 

    Sequence            currRevealingSeq;
    int                 currRevealingSeqStep;
    TermScheduler       scheduler;

    QString             markedFilename;
    QString             applicationDirName; 

    QString             searchQuery;
    QList<TermKey>      searchResults;

    // Custom clipboard because data property is disabled for QMimeSource in Zaurus' Qt.
    QByteArray          clipboard; 
    QString             clipboardDataType;

};

#endif
