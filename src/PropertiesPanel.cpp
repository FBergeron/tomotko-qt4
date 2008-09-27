#include "PropertiesPanel.h"

PropertiesPanel::PropertiesPanel( const Preferences& prefs, QWidget* parent ) 
    : QWidget( parent ), prefs( prefs ), editedVocab( NULL ), editedFolder( NULL ) {
    init();
}

void PropertiesPanel::setVocabulary( Vocabulary* vocab ) {
    removeListeners();

    editedVocab = vocab;

    mainLayout->setContentsMargins( 8, 8, 8, 8 );
    descriptionMultiLineEdit->setText( editedVocab->getDescription() );
    authorField->setText( editedVocab->getAuthor() );
    creationDateValueLabel->setText( editedVocab->getCreationDate().toString() );
    modificationDateValueLabel->setText( editedVocab->getModificationDate().toString() );

    updateCounters();

    addListeners();
}

void PropertiesPanel::setFolder( Folder* folder ) {
    removeListeners();

    editedFolder = folder;

    mainLayout->setContentsMargins( 0, 0, 0, 0 );
    descriptionMultiLineEdit->setText( editedFolder->getDescription() );
    authorField->setText( editedFolder->getAuthor() );
    creationDateValueLabel->setText( editedFolder->getCreationDate().toString() );
    modificationDateValueLabel->setText( editedFolder->getModificationDate().toString() );

    updateCounters();

    addListeners();
}

void PropertiesPanel::updateCounters() {
    if( getType() == QString( "Vocabulary" ) ) {
        uint termCount = 0;
        uint checkedTermCount = 0;
        uint selectedTermCount = 0;
        if( editedVocab ) {
            bool isReachableFromRoot = editedVocab->isMarkedForStudy() && editedVocab->getParent()->isReachableFromRoot();
            if( prefs.isLanguageFilterEnabled() )
                editedVocab->getItemsCount( &termCount, &checkedTermCount, &selectedTermCount, isReachableFromRoot, 
                    prefs.getFirstLanguage(), prefs.getTestLanguage() );
            else
                editedVocab->getItemsCount( &termCount, &checkedTermCount, &selectedTermCount, isReachableFromRoot );
        }

        contentListView->clear();

        QStringList values;
        values << tr( "Words" ) << QString::number( selectedTermCount ) << QString::number( checkedTermCount ) << QString::number( termCount );
        QTreeWidgetItem* termCountItem = new QTreeWidgetItem( contentListView, values );
        contentListView->addTopLevelItem( termCountItem );
    }
    else if( getType() == QString( "Folder" ) ) {
        uint termCount = 0;
        uint vocabCount = 0;
        uint folderCount = 0;
        uint checkedTermCount = 0;
        uint checkedVocabCount = 0;
        uint checkedFolderCount = 0;
        uint selectedTermCount = 0;
        uint selectedVocabCount = 0;
        uint selectedFolderCount = 0;

        if( editedFolder ) {
            if( prefs.isLanguageFilterEnabled() )
                editedFolder->getItemsCount( &termCount, &vocabCount, &folderCount, 
                    &checkedTermCount, &checkedVocabCount, &checkedFolderCount, 
                        &selectedTermCount, &selectedVocabCount, &selectedFolderCount, 
                            editedFolder->isReachableFromRoot(),
                                prefs.getFirstLanguage(), prefs.getTestLanguage() );
            else
                editedFolder->getItemsCount( &termCount, &vocabCount, &folderCount, 
                    &checkedTermCount, &checkedVocabCount, &checkedFolderCount,
                        &selectedTermCount, &selectedVocabCount, &selectedFolderCount, editedFolder->isReachableFromRoot() );
        }

        contentListView->clear();

        QStringList termCountValues;
        termCountValues << tr( "Words" ) << QString::number( selectedTermCount ) << QString::number( checkedTermCount ) << QString::number( termCount );
        QTreeWidgetItem* termCountItem = new QTreeWidgetItem( contentListView, termCountValues );
        contentListView->addTopLevelItem( termCountItem );

        QStringList vocabCountValues;
        vocabCountValues << tr( "Glossaries" ) << QString::number( selectedVocabCount ) << QString::number( checkedVocabCount ) << QString::number( vocabCount );
        QTreeWidgetItem* vocabCountItem = new QTreeWidgetItem( contentListView, vocabCountValues );
        contentListView->addTopLevelItem( vocabCountItem );

        QStringList folderCountValues;
        folderCountValues << tr( "Folders" ) << QString::number( selectedFolderCount ) << QString::number( checkedFolderCount ) << QString::number( folderCount );
        QTreeWidgetItem* folderCountItem = new QTreeWidgetItem( contentListView, folderCountValues );
        contentListView->addTopLevelItem( folderCountItem );
    }
    contentListView->updateGeometry();
    layout()->invalidate();
}

