#include "VocabularyManagerFrame.h"
#include "icons/addFolder.xpm"
#include "icons/addVocab.xpm"
#include "icons/removeItem.xpm"
#include "icons/addTerm.xpm"
#include "icons/editTerm.xpm"
#include "icons/removeTerm.xpm"
#include "icons/checkAllTerms.xpm"
#include "icons/inverseCheckedTerms.xpm"
#include "icons/maximize.xpm"

VocabularyManagerFrame::VocabularyManagerFrame( Controller* controller, QWidget* parent /*= 0*/ )
    : QWidget( parent ), searchDialog( NULL ), controller( controller ) {

    mainLayout = new QVBoxLayout();

    treePanel = new QWidget();
    treePanelLayout = new QVBoxLayout();
    treePanelLayout->setContentsMargins( 0, 0, 0, 0 );

    vocabTreeView = new VocabTreeView( *controller );
    //vocabTreeView->setAnimated( true );
    vocabTreeView->setHeaderLabel( tr( "Glossaries" ) );

    treeButtonPanel = new QWidget();
    treeButtonPanelLayout = new QHBoxLayout();
    treeButtonPanelLayout->setContentsMargins( 0, 0, 0, 0 );

    treePanelLayout->addWidget( vocabTreeView, 1 );
    treePanelLayout->addWidget( treeButtonPanel );
    treePanel->setLayout( treePanelLayout );

    addFolderButton = new QPushButton( /*tr( "AddFolder" )*/ );
    addFolderButton->setIcon( QIcon( QPixmap( addFolder_xpm ) ) );
    addFolderButton->setIconSize( QSize( 32, 32 ) );
    addFolderButton->setToolTip( tr( "AddFolder" ) );
    connect( addFolderButton, SIGNAL( clicked() ), this, SLOT( addFolder() ) );

    addVocabButton = new QPushButton( /*tr( "AddGlossary" )*/ );
    addVocabButton->setIcon( QIcon( QPixmap( addVocab_xpm ) ) );
    addVocabButton->setIconSize( QSize( 32, 32 ) );
    addVocabButton->setToolTip( tr( "AddGlossary" ) );
    connect( addVocabButton, SIGNAL( clicked() ), this, SLOT( addVocab() ) );

    removeItemButton = new QPushButton( /*tr( "RemoveItem" )*/ );
    removeItemButton->setIcon( QIcon( QPixmap( removeItem_xpm ) ) );
    removeItemButton->setIconSize( QSize( 32, 32 ) );
    removeItemButton->setToolTip( tr( "RemoveItem" ) );
    connect( removeItemButton, SIGNAL( clicked() ), this, SLOT( removeItem() ) );

    treeButtonPanelLayout->addWidget( addFolderButton );
    treeButtonPanelLayout->addWidget( addVocabButton );
    treeButtonPanelLayout->addWidget( removeItemButton );

    treeButtonPanel->setLayout( treeButtonPanelLayout );

    detailsPanel = new QStackedWidget();

    folderDetailsPanel = new QWidget();
    folderDetailsPanelLayout = new QVBoxLayout();
    folderDetailsPanelLayout->setContentsMargins( 0, 0, 0, 0 );

    folderDetailsFolderHeaderPanel = new QWidget();
    folderDetailsFolderHeaderPanelLayout = new QHBoxLayout();
    folderDetailsFolderHeaderPanelLayout->setContentsMargins( 0, 0, 0, 0 );

    folderDetailsTitle = new QLabel( tr( "Folder" ) );
    folderDetailsFolderHeaderPanelLayout->addWidget( folderDetailsTitle );
    folderDetailsFolderHeaderPanel->setLayout( folderDetailsFolderHeaderPanelLayout );
    folderDetailsPanelLayout->addWidget( folderDetailsFolderHeaderPanel );
    folderDetailsFolderTitlePanel = new QWidget();
    folderDetailsFolderTitlePanelLayout = new QHBoxLayout();
    folderDetailsFolderTitlePanelLayout->setContentsMargins( 0, 0, 0, 0 );
    folderDetailsFolderTitlePanel->setLayout( folderDetailsFolderTitlePanelLayout );
    folderDetailsFolderHeaderPanelLayout->addWidget( folderDetailsFolderTitlePanel );
    folderDetailsFolderTitleLabel = new QLabel( tr( "Title" ) );
    folderDetailsFolderTitleLineEdit = new DigraphLineEdit();

    folderDetailsFolderMaximizeDetailsButton = new QPushButton();
    folderDetailsFolderMaximizeDetailsButton->setIcon( QIcon( QPixmap( maximize_xpm ) ) );
    folderDetailsFolderMaximizeDetailsButton->setIconSize( QSize( 32, 32 ) );
    folderDetailsFolderMaximizeDetailsButton->setToolTip( tr( "MaximizeDetails" ) );
    folderDetailsFolderMaximizeDetailsButton->setCheckable( true );
    folderDetailsFolderMaximizeDetailsButton->setMaximumHeight( 
    folderDetailsFolderTitleLineEdit->sizeHint().height() > 24 ? folderDetailsFolderTitleLineEdit->sizeHint().height() : 24 );
    folderDetailsFolderMaximizeDetailsButton->setMaximumWidth( folderDetailsFolderMaximizeDetailsButton->maximumHeight() );
    connect( folderDetailsFolderMaximizeDetailsButton, SIGNAL( toggled( bool ) ), this, SLOT( toggleMaximizeDetails( bool ) ) );

    folderDetailsFolderTitlePanelLayout->addWidget( folderDetailsFolderTitleLabel );
    folderDetailsFolderTitlePanelLayout->addWidget( folderDetailsFolderTitleLineEdit );
    folderDetailsFolderTitlePanelLayout->addWidget( folderDetailsFolderMaximizeDetailsButton );

    folderDetailsPropsPanel = new PropertiesPanel( controller->getPreferences(), folderDetailsPanel );
    folderDetailsPanelLayout->addWidget( folderDetailsPropsPanel );

    folderDetailsPanel->setLayout( folderDetailsPanelLayout );

    vocabDetailsPanel = new QWidget();
    vocabDetailsPanelLayout = new QVBoxLayout();
    vocabDetailsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    vocabDetailsPanel->setLayout( vocabDetailsPanelLayout );
    vocabDetailsVocabHeaderPanel = new QWidget();
    vocabDetailsVocabHeaderPanelLayout = new QHBoxLayout();
    vocabDetailsVocabHeaderPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    vocabDetailsVocabHeaderPanel->setLayout( vocabDetailsVocabHeaderPanelLayout );

    vocabDetailsPanelLayout->addWidget( vocabDetailsVocabHeaderPanel );
    vocabDetailsTitle = new QLabel( tr( "Glossary" ) );
    vocabDetailsVocabHeaderPanelLayout->addWidget( vocabDetailsTitle );
    vocabDetailsVocabTitlePanel = new QWidget();
    vocabDetailsVocabTitlePanelLayout = new QHBoxLayout();
    vocabDetailsVocabTitlePanelLayout->setContentsMargins( 0, 0, 0, 0 );
    vocabDetailsVocabTitlePanel->setLayout( vocabDetailsVocabTitlePanelLayout );
    vocabDetailsVocabHeaderPanelLayout->addWidget( vocabDetailsVocabTitlePanel );
    vocabDetailsVocabTitleLabel = new QLabel( tr( "Title" ) );
    vocabDetailsVocabTitleLineEdit = new DigraphLineEdit();

    vocabDetailsVocabMaximizeDetailsButton = new QPushButton();
    vocabDetailsVocabMaximizeDetailsButton->setIcon( QIcon( QPixmap( maximize_xpm ) ) );
    vocabDetailsVocabMaximizeDetailsButton->setIconSize( QSize( 32, 32 ) );
    vocabDetailsVocabMaximizeDetailsButton->setToolTip( tr( "MaximizeDetails" ) );
    vocabDetailsVocabMaximizeDetailsButton->setCheckable( true );
    vocabDetailsVocabMaximizeDetailsButton->setMaximumHeight( 
    folderDetailsFolderTitleLineEdit->sizeHint().height() > 24 ? folderDetailsFolderTitleLineEdit->sizeHint().height() : 24 );
    vocabDetailsVocabMaximizeDetailsButton->setMaximumWidth( folderDetailsFolderMaximizeDetailsButton->maximumHeight() );
    connect( vocabDetailsVocabMaximizeDetailsButton, SIGNAL( toggled( bool ) ), this, SLOT( toggleMaximizeDetails( bool ) ) );

    vocabDetailsVocabTitlePanelLayout->addWidget( vocabDetailsVocabTitleLabel );
    vocabDetailsVocabTitlePanelLayout->addWidget( vocabDetailsVocabTitleLineEdit );
    vocabDetailsVocabTitlePanelLayout->addWidget( vocabDetailsVocabMaximizeDetailsButton );

    vocabDetailsTabWidget = new QTabWidget();
    vocabDetailsPanelLayout->addWidget( vocabDetailsTabWidget );

    vocabDetailsPropsPanel = new PropertiesPanel( controller->getPreferences(), vocabDetailsTabWidget );
    connect( vocabDetailsTabWidget, SIGNAL( currentChanged( int ) ), vocabDetailsPropsPanel, SLOT( updateCounters() ) ); 

    vocabDetailsTermsPanel = new QWidget();
    vocabDetailsTermsPanelLayout = new QVBoxLayout();
    vocabDetailsTermsPanel->setLayout( vocabDetailsTermsPanelLayout );
    termList = new QTreeWidget();
    termList->header()->setMovable( false );
    termList->setSortingEnabled( true );
    termList->setSelectionMode( QAbstractItemView::ExtendedSelection );
    termList->setColumnCount( 2 );
    QStringList headerLabels = QStringList();
    headerLabels << QApplication::translate( "QObject", controller->getPreferences().getFirstLanguage().toLatin1().data() ) << QApplication::translate( "QObject", controller->getPreferences().getTestLanguage().toLatin1().data() );
    termList->setHeaderLabels( headerLabels );
    termList->header()->setResizeMode( QHeaderView::Stretch );
    connect( termList, SIGNAL( itemSelectionChanged() ), this, SLOT( updateTermList() ) ); 

    termControlPanel = new QWidget();
    termControlPanelLayout = new QHBoxLayout();
    termControlPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    termControlPanel->setLayout( termControlPanelLayout );
    vocabDetailsTermsPanelLayout->addWidget( termList );
    vocabDetailsTermsPanelLayout->addWidget( termControlPanel );

    checkControlPanel = new QWidget();
    checkControlPanelLayout = new QHBoxLayout();
    checkControlPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    checkControlPanel->setLayout( checkControlPanelLayout );

    checkAllTermsButton = new QPushButton();
    checkAllTermsButton->setIcon( QIcon( QPixmap( checkAllTerms_xpm ) ) ); 
    checkAllTermsButton->setIconSize( QSize( 32, 32 ) );
    checkAllTermsButton->setToolTip( tr( "CheckAllTerms" ) );
    connect( checkAllTermsButton, SIGNAL( clicked() ), this, SLOT( checkAllTerms() ) );

    inverseCheckedTermsButton = new QPushButton();
    inverseCheckedTermsButton->setIcon( QIcon( QPixmap( inverseCheckedTerms_xpm ) ) ); 
    inverseCheckedTermsButton->setIconSize( QSize( 32, 32 ) );
    inverseCheckedTermsButton->setToolTip( tr( "InverseCheckedTerms" ) );
    connect( inverseCheckedTermsButton, SIGNAL( clicked() ), this, SLOT( inverseCheckedTerms() ) );

    checkControlPanelLayout->addWidget( checkAllTermsButton );
    checkControlPanelLayout->addWidget( inverseCheckedTermsButton );

    addRemoveTermPanel = new QWidget();
    addRemoveTermPanelLayout = new QHBoxLayout();
    addRemoveTermPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    addRemoveTermPanel->setLayout( addRemoveTermPanelLayout );

    addTermButton = new QPushButton( /*tr( "AddTerm" )*/ );
    addTermButton->setIcon( QIcon( QPixmap( addTerm_xpm ) ) ); 
    addTermButton->setIconSize( QSize( 32, 32 ) );
    addTermButton->setToolTip( tr( "AddTerm" ) );
    connect( addTermButton, SIGNAL( clicked() ), this, SLOT( addTerm() ) );

    editTermButton = new QPushButton( /*tr( "EditTerm" )*/ );
    editTermButton->setIcon( QIcon( QPixmap( editTerm_xpm ) ) ); 
    editTermButton->setIconSize( QSize( 32, 32 ) );
    editTermButton->setToolTip( tr( "EditTerm" ) );
    connect( editTermButton, SIGNAL( clicked() ), this, SLOT( editTerm() ) );

    removeTermButton = new QPushButton( /*tr( "RemoveTerm" )*/ );
    removeTermButton->setIcon( QIcon( QPixmap( removeTerm_xpm ) ) );
    removeTermButton->setIconSize( QSize( 32, 32 ) );
    removeTermButton->setToolTip( tr( "RemoveTerm" ) );
    connect( removeTermButton, SIGNAL( clicked() ), this, SLOT( removeTerms() ) );
  
    addRemoveTermPanelLayout->addWidget( addTermButton );
    addRemoveTermPanelLayout->addWidget( editTermButton );
    addRemoveTermPanelLayout->addWidget( removeTermButton );

    termControlPanelLayout->addWidget( checkControlPanel );
    termControlPanelLayout->addWidget( addRemoveTermPanel, 1 );
    vocabDetailsTabWidget->addTab( vocabDetailsPropsPanel, tr( "Properties" ) );
    vocabDetailsTabWidget->addTab( vocabDetailsTermsPanel, tr( "Words" ) );

    detailsPanel->addWidget( folderDetailsPanel );
    detailsPanel->addWidget( vocabDetailsPanel );

    splitter = new QSplitter( this );
    splitter->addWidget( treePanel );
    splitter->addWidget( detailsPanel );
    
    mainLayout->addWidget( splitter );
    setLayout( mainLayout );

    loadData();
    updateTree();
    vocabTreeView->setCurrentItem( vocabTreeRoot );

    detailsPanel->setCurrentIndex( panelFolderIndex );

    // We add the tree listener at the end to ignore events generated when creating
    // the text widgets.
    addListeners();

    updateFonts();
    updateUi();
}

