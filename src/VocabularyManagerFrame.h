#ifndef VOCAB_MANAGER_H
#define VOCAB_MANAGER_H

#include <iostream>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSplitter>
#include <QStackedWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>
#include "Controller.h"
#include "DigraphLineEdit.h"
#include "PropertiesPanel.h"
#include "SearchDialog.h"
#include "TranslationSelectionDialog.h"
#include "TermDialog.h"
#include "TermListItem.h"
#include "Util.h"
#include "VocabTreeView.h"
#include "VocabTreeItem.h"

using namespace std;

extern QAction* action[ ACTION_COUNT ];

class VocabularyManagerFrame : public QWidget {

    static const int panelFolderIndex   = 0;
    static const int panelVocabIndex    = 1;

    Q_OBJECT

public:

    VocabularyManagerFrame( Controller* controller, QWidget* parent = 0 );
    ~VocabularyManagerFrame();

    void updateShownItems();

    void loadData();
    bool saveData();

    bool isExportAllowed() const;
    bool isImportAllowed() const;

    bool isDigraphEnabled() const;

public slots:

    void copy();
    void cut();
    void paste();

    void search();

    void importData();
    void exportData();
    void exportVocabulary( Vocabulary* vocab );
    void exportFolder( Folder* folder );

    void updateUi();
    void updateCurrentTreeItem( QTreeWidgetItem* currItem, QTreeWidgetItem* prevItem );
    void updateFonts();
    void setDigraphEnabled( bool isEnabled );
    void retranslateUi();

    bool areDetailsMaximized() const;
    void toggleMaximizeDetails( bool isOn );

signals:

    void selectionChanged( QTreeWidgetItem* item );

private slots:

    void updateTree();
    void updateCurrentFolder( FolderTreeItem* folderItem );
    void updateCurrentVocab( VocabTreeItem* vocabItem );
    void updateTermList();

    void keepSelection();
    void restoreTreeSelection();
    void restoreVocabSelection();

    FolderTreeItem* addFolder();
    VocabTreeItem* addVocab();
    void removeItem();
    void checkAllTerms();
    void inverseCheckedTerms();
    void addTerm();
    void editTerm();
    void removeTerms();
    void showTerm( const TermKey& termKey );

    void updateTermItemState( QTreeWidgetItem* item, int column );
    void updateTreeItemState( QTreeWidgetItem* item, int column );

    void updateCurrentFolderTitle( const QString& title );
    void updateCurrentVocabTitle( const QString& title );

private:

    FolderTreeItem* addFolder( Folder* folder, QMap<int,Vocabulary>* vocabularies = NULL );
    VocabTreeItem* addVocab( Vocabulary* vocab );

    FolderTreeItem* buildTreeRec( VocabTreeView* vocabTreeView, FolderTreeItem* parentItem, Folder* folder, bool closeFolder = false );
    VocabTreeItem* buildTreeRec( FolderTreeItem* parentItem, Vocabulary* vocab );
    void doRemoveTerms( bool allowSelectTrans = true, bool confirmBeforeRemove = true );
    void doRemoveItem( bool confirmBeforeRemove = true );

    void copyTerms() const;
    void pasteTerms();
    void pasteVocabulary();
    void pasteFolder();

    void addListeners();
    void removeListeners();

    void maximizeDetailsPanel();
    void restoreDetailsPanel();

    uint getSelectedTermCount() const;

    int                     currentFolderId;
    int                     currentVocabId;
    int                     currentTermId;
    QList<int>              selectedTermIdList;

    QVBoxLayout*            mainLayout;

    QSplitter*              splitter;

    QWidget*                treePanel;
    QVBoxLayout*            treePanelLayout;
    
    QWidget*                treeButtonPanel;
    QHBoxLayout*            treeButtonPanelLayout;
    QPushButton*            addFolderButton;
    QPushButton*            addVocabButton;
    QPushButton*            removeItemButton;
    VocabTreeView*          vocabTreeView;
    FolderTreeItem*         vocabTreeRoot;

    QStackedWidget*         detailsPanel;
    QWidget*                folderDetailsPanel;
    QVBoxLayout*            folderDetailsPanelLayout;
    QLabel*                 folderDetailsTitle;
    QWidget*                folderDetailsFolderHeaderPanel;
    QHBoxLayout*            folderDetailsFolderHeaderPanelLayout;
    QWidget*                folderDetailsFolderTitlePanel;
    QHBoxLayout*            folderDetailsFolderTitlePanelLayout;
    QLabel*                 folderDetailsFolderTitleLabel;
    DigraphLineEdit*        folderDetailsFolderTitleLineEdit;
    QPushButton*            folderDetailsFolderMaximizeDetailsButton;
    PropertiesPanel*        folderDetailsPropsPanel; 

    QWidget*                vocabDetailsPanel;
    QVBoxLayout*            vocabDetailsPanelLayout;
    QWidget*                vocabDetailsVocabHeaderPanel;
    QHBoxLayout*            vocabDetailsVocabHeaderPanelLayout;
    QLabel*                 vocabDetailsTitle;
    QLabel*                 vocabDetailsVocabTitleLabel;
    DigraphLineEdit*        vocabDetailsVocabTitleLineEdit;
    QPushButton*            vocabDetailsVocabMaximizeDetailsButton;
    QTabWidget*             vocabDetailsTabWidget;
    QWidget*                vocabDetailsTermsPanel;
    QVBoxLayout*            vocabDetailsTermsPanelLayout;
    PropertiesPanel*        vocabDetailsPropsPanel;
    QWidget*                vocabDetailsVocabTitlePanel;
    QHBoxLayout*            vocabDetailsVocabTitlePanelLayout;
    QTreeWidget*            termList;
   
    QWidget*                termControlPanel;
    QHBoxLayout*            termControlPanelLayout;

    QWidget*                checkControlPanel;
    QHBoxLayout*            checkControlPanelLayout;
    QPushButton*            checkAllTermsButton;
    QPushButton*            inverseCheckedTermsButton;

    QWidget*                addRemoveTermPanel;
    QHBoxLayout*            addRemoveTermPanelLayout;
    QPushButton*            addTermButton;
    QPushButton*            editTermButton;
    QPushButton*            removeTermButton;

    SearchDialog*           searchDialog;

    Controller*             controller;

};

#endif
