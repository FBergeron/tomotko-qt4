#include "TermDialog.h"
#include "icons/editcut.xpm"
#include "icons/editcopy.xpm"
#include "icons/editpaste.xpm"

TermDialog::TermDialog( Vocabulary& vocab, Controller* controller, QWidget* parent ) 
    : QDialog( parent ), vocab( vocab ), controller( controller ), editedTerm( new Term( vocab.getMaxTermId() + 1, vocab.getId() ) ), pixmap( NULL ), movie( NULL ) {
    init();
}

TermDialog::TermDialog( Vocabulary& vocab, Controller* controller, QWidget* parent, const Term& term ) 
    : QDialog( parent ), vocab( vocab ), controller( controller ), editedTerm( new Term( term ) ), pixmap( NULL ), movie( NULL ) {
    init();
}

void TermDialog::init() {
    setWindowFlags( Qt::Window | Qt::WindowMaximizeButtonHint );
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
    topPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    topPanel->setLayout( topPanelLayout );

    topLeftPanel = new QWidget();
    topLeftPanel->setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum ) ); 
    topLeftPanelLayout = new QVBoxLayout();
    topLeftPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    topLeftPanel->setLayout( topLeftPanelLayout );
    topPanelLayout->addWidget( topLeftPanel, 1 );

    firstLangPanel = new QGroupBox( QApplication::translate( "QObject", firstLang.toLatin1().data() ) );
    firstLangPanelLayout = new QHBoxLayout();
    firstLangPanelLayout->setContentsMargins( 0, 0, 0, 0 ); 
    firstLangPanel->setLayout( firstLangPanelLayout );
    topLeftPanelLayout->addWidget( firstLangPanel );

    testLangPanel = new QGroupBox( QApplication::translate( "QObject", testLang.toLatin1().data() ) );
    testLangPanelLayout = new QVBoxLayout();
    testLangPanelLayout->setContentsMargins( 0, 0, 0, 0 );
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
    testLangLabelsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    testLangLabelsPanel->setLayout( testLangLabelsPanelLayout );

    testLangTermAltLabel = new QLabel( tr( "Alt./Phon." ) );
    testLangLabelsPanelLayout->addWidget( testLangTermAltLabel );
    testLangTermLabel = new QLabel( tr( "Word/Expr." ) );
    testLangLabelsPanelLayout->addWidget( testLangTermLabel );

    testLangFieldsPanel = new QWidget();
    testLangFieldsPanelLayout = new QVBoxLayout();
    testLangFieldsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
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

    commentBox = new QWidget();
    commentBoxLayout = new QVBoxLayout();
    commentBoxLayout->setContentsMargins( 0, 0, 0, 0 );
    commentBox->setLayout( commentBoxLayout );

    commentLabel = new QLabel( tr( "Examples/Comments" ) );
    commentMultiLineEdit = new DigraphMultiLineEdit();
    commentMultiLineEdit->setFont( prefs.getBestFont( firstLang, testLang ) );
    commentMultiLineEdit->setDigraphEnabled( isDigraphEnabled );
    commentBoxLayout->addWidget( commentLabel );
    commentBoxLayout->addWidget( commentMultiLineEdit, 1 );

    imageBox = new QGroupBox( tr( "Image" ) );
    imageBoxLayout = new QVBoxLayout();
    imageBox->setLayout( imageBoxLayout );
    topPanelLayout->addWidget( imageBox );

    image = new QLabel();
    image->setAlignment( Qt::AlignCenter );

    imageButtonsPanel = new QWidget();
    imageButtonsPanelLayout = new QHBoxLayout();
    imageButtonsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    imageButtonsPanel->setLayout( imageButtonsPanelLayout );
    setImageButton = new QPushButton( tr( "setImage" ) );
    imageButtonsPanelLayout->addWidget( setImageButton );
    setImageButton->setToolTip( tr( "setImageTooltip" ) );
    connect( setImageButton, SIGNAL( clicked() ), this, SLOT( setImage() ) );
    clearImageButton = new QPushButton( tr( "clearImage" ) );
    imageButtonsPanelLayout->addWidget( clearImageButton );
    clearImageButton->setToolTip( tr( "clearImageTooltip" ) );
    connect( clearImageButton, SIGNAL( clicked() ), this, SLOT( clearImage() ) );

    imageBoxLayout->addWidget( image, 1 );
    imageBoxLayout->addWidget( imageButtonsPanel );

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

    mainLayout->setMenuBar( menuBar );
    mainLayout->addWidget( topPanel );
    mainLayout->addWidget( commentBox, 1 );
    mainLayout->addWidget( bottomButtonsPanel );
    mainLayout->activate();

    setWindowTitle( tr( "EditTerm" ) );

    updateUi();
}