VocabularyManagerFrame::~VocabularyManagerFrame() {
}

void VocabularyManagerFrame::updateShownItems() {
    keepSelection();
    updateTree();
    restoreTreeSelection();
    updateUi();
    restoreVocabSelection();
}

void VocabularyManagerFrame::updateTree() {
    if( vocabTreeView ) {
        removeListeners();
        vocabTreeView->setUpdatesEnabled( false );
        vocabTreeView->clear();
    }
    vocabTreeRoot = buildTreeRec( vocabTreeView, NULL, controller->getVocabTree() );
    vocabTreeView->sortItems( 0, Qt::AscendingOrder );

    //vocabTreeView->setCurrentItem( vocabTreeRoot );
    //if( currentFolderId != -1 || currentVocabId != -1 )
    //    restoreSelection( currentFolderId, currentVocabId, currentTermId, selectedTermIdList );
    //else
    //    vocabTreeView->setCurrentItem( vocabTreeRoot );
    if( vocabTreeView ) {
        addListeners();
        vocabTreeView->setUpdatesEnabled( true );
    }
}

void VocabularyManagerFrame::loadData() {
    controller->loadData(); // If this returns false, handle the error.
}

bool VocabularyManagerFrame::saveData() {
    bool isOk = controller->saveData();
    return( isOk );
}

bool VocabularyManagerFrame::isExportAllowed() const {
    TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
    if( selectedItem->isFolder() ) {
        FolderTreeItem* folderItem = (FolderTreeItem*)selectedItem;
        return( !folderItem->getFolder()->isEmpty() );
    }
    else {
        VocabTreeItem* vocabItem = (VocabTreeItem*)selectedItem;
        return( !vocabItem->getVocabulary()->isEmpty() );
    }
}

