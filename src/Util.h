#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <QAction>
#include <QApplication>
#include <QDir>
#include <QFont>
#include <QFontDatabase>
#include <QKeySequence>
#include <QString>
#include <QStringList>
#include <QTextDocument>
#include <QTextStream>
#include "Term.h"

using namespace std;

typedef QList<int> IdList;
typedef QMap<int,IdList> IdListMap;

enum Action {
    ACTION_REVEAL = 0,
    ACTION_RIGHT_ANSWER,
    ACTION_WRONG_ANSWER,
    ACTION_EDIT_QUIZ_TERM,
    ACTION_START_QUIZ,
    ACTION_MANAGE_GLOSSARIES,
    ACTION_IMPORT,
    ACTION_EXPORT,
    ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS,
    ACTION_PREFERENCES,
    ACTION_QUIT,
    ACTION_ADD_FOLDER,
    ACTION_ADD_GLOSSARY,
    ACTION_REMOVE_ITEM,
    ACTION_ADD_TERM,
    ACTION_EDIT_TERM,
    ACTION_REMOVE_TERMS,
    ACTION_CHECK_ALL_TERMS,
    ACTION_INVERSE_CHECKED_TERMS,
    ACTION_MAXIMIZE,
    ACTION_SEARCH,
    ACTION_COUNT
};

const QString actionId[] = {
    "reveal",
    "rightAnswer",
    "wrongAnswer",
    "editQuizTerm",
    "startQuiz",
    "manageGlossaries",
    "import",
    "export",
    "showAllGlossariesAndTerms",
    "preferences",
    "quit",
    "addFolder",
    "addGlossary",
    "removeItem",
    "addTerm",
    "editTerm",
    "removeTerms",
    "checkAllTerms",
    "inverseCheckedTerms",
    "maximize",
    "search"
};


class Util {

public:

    static const QString languageCodeList[];
    static QString getLanguageCode( const QString& language );

    static bool deleteDirectory( const QString& dir );
    static QString term2Xml( const Term& term, QStringList* languages = NULL, uint indentLevel = 0 );
    static int getDefaultQuizLength();
    static QString getDefaultLabelsFontFamily();
    static int getDefaultLabelsFontSizeModifier();
    static QString getDefaultFontFamily();
    static int getDefaultFontSizeModifier();

    static QString getDigraph( const QString& buffer );
    static QAction* createAction( const QString& name, const char** icon, QObject* receiver, const char* slot, QKeySequence defaultKey = QKeySequence(), bool toggle = false );

private:

    static QMap<QString, QString>* digraphMap; 

    static void initDigraphs();

};

#endif
