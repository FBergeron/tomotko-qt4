#ifndef TRANSLATION_SELECTION_DIALOG_H
#define TRANSLATION_SELECTION_DIALOG_H 

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QWidget>
#include "Controller.h"

class TranslationSelectionDialog : public QDialog {

    Q_OBJECT
    
public:

    // Should be an enumeration?
    static const int selectionModeTargetLanguage    = 0;
    static const int selectionModeStudyLanguages    = 1;
    static const int selectionModeAllLanguages      = 2;

    TranslationSelectionDialog( const QString& caption, const QString& message, const QStringList& languages, int selectionMode, Controller* controller, QWidget* parent );
    ~TranslationSelectionDialog(); 

    QStringList getSelectedLanguages();

private slots:

    void checkAllLanguages() const;

private:

    void init( const QString& caption, const QString& message, const QStringList& languages, int selectionMode );

    QLabel*         messageLabel;
    QTreeWidget*    languageList;
    QPushButton*    checkAllLanguagesButton;

    QVBoxLayout*    mainLayout;

    QWidget*        bottomButtonsPanel;
    QHBoxLayout*    bottomButtonsPanelLayout;
    QPushButton*    acceptButton;
    QPushButton*    cancelButton;

    Controller*     controller;

};

#endif