bool VocabularyManagerFrame::isImportAllowed() const {
    TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
    return( selectedItem && selectedItem->isFolder() );
}

void VocabularyManagerFrame::cut() {
    QWidget* widget = qApp->focusWidget();
    if( widget ) {
        if( widget == vocabTreeView ) {
            TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
            if( selectedItem ) {
                if( selectedItem->isFolder() ) {
                    FolderTreeItem* folderItem = (FolderTreeItem*)selectedItem;
                    Folder* folder = folderItem->getFolder();
                    controller->copy( folder );
                    doRemoveItem( false );
                }
                else {
                    VocabTreeItem* vocabItem = (VocabTreeItem*)selectedItem;
                    Vocabulary* vocab = vocabItem->getVocabulary();
                    controller->copy( vocab );
                    doRemoveItem( false );
                }
            }
        }
        else if( widget == termList ) {
            copyTerms();
            doRemoveTerms( false, false );
        }
    }
}

void VocabularyManagerFrame::copy() {
    QWidget* widget = qApp->focusWidget();
    if( widget ) {
        if( widget == vocabTreeView ) {
            TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
            if( selectedItem ) {
                if( selectedItem->isFolder() ) {
                    FolderTreeItem* folderItem = (FolderTreeItem*)selectedItem;
                    Folder* folder = folderItem->getFolder();
                    controller->copy( folder );
                }
                else {
                    VocabTreeItem* vocabItem = (VocabTreeItem*)selectedItem;
                    Vocabulary* vocab = vocabItem->getVocabulary();
                    controller->copy( vocab );
                }
            }
        }
        else if( widget == termList )
            copyTerms();
    }
}

void VocabularyManagerFrame::paste() {
    QWidget* widget = qApp->focusWidget();
    if( widget ) {
        if( widget == vocabTreeView ) {
            TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
            if( selectedItem ) {
                QString dataType = controller->getClipboardDataType();
                if( selectedItem->isFolder() ) {
                    if( dataType == QString( "vocabulary" ) )
                        pasteVocabulary();
                    else if( dataType == QString( "folder" ) )
                        pasteFolder();
                }
                else {
                    if( dataType == QString( "terms" ) )
                        pasteTerms();
                }
            }
        }
        else if( widget == termList ) {
            QString dataType = controller->getClipboardDataType();
            if( dataType == QString( "terms" ) )
                pasteTerms();
        }
    }
}

void VocabularyManagerFrame::importData() {
    TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
    if( selectedItem && selectedItem->isFolder() ) {
        FolderTreeItem* folderItem = (FolderTreeItem*)selectedItem;
        Folder* folder = folderItem->getFolder(); 
        QString importFile = QFileDialog::getOpenFileName( this, tr( "Import..." ), QDir::homePath(), tr( "Glossaries (*.zip)" ) );
        if( !importFile.isEmpty() ) {
            QString fileExtension = importFile.right( 4 );
            if( fileExtension == QString( ".zip" ) ) {
                QStringList translationLanguages = controller->getTranslationLanguagesFromFile( importFile );

                QStringList languagesToImport( translationLanguages );
                if( translationLanguages.count() > 2 ) {
                    TranslationSelectionDialog msgBox( tr( "Import..." ), tr( "MultipleTranslationsDetectedForImport" ), 
                        translationLanguages, TranslationSelectionDialog::selectionModeStudyLanguages, controller, this );
                    msgBox.setMaximumHeight( size().height() - 40 );
                    msgBox.setMaximumWidth( size().width() - 40 );
                    int response = msgBox.exec();
                    if( response )
                        languagesToImport = msgBox.getSelectedLanguages();
                    else  {
                        return; // Cancel import.
                    }
                }

                for( QStringList::ConstIterator it = languagesToImport.begin(); it != languagesToImport.end(); it++ ) {
                    const QString& lang = *it;
                    if( !controller->getPreferences().isStudyLanguage( lang ) )
                        controller->getPreferences().addStudyLanguage( lang );
                }

                Base* newItem = controller->importData( folder, importFile, languagesToImport );
                if( newItem ) {
                    TreeItem* newTreeItem = NULL;
                    if( strcmp( newItem->className(), "Folder" ) == 0 ) {
                        Folder* newFolder = (Folder*)newItem;
                        newFolder->setModificationDate( QDateTime::currentDateTime() );
                        newFolder->setDirty( true );
                        folder->add( newFolder );
                        newTreeItem = buildTreeRec( vocabTreeView, folderItem, newFolder, true ); 
                    }
                    else if( strcmp( newItem->className(), "Vocabulary" ) == 0 ) {
                        Vocabulary* newVocab = (Vocabulary*)newItem;
                        newVocab->setModificationDate( QDateTime::currentDateTime() );
                        newVocab->setDirty( true );
                        folder->add( newVocab );
                        newTreeItem = buildTreeRec( folderItem, newVocab ); 
                    }
                    if( newTreeItem ) {
                        vocabTreeView->scrollToItem( newTreeItem, QAbstractItemView::PositionAtCenter );
                        vocabTreeView->setCurrentItem( newTreeItem );
                        newTreeItem->setExpanded( true );
                    }
                    else {
                        // This happens when the imported data is from a different language pair
                        // than the current one.  We reselect the selectedItem in this case.
                        vocabTreeView->setCurrentItem( selectedItem );
                    }
                    QString msg = tr( "ImportSuccessful" );
                    if( !newTreeItem )
                        msg += tr( "InvisibleImport" );
                    QMessageBox::information( this, QObject::tr( "OperationSuccessful" ), msg );
                }
                else
                    QMessageBox::warning( this, QObject::tr( "OperationFailed" ), tr( "ItemImportFailed" ) );
            }
            else
                QMessageBox::warning( this, QObject::tr( "OperationFailed" ), tr( "ItemImportFailedZipFileExpected" ) );
        }
    }
}

void VocabularyManagerFrame::exportData() {
    TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
    if( selectedItem ) {
        if( selectedItem->isFolder() ) {
            FolderTreeItem* folderItem = (FolderTreeItem*)selectedItem;
            exportFolder( folderItem->getFolder() );
        }
        else {
            VocabTreeItem* vocabItem = (VocabTreeItem*)selectedItem;
            exportVocabulary( vocabItem->getVocabulary() );
        }
    }
}

void VocabularyManagerFrame::exportVocabulary( Vocabulary* vocab ) {
    QStringList translationLanguages = vocab->getTranslationLanguages();
    QStringList languagesToExport = translationLanguages;
    if( translationLanguages.count() > 2 ) {
        TranslationSelectionDialog msgBox( tr( "Export..." ), tr( "MultipleTranslationsDetectedForExport" ), 
            translationLanguages, TranslationSelectionDialog::selectionModeAllLanguages, controller, this );
        msgBox.setMaximumHeight( size().height() - 40 );
        msgBox.setMaximumWidth( size().width() - 40 );
        int response = msgBox.exec();
        if( response )
            languagesToExport = msgBox.getSelectedLanguages();
        else 
            return; // Cancel export.
    }

    QString exportFile = QFileDialog::getSaveFileName( this, tr( "Export..." ), QDir::homePath() + "/" + vocab->getTitle() + ".zip",
        tr( "Glossaries (*.zip)" ) );
    if( !exportFile.isEmpty() ) {
        bool isOk = controller->exportData( vocab, exportFile, &languagesToExport );
        if( isOk )
            QMessageBox::information( this, QObject::tr( "OperationSuccessful" ), tr( "GlossaryExportSuccessful" ) );
        else
            QMessageBox::warning( this, QObject::tr( "OperationFailed" ), tr( "GlossaryExportFailed" ) );
    }
}

