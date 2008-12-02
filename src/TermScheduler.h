#ifndef TERM_SCHEDULER_H
#define TERM_SCHEDULER_H

#include <iostream>
#include <QList>
#include "BilingualKey.h"
#include "Folder.h"
#include "Preferences.h"
#include "StandbyTerm.h"
#include "TermKey.h"
#include "Vocabulary.h"

class TermScheduler {

    static const qint32 magicNumber;
    static const int standbyPoolSize = 5;

public:

    static const int poolCount  = 5;

    TermScheduler( const Preferences& prefs );
    TermScheduler( const TermScheduler& scheduler );
    ~TermScheduler();
 
    bool isResumableQuizAvailable( const BilingualKey& key ) const;
    bool load( const BilingualKey& key );
    bool save();

    void concludeQuiz();
    void setApplicationDirName( const QString& applDir );

    QString getQuizFirstLanguage() const;
    QString getQuizTestLanguage() const;
    bool isQuizInProgress() const;
    void init( const QString& firstLanguage, const QString& testLanguage, Folder* rootFolder );
    void reinit();
    void addTerm( const TermKey& termKey, const int priority = 0 );
    void discardCurrentTerm();
    const TermKey getCurrentTerm() const;
    bool hasNextTerm() const;
    const TermKey getNextTerm();
    void increaseTermPriority();
    void decreaseTermPriority();
    void rightAnswer();
    void wrongAnswer();
    int getProgress() const;
    int getInitialTermCount() const;    

private:

    void initRec( const QString& firstLanguage, const QString& testLanguage, Folder* folder );
    void initRec( const QString& firstLanguage, const QString& testLanguage, Vocabulary* vocab );
    void putCurrentTermOnStandby();
    void reintroduceStandbyTerm();
    QString getFilename( const BilingualKey& key ) const;

    QString                 quizFirstLang;
    QString                 quizTestLang;

    QList<TermKey>          termPool[ poolCount ];
    QList<StandbyTerm>      standbyPool; 

    QList<TermKey>          allTerms;

    int                     initQuizLength;
    int                     initTermCount;
    int                     currTermPool;
    TermKey                 currTerm;

    const Preferences&      prefs;

    QString                 applDir;

    friend QDataStream& operator<<( QDataStream& out, const TermScheduler& scheduler );
    friend QDataStream& operator>>( QDataStream& in, TermScheduler& scheduler );

};

#endif
