#ifndef SEARCH_DIALOG_H
#define SEARCH_DIALOG_H

#include <iostream>
#include <QDialog>
//#include <qfontmetrics.h>
//#include <qheader.h>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QList>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "Controller.h"
#include "HistoryField.h"
#include "ResultListItem.h"
//#include "SmartListView.h"
#include "TermDialog.h"
#include "TermKey.h"
#include "TranslationSelectionDialog.h"

class SearchDialog : public QDialog {

    Q_OBJECT
    
public:

    SearchDialog( Controller* controller, QWidget* parent = 0 );
    ~SearchDialog();

public slots:

    void show();
    void updateFonts();
    void retranslateUi();

signals:

    void showTermRequested( const TermKey& termKey );
    void termsRemoved();

private slots:

    void search();
    void reset();
    void goResultVocab();
    void editResultTerm();
    void removeResultTerms();
    void updateUi();
    
private:

    void init();
    uint getSelectedTermCount() const;
    void doRemoveTerms( bool allowSelectTrans = true, bool confirmBeforeRemove = true );

    Controller*             controller;

    QVBoxLayout*            mainLayout;

    QWidget*                queryPanel;
    QHBoxLayout*            queryPanelLayout;

    HistoryField*           queryField;
    QPushButton*            searchButton;
    QPushButton*            resetButton;

    QWidget*                resultsHeaderPanel;
    QHBoxLayout*            resultsHeaderPanelLayout;
    
    QLabel*                 resultsHeaderLabel;
    QWidget*                resultsHeaderFiller;
    QLabel*                 resultsCounterLabel;

    QTreeWidget*            resultsListView;//SmartListView*          resultsListView;

    QWidget*                resultsButtonsPanel;
    QHBoxLayout*            resultsButtonsPanelLayout;
    QPushButton*            goResultVocabButton;
    QPushButton*            editResultTermButton;
    QPushButton*            removeResultTermButton;

};

#endif