void VocabularyManagerFrame::exportFolder( Folder* folder ) {
    QStringList translationLanguages = folder->getTranslationLanguages();
    QStringList languagesToExport = translationLanguages;
    if( translationLanguages.count() > 2 ) {
        TranslationSelectionDialog msgBox( tr( "Export..." ), tr( "MultipleTranslationsDetectedForFolderExport" ), 
            translationLanguages, TranslationSelectionDialog::selectionModeAllLanguages, controller, this );
        msgBox.setMaximumHeight( size().height() - 40 );
        msgBox.setMaximumWidth( size().width() - 40 );
        int response = msgBox.exec();
        if( response )
            languagesToExport = msgBox.getSelectedLanguages();
        else 
            return; // Cancel export.
    }

    QString exportFile = QFileDialog::getSaveFileName( this, tr( "Export..." ), QDir::homePath() + "/" + folder->getTitle() + ".zip",
        tr( "Glossaries (*.zip)" ) );
    if( !exportFile.isEmpty() ) {
        bool isOk = controller->exportData( folder, exportFile, &languagesToExport );
        if( isOk )
            QMessageBox::information( this, QObject::tr( "OperationSuccessful" ), tr( "FolderExportSuccessful" ) );
        else
            QMessageBox::warning( this, QObject::tr( "OperationFailed" ), tr( "FolderExportFailed" ) );
    }
}

void VocabularyManagerFrame::updateFonts() {
    QFont mediumFont( controller->getPreferences().getMediumFont() );
    QFont labelsFont( controller->getPreferences().getLabelsFont() );

    vocabTreeView->setFont( mediumFont );
    vocabTreeView->headerItem()->setFont( 0, labelsFont );
    folderDetailsTitle->setFont( labelsFont ); 
    folderDetailsFolderTitleLabel->setFont( labelsFont );
    folderDetailsFolderTitleLineEdit->setFont( mediumFont );
    folderDetailsPropsPanel->updateFonts();
    folderDetailsFolderTitleLineEdit->setFont( mediumFont );
    vocabDetailsTitle->setFont( labelsFont );
    vocabDetailsVocabTitleLabel->setFont( labelsFont );
    vocabDetailsVocabTitleLineEdit->setFont( mediumFont );
    vocabDetailsPropsPanel->updateFonts();
    termList->setFont( mediumFont );
    termList->headerItem()->setFont( 0, labelsFont );
    termList->headerItem()->setFont( 1, labelsFont );
    for( int i = 0; i < termList->topLevelItemCount(); i++ ) {
        TermListItem* item = (TermListItem*)termList->topLevelItem( i );
        item->setFont( 0, controller->getPreferences().getMediumFont( controller->getPreferences().getFirstLanguage() ) );
        item->setFont( 1, controller->getPreferences().getMediumFont( controller->getPreferences().getTestLanguage() ) );
    }
}

bool VocabularyManagerFrame::isDigraphEnabled() const {
    return( folderDetailsFolderTitleLineEdit->isDigraphEnabled() );
}

void VocabularyManagerFrame::keepSelection() {
    currentFolderId = -1; 
    currentVocabId = -1; 
    currentTermId = -1;
    selectedTermIdList.clear();

    TreeItem* currentItem = (TreeItem*)vocabTreeView->currentItem();
    if( currentItem ) {
        if( currentItem->isFolder() ) {
            FolderTreeItem* folderItem = (FolderTreeItem*)currentItem;
            currentFolderId = folderItem->getFolder()->getId();
        }
        else {
            VocabTreeItem* vocabItem = (VocabTreeItem*)currentItem;
            currentVocabId = vocabItem->getVocabulary()->getId();
            for( int i = 0; i < termList->topLevelItemCount(); i++ ) {
                TermListItem* termItem = (TermListItem*)termList->topLevelItem( i );
                if( termItem->isSelected() ) {
                    if( currentTermId == -1 )
                        currentTermId = termItem->getTerm()->getId();
                    selectedTermIdList.append( termItem->getTerm()->getId() );
                }
            }
        }
    }
}

void VocabularyManagerFrame::restoreTreeSelection() {
    bool isTreeItemFound = false;
    for( TreeItem* item = (TreeItem*)vocabTreeView->topLevelItem( 0 ); item; item = (TreeItem*)vocabTreeView->itemBelow( item ) ) {
        if( item->isFolder() ) {
            FolderTreeItem* folderItem = (FolderTreeItem*)item;
            if( currentFolderId == folderItem->getFolder()->getId() ) {
                isTreeItemFound = true;
                vocabTreeView->scrollToItem( item, QAbstractItemView::PositionAtCenter );
                vocabTreeView->setCurrentItem( item );
                break;
            }
        }
        else {
            VocabTreeItem* vocabItem = (VocabTreeItem*)item;
            if( currentVocabId == vocabItem->getVocabulary()->getId() ) {
                isTreeItemFound = true;
                vocabTreeView->scrollToItem( item, QAbstractItemView::PositionAtCenter );
                vocabTreeView->setCurrentItem( item );
                break;
            }
        }
    }
    if( !isTreeItemFound ) {
        vocabTreeView->setCurrentItem( vocabTreeRoot );
        vocabTreeView->scrollToItem( vocabTreeRoot, QAbstractItemView::PositionAtCenter );
    }
}

void VocabularyManagerFrame::restoreVocabSelection() {
    if( currentTermId != -1 ) {
        termList->clearSelection();
        for( int i = 0; i < termList->topLevelItemCount(); i++ ) {
            TermListItem* termItem = (TermListItem*)termList->topLevelItem( i );
            int termItemId = termItem->getTerm()->getId();
            if( selectedTermIdList.contains( termItemId ) )
                termList->setCurrentItem( termItem );
            if( currentTermId == termItemId ) {
                termList->scrollToItem( termItem, QAbstractItemView::PositionAtCenter );
                termList->setCurrentItem( termItem );
                updateTermList();
                break;
            }
        }
    }
}

void VocabularyManagerFrame::setDigraphEnabled( bool isEnabled ) {
    folderDetailsFolderTitleLineEdit->setDigraphEnabled( isEnabled );
    vocabDetailsVocabTitleLineEdit->setDigraphEnabled( isEnabled );
    folderDetailsPropsPanel->setDigraphEnabled( isEnabled );
    vocabDetailsPropsPanel->setDigraphEnabled( isEnabled );
    if( searchDialog )
        searchDialog->setDigraphEnabled( isEnabled );
}

void VocabularyManagerFrame::retranslateUi() {
    vocabTreeView->setHeaderLabel( tr( "Glossaries" ) );
    folderDetailsTitle->setText( tr( "Folder" ) );
    folderDetailsFolderTitleLabel->setText( tr( "Title" ) );
    vocabDetailsTitle->setText( tr( "Glossary" ) );
    vocabDetailsVocabTitleLabel->setText( tr( "Title" ) );
    QStringList headerLabels;
    headerLabels << QApplication::translate( "QObject", controller->getPreferences().getFirstLanguage().toLatin1().data() );
    headerLabels << QApplication::translate( "QObject", controller->getPreferences().getTestLanguage().toLatin1().data() );
    termList->setHeaderLabels( headerLabels );
    folderDetailsPropsPanel->retranslateUi();
    vocabDetailsPropsPanel->retranslateUi();
    vocabDetailsTabWidget->setTabText( 0, tr( "Properties" ) );
    vocabDetailsTabWidget->setTabText( 1, tr( "Words" ) );
    //addFolderButton->setText( tr( "AddFolder" ) );
    addFolderButton->setToolTip( tr( "AddFolder" ) );
    //addVocabButton->setText( tr( "AddGlossary" ) );
    addVocabButton->setToolTip( tr( "AddGlossary" ) );
    //removeItemButton->setText( tr( "RemoveItem" ) );
    removeItemButton->setToolTip( tr( "RemoveItem" ) );
    //addTermButton->setText( tr( "AddTerm" ) );
    addTermButton->setToolTip( tr( "AddTerm" ) );
    //editTermButton->setText( tr( "EditTerm" ) );
    editTermButton->setToolTip( tr( "EditTerm" ) );
    //removeTermButton->setText( tr( "RemoveTerm" ) );
    removeTermButton->setToolTip( tr( "RemoveTerm" ) );
    folderDetailsFolderMaximizeDetailsButton->setToolTip( tr( "MaximizeDetails" ) ); 
    vocabDetailsVocabMaximizeDetailsButton->setToolTip( tr( "MaximizeDetails" ) );
    checkAllTermsButton->setToolTip( tr( "CheckAllTerms" ) );
    inverseCheckedTermsButton->setToolTip( tr( "InverseCheckedTerms" ) );
    if( searchDialog )
        searchDialog->retranslateUi();
    updateUi();
}

