/*
 * This class contains some code borrowed from 
 * keyboard_shortcuts.h (rev. 1.2) and
 * keyboard_shortcuts.cpp (rev. 1.3) of ZBEDic.
 * Copyright (C) 2004 Rafal Mantiuk <rafm@users.sourceforge.org>
 */
#ifndef PREFERENCES_DIALOG_H
#define PREFERENCES_DIALOG_H 

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QList>
#include <QMessageBox>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QSlider>
#include <QString>
#include <QTabWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include "KeyActionListViewItem.h"
#include "Preferences.h"
#include "SequenceDialog.h"
#include "SequenceListItem.h"
#include "TermScheduler.h"
#include "Util.h"

extern QAction* action[ ACTION_COUNT ];

class PreferencesDialog : public QDialog {

    Q_OBJECT
    
public:

    static const QString fontSizeNameList[];
    static const QString interfaceLanguageList[];
    static const QString interfaceLanguageCodeList[];
    static const QString firstLanguageList[];
    static const QString studyLanguageList[];
    static const QString studyLanguageCodeList[];

    PreferencesDialog( QWidget* parent, Preferences* prefs );
    ~PreferencesDialog(); 

public slots:

protected slots:

    virtual void accept();

protected:

    virtual void resizeEvent( QResizeEvent* evt );

private slots:

    void resetDefaultLabelsFont();
    void resetDefaultFont();
    void addFontOverride( const QString& language );
    void removeFontOverride( const QString& language );
    void updateFontOverride( QTreeWidgetItem* item, int column );
    void clearAccelKey();
    void setAccelKey();
    void resetAccelKey();
    void cancelSetAccelKey();
    void addSequence();
    void removeSequence();
    void updateUi();

private:

    void init();
    void initFontFamilyValues( QComboBox* comboBox, bool withEmptyEntry = false ) const;
    void initFontSizeValues( QComboBox* comboBox, bool withEmptyEntry = false ) const;
    void initFontOverrides();
    void initSequences();
    void initStudyLanguageValues() const;
    void selectFontFamily( QComboBox* comboBox, const QString& fontFamily );
    void selectFontSize( QComboBox* comboBox, int fontSizeModifier, bool withEmptyEntry = false );
    void selectLanguage( QComboBox* comboBox, const QString& langCode );
    bool isRevealingSequenceSelectionValid() const;
    bool isRevealingSequenceDefined( const QString& seqStr ) const;
    bool isStudyLanguageSelectionValid() const;
    void keyPressEvent( QKeyEvent *evt );

    Preferences*                prefs;

    QVBoxLayout*                mainLayout;
    QTabWidget*                 tab;

    QWidget*                    languagePage;
    QWidget*                    quizPage;
    QWidget*                    fontPage;
#if defined(Q_WS_HILDON)
    QScrollArea*                interfacePage;
    QWidget*                    interfacePageBox;
#else
    QWidget*                    interfacePage;
#endif

    QVBoxLayout*                languageLayout;

    QWidget*                    languagesPanel;
    QVBoxLayout*                languagesPanelLayout;

    QTreeWidget*                studyLanguagesListView;

    QVBoxLayout*                quizLayout;

    QGroupBox*                  quizLengthOptionsPanel;
    QVBoxLayout*                quizLengthOptionsPanelLayout;
    QGroupBox*                  revealingOptionsPanel;
    QHBoxLayout*                revealingOptionsPanelLayout;

    QWidget*                    sequencesViewPanel;
    QVBoxLayout*                sequencesViewPanelLayout;
    QTreeWidget*                sequencesView;
    QWidget*                    sequencesViewButtons;
    QHBoxLayout*                sequencesViewButtonsLayout;
    QPushButton*                addSequenceButton;
    QPushButton*                removeSequenceButton;

    QGroupBox*                  sequencesLabelBox;
    QVBoxLayout*                sequencesLabelBoxLayout;
    QLabel*                     sequencesLabel;

    QSlider*                    quizLengthSlider;
    QWidget*                    quizLengthLabelsPanel;
    QHBoxLayout*                quizLengthLabelsPanelLayout;
    QLabel*                     quizLengthShortestLabel;
    QLabel*                     quizLengthMediumLabel;
    QLabel*                     quizLengthLongestLabel;
   
    QVBoxLayout*                fontPageLayout;
    QVBoxLayout*                interfacePageLayout;
    QHBoxLayout*                keyboardAccelButtonPanelLayout;

    QWidget*                    labelsFontPanel;
    QHBoxLayout*                labelsFontPanelLayout;
    QLabel*                     labelsFontLabel;
    QComboBox*                  labelsFontFamilyComboBox;
    QComboBox*                  labelsFontSizeComboBox;
    QPushButton*                resetDefaultLabelsFontButton;

    QWidget*                    fontsPanel;
    QVBoxLayout*                fontsPanelLayout;

    QWidget*                    fontPanel;
    QHBoxLayout*                fontPanelLayout;
    QLabel*                     fontLabel;
    QComboBox*                  fontFamilyComboBox;
    QComboBox*                  fontSizeComboBox;
    QPushButton*                resetDefaultFontButton;

    QGroupBox*                  fontOverridesPanel;
    QVBoxLayout*                fontOverridesPanelLayout;
    QScrollArea*                fontOverridesScrollView;
    QWidget*                    fontOverridesBox;
    QVBoxLayout*                fontOverridesBoxLayout;
    QWidget*                    fontOverridesBoxFiller;

    QList<QWidget*>             fontOverrideBoxes;
    QList<QLabel*>              fontOverrideLabels;
    QList<QComboBox*>           fontOverrideFamilyComboBoxes;
    QList<QComboBox*>           fontOverrideSizeComboBoxes;

    QWidget*                    miscInterfaceOptionsPanel;
    QGridLayout*                miscInterfaceOptionsPanelLayout;

    QGroupBox*                  digraphPanel;
    QVBoxLayout*                digraphPanelLayout;
    QCheckBox*                  digraphCheckBox;

    QGroupBox*                  hideQuizButtonPanel;
    QVBoxLayout*                hideQuizButtonPanelLayout;
    QCheckBox*                  hideQuizButtonCheckBox;

    QGroupBox*                  showAltTextInTermListPanel;
    QVBoxLayout*                showAltTextInTermListPanelLayout;
    QCheckBox*                  showAltTextInTermListCheckBox;

    QGroupBox*                  keyboardAccelPanel;
    QVBoxLayout*                keyboardAccelPanelLayout;

    QTreeWidgetItem*            grabAccelKeyFor;
    QTreeWidget*                keyboardAccelListView;
    bool                        keyboardAccelModified;

    QWidget*                    keyboardAccelButtonPanel;
    QPushButton*                clearAccelKeyButton;
    QPushButton*                setAccelKeyButton;
    QPushButton*                resetAccelKeyButton;

    QWidget*                    bottomButtonsPanel;
    QHBoxLayout*                bottomButtonsPanelLayout;
    QPushButton*                acceptButton;
    QPushButton*                cancelButton;

};

#endif
