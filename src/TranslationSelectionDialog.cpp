#include "TranslationSelectionDialog.h"

TranslationSelectionDialog::TranslationSelectionDialog( const QString& caption, const QString& message, const QStringList& languages, int selectionMode, Controller* controller, QWidget* parent ) 
    : QDialog( parent ), controller( controller ) {
    init( caption, message, languages, selectionMode );
}

void TranslationSelectionDialog::init( const QString& caption, const QString& message, const QStringList& languages, int selectionMode ) {
    QFont mediumFont( controller->getPreferences().getMediumFont() );

    messageLabel = new QLabel( message );
    messageLabel->setWordWrap( true );

    languageList = new QTreeWidget();
    languageList->setHeaderLabel( tr( "Languages" ) );
    for( QStringList::ConstIterator it = languages.begin(); it != languages.end(); it++ ) {
        QString lang = *it;
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText( 0, QApplication::translate( "QObject", lang.toLatin1().data() ) );
        languageList->addTopLevelItem( item );
        item->setCheckState( 0, Qt::Unchecked );
        switch( selectionMode ) {
            case selectionModeTargetLanguage : 
                if( lang == controller->getPreferences().getTestLanguage() )
                    item->setCheckState( 0, Qt::Checked );
                break;
            case selectionModeStudyLanguages :
                if( controller->getPreferences().isStudyLanguage( lang ) )
                    item->setCheckState( 0, Qt::Checked );
                break;
            case selectionModeAllLanguages :
                item->setCheckState( 0, Qt::Checked );
                break;
        }
        languageList->addTopLevelItem( item );
    }
    checkAllLanguagesButton = new QPushButton( tr( "CheckAllLanguages" ) );
    connect( checkAllLanguagesButton, SIGNAL( clicked() ), this, SLOT( checkAllLanguages() ) );

    bottomButtonsPanel = new QWidget();
    bottomButtonsPanelLayout = new QHBoxLayout();
    bottomButtonsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    bottomButtonsPanel->setLayout( bottomButtonsPanelLayout );

    acceptButton = new QPushButton( tr( "Ok" ) );
    connect( acceptButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    cancelButton = new QPushButton( tr( "Cancel" ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    bottomButtonsPanelLayout->addStretch();
    bottomButtonsPanelLayout->addWidget( acceptButton );
    bottomButtonsPanelLayout->addWidget( cancelButton );

    mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( messageLabel );
    mainLayout->addWidget( languageList, 1 );
    mainLayout->addWidget( checkAllLanguagesButton );
    mainLayout->addWidget( bottomButtonsPanel );

    setWindowTitle( caption );
}

TranslationSelectionDialog::~TranslationSelectionDialog() {
}

QStringList TranslationSelectionDialog::getSelectedLanguages() {
    QStringList selectedLanguages;
    for( int i = 0; i < languageList->topLevelItemCount(); i++ ) {
        QTreeWidgetItem* item = languageList->topLevelItem( i );
        if( item->checkState( 0 ) == Qt::Checked )
            selectedLanguages.append( Util::getLanguageCode( item->text( 0 ) ) );
    }
    return( selectedLanguages );
}

void TranslationSelectionDialog::checkAllLanguages() const {
    for( int i = 0; i < languageList->topLevelItemCount(); i++ ) {
        QTreeWidgetItem* item = languageList->topLevelItem( i );
        item->setCheckState( 0, Qt::Checked );
    }
}