bool VocabularyManagerFrame::areDetailsMaximized() const {
    TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
    if( selectedItem ) {
        return( selectedItem->isFolder() ? 
            folderDetailsFolderMaximizeDetailsButton->isChecked() : 
                vocabDetailsVocabMaximizeDetailsButton->isChecked() );
    }
    return( false );
}


void VocabularyManagerFrame::toggleMaximizeDetails( bool isOn ) {
    if( isOn )
        maximizeDetailsPanel();
    else
        restoreDetailsPanel();
}

void VocabularyManagerFrame::updateUi() {
    removeListeners();
    TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
    removeItemButton->setEnabled( selectedItem );
    if( selectedItem ) {
        if( selectedItem->isFolder() ) {
            FolderTreeItem* folderItem = (FolderTreeItem*)selectedItem;
            updateCurrentFolder( folderItem );
        }
        else {
            VocabTreeItem* vocabItem = (VocabTreeItem*)selectedItem;
            updateCurrentVocab( vocabItem );
        }
    }
    if( termList && termList->topLevelItemCount() > 0 ) {
        checkAllTermsButton->setEnabled( true );
        inverseCheckedTermsButton->setEnabled( true );
    }
    else {
        checkAllTermsButton->setEnabled( false );
        inverseCheckedTermsButton->setEnabled( false );
    }
    emit( selectionChanged( selectedItem ) );
    addListeners();
}

void VocabularyManagerFrame::updateCurrentTreeItem( QTreeWidgetItem* currItem, QTreeWidgetItem* prevItem ) {
    if( prevItem ) {
        TreeItem* treeItem = (TreeItem*)prevItem;
        if( !treeItem->isFolder() )
            treeItem->setOpen( false );
    }
    if( currItem ) {
        TreeItem* treeItem = (TreeItem*)currItem;
        if( !treeItem->isFolder() )
            treeItem->setOpen( true );
    }
    updateUi();
}

void VocabularyManagerFrame::updateCurrentFolder( FolderTreeItem* folderItem ) {
    QString firstLang = controller->getPreferences().getFirstLanguage();
    QString testLang = controller->getPreferences().getTestLanguage();
    bool isBothLangSet = ( firstLang != QString::null && testLang != QString::null );
    Folder* folder = folderItem->getFolder();
    folderDetailsFolderTitleLineEdit->setText( folder->getTitle() );
    folderDetailsPropsPanel->setFolder( folder );
    addVocabButton->setEnabled( isBothLangSet );
    addFolderButton->setEnabled( isBothLangSet );
    removeItemButton->setEnabled( folderItem != vocabTreeRoot );
    detailsPanel->setCurrentIndex( panelFolderIndex );
}

void VocabularyManagerFrame::updateCurrentVocab( VocabTreeItem* vocabItem ) {
    const Preferences& prefs = controller->getPreferences();
    Vocabulary* vocab = vocabItem->getVocabulary();
    vocabDetailsVocabTitleLineEdit->setText( vocab->getTitle() );
    vocabDetailsPropsPanel->setVocabulary( vocab );
    QStringList termListHeaders;
    termListHeaders << QApplication::translate( "QObject", prefs.getFirstLanguage().toLatin1().data() );
    termListHeaders << QApplication::translate( "QObject", prefs.getTestLanguage().toLatin1().data() );
    termList->setHeaderLabels( termListHeaders );
    termList->setSortingEnabled( false );
    termList->clear();
    for( Vocabulary::TermMap::Iterator it = vocab->begin(); it != vocab->end(); it++ ) {
        Term& term = *it; 
        if( !prefs.isLanguageFilterEnabled() ||
                ( term.isTranslationExists( prefs.getFirstLanguage() ) &&
                term.isTranslationExists( prefs.getTestLanguage() ) ) ) {
            TermListItem* termItem = new TermListItem( termList, &term, prefs.getFirstLanguage(), prefs.getTestLanguage(), prefs.isAltInTermListShown() );
            termItem->setOn( term.isMarkedForStudy() );
            termItem->setFont( 0, prefs.getMediumFont( prefs.getFirstLanguage() ) );
            termItem->setFont( 1, prefs.getMediumFont( prefs.getTestLanguage() ) );
        }
    }
    termList->sortItems( 0, Qt::AscendingOrder );
    termList->setSortingEnabled( true );
    vocabItem->setOpen( true );
    addVocabButton->setEnabled( false );
    addFolderButton->setEnabled( false );
    detailsPanel->setCurrentIndex( panelVocabIndex );
    updateTermList();
}

void VocabularyManagerFrame::updateTermList() {
    uint selectedTermCount = getSelectedTermCount();
    editTermButton->setEnabled( selectedTermCount == 1 );
    removeTermButton->setEnabled( selectedTermCount > 0 );
}

FolderTreeItem* VocabularyManagerFrame::addFolder() {
    return( addFolder( NULL ) );
}

FolderTreeItem* VocabularyManagerFrame::addFolder( Folder* folder, QMap<int,Vocabulary>* vocabularies /*= NULL*/ ) {
    FolderTreeItem* newFolderItem = vocabTreeView->addFolder( folder, vocabularies );
    if( newFolderItem ) {
        newFolderItem->setPropertiesPanel( folderDetailsPropsPanel );
        folderDetailsPropsPanel->setFolder( newFolderItem->getFolder() );
        folderDetailsPropsPanel->updateCounters();
        vocabTreeView->scrollToItem( newFolderItem, QAbstractItemView::PositionAtCenter );
    }
    return( newFolderItem );
}

VocabTreeItem* VocabularyManagerFrame::addVocab() {
    return( addVocab( NULL ) );
}

VocabTreeItem* VocabularyManagerFrame::addVocab( Vocabulary* vocab ) {
    VocabTreeItem* newVocabItem = vocabTreeView->addVocab( vocab );
    if( newVocabItem ) {
        newVocabItem->setPropertiesPanel( vocabDetailsPropsPanel );
        vocabDetailsPropsPanel->setVocabulary( newVocabItem->getVocabulary() );
        vocabDetailsPropsPanel->updateCounters();
        vocabTreeView->scrollToItem( newVocabItem, QAbstractItemView::PositionAtCenter );
    }
    return( newVocabItem );
}

void VocabularyManagerFrame::removeItem() {
    doRemoveItem();
}

void VocabularyManagerFrame::doRemoveItem( bool confirmBeforeRemove /*= true*/ ) {
    int response;
    if( confirmBeforeRemove ) {
        QMessageBox msgBox( QObject::tr( "Warning" ), tr( "ConfirmRemoveItem" ),
            QMessageBox::Warning,
            QMessageBox::Yes,
            QMessageBox::No | QMessageBox::Default | QMessageBox::Escape,
            QMessageBox::NoButton,
            this );
        msgBox.setButtonText( QMessageBox::Yes, tr( "Yes" ) );
        msgBox.setButtonText( QMessageBox::No, tr( "No" ) );

        response = msgBox.exec();
    }
    else
        response = QMessageBox::Yes;

    if( response == QMessageBox::Yes ) {
        TreeItem* currentItem = (TreeItem*)vocabTreeView->currentItem();
        if( currentItem->isFolder() ) {
            FolderTreeItem* folderItem = (FolderTreeItem*)currentItem;
            Folder* folder = folderItem->getFolder();
            folder->setMarkedForDeletion( true );
        }
        else {
            VocabTreeItem* vocabItem = (VocabTreeItem*)currentItem;
            Vocabulary* vocab = vocabItem->getVocabulary();
            vocab->setMarkedForDeletion( true );
        }
        vocabTreeView->removeItem();
    }
}