void PropertiesPanel::init() {
    QFont mediumFont( prefs.getMediumFont() );
    QFont labelsFont( prefs.getLabelsFont() );
    bool isDigraphEnabled( prefs.isDigraphEnabled() );

    mainLayout = new QVBoxLayout();
    setLayout( mainLayout );

    descriptionLabel = new QLabel( tr( "Description" ) );
    descriptionMultiLineEdit = new DigraphMultiLineEdit();
    //descriptionMultiLineEdit->setWrapPolicy( QMultiLineEdit::Anywhere );
    //descriptionMultiLineEdit->setWordWrap( QMultiLineEdit::WidgetWidth );
    descriptionMultiLineEdit->setFont( mediumFont );
    descriptionMultiLineEdit->setDigraphEnabled( isDigraphEnabled );
    //setStretchFactor( descriptionMultiLineEdit, 1 );

    contentLabel = new QLabel( tr( "Content" ) );
    contentListView = new QTreeWidget();//contentListView = new ContentListView( this, "ContentListView" );
    //contentListView->setAllColumnsShowFocus( true );
    contentListView->setSelectionMode( QAbstractItemView::ExtendedSelection );
    QStringList headerLabels;
    headerLabels << tr( "Items" ) << tr( "Selected" ) << tr( "Checked" ) << tr( "Total" );
    contentListView->setHeaderLabels( headerLabels );
    //contentListView->setColumnAlignment( colSelected, QListView::AlignRight );
    //contentListView->setColumnAlignment( colChecked, QListView::AlignRight );
    //contentListView->setColumnAlignment( colTotal, QListView::AlignRight );
    //contentListView->setStretchColumn( 0 );

    simplePropsPanel = new QWidget();
    simplePropsPanelLayout = new QHBoxLayout();
    simplePropsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    simplePropsPanel->setLayout( simplePropsPanelLayout );
    //simplePropsPanel->setSpacing( 10 );

    simplePropsLabelsPanel = new QWidget();
    simplePropsLabelsPanelLayout = new QVBoxLayout();
    simplePropsLabelsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    simplePropsLabelsPanel->setLayout( simplePropsLabelsPanelLayout );
    simplePropsPanelLayout->addWidget( simplePropsLabelsPanel );

    simplePropsFieldsPanel = new QWidget();
    simplePropsFieldsPanelLayout = new QVBoxLayout();
    simplePropsFieldsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    simplePropsFieldsPanel->setLayout( simplePropsFieldsPanelLayout );
    simplePropsPanelLayout->addWidget( simplePropsFieldsPanel );

    authorLabel = new QLabel( tr( "Author" ) );
    simplePropsLabelsPanelLayout->addWidget( authorLabel );
    authorField = new DigraphLineEdit(); 
    simplePropsFieldsPanelLayout->addWidget( authorField );
    authorField->setFont( mediumFont );
    authorField->setDigraphEnabled( isDigraphEnabled );

    creationDateLabel = new QLabel( tr( "CreationDate" ) );
    simplePropsLabelsPanelLayout->addWidget( creationDateLabel );
    creationDateValueLabel = new QLabel();
    simplePropsFieldsPanelLayout->addWidget( creationDateValueLabel );
    creationDateValueLabel->setFont( mediumFont );

    modificationDateLabel = new QLabel( tr( "ModificationDate" ) );
    simplePropsLabelsPanelLayout->addWidget( modificationDateLabel );
    modificationDateValueLabel = new QLabel();
    simplePropsFieldsPanelLayout->addWidget( modificationDateValueLabel );
    modificationDateValueLabel->setFont( mediumFont );

    mainLayout->addWidget( descriptionLabel );
    mainLayout->addWidget( descriptionMultiLineEdit, 1 );
    mainLayout->addWidget( contentLabel );
    mainLayout->addWidget( contentListView );
    mainLayout->addWidget( simplePropsPanel );
}