TermDialog::~TermDialog() {
    if( pixmap ) {
        delete( pixmap );
        pixmap = NULL;
    }
    if( movie ) {
        delete( movie );
        movie = NULL;
    }
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
    QString vocabLocation = controller->getApplicationDirName() + "/" + vocab.getParent()->getPath() +
        "/v-" + QString::number( vocab.getId() ) + "/";
    QString imagePath = tempImagePath.left( vocabLocation.length() ) == vocabLocation ? 
            tempImagePath.right( tempImagePath.length() - vocabLocation.length() ) : tempImagePath;
    editedTerm->setImagePath( imagePath );
}

void TermDialog::cut() {
    QWidget* widget = qApp->focusWidget();
    if( widget ) {
        if( widget->inherits( "DigraphLineEdit" ) )
            ((DigraphLineEdit*)widget)->cut();
        else if( widget->inherits( "DigraphMultiLineEdit" ) )
            ((DigraphMultiLineEdit*)widget)->cut();
    }
}

void TermDialog::copy() {
    QWidget* widget = qApp->focusWidget();
    if( widget != NULL ) {
        if( widget->inherits( "DigraphLineEdit" ) )
            ((DigraphLineEdit*)widget)->copy();
        else if( widget->inherits( "DigraphMultiLineEdit" ) )
            ((DigraphMultiLineEdit*)widget)->copy();
    }
}

void TermDialog::paste() {
    QWidget* widget = qApp->focusWidget();
    if( widget != NULL ) {
        if( widget->inherits( "DigraphLineEdit" ) )
            ((DigraphLineEdit*)widget)->paste();
        else if( widget->inherits( "DigraphMultiLineEdit" ) )
            ((DigraphMultiLineEdit*)widget)->paste();
    }
}

void TermDialog::setImage() {
    QDir dir = QDir::home();
    if( !tempImagePath.isEmpty() )
        dir = QFileInfo( tempImagePath ).dir();

    QString imageFile = QFileDialog::getOpenFileName( this, tr( "SetImage..." ), dir.path(), tr( "Images (*.png *.gif)" ) );
    if( !imageFile.isEmpty() ) {
        initImage( imageFile );
        resizeImageBox();
    }
}

void TermDialog::clearImage() {
    image->clear();
    tempImagePath = QString::null;
    if( pixmap ) {
        delete( pixmap );
        pixmap = NULL;
    }
    if( movie ) {
        delete( movie );
        movie = NULL;
    }
}

void TermDialog::initImage( const QString& path ) {
    clearImage();
    if( !path.isNull() ) {
        QFileInfo info( path );
        if( info.exists() ) {
            QByteArray imageFormat = QImageReader::imageFormat( path );
            if( imageFormat == "gif" || imageFormat == "png" ) {
                tempImagePath = path;
                // Even for animated gif, we create a pixmap.  
                // It will be used to determine the size of the movie.
                pixmap = new QPixmap( path );
                if( pixmap && pixmap->isNull() ) {
                    QMessageBox::warning( this, QObject::tr( "Error" ), tr( "CannotReadImage" ) );
                    pixmap = NULL;
                    return;
                }
                if( imageFormat == "gif" )
                    movie = new QMovie( path );
            }
        }
    }
}

void TermDialog::resizeEvent( QResizeEvent* /*evt*/ ) {
    resizeImageBox();
}

void TermDialog::resizeImageBox() const {
    imageBox->setMaximumHeight( topLeftPanel->sizeHint().height() );
    imageBoxLayout->activate();
    if( movie || pixmap ) {
        int left, top, right, bottom;
        imageBoxLayout->getContentsMargins( &left, &top, &right, &bottom );
        int imageHeight = imageBoxLayout->contentsRect().height() - top - bottom - imageButtonsPanel->sizeHint().height();
        if( movie ) {
            // Use the pixmap to compute the scaled size.
            int proportionalWidth = imageHeight * pixmap->width() / pixmap->height(); 
            movie->setScaledSize( QSize( proportionalWidth, imageHeight ) ); 
            image->setMovie( movie );
            movie->start();
        }
        else if( pixmap ) {
            QPixmap scaledPixmap( pixmap->scaledToHeight( imageHeight, Qt::SmoothTransformation ) );
            image->setPixmap( scaledPixmap );
        }
    }
}

const Term& TermDialog::getTerm() {
    updateModel();
    return( *editedTerm );
}

QSize TermDialog::sizeHint() const {
    return( QSize( 640, 480 ) );
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
            if( editedTerm->isCommentExists( commentKey ) )
                commentMultiLineEdit->setText( editedTerm->getComment( commentKey ) );
        }

        QString absPath = controller->getResolvedImagePath( editedTerm->getImagePath(), vocab );
        initImage( absPath );
    }
}
