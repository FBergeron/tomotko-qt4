#include "TermDialog.h"
#include "icons/editcut.xpm"
#include "icons/editcopy.xpm"
#include "icons/editpaste.xpm"
#include "icons/void.xpm"

TermDialog::TermDialog( Vocabulary& vocab, Controller* controller, QWidget* parent ) 
    : QDialog( parent ), vocab( vocab ), controller( controller ), editedTerm( new Term( vocab.getMaxTermId() + 1, vocab.getId() ) ) {
    init();
}

TermDialog::TermDialog( Vocabulary& vocab, Controller* controller, QWidget* parent, const Term& term ) 
    : QDialog( parent ), vocab( vocab ), controller( controller ), editedTerm( new Term( term ) ) {
    init();
}

void TermDialog::init() {
    setModal( true );

    Preferences& prefs = controller->getPreferences();

    QString firstLang( prefs.getFirstLanguage() );
    QString testLang( prefs.getTestLanguage() );
    bool isDigraphEnabled( prefs.isDigraphEnabled() );

    mainLayout = new QVBoxLayout();
    setLayout( mainLayout );

    menuBar = new QMenuBar( this );

    QMenu* edition = new QMenu( QApplication::translate( "QObject", "Edition" ), this );
    menuBar->addMenu( edition );

    cutAction = Util::createAction( tr( "Cut" ), editcut_xpm, this, SLOT( cut() ), QKeySequence( Qt::CTRL + Qt::Key_X ) );
    connect( cutAction, SIGNAL( triggered() ), this, SLOT( cut() ) );
    edition->addAction( cutAction );

    copyAction = Util::createAction( QApplication::translate( "QObject", "Copy" ), editcopy_xpm, this, SLOT( copy() ), QKeySequence( Qt::CTRL + Qt::Key_C ) );
    connect( copyAction, SIGNAL( triggered() ), this, SLOT( copy() ) );
    edition->addAction( copyAction );

    pasteAction = Util::createAction( QApplication::translate( "QObject", "Paste" ), editpaste_xpm, this, SLOT( paste() ), QKeySequence( Qt::CTRL + Qt::Key_V ) );
    connect( pasteAction, SIGNAL( triggered() ), this, SLOT( paste() ) );
    edition->addAction( pasteAction );

    topPanel = new QWidget();
    topPanelLayout = new QHBoxLayout();
    topPanel->setLayout( topPanelLayout );

    topLeftPanel = new QWidget();
    topLeftPanelLayout = new QVBoxLayout();
    topLeftPanel->setLayout( topLeftPanelLayout );
    topPanelLayout->addWidget( topLeftPanel );

    firstLangPanel = new QGroupBox( QApplication::translate( "QObject", firstLang.toLatin1().data() ) );
    firstLangPanelLayout = new QHBoxLayout();
    firstLangPanel->setLayout( firstLangPanelLayout );
    topLeftPanelLayout->addWidget( firstLangPanel );

    testLangPanel = new QGroupBox( QApplication::translate( "QObject", testLang.toLatin1().data() ) );
    testLangPanelLayout = new QVBoxLayout();
    testLangPanel->setLayout( testLangPanelLayout );
    topLeftPanelLayout->addWidget( testLangPanel );

    firstLangTermPanel = new QWidget();
    firstLangTermPanelLayout = new QHBoxLayout();
    firstLangTermPanel->setLayout( firstLangTermPanelLayout );
    firstLangPanelLayout->addWidget( firstLangTermPanel );

    firstLangTermLabel = new QLabel( tr( "Word/Expr." ) );
    firstLangTermPanelLayout->addWidget( firstLangTermLabel );
    firstLangTermLineEdit = new DigraphLineEdit();
    firstLangTermPanelLayout->addWidget( firstLangTermLineEdit );
    firstLangTermLineEdit->setFont( prefs.getMediumFont( firstLang ) );
    firstLangTermLineEdit->setDigraphEnabled( isDigraphEnabled );

    testLangTopPanel = new QWidget();
    testLangTopPanelLayout = new QHBoxLayout();
    testLangTopPanel->setLayout( testLangTopPanelLayout );
    testLangPanelLayout->addWidget( testLangTopPanel );

    testLangLabelsPanel = new QWidget();
    testLangLabelsPanelLayout = new QVBoxLayout();
    testLangLabelsPanel->setLayout( testLangLabelsPanelLayout );

    testLangTermAltLabel = new QLabel( tr( "Alt./Phon." ) );
    testLangLabelsPanelLayout->addWidget( testLangTermAltLabel );
    testLangTermLabel = new QLabel( tr( "Word/Expr." ) );
    testLangLabelsPanelLayout->addWidget( testLangTermLabel );

    testLangFieldsPanel = new QWidget();
    testLangFieldsPanelLayout = new QVBoxLayout();
    testLangFieldsPanel->setLayout( testLangFieldsPanelLayout );

    testLangTermAltLineEdit = new DigraphLineEdit();
    testLangFieldsPanelLayout->addWidget( testLangTermAltLineEdit );
    testLangTermAltLineEdit->setFont( prefs.getMediumFont( testLang ) );
    testLangTermAltLineEdit->setDigraphEnabled( isDigraphEnabled );
    testLangTermLineEdit = new DigraphLineEdit();
    testLangFieldsPanelLayout->addWidget( testLangTermLineEdit );
    testLangTermLineEdit->setFont( prefs.getLargeFont( testLang ) );
    testLangTermLineEdit->setDigraphEnabled( isDigraphEnabled );
   
    testLangTopPanelLayout->addWidget( testLangLabelsPanel );
    testLangTopPanelLayout->addWidget( testLangFieldsPanel, 1 );

    commentLabel = new QLabel( tr( "Examples/Comments" ) );
    commentMultiLineEdit = new DigraphMultiLineEdit();
    //commentMultiLineEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
    //commentMultiLineEdit->setWrapPolicy( QMultiLineEdit::Anywhere );
    //commentMultiLineEdit->setWordWrap( QMultiLineEdit::WidgetWidth );
    //commentMultiLineEdit->setFont( prefs.getBestFont( firstLang, testLang ) );
    commentMultiLineEdit->setDigraphEnabled( isDigraphEnabled );

    imageBox = new QGroupBox( tr( "Image" ) );
    imageBoxLayout = new QVBoxLayout();
    imageBox->setLayout( imageBoxLayout );
    topPanelLayout->addWidget( imageBox );

    imagePanel = new QWidget();
    imagePanelLayout = new QVBoxLayout();
    imagePanel->setLayout( imagePanelLayout );
    imageBoxLayout->addWidget( imagePanel );
    //imagePanelLayout->setSpacing( 2 );
    imageWrapper = new QWidget();
    imageWrapperLayout = new QVBoxLayout();
    imageWrapper->setLayout( imageWrapperLayout );

    image = new QLabel();
    imageWrapperLayout->addWidget( image );
    image->setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Expanding ) );
    //image->setAlignment( AlignHCenter | AlignVCenter );
    image->setScaledContents( true );

    imageBox->setMaximumHeight( topLeftPanel->sizeHint().height() );

    imageButtonsPanel = new QWidget();
    imageButtonsPanelLayout = new QHBoxLayout();
    imageButtonsPanel->setLayout( imageButtonsPanelLayout );
    setImageButton = new QPushButton( tr( "setImage" ) );
    imageButtonsPanelLayout->addWidget( setImageButton );
    setImageButton->setToolTip( tr( "setImageTooltip" ) );
    connect( setImageButton, SIGNAL( clicked() ), this, SLOT( setImage() ) );
    clearImageButton = new QPushButton( tr( "clearImage" ) );
    imageButtonsPanelLayout->addWidget( clearImageButton );
    clearImageButton->setToolTip( tr( "clearImageTooltip" ) );
    connect( clearImageButton, SIGNAL( clicked() ), this, SLOT( clearImage() ) );
    imagePanelLayout->addWidget( imageWrapper, 1 );
    imagePanelLayout->addWidget( imageButtonsPanel );

    bottomButtonsPanel = new QWidget();
    bottomButtonsPanelLayout = new QHBoxLayout();
    bottomButtonsPanel->setLayout( bottomButtonsPanelLayout );

    acceptButton = new QPushButton( tr( "Ok" ) );
    connect( acceptButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    cancelButton = new QPushButton( tr( "Cancel" ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    bottomButtonsPanelLayout->addStretch();
    bottomButtonsPanelLayout->addWidget( acceptButton );
    bottomButtonsPanelLayout->addWidget( cancelButton );

    //mainLayout->setSpacing( 2 );
    mainLayout->setMenuBar( menuBar );
    mainLayout->addWidget( topPanel );
    mainLayout->addWidget( commentLabel );
    mainLayout->addWidget( commentMultiLineEdit, 1 );
    mainLayout->addWidget( bottomButtonsPanel );
    mainLayout->activate();

    setWindowTitle( tr( "EditTerm" ) );

    updateUi();
}

TermDialog::~TermDialog() {
}

void TermDialog::show() {
    QDialog::show();
    resizeImage();
}

void TermDialog::updateModel() {
    if( !editedTerm->isTranslationExists( controller->getPreferences().getFirstLanguage() ) )
        editedTerm->addTranslation( controller->getPreferences().getFirstLanguage() );
    Translation& firstLangTranslation = editedTerm->getTranslation( controller->getPreferences().getFirstLanguage() );
    firstLangTranslation.setWord( firstLangTermLineEdit->text() );

    if( !editedTerm->isTranslationExists( controller->getPreferences().getTestLanguage() ) )
        editedTerm->addTranslation( controller->getPreferences().getTestLanguage() );

    Translation& testLangTranslation = editedTerm->getTranslation( controller->getPreferences().getTestLanguage() );
    testLangTranslation.setWord( testLangTermLineEdit->text() );
    testLangTranslation.setAlt( testLangTermAltLineEdit->text() );

    BilingualKey commentKey( controller->getPreferences().getFirstLanguage(), controller->getPreferences().getTestLanguage() );
    editedTerm->addComment( commentKey, commentMultiLineEdit->toPlainText() );

    // If the path refers to an image in toMOTko's vocabulary, we use a relative path instead.
    //QString vocabLocation = controller->getApplicationDirName() + "/" + vocab.getParent()->getPath() +
    //    "/v-" + QString::number( vocab.getId() ) + "/";
    //QString imagePath = tempImagePath.left( vocabLocation.length() ) == vocabLocation ? 
    //        tempImagePath.right( tempImagePath.length() - vocabLocation.length() ) : tempImagePath;
    //editedTerm->setImagePath( imagePath );
}

void TermDialog::cut() {
    //QWidget* widget = qApp->focusWidget();
    //if( widget != NULL ) {
    //    const char* className = widget->className();
    //    if( strcmp( className, "DigraphLineEdit" ) == 0 )
    //        ((DigraphLineEdit*)widget)->cut();
    //    else if( strcmp( className, "DigraphMultiLineEdit" ) == 0 )
    //        ((DigraphMultiLineEdit*)widget)->cut();
    //}
}

void TermDialog::copy() {
    //QWidget* widget = qApp->focusWidget();
    //if( widget != NULL ) {
    //    const char* className = widget->className();
    //    if( strcmp( className, "DigraphLineEdit" ) == 0 )
    //        ((DigraphLineEdit*)widget)->copy();
    //    else if( strcmp( className, "DigraphMultiLineEdit" ) == 0 )
    //        ((DigraphMultiLineEdit*)widget)->copy();
    //}
}

void TermDialog::paste() {
    //QWidget* widget = qApp->focusWidget();
    //if( widget != NULL ) {
    //    const char* className = widget->className();
    //    if( strcmp( className, "DigraphLineEdit" ) == 0 )
    //        ((DigraphLineEdit*)widget)->paste();
    //    else if( strcmp( className, "DigraphMultiLineEdit" ) == 0 )
    //        ((DigraphMultiLineEdit*)widget)->paste();
    //}
}

void TermDialog::setImage() {
    QDir dir = QDir::home();//QPEApplication::documentDir();
    if( !tempImagePath.isEmpty() )
        dir = QFileInfo( tempImagePath ).dir();

    QString imageFile = QFileDialog::getOpenFileName( this, tr( "SetImage..." ), dir.path(), tr( "Images (*.png *.gif)" ) );
    if( !imageFile.isEmpty() )
        initImage( imageFile );
}

void TermDialog::clearImage() {
    image->setPixmap( ZPIXMAP( void_xpm ) );
    tempImagePath = QString::null;
    imageFormat = QByteArray();
}

void TermDialog::initImage( const QString& imagePath ) {
    clearImage();
    if( !imagePath.isNull() ) {
        QFileInfo info( imagePath );
        if( info.exists() ) {
            imageFormat = QImageReader::imageFormat( imagePath );
            if( imageFormat == "gif" || imageFormat == "png" ) {
                tempImagePath = imagePath;
                if( imageFormat == "gif" ) {
                    QMovie* movie = new QMovie( imagePath );
                    image->setMovie( movie );
                }
                else if( imageFormat == "png" ) {
                    QPixmap pixmap( imagePath );
                    image->setPixmap( pixmap );
                    resizeImage();
                }
            }
        }
    }
}

void TermDialog::resizeImage() const {
    if( !tempImagePath.isNull() ) {
        if( imageFormat == "gif" ) {
            // Nothing to do for this format.  The default behavior works fine.
        }
        else if( imageFormat == "png" ) {
            int proportionalWidth = image->height() * image->pixmap()->width() / image->pixmap()->height();
            image->setMaximumWidth( proportionalWidth );
        }
    }
}

const Term& TermDialog::getTerm() {
    updateModel();
    return( *editedTerm );
}

void TermDialog::updateUi() {
    if( editedTerm ) {
        if( editedTerm->isTranslationExists( controller->getPreferences().getFirstLanguage() ) ) {
            Translation& firstLangTranslation = editedTerm->getTranslation( controller->getPreferences().getFirstLanguage() );
            firstLangTermLineEdit->setText( firstLangTranslation.getWord() );
            firstLangTermLineEdit->setCursorPosition( 0 );
        }

        if( editedTerm->isTranslationExists( controller->getPreferences().getTestLanguage() ) ) {
            Translation& testLangTranslation = editedTerm->getTranslation( controller->getPreferences().getTestLanguage() );
            testLangTermLineEdit->setText( testLangTranslation.getWord() );
            testLangTermLineEdit->setCursorPosition( 0 );
            testLangTermAltLineEdit->setText( testLangTranslation.getAlt() );
            testLangTermAltLineEdit->setCursorPosition( 0 );
            BilingualKey commentKey( controller->getPreferences().getFirstLanguage(), controller->getPreferences().getTestLanguage() );
            if( editedTerm->isCommentExists( commentKey ) ) {
                commentMultiLineEdit->setText( editedTerm->getComment( commentKey ) );
                //commentMultiLineEdit->setCursorPosition( 0 );
            }
        }

        QString absPath = controller->getResolvedImagePath( editedTerm->getImagePath(), vocab );
        initImage( absPath );
    }
}
