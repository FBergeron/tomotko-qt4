#ifndef QUIZ_H
#define QUIZ_H

#include <QApplication>
#include <QFont>
#include <QLineEdit>
#include <QMessageBox>
#include <QMovie>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QTextEdit>
#include <QWidget>
#include "Controller.h"
#include "ScrollableLineEdit.h"
#include "ScrollableMultiLineEdit.h"
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
    void resizeEvent( QResizeEvent* evt );

public slots:

    void scrollLeft();
    void scrollRight();
    void scrollUp();
    void scrollDown();
    void scrollPageUp();
    void scrollPageDown();

    void setImage( const QString& imagePath );

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
    bool containsValidImage() const;

    void editCurrentTerm();

    void resizeImageBox();

private:

    void init();
    void clearImage();

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
    ScrollableLineEdit*         firstLangTermLineEdit;
    QPushButton*                firstLangTermButton;

    QWidget*                    testLangTopPanel;
    QBoxLayout*                 testLangTopPanelLayout;
    QWidget*                    testLangLabelsPanel;
    QVBoxLayout*                testLangLabelsPanelLayout;
    QWidget*                    testLangFieldsPanel;
    QVBoxLayout*                testLangFieldsPanelLayout;
    
    QLabel*                     testLangTermLabel;
    QStackedWidget*             testLangTermStack;
    ScrollableLineEdit*         testLangTermLineEdit;
    QPushButton*                testLangTermButton;

    QLabel*                     testLangTermAltLabel;
    QStackedWidget*             testLangTermAltStack;
    ScrollableLineEdit*         testLangTermAltLineEdit;
    QPushButton*                testLangTermAltButton;
   
    QWidget*                    commentBox;
    QVBoxLayout*                commentBoxLayout;
    QWidget*                    commentLabelPanel;
    QHBoxLayout*                commentLabelPanelLayout;
    QLabel*                     commentLabel;
    QPushButton*                maximizeCommentButton;
    QStackedWidget*             commentStack;
    ScrollableMultiLineEdit*    commentMultiLineEdit;
    QPushButton*                commentButton;

    QGroupBox*                  imageBox;
    QVBoxLayout*                imageBoxLayout;
    QStackedWidget*             imageStack;
    QPushButton*                imageButton;
    QLabel*                     image;

    QAction*                    scrollLeftAction;
    QAction*                    scrollRightAction;
    QAction*                    scrollUpAction;
    QAction*                    scrollDownAction;
    QAction*                    scrollPageUpAction;
    QAction*                    scrollPageDownAction;

    QPixmap*                    pixmap;
    QMovie*                     movie;

    bool                        buttonsHidden;

};

#endif