void VocabularyManagerFrame::checkAllTerms() {
    for( int i = 0; i < termList->topLevelItemCount(); i++ ) {
        TermListItem* termItem = (TermListItem*)termList->topLevelItem( i );
        termItem->setCheckState( 0, Qt::Checked );
    }
    vocabDetailsPropsPanel->updateCounters();
}

void VocabularyManagerFrame::inverseCheckedTerms() {
    for( int i = 0; i < termList->topLevelItemCount(); i++ ) {
        TermListItem* termItem = (TermListItem*)termList->topLevelItem( i );
        termItem->setCheckState( 0, termItem->checkState( 0 ) == Qt::Checked ? Qt::Unchecked : Qt::Checked );
    }
    vocabDetailsPropsPanel->updateCounters();
}

void VocabularyManagerFrame::addTerm() {
    TreeItem* currItem = (TreeItem*)vocabTreeView->currentItem();
    if( currItem && !currItem->isFolder() ) {
        VocabTreeItem* vocabItem = (VocabTreeItem*)vocabTreeView->currentItem();
        if( vocabItem ) {
            Vocabulary* vocab = vocabItem->getVocabulary();
            TermDialog dialog( *vocab, controller, this );
#if defined(Q_WS_HILDON)
            dialog.showFullScreen();
#endif
            int result = dialog.exec();
            if( result ) {
                Term newTerm( dialog.getTerm() );
                vocab->addTerm( newTerm );
                Term& term = vocab->getTerm( newTerm.getId() );
                const Preferences& prefs = controller->getPreferences();
                TermListItem* newTermListItem = new TermListItem( termList, &term, 
                    prefs.getFirstLanguage(), prefs.getTestLanguage(), prefs.isAltInTermListShown() );
                newTermListItem->setOn( true );
                vocab->setModificationDate( QDateTime::currentDateTime() );
                vocab->setDirty( true );
                vocabDetailsPropsPanel->updateCounters();
                updateUi(); 
                for( int i = 0; i < termList->topLevelItemCount(); i++ ) {
                    TermListItem* termItem = (TermListItem*)termList->topLevelItem( i );
                    Term* termToSelect = termItem->getTerm();
                    if( termToSelect->getId() == newTerm.getId() ) {
                        termList->setCurrentItem( termItem );
                        termList->scrollToItem( termItem, QAbstractItemView::PositionAtCenter );
                        break;
                    }
                }
            }
        }
    }
}

void VocabularyManagerFrame::editTerm() {
    VocabTreeItem* vocabItem = (VocabTreeItem*)vocabTreeView->currentItem();
    if( vocabItem ) {
        Vocabulary* vocab = vocabItem->getVocabulary();

        for( int i = 0; i < termList->topLevelItemCount(); i++ ) {
            TermListItem* termItem = (TermListItem*)termList->topLevelItem( i );
            if( termItem->isSelected() ) {
                TermDialog dialog( *vocab, controller, this, *termItem->getTerm() );
#if defined(Q_WS_HILDON)
                dialog.showFullScreen();
#endif
                int result = dialog.exec();
                if( result ) { 
                    Term newTerm = dialog.getTerm();
                    termItem->getTerm()->addTranslation( newTerm.getTranslation( controller->getPreferences().getFirstLanguage() ) );
                    termItem->getTerm()->addTranslation( newTerm.getTranslation( controller->getPreferences().getTestLanguage() ) );
                    BilingualKey commentKey( controller->getPreferences().getFirstLanguage(), controller->getPreferences().getTestLanguage() );
                    termItem->getTerm()->addComment( commentKey, newTerm.getComment( commentKey ) );
                    termItem->getTerm()->setImagePath( newTerm.getImagePath() );
                    termItem->updateUi();
                    vocab->setModificationDate( QDateTime::currentDateTime() );
                    vocab->setDirty( true );
                }
                break;
            }
        }
    }
}

void VocabularyManagerFrame::removeTerms() {
    doRemoveTerms();
}

void VocabularyManagerFrame::doRemoveTerms( bool allowSelectTrans /*= true*/, bool confirmBeforeRemove /*= true*/ ) {
    // Find all the translation languages of the selected terms.
    QStringList translationLanguages;
    QList<TermListItem*> itemsToDelete;

    for( int i = 0; i < termList->topLevelItemCount(); i++ ) {
        TermListItem* termItem = (TermListItem*)termList->topLevelItem( i );
        if( termItem->isSelected() ) {
            itemsToDelete << termItem;
            Term* term = termItem->getTerm();
            for( Term::TranslationMap::ConstIterator it = term->translationsBegin(); it != term->translationsEnd(); it++ ) {
                const Translation& trans = *it;
                if( !translationLanguages.contains( trans.getLanguage() ) )
                    translationLanguages.append( trans.getLanguage() );
            }
        }
    }
    if( itemsToDelete.size() == 0 )
        return;
    if( translationLanguages.count() <= 2 ) {
        int response;

        if( confirmBeforeRemove ) {
            QMessageBox msgBox( QObject::tr( "Warning" ), tr( "ConfirmRemoveSelectedTerms" ),
                QMessageBox::Warning,
                QMessageBox::Yes,
                QMessageBox::No | QMessageBox::Default | QMessageBox::Escape,
                QMessageBox::NoButton,
                this );
            msgBox.setButtonText( QMessageBox::Yes, tr( "Yes" ) );
            msgBox.setButtonText( QMessageBox::No, tr( "No" ) );

            response = msgBox.exec();
        }
        else 
            response = QMessageBox::Yes;

        if( response == QMessageBox::Yes ) {
            VocabTreeItem* vocabItem = (VocabTreeItem*)vocabTreeView->currentItem();
            Vocabulary* vocab = vocabItem->getVocabulary();

            for( int i = 0; i < itemsToDelete.size(); i++ ) {
                TermListItem* termItem = (TermListItem*)itemsToDelete[ i ];
                Term* term = termItem->getTerm();
                if( !term->getImagePath().isNull() ) {
                    QDir imagePath( term->getImagePath() );
                    if( imagePath.isRelative() ) {
                        const QString& imagePath = controller->getApplicationDirName() + "/" + vocab->getParent()->getPath() +
                            "/v-" + QString::number( vocab->getId() ) + "/" + term->getImagePath();
                        QFile imageFile( imagePath );
                        if( imageFile.exists() ) {
                            if( !imageFile.remove() )
                                cerr << "Could not remove image " << qPrintable( imagePath ) << endl;
                        }
                    }
                }
                vocab->removeTerm( term->getId() );
                delete( termItem );
            }

            vocab->setModificationDate( QDateTime::currentDateTime() );
            vocab->setDirty( true );
            termList->clearSelection();
            vocabDetailsPropsPanel->updateCounters();
            updateUi();
        }
    }
    else {
        int response;
        QStringList selectedLanguages;
        if( allowSelectTrans ) {
            TranslationSelectionDialog msgBox( tr( "MultipleTranslationsDetectedForRemoveTermsCaption" ), tr( "MultipleTranslationsDetectedForRemoveTerms" ), 
                translationLanguages, TranslationSelectionDialog::selectionModeTargetLanguage, controller, this );
            msgBox.setMaximumHeight( size().height() - 40 );
            msgBox.setMaximumWidth( size().width() - 40 );
            response = msgBox.exec();
            if( response )
                selectedLanguages = msgBox.getSelectedLanguages();
        }
        else {
            selectedLanguages = QStringList();
            selectedLanguages.append( controller->getPreferences().getFirstLanguage() );
            selectedLanguages.append( controller->getPreferences().getTestLanguage() );
        }
        if( selectedLanguages.count() == 0 )
            return;

        VocabTreeItem* vocabItem = (VocabTreeItem*)vocabTreeView->currentItem();
        Vocabulary* vocab = vocabItem->getVocabulary();

        for( int i = 0; i < itemsToDelete.size(); i++ ) {
            TermListItem* termItem = itemsToDelete[ i ];

            Term* term = termItem->getTerm();

            for( QStringList::ConstIterator it = selectedLanguages.begin(); it != selectedLanguages.end(); it++ ) {
                QString lang = *it;
                term->removeTranslation( lang );
            }
            
            if( term->getTranslationCount() == 0 ) {
                if( !term->getImagePath().isNull() ) {
                    QDir imagePath( term->getImagePath() );
                    if( imagePath.isRelative() ) {
                        const QString& imagePath = controller->getApplicationDirName() + "/" + vocab->getParent()->getPath() +
                            "/v-" + QString::number( vocab->getId() ) + "/" + term->getImagePath();
                        QFile imageFile( imagePath );
                        if( imageFile.exists() ) {
                            if( !imageFile.remove() )
                                cerr << "Could not remove image " << qPrintable( imagePath ) << endl;
                        }
                    }
                }
                vocab->removeTerm( term->getId() );
                delete( termItem );
            }
        }

        vocab->setModificationDate( QDateTime::currentDateTime() );
        vocab->setDirty( true );
        termList->clearSelection();
        vocabDetailsPropsPanel->updateCounters();
        updateUi();
    }
}

