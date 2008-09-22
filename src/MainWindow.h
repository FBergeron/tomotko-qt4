#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <QActionGroup>
#include <QApplication>
//#include <qclipboard.h>
#include <QCloseEvent>
#include <QComboBox>
#include <QDir>
//#include <qfile.h>
//#include <qfont.h>
//#include <qfontdatabase.h>
#include <QKeySequence>
#include <QLabel>
//#include <qlistview.h>
//#include <qpixmap.h>
#include <QProgressBar>
#include <QMainWindow> 
#include <QMap>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
//#include <qpe/fileselector.h>
//#include <qpe/resource.h>
//#include <qpixmap.h>
//#include <qpushbutton.h>
#include <QStatusBar>
#include <QString>
#include <QStringList>
//#include <qthread.h>
#include <QToolBar>
#include <QTranslator>
#include <QStackedWidget>
//#include <stdio.h>
#include "Controller.h"
//#include "PixMap.h"
#include "PreferencesDialog.h"
#include "QuizFrame.h"
#include "VocabularyManagerFrame.h"
//#include "Term.h"

extern QAction* action[ ACTION_COUNT ];

class MainWindow : public QMainWindow {

    // Should be an enumeration.
    static const int frameQuizIndex         = 0;
    static const int frameVocabManagerIndex = 1;

    Q_OBJECT
    
public:

    MainWindow( QApplication& app, Controller* controller );
    ~MainWindow();

    Controller* controller();

    bool isDigraphEnabled() const;

public slots:

    void updateMenus( QTreeWidgetItem* item );
    void updateFonts();
    void setDigraphEnabled( bool isEnabled );
    void retranslateUi();
    void switchLanguage( const QString& language );
    void switchLanguage( QAction* langAction );

protected:

    void closeEvent( QCloseEvent* );

private slots:

    void about();
    void startQuiz();
    void showQuiz();
//    void resumeQuiz();
    void invokeVocabularyManager();
    void importData();
    void exportData();
    void preferences();
    void copy();
    void cut();
    void paste();
    void toggleLanguageFilter();
    void quit();
    void setFirstLanguage( const QString& lang );
    void setTestLanguage( const QString& lang );
    void switchFirstAndTestLanguages();
    void toggleMaximize( bool isOn ); 
    void search();
    
private:

    void setLanguageFilterEnabled( bool isEnabled );
    void updateFirstLanguageValues();
    void updateTestLanguageValues();
    void selectLanguage( QComboBox* comboBox, const QString& langCode );
    void updateLanguageSelector( QComboBox* comboBox );

    QMap<QString,QString>   availableLanguages;

//    int                     editionMenuId;
//    int                     showAllVocabSeparatorId;
//
    QApplication&           app;
    Controller*             control;

    QStackedWidget*         mainPanel;

    QuizFrame*              quizFrame;
    VocabularyManagerFrame* vocabManagerFrame;

    QToolBar*               toolBar;

    QMenu*                  actionsMenu;
    QMenu*                  helpMenu;

    QAction*                copyAction;
    QAction*                cutAction;
    QAction*                pasteAction;

    QAction*                aboutAction;

    QActionGroup*           languageActionGroup;

    QWidget*                languageSelectorPanel;
    QHBoxLayout*            languageSelectorPanelLayout;
    QComboBox*              firstLanguageComboBox;
    QLabel*                 languageSelectorLabel;
    QComboBox*              testLanguageComboBox;

    //QStatusBar*             statusBar;
    QProgressBar*           progressBar;

    QMenu*                  editionMenu;

    QTranslator*            translator;

};

#endif
