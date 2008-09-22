#ifndef QUIZ_H
#define QUIZ_H

#include <QApplication>
//#include <qevent.h>
#include <QFont>
//#include <qframe.h>
//#include <qhbox.h>
//#include <qhgroupbox.h>
//#include <qiconset.h>
//#include <qlabel.h>
//#include <qlayout.h>
#include <QLineEdit>
#include <QMessageBox>
//#include <qmultilineedit.h>
//#include <qobjectlist.h>
//#include <qpushbutton.h>
//#include <qsizepolicy.h>
#include <QStackedWidget>
//#include <qstringlist.h>
#include <QTextEdit>
//#include <qtooltip.h>
//#include <qvbox.h>
//#include <qvgroupbox.h>
#include <QWidget>
#include "Controller.h"
//#include "ImageBox.h"
//#include "ScrollableLineEdit.h"
//#include "ScrollableMultiLineEdit.h"
#include "Term.h"
#include "TermDialog.h"
#include "Translation.h"
#include "Util.h"

extern QAction* action[ ACTION_COUNT ];

class QuizFrame : public QWidget {

    Q_OBJECT

public:

    QuizFrame( Controller* controller, QWidget* parent = 0 );
    ~QuizFrame();

    void startQuiz();
    void restartQuiz();
    void resumeQuiz();
    void setTerm( const Term& term );
    void concludeQuiz();
    Term* askCurrentTerm();
    Term* askNextTerm();
    void askTerm( const Term& term );
    void updateLanguageLabels();
    void setButtonsHidden( bool areHidden );
    bool areButtonsHidden() const;

protected:

    void showEvent( QShowEvent* showEvt );
    void hideEvent( QHideEvent* hideEvt );
    bool event( QEvent* evt );

public slots:

    //virtual bool eventFilter( QObject* obj, QEvent* evt );

    //void scrollLeft();
    //void scrollRight();

    void updateFonts();
    void retranslateUi();

    void toggleMaximizeComment( bool isOn );

signals:

    void quizShown();
    void quizHidden();

private slots:

    void rightAnswer();
    void wrongAnswer();
    void reveal();
    void revealAll();
    void revealAltTerm();
    void revealFirstLangTerm();
    void revealTestLangTerm();
    void revealComment();
    void revealImage();

    void editCurrentTerm();

private:

    void init();

    void hideAnswers();
    void setButtonsEnabled( bool isEnabled );

    bool isFirstLangTermRevealed() const;
    bool isAltTermRevealed() const;
    bool isTestLangTermRevealed() const;
    bool isCommentRevealed() const;
    bool isImageRevealed() const;

    void maximizeCommentField();
    void restoreCommentField();

    Controller*                 controller;

    QVBoxLayout*                mainLayout;

    QWidget*                    topPanel;
    QHBoxLayout*                topPanelLayout;
    QWidget*                    topLeftPanel;
    QVBoxLayout*                topLeftPanelLayout;

    QGroupBox*                  firstLangPanel;
    QHBoxLayout*                firstLangPanelLayout;
    QGroupBox*                  testLangPanel;
    QVBoxLayout*                testLangPanelLayout;

    QWidget*                    controlPanel;
    QHBoxLayout*                controlPanelLayout;
    QWidget*                    answerControlPanel;
    QHBoxLayout*                answerControlPanelLayout;
    QPushButton*                revealAllDataButton;
    QPushButton*                rightAnswerButton;
    QPushButton*                wrongAnswerButton;
    QPushButton*                editionButton;

    QWidget*                    firstLangTermPanel;
    QHBoxLayout*                firstLangTermPanelLayout;
    
    QLabel*                     firstLangTermLabel;
    QStackedWidget*             firstLangTermStack;
    QLineEdit*                  firstLangTermLineEdit;//ScrollableLineEdit*         firstLangTermLineEdit;
    QPushButton*                firstLangTermButton;

    QWidget*                    testLangTopPanel;
    QBoxLayout*                 testLangTopPanelLayout;
    QWidget*                    testLangLabelsPanel;
    QVBoxLayout*                testLangLabelsPanelLayout;
    QWidget*                    testLangFieldsPanel;
    QVBoxLayout*                testLangFieldsPanelLayout;
    
    QLabel*                     testLangTermLabel;
    QStackedWidget*             testLangTermStack;
    QLineEdit*                  testLangTermLineEdit;//ScrollableLineEdit*         testLangTermLineEdit;
    QPushButton*                testLangTermButton;

    QLabel*                     testLangTermAltLabel;
    QStackedWidget*             testLangTermAltStack;
    QLineEdit*                  testLangTermAltLineEdit;//ScrollableLineEdit*         testLangTermAltLineEdit;
    QPushButton*                testLangTermAltButton;
   
    QWidget*                    commentBox;
    QVBoxLayout*                commentBoxLayout;
    QWidget*                    commentLabelPanel;
    QHBoxLayout*                commentLabelPanelLayout;
    QLabel*                     commentLabel;
    QPushButton*                maximizeCommentButton;
    QStackedWidget*             commentStack;
    QTextEdit*                  commentMultiLineEdit;//ScrollableMultiLineEdit*    commentMultiLineEdit;
    QPushButton*                commentButton;

    //ImageBox*                   imageBox;
    //QPushButton*                imageButton;

    bool                        buttonsHidden;

};

#endif