void VocabularyManagerFrame::updateCurrentFolderTitle( const QString& title ) {
    FolderTreeItem* folderItem = (FolderTreeItem*)vocabTreeView->currentItem();
    folderItem->setText( 0, title );
    Folder* folder = folderItem->getFolder();
    folder->setTitle( title );
    folder->setModificationDate( QDateTime::currentDateTime() );
    folder->setDirty( true );
}

void VocabularyManagerFrame::updateCurrentVocabTitle( const QString& title ) {
    VocabTreeItem* vocabItem = (VocabTreeItem*)vocabTreeView->currentItem();
    vocabItem->setText( 0, title );
    Vocabulary* vocab = vocabItem->getVocabulary();
    vocab->setTitle( title );
    vocab->setModificationDate( QDateTime::currentDateTime() );
    vocab->setDirty( true );
}

FolderTreeItem* VocabularyManagerFrame::buildTreeRec( VocabTreeView* vocabTreeView, FolderTreeItem* parentItem, Folder* folder, bool closeFolder /*= false*/ ) {
    FolderTreeItem* folderItem = NULL;
    if( !folder->isMarkedForDeletion() ) {
        if( parentItem )
            folderItem = new FolderTreeItem( parentItem, folder, controller->getPreferences() ); 
        else {
            // Root folder.
            folderItem = new FolderTreeItem( vocabTreeView, folder, controller->getPreferences() );
            //folderItem->setDisabled( true ); Doesn't work on qt4 as all the children are also disabled.  Maybe use setFlags() instead.
        }
        if( folderItem ) {
            if( closeFolder )
                folderItem->setOpen( false );
            else
                folderItem->setOpen( controller->getPreferences().isFolderOpen( folder->getId() ) );
            folderItem->setPropertiesPanel( folderDetailsPropsPanel );
            if( !folder->isEmpty() ) {
                QString firstLang = controller->getPreferences().getFirstLanguage();
                QString testLang = controller->getPreferences().getTestLanguage();
                bool isBothLangSet = ( firstLang != QString::null && testLang != QString::null );
                if( isBothLangSet ) {
                    for( int i = 0; i < folder->getChildrenCount(); i++ ) {
                        Base* folderChild = folder->childAt( i );
                        if( strcmp( folderChild->className(), "Folder" ) == 0 ) {
                            Folder* childFolder = (Folder*)folderChild;
                            buildTreeRec( vocabTreeView, folderItem, childFolder, closeFolder );
                        }
                        else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 ) {
                            Vocabulary* vocab = (Vocabulary*)folderChild;
                            buildTreeRec( folderItem, vocab );
                        }
                    }
                    if( folderItem->childCount() == 0 && folderItem->getFolder()->getParent() ) {
                        delete folderItem;
                        folderItem = NULL;
                    }
                }
            }
        }
    }
    return( folderItem );
}

VocabTreeItem* VocabularyManagerFrame::buildTreeRec( FolderTreeItem* parentItem, Vocabulary* vocab ) {
    QString firstLang = controller->getPreferences().getFirstLanguage();
    QString testLang = controller->getPreferences().getTestLanguage();
    bool isVocabVisible = !vocab->isMarkedForDeletion() && 
        ( !controller->getPreferences().isLanguageFilterEnabled() || vocab->containsTermWithTranslations( firstLang, testLang ) );
    if( isVocabVisible ) {
        VocabTreeItem* vocabItem = new VocabTreeItem( parentItem, vocab );    
        vocabItem->setOpen( false );
        vocabItem->setPropertiesPanel( vocabDetailsPropsPanel );
        return( vocabItem );
    }
    else
        return( NULL );
}

void VocabularyManagerFrame::copyTerms() const {
    QString firstLang( controller->getPreferences().getFirstLanguage() );
    QString testLang( controller->getPreferences().getTestLanguage() );
    QList<Term> termsToCopy;

    for( int i = 0; i < termList->topLevelItemCount(); i++ ) {
        TermListItem* termItem = (TermListItem*)termList->topLevelItem( i );
        if( termItem->isSelected() ) {
            Term* term = termItem->getTerm();
            Term* termCopy = new Term( 0, 0 ); // Don't need to copy ids.

            // We copy only translations for the currenlty selected languages.
            QStringList languages;
            languages << firstLang << testLang;

            for( QStringList::ConstIterator it = languages.begin(); it != languages.end(); it++ ) {
                const QString& lang = *it;
                if( term->isTranslationExists( lang ) ) {
                    Translation transCopy( term->getTranslation( lang ) );
                    termCopy->addTranslation( transCopy );
                }
            }
            BilingualKey commentKey( firstLang, testLang );
            if( term->isCommentExists( commentKey ) )
                termCopy->addComment( commentKey, term->getComment( commentKey ) );

            if( !term->getImagePath().isNull() ) {
                QDir imagePath( term->getImagePath() );
                if( imagePath.isRelative() ) {
                    TreeItem* currItem = (TreeItem*)vocabTreeView->currentItem();
                    if( currItem && !currItem->isFolder() ) {
                        VocabTreeItem* vocabItem = (VocabTreeItem*)vocabTreeView->currentItem();
                        Vocabulary* vocab = vocabItem->getVocabulary();
                        QString absoluteImagePath = controller->getApplicationDirName() + "/" + vocab->getParent()->getPath() +
                            "/v-" + QString::number( vocab->getId() ) + "/" + term->getImagePath();
                        termCopy->setImagePath( absoluteImagePath );
                    }
                }
                else 
                    termCopy->setImagePath( term->getImagePath() );
            }

            termsToCopy.append( *termCopy );
        }
    }

    if( !termsToCopy.isEmpty() ) {
        QByteArray data;
        QDataStream out( &data, QIODevice::WriteOnly );
        out << termsToCopy;
        controller->setClipboardData( QString( "terms" ), qCompress( data ) );
    }
}

void VocabularyManagerFrame::pasteTerms() {
    QByteArray data( qUncompress( controller->getClipboardData() ) );
    QDataStream in( &data, QIODevice::ReadOnly );

    QList<Term> termsToPaste;
    in >> termsToPaste;

    VocabTreeItem* currVocabItem = (VocabTreeItem*)vocabTreeView->currentItem();
    Vocabulary* currVocab = currVocabItem->getVocabulary();
    for( QList<Term>::ConstIterator it = termsToPaste.begin(); it != termsToPaste.end(); it++ ) {
        const Term& term = *it;
        Term newTerm( currVocab->getMaxTermId() + 1, currVocab->getId() );
        for( Term::TranslationMap::ConstIterator it2 = term.translationsBegin(); it2 != term.translationsEnd(); it2++ ) {
            const Translation& trans = *it2;
            newTerm.addTranslation( trans );
        }
        for( Term::CommentMap::ConstIterator it3 = term.commentsBegin(); it3 != term.commentsEnd(); it3++ ) {
            const BilingualKey& key = it3.key();
            const QString& comment = *it3;
            newTerm.addComment( key, comment );
        }
        if( !term.getImagePath().isNull() ) {
            // If the path refers to a file outside toMOTko's directory, we just copy the path.
            // Otherwise, we copy the image as well as the container vocabulary may be moved or deleted ulteriorly.
            if( term.getImagePath().left( controller->getApplicationDirName().length() ) == controller->getApplicationDirName() ) {
                const QString& vocabLocation = controller->getApplicationDirName() + "/" + currVocab->getParent()->getPath() +
                    "/v-" + QString::number( currVocab->getId() ) + "/";
                QFileInfo imageToCopyInfo( term.getImagePath() );
                QString imageFilename = imageToCopyInfo.fileName();
                if( QFile::copy( term.getImagePath(), vocabLocation + imageFilename ) )
                    newTerm.setImagePath( imageFilename ); 
                else
                    cerr << "Could not copy " << qPrintable( term.getImagePath() ) << " to " << qPrintable( vocabLocation + imageFilename ) << endl;
            }
            else 
                newTerm.setImagePath( term.getImagePath() ); 
        }

        currVocab->addTerm( newTerm );
        currVocab->setModificationDate( QDateTime::currentDateTime() );
        currVocab->setDirty( true );
    }
    vocabDetailsPropsPanel->updateCounters();
    updateUi();
}