PropertiesPanel::~PropertiesPanel() {
}

void PropertiesPanel::updateAuthor( const QString& author ) {
    if( getType() == QString( "Vocabulary" ) ) {
        editedVocab->setAuthor( author );
        editedVocab->setModificationDate( QDateTime::currentDateTime() );
        editedVocab->setDirty( true );
    }
    else if( getType() == QString( "Folder" ) ) {
        editedFolder->setAuthor( author );
        editedFolder->setModificationDate( QDateTime::currentDateTime() );
        editedFolder->setDirty( true );
    }
}

void PropertiesPanel::updateDescription() {
    if( getType() == QString( "Vocabulary" ) ) {
        editedVocab->setDescription( descriptionMultiLineEdit->toPlainText() );
        editedVocab->setModificationDate( QDateTime::currentDateTime() );
        editedVocab->setDirty( true );
    }
    else if( getType() == QString( "Folder" ) ) {
        editedFolder->setDescription( descriptionMultiLineEdit->toPlainText() );
        editedFolder->setModificationDate( QDateTime::currentDateTime() );
        editedFolder->setDirty( true );
    }
}

QString PropertiesPanel::getType() const {
    return( editedVocab ? QString( "Vocabulary" ) : QString( "Folder" ) );
}

void PropertiesPanel::updateFonts() {
    QFont mediumFont( prefs.getMediumFont() );
    QFont labelsFont( prefs.getLabelsFont() );

    descriptionLabel->setFont( labelsFont );
    descriptionMultiLineEdit->setFont( mediumFont );
    contentLabel->setFont( labelsFont );
    contentListView->setFont( mediumFont ); 
    authorLabel->setFont( labelsFont );
    authorField->setFont( mediumFont );
    creationDateLabel->setFont( labelsFont );
    creationDateValueLabel->setFont( mediumFont );
    modificationDateLabel->setFont( labelsFont );
    modificationDateValueLabel->setFont( mediumFont );
}

void PropertiesPanel::setDigraphEnabled( bool isEnabled ) {
    descriptionMultiLineEdit->setDigraphEnabled( isEnabled );
    authorField->setDigraphEnabled( isEnabled );
}

void PropertiesPanel::retranslateUi() {
    descriptionLabel->setText( tr( "Description" ) );
    contentLabel->setText( tr( "Content" ) );
    QStringList headerLabels;
    headerLabels << tr( "Items" ) << tr( "Selected" ) << tr( "Checked" ) << tr( "Total" );
    contentListView->setHeaderLabels( headerLabels );
    authorLabel->setText( tr( "Author" ) );
    creationDateLabel->setText( tr( "CreationDate" ) );
    modificationDateLabel->setText( tr( "ModificationDate" ) );
}

void PropertiesPanel::addListeners() {
    connect( authorField, SIGNAL( textChanged( const QString& ) ),
        this, SLOT( updateAuthor( const QString& ) ) );
    connect( descriptionMultiLineEdit, SIGNAL( textChanged() ),
        this, SLOT( updateDescription() ) );
}

void PropertiesPanel::removeListeners() {
    disconnect( authorField, SIGNAL( textChanged( const QString& ) ),
        this, SLOT( updateAuthor( const QString& ) ) );
    disconnect( descriptionMultiLineEdit, SIGNAL( textChanged() ),
        this, SLOT( updateDescription() ) );
}

