#include "SearchDialog.h"
#include "icons/editTerm.xpm"
#include "icons/goVocab.xpm"
#include "icons/removeTerm.xpm"
#include "icons/resetQueryForm.xpm"
#include "icons/search.xpm"

SearchDialog::SearchDialog( Controller* controller, QWidget* parent /* = 0 */ ) : QDialog( parent ), controller( controller ) {
    init();
}

SearchDialog::~SearchDialog() {
}

void SearchDialog::init() {
    setWindowFlags( Qt::Window | Qt::WindowMaximizeButtonHint );
    setModal( true );
    queryPanel = new QWidget();
    queryPanelLayout = new QHBoxLayout();
    queryPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    queryPanel->setLayout( queryPanelLayout );

    resetButton = new QPushButton();
    resetButton->setIcon( QIcon( QPixmap( ZPIXMAP( resetQueryForm_xpm ) ) ) );
    resetButton->setToolTip( tr( "Reset" ) );
    connect( resetButton, SIGNAL( clicked() ), this, SLOT( reset() ) );
    queryField = new HistoryField();
    queryField->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
    searchButton = new QPushButton( /*tr( "Search" ),*/ this ); // Passing this inits autoDefault property.
    searchButton->setIcon( QIcon( ZPIXMAP( search_xpm ) ) );
    searchButton->setIconSize( QSize( 32, 32 ) );
    searchButton->setToolTip( tr( "Search" ) );
    searchButton->setDefault( true );
    connect( searchButton, SIGNAL( clicked() ), queryField->lineEdit(), SIGNAL( returnPressed() ) );
    connect( searchButton, SIGNAL( clicked() ), this, SLOT( search() ) );

    queryPanelLayout->addWidget( resetButton );
    queryPanelLayout->addWidget( queryField );
    queryPanelLayout->addWidget( searchButton );

    int prefHeight = resetButton->sizeHint().height();
    queryField->setMaximumHeight( prefHeight );

    resultsHeaderPanel = new QWidget();
    resultsHeaderPanelLayout = new QHBoxLayout();
    resultsHeaderPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    resultsHeaderPanel->setLayout( resultsHeaderPanelLayout );
    resultsHeaderLabel = new QLabel( tr( "Results" ) );
    resultsCounterLabel = new QLabel();
    resultsCounterLabel->setAlignment( Qt::AlignRight );
    resultsHeaderPanelLayout->addWidget( resultsHeaderLabel );
    resultsHeaderPanelLayout->addStretch();
    resultsHeaderPanelLayout->addWidget( resultsCounterLabel );

    resultsListView = new QTreeWidget();
    resultsListView->setColumnCount( 4 );
    resultsListView->setSortingEnabled( true );
    resultsListView->header()->setMovable( false );
    QStringList headerLabels;
    headerLabels << QApplication::translate( "QObject", controller->getPreferences().getFirstLanguage().toLatin1().data() );
    headerLabels << QApplication::translate( "QObject", controller->getPreferences().getTestLanguage().toLatin1().data() );
    headerLabels << tr( "Glossary" ) << tr( "Location" );
    resultsListView->setHeaderLabels( headerLabels );
    resultsListView->header()->setResizeMode( QHeaderView::Stretch );
    resultsListView->setAllColumnsShowFocus( true );
    resultsListView->setSelectionMode( QAbstractItemView::ExtendedSelection );
    connect( resultsListView, SIGNAL( itemSelectionChanged() ), this, SLOT( updateUi() ) );

    resultsButtonsPanel = new QWidget();
    resultsButtonsPanelLayout = new QHBoxLayout();
    resultsButtonsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    resultsButtonsPanel->setLayout( resultsButtonsPanelLayout );
    goResultVocabButton = new QPushButton( tr( "View Glossary" ) );
    goResultVocabButton->setIcon( QIcon( ZPIXMAP( goVocab_xpm ) ) );
    goResultVocabButton->setIconSize( QSize( 32, 32 ) );
    goResultVocabButton->setToolTip( tr( "View Glossary" ) );
    connect( goResultVocabButton, SIGNAL( clicked() ), this, SLOT( goResultVocab() ) );
    editResultTermButton = new QPushButton( tr( "Edit Term" ) );
    editResultTermButton->setIcon( QIcon( ZPIXMAP( editTerm_xpm ) ) );
    editResultTermButton->setIconSize( QSize( 32, 32 ) );
    editResultTermButton->setToolTip( tr( "Edit Term" ) );
    connect( editResultTermButton, SIGNAL( clicked() ), this, SLOT( editResultTerm() ) );
    removeResultTermButton = new QPushButton( tr( "Remove Term(s)" ) );
    removeResultTermButton->setIcon( QIcon( ZPIXMAP( removeTerm_xpm ) ) );
    removeResultTermButton->setIconSize( QSize( 32, 32 ) );
    removeResultTermButton->setToolTip( tr( "Remove Term(s)" ) );
    connect( removeResultTermButton, SIGNAL( clicked() ), this, SLOT( removeResultTerms() ) );

    resultsButtonsPanelLayout->addWidget( goResultVocabButton );
    resultsButtonsPanelLayout->addWidget( editResultTermButton );
    resultsButtonsPanelLayout->addWidget( removeResultTermButton );

    prefHeight = removeResultTermButton->sizeHint().height();
    goResultVocabButton->setMaximumHeight( prefHeight );
    editResultTermButton->setMaximumHeight( prefHeight );

    mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( queryPanel );
    mainLayout->addWidget( resultsHeaderPanel );
    mainLayout->addWidget( resultsListView, 1 );
    mainLayout->addWidget( resultsButtonsPanel );
#if defined(Q_WS_HILDON)
    windowButtonsPanel = new QWidget();
    windowButtonsPanelLayout = new QHBoxLayout();
    windowButtonsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    windowButtonsPanel->setLayout( windowButtonsPanelLayout );
    closeButton = new QPushButton( tr( "Close" ) );
    connect( closeButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    windowButtonsPanelLayout->addStretch( 0 );
    windowButtonsPanelLayout->addWidget( closeButton );
    mainLayout->addWidget( windowButtonsPanel );
#endif

    setWindowTitle( tr( "Search..." ) );

    updateFonts();
    updateUi();
}

void SearchDialog::show() {
    Preferences& prefs = controller->getPreferences();
    if( Util::getLanguageCode( resultsListView->headerItem()->text( 0 ) ) != prefs.getFirstLanguage() ||
        Util::getLanguageCode( resultsListView->headerItem()->text( 1 ) ) != prefs.getTestLanguage() ) {
        resultsListView->headerItem()->setText( 0, QApplication::translate( "QObject", prefs.getFirstLanguage().toLatin1().data() ) );
        resultsListView->headerItem()->setText( 1, QApplication::translate( "QObject", prefs.getTestLanguage().toLatin1().data() ) );
        reset();
    }
    QDialog::show();
    queryField->setFocus();
}

void SearchDialog::updateFonts() {
    QFont mediumFont( controller->getPreferences().getMediumFont() );
    QFont labelsFont( controller->getPreferences().getLabelsFont() );

    queryField->setFont( mediumFont );
    searchButton->setFont( labelsFont );
    resetButton->setFont( labelsFont );

    resultsHeaderPanel->setFont( labelsFont );
    resultsCounterLabel->setFont( labelsFont );

    QFont firstLangFont = controller->getPreferences().getMediumFont( controller->getPreferences().getFirstLanguage() );
    QFont testLangFont = controller->getPreferences().getMediumFont( controller->getPreferences().getTestLanguage() );
    resultsListView->setFont( mediumFont );
    resultsListView->header()->setFont( labelsFont );
    for( int i = 0; i < resultsListView->topLevelItemCount(); i++ ) {
        ResultListItem* item = (ResultListItem*)resultsListView->topLevelItem( i );
        item->setFont( 0, firstLangFont );
        item->setFont( 1, testLangFont );
    }

    goResultVocabButton->setFont( labelsFont );
    editResultTermButton->setFont( labelsFont );
    removeResultTermButton->setFont( labelsFont );
}

void SearchDialog::updateUi() {
    uint selectedTermCount = getSelectedTermCount();
    goResultVocabButton->setEnabled( selectedTermCount == 1 );
    editResultTermButton->setEnabled( selectedTermCount == 1 );
    removeResultTermButton->setEnabled( selectedTermCount > 0 );
}

void SearchDialog::retranslateUi() {
    resetButton->setToolTip( tr( "Reset" ) );
    //searchButton->setText( tr( "Search" ) );
    searchButton->setToolTip( tr( "Search" ) );
    resultsHeaderLabel->setText( tr( "Results" ) );
    resultsCounterLabel->setText( tr( "%1 term(s) found" ).arg( controller->getSearchResultsCount() ) );
    QStringList headerLabels;
    headerLabels << QApplication::translate( "QObject", controller->getPreferences().getFirstLanguage().toLatin1().data() );
    headerLabels << QApplication::translate( "QObject", controller->getPreferences().getTestLanguage().toLatin1().data() );
    headerLabels << tr( "Glossary" ) << tr( "Location" );
    resultsListView->setHeaderLabels( headerLabels );
    goResultVocabButton->setText( tr( "View Glossary" ) );
    goResultVocabButton->setToolTip( tr( "View Glossary" ) );
    editResultTermButton->setText( tr( "Edit Term" ) );
    editResultTermButton->setToolTip( tr( "Edit Term" ) );
    removeResultTermButton->setText( tr( "Remove Term(s)" ) );
    removeResultTermButton->setToolTip( tr( "Remove Term(s)" ) );
    goResultVocabButton->adjustSize();
    editResultTermButton->adjustSize();
    removeResultTermButton->adjustSize();
    setWindowTitle( tr( "Search..." ) );

    updateUi();
}

void SearchDialog::search() {
    const Preferences& prefs = controller->getPreferences();
    QList<TermKey> results = controller->search( queryField->currentText(), prefs.getFirstLanguage(), prefs.getTestLanguage() );
    resultsListView->clear();
    for( QList<TermKey>::ConstIterator it = results.begin(); it != results.end(); it++ ) {
        const TermKey& termKey = *it;
        Term* term = controller->getTerm( termKey );
        Vocabulary* vocab = controller->getVocabTree()->getVocabulary( termKey.getVocabId() );
        if( vocab ) {
            ResultListItem* resultItem = new ResultListItem( resultsListView, term, 
                prefs.getFirstLanguage(), prefs.getTestLanguage(), vocab->getTitle(), vocab->getParent()->getHumanReadablePath(),
                    prefs.isAltInTermListShown() );
            resultItem->setFont( 0, prefs.getMediumFont( prefs.getFirstLanguage() ) );
            resultItem->setFont( 1, prefs.getMediumFont( prefs.getTestLanguage() ) );
        }
    }
    resultsCounterLabel->setText( tr( "%1 term(s) found" ).arg( results.count() ) );
    updateUi();
}

void SearchDialog::reset() {
    controller->clearSearch();
    resultsListView->clear();
    resultsCounterLabel->clear();
    updateUi();
    queryField->clearEditText();
    queryField->setFocus();
}

void SearchDialog::goResultVocab() {
    ResultListItem* currItem = (ResultListItem*)resultsListView->currentItem();
    if( currItem ) {
        Term* term = currItem->getTerm();
        if( term ) {
            emit showTermRequested( TermKey( term->getId(), term->getVocabId() ) );
            close();
        }
    }
}

void SearchDialog::editResultTerm() {
    ResultListItem* currItem = (ResultListItem*)resultsListView->currentItem();
    if( currItem ) {
        Term* term = currItem->getTerm();
        if( term ) {
            Vocabulary* vocab = controller->getVocabTree()->getVocabulary( term->getVocabId() );
            TermDialog dialog( *vocab, controller, this, *term );
#if defined(Q_WS_HILDON)
            dialog.showFullScreen();
#endif
            int result = dialog.exec();
            if( result ) { 
                Term newTerm = dialog.getTerm();
                term->addTranslation( newTerm.getTranslation( controller->getPreferences().getFirstLanguage() ) );
                term->addTranslation( newTerm.getTranslation( controller->getPreferences().getTestLanguage() ) );
                BilingualKey commentKey( controller->getPreferences().getFirstLanguage(), controller->getPreferences().getTestLanguage() );
                term->addComment( commentKey, newTerm.getComment( commentKey ) );
                term->setImagePath( newTerm.getImagePath() );
                currItem->updateUi();
                vocab->setModificationDate( QDateTime::currentDateTime() );
                vocab->setDirty( true );
            }
        }
    }
}

void SearchDialog::removeResultTerms() {
    doRemoveTerms();
}

uint SearchDialog::getSelectedTermCount() const {
    return( resultsListView->selectedItems().count() );
}

void SearchDialog::doRemoveTerms( bool allowSelectTrans /* = true */, bool confirmBeforeRemove /* = true */ ) {
    int selectedItemCount = 0;
    // Find all the translation languages of the selected terms.
    QStringList translationLanguages;

    for( int i = 0; i < resultsListView->topLevelItemCount(); i++ ) {
        ResultListItem* termItem = (ResultListItem*)resultsListView->topLevelItem( i );
        if( termItem->isSelected() ) {
            selectedItemCount++;
            Term* term = termItem->getTerm();
            for( Term::TranslationMap::ConstIterator it = term->translationsBegin(); it != term->translationsEnd(); it++ ) {
                const Translation& trans = *it;
                if( !translationLanguages.contains( trans.getLanguage() ) )
                    translationLanguages.append( trans.getLanguage() );
            }
        }
    }

    if( selectedItemCount == 0 )
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
            for( int i = 0; i < resultsListView->topLevelItemCount(); i++ ) {
                ResultListItem* termItem = (ResultListItem*)resultsListView->topLevelItem( i );
                if( termItem->isSelected() ) {
                    Term* term = termItem->getTerm();
                    Vocabulary* vocab = controller->getVocabTree()->getVocabulary( term->getVocabId() );
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
                    vocab->setModificationDate( QDateTime::currentDateTime() );
                    vocab->setDirty( true );
                }
            }

            resultsListView->clearSelection();
            updateUi();
            emit termsRemoved();
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

        for( int i = 0; i < resultsListView->topLevelItemCount(); i++ ) {
            ResultListItem* termItem = (ResultListItem*)resultsListView->topLevelItem( i );

            if( termItem->isSelected() ) {
                Term* term = termItem->getTerm();
                Vocabulary* vocab = controller->getVocabTree()->getVocabulary( term->getVocabId() );

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
                    vocab->setModificationDate( QDateTime::currentDateTime() );
                    vocab->setDirty( true );
                }
            }
        }

        resultsListView->clearSelection();
        updateUi();
    }
}

QSize SearchDialog::sizeHint() const {
    return( QSize( 700, 500 ) );
}

void SearchDialog::setDigraphEnabled( bool isDigraphEnabled ) {
    queryField->setDigraphEnabled( isDigraphEnabled );
}