void VocabularyManagerFrame::pasteVocabulary() {
    QByteArray data( qUncompress( controller->getClipboardData() ) );
    QDataStream in( &data, QIODevice::ReadOnly );

    Vocabulary vocabToPaste;
    in >> vocabToPaste;

    VocabTreeItem* newVocabItem = addVocab( &vocabToPaste );
    vocabTreeView->setCurrentItem( newVocabItem );
}

void VocabularyManagerFrame::pasteFolder() {
    QByteArray data( qUncompress( controller->getClipboardData() ) );
    QDataStream in( &data, QIODevice::ReadOnly );

    Folder folderToPaste;
    QMap<int,Vocabulary> vocabularies;
    in >> folderToPaste;
    in >> vocabularies;

    FolderTreeItem* newFolderItem = addFolder( &folderToPaste, &vocabularies );
    // As the tree updates are disabled when adding new folders recursively,
    // we close the parent node.  When we select the new node, the parent node
    // will open when we call ensureItemVisible().  This way, the tree is updated
    // properly.
    newFolderItem->parent()->setExpanded( false );
    vocabTreeView->setCurrentItem( newFolderItem );
    vocabTreeView->scrollToItem( newFolderItem, QAbstractItemView::PositionAtCenter );
}

void VocabularyManagerFrame::addListeners() {
    connect( folderDetailsFolderTitleLineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( updateCurrentFolderTitle( const QString& ) ) );
    connect( vocabDetailsVocabTitleLineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( updateCurrentVocabTitle( const QString& ) ) );
    connect( termList, SIGNAL( itemChanged( QTreeWidgetItem*, int ) ), this, SLOT( updateTermItemState( QTreeWidgetItem*, int ) ) );
    connect( vocabTreeView, SIGNAL( currentItemChanged( QTreeWidgetItem*, QTreeWidgetItem* ) ), this, SLOT( updateCurrentTreeItem( QTreeWidgetItem*, QTreeWidgetItem* ) ) ); 
    connect( vocabTreeView, SIGNAL( itemChanged( QTreeWidgetItem*, int ) ), this, SLOT( updateTreeItemState( QTreeWidgetItem*, int ) ) );
}

void VocabularyManagerFrame::removeListeners() {
    disconnect( folderDetailsFolderTitleLineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( updateCurrentFolderTitle( const QString& ) ) );
    disconnect( vocabDetailsVocabTitleLineEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( updateCurrentVocabTitle( const QString& ) ) );
    disconnect( termList, SIGNAL( itemChanged( QTreeWidgetItem*, int ) ), this, SLOT( updateTermItemState( QTreeWidgetItem*, int ) ) );
    disconnect( vocabTreeView, SIGNAL( currentItemChanged( QTreeWidgetItem*, QTreeWidgetItem* ) ), this, SLOT( updateCurrentTreeItem( QTreeWidgetItem*, QTreeWidgetItem* ) ) ); 
    disconnect( vocabTreeView, SIGNAL( itemChanged( QTreeWidgetItem*, int ) ), this, SLOT( updateTreeItemState( QTreeWidgetItem*, int ) ) );
}

void VocabularyManagerFrame::maximizeDetailsPanel() {
    action[ ACTION_MAXIMIZE ]->setChecked( true );
    // Set both folder and vocab maximize buttons to the same state because easier to manage.
    folderDetailsFolderMaximizeDetailsButton->setChecked( true );
    vocabDetailsVocabMaximizeDetailsButton->setChecked( true );
    treePanel->hide();
    updateGeometry();
    update();
}

void VocabularyManagerFrame::restoreDetailsPanel() {
    action[ ACTION_MAXIMIZE ]->setChecked( false );
    // Set both folder and vocab maximize buttons to the same state because easier to manage.
    folderDetailsFolderMaximizeDetailsButton->setChecked( false );
    vocabDetailsVocabMaximizeDetailsButton->setChecked( false );
    treePanel->show();
    updateGeometry();
    update();
}

uint VocabularyManagerFrame::getSelectedTermCount() const {
    return( termList->selectedItems().size() );
}

void VocabularyManagerFrame::search() {
    if( !searchDialog ) {
        searchDialog = new SearchDialog( controller, this );
        searchDialog->setDigraphEnabled( controller->getPreferences().isDigraphEnabled() ); 
        connect( searchDialog, SIGNAL( showTermRequested( const TermKey& ) ), this, SLOT( showTerm( const TermKey& ) ) ); 
        connect( searchDialog, SIGNAL( termsRemoved() ), this, SLOT( updateUi() ) );
    }
    searchDialog->updateFonts();
#if defined(WINCE)
    searchDialog->showMaximized();
#elif defined(Q_WS_HILDON)
    searchDialog->showFullScreen();
#else
    searchDialog->show();
#endif
    searchDialog->exec();
}

void VocabularyManagerFrame::showTerm( const TermKey& termKey ) {
    VocabTreeItem* vocabTreeItem = vocabTreeView->getVocabTreeItem( termKey.getVocabId() );
    if( vocabTreeItem ) {
        vocabTreeView->scrollToItem( vocabTreeItem, QAbstractItemView::PositionAtCenter );
        vocabTreeView->setCurrentItem( vocabTreeItem );
        detailsPanel->setCurrentIndex( panelVocabIndex );
        vocabDetailsTabWidget->setCurrentWidget( vocabDetailsTermsPanel );
        for( int i = 0; i < termList->topLevelItemCount(); i++ ) {
            TermListItem* termItem = (TermListItem*)termList->topLevelItem( i );
            int termItemId = termItem->getTerm()->getId();
            if( termItemId == termKey.getTermId() ) {
                termList->clearSelection();
                termList->setCurrentItem( termItem );
                termList->scrollToItem( termItem, QAbstractItemView::PositionAtCenter );
                break;
            }
        }
    }
}

void VocabularyManagerFrame::updateTreeItemState( QTreeWidgetItem* item, int column ) {
    if( column == 0 ) {
        bool isChecked = ( item->checkState( 0 ) != Qt::Unchecked );
        TreeItem* treeItem = (TreeItem*)item;
        if( treeItem->isFolder() ) {
            FolderTreeItem* folderItem = (FolderTreeItem*)treeItem;
            if( folderItem->getFolder()->isMarkedForStudy() != isChecked )
                folderItem->setOn( isChecked );
        }
        else {
            VocabTreeItem* vocabItem = (VocabTreeItem*)treeItem;
            if( vocabItem->getVocabulary()->isMarkedForStudy() != isChecked )
                vocabItem->setOn( isChecked );
        }
    }
}

void VocabularyManagerFrame::updateTermItemState( QTreeWidgetItem* item, int column ) {
    if( column == 0 ) {
        bool isChecked = ( item->checkState( 0 ) != Qt::Unchecked );
        TermListItem* termListItem = (TermListItem*)item;
        if( termListItem->getTerm()->isMarkedForStudy() != isChecked )
            termListItem->setOn( isChecked );
    }
}
