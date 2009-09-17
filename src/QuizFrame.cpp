#include "QuizFrame.h"
#include "icons/eye.xpm"
#include "icons/goodAns.xpm"
#include "icons/badAns.xpm"
#include "icons/editTerm.xpm"
#include "icons/maximize.xpm"

QuizFrame::QuizFrame( Controller* controller, QWidget* parent /*= 0*/ )
    : QWidget( parent ), controller( controller ), pixmap( NULL ), movie( NULL ) {
    init();
}

void QuizFrame::init() {
    Preferences& prefs = controller->getPreferences();
    QFont largeFont = prefs.getLargeFont();
    QFont mediumFont = prefs.getMediumFont();
    QFont labelsFont = prefs.getLabelsFont();
    QString firstLang( controller->getQuizFirstLanguage() );
    QString testLang( controller->getQuizTestLanguage() );

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

    controlPanel = new QWidget();
    controlPanelLayout = new QHBoxLayout();
    controlPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    controlPanel->setLayout( controlPanelLayout );

    answerControlPanel = new QWidget();
    answerControlPanelLayout = new QHBoxLayout();
    answerControlPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    answerControlPanel->setLayout( answerControlPanelLayout );
    controlPanelLayout->addWidget( answerControlPanel );

    revealAllDataButton = new QPushButton( /*tr( "Reveal" )*/ );
    revealAllDataButton->setIcon( QIcon( ZPIXMAP( eye_xpm ) ) );
    revealAllDataButton->setIconSize( QSize( 32, 32 ) );
    revealAllDataButton->setEnabled( false );
    revealAllDataButton->setToolTip( tr( "Reveal" ) );
    connect( revealAllDataButton, SIGNAL( clicked() ), this, SLOT( reveal() ) );

    rightAnswerButton = new QPushButton( /*tr( "RightAnswer" )*/ );
    rightAnswerButton->setIcon( QIcon( ZPIXMAP( goodAns_xpm ) ) ); 
    rightAnswerButton->setIconSize( QSize( 32, 32 ) );
    rightAnswerButton->setEnabled( false );
    rightAnswerButton->setToolTip( tr( "RightAnswer" ) );
    connect( rightAnswerButton, SIGNAL( clicked() ), this, SLOT( rightAnswer() ) );

    wrongAnswerButton = new QPushButton( /*tr( "WrongAnswer" )*/ );
    wrongAnswerButton->setIcon( QIcon( ZPIXMAP( badAns_xpm ) ) );
    wrongAnswerButton->setIconSize( QSize( 32, 32 ) );
    wrongAnswerButton->setEnabled( false );
    wrongAnswerButton->setToolTip( tr( "WrongAnswer" ) );
    connect( wrongAnswerButton, SIGNAL( clicked() ), this, SLOT( wrongAnswer() ) );

    answerControlPanelLayout->addWidget( revealAllDataButton );
    answerControlPanelLayout->addWidget( rightAnswerButton );
    answerControlPanelLayout->addWidget( wrongAnswerButton );

    editionButton = new QPushButton( /*tr( "EditWord" )*/ );
    editionButton->setIcon( QIcon( ZPIXMAP( editTerm_xpm ) ) ); 
    editionButton->setIconSize( QSize( 32, 32 ) );
    editionButton->setMinimumWidth( 100 );
    editionButton->setEnabled( false );
    editionButton->setToolTip( tr( "EditWord" ) );
    connect( editionButton, SIGNAL( clicked() ), this, SLOT( editCurrentTerm() ) );

    controlPanelLayout->addWidget( answerControlPanel, 1 );
    controlPanelLayout->addWidget( editionButton, 0 );
    
    firstLangTermPanel = new QWidget();
    firstLangTermPanelLayout = new QHBoxLayout();
    firstLangTermPanel->setLayout( firstLangTermPanelLayout );
    firstLangPanelLayout->addWidget( firstLangTermPanel );

    firstLangTermLabel = new QLabel( tr( "Word/Expr." ) );

    firstLangTermStack = new QStackedWidget();
    firstLangTermLineEdit = new ScrollableLineEdit();
    firstLangTermLineEdit->setReadOnly( true );
    firstLangTermButton = new QPushButton( tr( "???" ) );
    firstLangTermButton->setToolTip( tr( "Reveal" ) );
    connect( firstLangTermButton, SIGNAL( clicked() ), this, SLOT( revealFirstLangTerm() ) );  
    firstLangTermStack->addWidget( firstLangTermLineEdit );
    firstLangTermStack->addWidget( firstLangTermButton );

    firstLangTermPanelLayout->addWidget( firstLangTermLabel, 0 );
    firstLangTermPanelLayout->addWidget( firstLangTermStack, 1 );
    
    testLangTopPanel = new QWidget();
    testLangPanelLayout->addWidget( testLangTopPanel );
    testLangTopPanelLayout = new QHBoxLayout();
    testLangTopPanel->setLayout( testLangTopPanelLayout );

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
    testLangTermAltStack = new QStackedWidget();
    testLangFieldsPanelLayout->addWidget( testLangTermAltStack );
    testLangTermAltLineEdit = new ScrollableLineEdit();
    testLangTermAltLineEdit->setReadOnly( true );
    testLangTermAltButton = new QPushButton( tr( "???" ) );
    testLangTermAltButton->setToolTip( tr( "Reveal" ) );
    connect( testLangTermAltButton, SIGNAL( clicked() ), this, SLOT( revealAltTerm() ) );  
    testLangTermAltStack->addWidget( testLangTermAltLineEdit );
    testLangTermAltStack->addWidget( testLangTermAltButton );

    testLangTermStack = new QStackedWidget();
    testLangFieldsPanelLayout->addWidget( testLangTermStack );
    testLangTermLineEdit = new ScrollableLineEdit();
    testLangTermLineEdit->setReadOnly( true );
    testLangTermButton = new QPushButton( tr( "???" ) );
    testLangTermButton->setToolTip( tr( "Reveal" ) );
    connect( testLangTermButton, SIGNAL( clicked() ), this, SLOT( revealTestLangTerm() ) );  

    testLangTermStack->addWidget( testLangTermLineEdit );
    testLangTermStack->addWidget( testLangTermButton );

    testLangTopPanelLayout->addWidget( testLangLabelsPanel );
    testLangTopPanelLayout->addWidget( testLangFieldsPanel, 1 );

    commentBox = new QWidget();
    commentBoxLayout = new QVBoxLayout();
    commentBoxLayout->setContentsMargins( 0, 0, 0, 0 );
    commentBox->setLayout( commentBoxLayout );

    commentLabelPanel = new QWidget();
    commentLabelPanelLayout = new QHBoxLayout();
    commentLabelPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    commentLabelPanel->setLayout( commentLabelPanelLayout );
    commentBoxLayout->addWidget( commentLabelPanel );
    commentLabel = new QLabel( tr( "Examples/Comments" ) );
    maximizeCommentButton = new QPushButton();
    maximizeCommentButton->setIcon( QIcon( ZPIXMAP( maximize_xpm ) ) );
    maximizeCommentButton->setIconSize( QSize( 32, 32 ) );
    maximizeCommentButton->setCheckable( true );
    maximizeCommentButton->setMaximumHeight( commentLabel->sizeHint().height() > 24 ? commentLabel->sizeHint().height() : 24 );
    maximizeCommentButton->setMaximumWidth( maximizeCommentButton->maximumHeight() );
    maximizeCommentButton->setToolTip( tr( "MaximizeComment" ) );
    connect( maximizeCommentButton, SIGNAL( toggled( bool ) ), this, SLOT( toggleMaximizeComment( bool ) ) );
    commentLabelPanelLayout->addWidget( commentLabel );
    commentLabelPanelLayout->addStretch();
    commentLabelPanelLayout->addWidget( maximizeCommentButton );

    commentStack = new QStackedWidget();
    commentBoxLayout->addWidget( commentStack );

    commentMultiLineEdit = new ScrollableMultiLineEdit();
    commentMultiLineEdit->setReadOnly( true );
    commentButton = new QPushButton( tr( "???" ) );
    commentButton->setToolTip( tr( "Reveal" ) );
    connect( commentButton, SIGNAL( clicked() ), this, SLOT( revealComment() ) );  
    commentStack->addWidget( commentMultiLineEdit );
    commentStack->addWidget( commentButton );

    imageBox = new QGroupBox( tr( "Image" ) );
    imageBoxLayout = new QVBoxLayout();
    imageBox->setLayout( imageBoxLayout );

    imageStack = new QStackedWidget();
    imageBoxLayout->addWidget( imageStack );
    imageButton = new QPushButton( tr( "???" ) );
    imageButton->setToolTip( tr( "Reveal" ) );
    connect( imageButton, SIGNAL( clicked() ), this, SLOT( revealImage() ) );
    image = new QLabel();
    imageStack->addWidget( image );
    imageStack->addWidget( imageButton );

    topPanelLayout->addWidget( imageBox );

    scrollLeftAction = new QAction( this );
    scrollLeftAction->setShortcut( QKeySequence( QKeySequence::MoveToPreviousChar ) );
    connect( scrollLeftAction, SIGNAL( triggered() ), this, SLOT( scrollLeft() ) );
    addAction( scrollLeftAction );

    scrollRightAction = new QAction( this );
    scrollRightAction->setShortcut( QKeySequence( QKeySequence::MoveToNextChar ) );
    connect( scrollRightAction, SIGNAL( triggered() ), this, SLOT( scrollRight() ) );
    addAction( scrollRightAction );

    scrollUpAction = new QAction( this );
    scrollUpAction->setShortcut( QKeySequence( QKeySequence::MoveToPreviousLine ) );
    connect( scrollUpAction, SIGNAL( triggered() ), this, SLOT( scrollUp() ) );
    addAction( scrollUpAction );

    scrollDownAction = new QAction( this );
    scrollDownAction->setShortcut( QKeySequence( QKeySequence::MoveToNextLine ) );
    connect( scrollDownAction, SIGNAL( triggered() ), this, SLOT( scrollDown() ) );
    addAction( scrollDownAction );

    scrollPageUpAction = new QAction( this );
    scrollPageUpAction->setShortcut( QKeySequence( QKeySequence::MoveToPreviousPage ) );
    connect( scrollPageUpAction, SIGNAL( triggered() ), this, SLOT( scrollPageUp() ) );
    addAction( scrollPageUpAction );

    scrollPageDownAction = new QAction( this );
    scrollPageDownAction->setShortcut( QKeySequence( QKeySequence::MoveToNextPage ) );
    connect( scrollPageDownAction, SIGNAL( triggered() ), this, SLOT( scrollPageDown() ) );
    addAction( scrollPageDownAction );

    mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( topPanel );
    mainLayout->addWidget( commentBox, 1 );
    mainLayout->addWidget( controlPanel );
    setLayout( mainLayout );
    mainLayout->activate();

    updateFonts();

    setButtonsHidden( prefs.areQuizButtonsHidden() );
}

QuizFrame::~QuizFrame() {
    if( pixmap ) {
        delete( pixmap );
        pixmap = NULL;
    }
    if( movie ) {
        delete( movie );
        movie = NULL;
    }
}

void QuizFrame::startQuiz() {
    setButtonsEnabled( false );
    controller->startQuiz();
    controller->prepareQuiz();
    updateLanguageLabels();
    updateFonts();

    if( !controller->isQuizInProgress() ) {
        QMessageBox::warning( this, QObject::tr( "Information" ), tr( "NoTermsMarkedForStudy" ) );
        return;
    }

    askNextTerm();
}

void QuizFrame::restartQuiz() {
    setButtonsEnabled( false );
    controller->restartQuiz();
    controller->prepareQuiz();
    updateLanguageLabels();
    updateFonts();

    if( !controller->isQuizInProgress() ) {
        QMessageBox::warning( this, QObject::tr( "Information" ), tr( "NoTermsMarkedForStudy" ) );
        return;
    }

    askNextTerm();
}

void QuizFrame::resumeQuiz() {
    controller->resumeQuiz();
    controller->prepareQuiz();
    updateLanguageLabels();
    updateFonts();
    askCurrentTerm();
}

void QuizFrame::setTerm( const Term& term ) {
    Translation firstLangTranslation = term.getTranslation( controller->getQuizFirstLanguage() );
    Translation testLangTranslation = term.getTranslation( controller->getQuizTestLanguage() );

    const QString firstLangWord = firstLangTranslation.getWord();
    const QString testLangAlt = testLangTranslation.getAlt();
    const QString testLangWord = testLangTranslation.getWord();

    QString comment;
    BilingualKey commentKey( controller->getQuizFirstLanguage(), controller->getQuizTestLanguage() );
    if( term.isCommentExists( commentKey ) )
        comment = term.getComment( commentKey );

    firstLangTermLineEdit->setText( firstLangWord );
    firstLangTermLineEdit->setCursorPosition( 0 );

    testLangTermAltLineEdit->setText( testLangAlt );
    testLangTermAltLineEdit->setCursorPosition( 0 );

    testLangTermLineEdit->setText( testLangWord );
    testLangTermLineEdit->setCursorPosition( 0 );

    commentMultiLineEdit->setText( comment );

    Folder* vocabTree = controller->getVocabTree();
    Vocabulary* vocab = vocabTree->getVocabulary( term.getVocabId() );
    if( vocab ) {
        QString absPath = controller->getResolvedImagePath( term.getImagePath(), *vocab );
        setImage( absPath ); 
    }
}

bool QuizFrame::containsValidImage() const {
    return( pixmap );
}

void QuizFrame::clearImage() {
    if( pixmap ) {
        delete( pixmap );
        pixmap = NULL;
    }
    if( movie ) {
        delete( movie );
        movie = NULL;
    }
    image->clear();
}

void QuizFrame::setImage( const QString& path ) {
    clearImage();
    if( path.isNull() )
        imageBox->setVisible( false );
    else {
        QFileInfo info( path );
        if( info.exists() ) {
            QByteArray imageFormat = QImageReader::imageFormat( path );
            if( imageFormat == "gif" || imageFormat == "png" ) {
                // Even for animated gif, we create a pixmap.  
                // It will be used to determine the size of the movie.
                pixmap = new QPixmap( path );
                if( pixmap && pixmap->isNull() ) {
                    // For some reasons, the image cannot be read, so ignore it.
                    pixmap = NULL;
                    return;
                }
                if( imageFormat == "gif" )
                    movie = new QMovie( path );
                resizeImageBox();
                imageBox->setVisible( true );
            }
        }
    }
}

void QuizFrame::resizeImageBox() {
    imageBox->resize( imageBox->width(), topLeftPanel->height() );
    if( movie ) {
        // Use the pixmap to compute the scaled size.
        int proportionalWidth = image->height() * pixmap->width() / pixmap->height();
        movie->setScaledSize( QSize( proportionalWidth, image->height() ) );
        image->setMovie( movie );
        movie->start();
    }
    else if( pixmap ) {
        QPixmap scaledPixmap( pixmap->scaledToHeight( image->height(), Qt::SmoothTransformation ) );
        image->setPixmap( scaledPixmap  ); 
    }
    imageStack->updateGeometry(); // To resize the imageButton.
}

void QuizFrame::concludeQuiz() {
    setButtonsEnabled( false );
    QMessageBox::information( this, QObject::tr( "Information" ), tr( "WellDone" ).arg( controller->getInitialTermCount() ) );
}

Term* QuizFrame::askCurrentTerm() {
    restoreCommentField();

    Term* currTerm = controller->getCurrentTerm();
    if( currTerm )
        askTerm( *currTerm );

    return( currTerm );
}

Term* QuizFrame::askNextTerm() {
    Term* nextTerm = NULL;
    if( controller->hasNextTerm() )
        nextTerm = controller->getNextTerm(); // Can return NULL if the term has been deleted meanwhile.
       
    if( nextTerm ) {
        restoreCommentField();
        askTerm( *nextTerm );
        return( nextTerm );
    }
    else {
        concludeQuiz();
        return( NULL );
    }
}

void QuizFrame::askTerm( const Term& term ) {
    hideAnswers();
    controller->initRevealingSequence();
    setTerm( term );
    reveal();
    setButtonsEnabled( true );
}

void QuizFrame::updateLanguageLabels() {
    firstLangPanel->setTitle( QApplication::translate( "QObject", controller->getQuizFirstLanguage().toLatin1().data() ) );
    testLangPanel->setTitle( QApplication::translate( "QObject", controller->getQuizTestLanguage().toLatin1().data() ) );
}

void QuizFrame::setButtonsHidden( bool areHidden ) {
    if( areHidden ) {
        controlPanel->hide();
        maximizeCommentButton->hide();
        buttonsHidden = true;
    }
    else {
        // If the comment field is maximized, do not show the controlPanel yet.
        // It will be shown as soon as the comment field is restored.
        if( !maximizeCommentButton->isChecked() )
            controlPanel->show();
        maximizeCommentButton->show();
        buttonsHidden = false;
    }
}

bool QuizFrame::areButtonsHidden() const {
    return( buttonsHidden );
}

void QuizFrame::showEvent( QShowEvent* showEvt ) {
    QWidget::showEvent( showEvt );
    // We set the state of maximize action because it VocabularyManagerFrame may have changed it.
    action[ ACTION_MAXIMIZE ]->setChecked( maximizeCommentButton->isChecked() );  
    emit( quizShown() );
}

void QuizFrame::hideEvent( QHideEvent* hideEvt ) {
    QWidget::hideEvent( hideEvt );
    emit( quizHidden() );
}

void QuizFrame::resizeEvent( QResizeEvent* evt ) {
    QWidget::resizeEvent( evt );
    resizeImageBox();
}

void QuizFrame::rightAnswer() {
    if( controller->isQuizInProgress() ) {
        controller->rightAnswer();
        askNextTerm();
    }
}

void QuizFrame::wrongAnswer() {
    if( controller->isQuizInProgress() ) {
        controller->wrongAnswer();
        askNextTerm();
    }
}

void QuizFrame::reveal() {
    if( controller->isQuizInProgress() ) {
        Sequence seq = controller->getRevealingSequence();
        int groupCount = seq.getGroupCount();
        bool hasShownSomething = false;
        int i = controller->getRevealingSequenceStep();
        int stepCount = 0;
        //cerr << "currSeq=" << seq.toHumanReadableString() << " i=" << i << endl;
        for( ; i < groupCount && !hasShownSomething; i++, stepCount++ ) {
            Sequence::ItemList group = seq.getGroupAt( i );

            int itemCount = group.count();
            for( int j = 0; j < itemCount; j++ ) {
                Sequence::Item item = group[ j ];
                //cerr << "item=" << item << endl;
                switch( item ) {

                    case Sequence::FIRST_LANG_TERM :  
                        if( !isFirstLangTermRevealed() ) {
                            revealFirstLangTerm(); 
                            hasShownSomething = true;
                        }
                        break;

                    case Sequence::TEST_LANG_ALT :
                        if( !isAltTermRevealed() ) {
                            revealAltTerm(); 
                            hasShownSomething = true;
                        }
                        break;

                    case Sequence::TEST_LANG_TERM :
                        if( !isTestLangTermRevealed() ) {
                            revealTestLangTerm(); 
                            hasShownSomething = true;
                        }
                        break;

                    case Sequence::COMMENT :          
                        if( !isCommentRevealed() ) {
                            revealComment(); 
                            hasShownSomething = true;
                        }
                        break;

                    case Sequence::IMAGE :            
                        if( containsValidImage() && !isImageRevealed() ) {
                            revealImage(); 
                            hasShownSomething = true;
                        }
                        break;
                    
                    default:
                        break;
                }
            }

        }
        if( hasShownSomething ) {
            for( int k = 0; k < stepCount; k++ )
                controller->incrementRevealingSequenceStep();
        }
        else if( i == groupCount ) {
            if( isCommentRevealed() )
                toggleMaximizeComment( !maximizeCommentButton->isChecked() );
        }

        //QApplication::flushX();
    }
}

void QuizFrame::revealAll() {
    revealTestLangTerm();
    revealAltTerm();
    revealComment();
    revealImage();
    //QApplication::flushX();
}

void QuizFrame::scrollLeft() {
    firstLangTermLineEdit->scrollLeft();
    testLangTermLineEdit->scrollLeft();
    testLangTermAltLineEdit->scrollLeft();
}

void QuizFrame::scrollRight() {
    firstLangTermLineEdit->scrollRight();
    testLangTermLineEdit->scrollRight();
    testLangTermAltLineEdit->scrollRight();
}

void QuizFrame::scrollUp() {
    commentMultiLineEdit->scrollUp();
}

void QuizFrame::scrollDown() {
    commentMultiLineEdit->scrollDown();
}

void QuizFrame::scrollPageUp() {
    commentMultiLineEdit->scrollPageUp();
}

void QuizFrame::scrollPageDown() {
    commentMultiLineEdit->scrollPageDown();
}

void QuizFrame::updateFonts() {
    QFont largeFont( controller->getPreferences().getLargeFont() );
    QFont mediumFont( controller->getPreferences().getMediumFont() );
    QFont labelsFont( controller->getPreferences().getLabelsFont() );
    QString firstLang( controller->getQuizFirstLanguage() );
    QString testLang( controller->getQuizTestLanguage() );

    firstLangTermLabel->setFont( labelsFont );
    firstLangTermLineEdit->setFont( controller->getPreferences().getMediumFont( firstLang ) );
    firstLangTermButton->setFont( labelsFont );
    firstLangPanel->setFont( labelsFont );

    testLangTermAltLabel->setFont( labelsFont );
    testLangTermAltLineEdit->setFont( controller->getPreferences().getMediumFont( testLang ) );
    testLangTermLabel->setFont( labelsFont );

    testLangTermLineEdit->setFont( controller->getPreferences().getLargeFont( testLang ) );

    testLangPanel->setFont( labelsFont );

    commentLabel->setFont( labelsFont );
    commentMultiLineEdit->setFont( controller->getPreferences().getBestFont( firstLang, testLang ) );

    firstLangTermPanel->updateGeometry();
    firstLangTermPanel->layout()->invalidate();

    firstLangTermStack->updateGeometry();
   
    testLangLabelsPanel->updateGeometry();
    testLangLabelsPanel->layout()->invalidate();

    testLangFieldsPanel->updateGeometry();
    testLangFieldsPanel->layout()->invalidate();

    revealAllDataButton->setFont( labelsFont );
    rightAnswerButton->setFont( labelsFont );
    wrongAnswerButton->setFont( labelsFont );
    editionButton->setFont( labelsFont );

    update();
    updateGeometry();
}

void QuizFrame::retranslateUi() {
    updateLanguageLabels();
    firstLangTermLabel->setText( tr( "Word/Expr." ) );
    firstLangTermButton->setText( tr( "???" ) );
    testLangTermAltLabel->setText( tr( "Alt./Phon." ) );
    testLangTermLabel->setText( tr( "Word/Expr." ) );
    testLangTermAltButton->setText( tr( "???" ) );
    testLangTermButton->setText( tr( "???" ) );
    imageBox->setTitle( tr( "Image" ) );
    commentLabel->setText( tr( "Examples/Comments" ) );
    commentButton->setText( tr( "???" ) );
    //revealAllDataButton->setText( tr( "Reveal" ) );
    revealAllDataButton->setToolTip( tr( "Reveal" ) );
    //rightAnswerButton->setText( tr( "RightAnswer" ) );
    rightAnswerButton->setToolTip( tr( "RightAnswer" ) );
    //wrongAnswerButton->setText( tr( "WrongAnswer" ) );
    wrongAnswerButton->setToolTip( tr( "WrongAnswer" ) );
    //editionButton->setText( tr( "EditWord" ) );
    editionButton->setToolTip( tr( "EditWord" ) );

    firstLangTermButton->setToolTip( tr( "Reveal" ) );
    testLangTermAltButton->setToolTip( tr( "Reveal" ) );
    testLangTermButton->setToolTip( tr( "Reveal" ) );
    commentButton->setToolTip( tr( "Reveal" ) );
    imageButton->setToolTip( tr( "Reveal" ) );
    maximizeCommentButton->setToolTip( tr( "MaximizeComment" ) );
}

void QuizFrame::hideAnswers() {
    firstLangTermStack->setCurrentIndex( 1 );
    testLangTermAltStack->setCurrentIndex( 1 );
    testLangTermStack->setCurrentIndex( 1 );
    commentStack->setCurrentIndex( 1 );
    imageStack->setCurrentIndex( 1 );
}

void QuizFrame::setButtonsEnabled( bool isEnabled ) {
    revealAllDataButton->setEnabled( isEnabled );
    rightAnswerButton->setEnabled( isEnabled );
    wrongAnswerButton->setEnabled( isEnabled );
    editionButton->setEnabled( isEnabled );
}

void QuizFrame::revealFirstLangTerm() {
    firstLangTermStack->setCurrentIndex( 0 );
}

void QuizFrame::revealAltTerm() {
    testLangTermAltStack->setCurrentIndex( 0 );
}

void QuizFrame::revealTestLangTerm() {
    testLangTermStack->setCurrentIndex( 0 );
}

void QuizFrame::revealComment() {
    commentStack->setCurrentIndex( 0 );
}

void QuizFrame::revealImage() {
    imageStack->setCurrentIndex( 0 );
    if( movie )
        movie->start();
}

void QuizFrame::toggleMaximizeComment( bool isOn ) {
    if( isOn )
        maximizeCommentField();
    else
        restoreCommentField();
}

bool QuizFrame::isFirstLangTermRevealed() const {
    return( firstLangTermStack->currentWidget() == firstLangTermStack->widget( 0 ) );
}

bool QuizFrame::isAltTermRevealed() const {
    return( testLangTermAltStack->currentWidget() == testLangTermAltStack->widget( 0 ) );
}

bool QuizFrame::isTestLangTermRevealed() const {
    return( testLangTermStack->currentWidget() == testLangTermStack->widget( 0 ) );
}

bool QuizFrame::isCommentRevealed() const {
    return( commentStack->currentWidget() == commentStack->widget( 0 ) );
}

bool QuizFrame::isImageRevealed() const {
    return( imageStack->currentWidget() == imageStack->widget( 0 ) );
}

void QuizFrame::maximizeCommentField() {
    setUpdatesEnabled( false );
    action[ ACTION_MAXIMIZE ]->setChecked( true );
    maximizeCommentButton->setChecked( true );
    firstLangPanel->hide();
    testLangPanel->hide();
    imageBox->hide();
    controlPanel->hide();
    revealComment();
    setUpdatesEnabled( true );
}

void QuizFrame::restoreCommentField() {
    setUpdatesEnabled( false );
    action[ ACTION_MAXIMIZE ]->setChecked( false );
    maximizeCommentButton->setChecked( false );
    firstLangPanel->show();
    testLangPanel->show();

    if( containsValidImage() )
        imageBox->show();

    if( !buttonsHidden )
        controlPanel->show();
    setUpdatesEnabled( true );
}

void QuizFrame::editCurrentTerm() {
    if( controller->isQuizInProgress() ) {
        Folder* vocabTree = controller->getVocabTree();
        Term* term = controller->getCurrentTerm();
        if( !term ) {
            QMessageBox::warning( this, QObject::tr( "Information" ), tr( "DissociatedWord" ) );
            return;
        }

        Vocabulary* vocab = vocabTree->getVocabulary( term->getVocabId() );
        if( vocab == NULL || !vocab->isTermExists( term->getId() ) ) {
            QMessageBox::warning( this, QObject::tr( "Information" ), tr( "DissociatedWord" ) );
            return;
        }

        TermDialog dialog( *vocab, controller, this, *term );
#if defined(Q_WS_HILDON)
        dialog.showFullScreen();
#endif
        int result = dialog.exec();
        if( result ) { 
            QString firstLang( controller->getQuizFirstLanguage() );
            QString testLang( controller->getQuizTestLanguage() );
            Term newTerm = dialog.getTerm();
            Translation firstLangTrans = newTerm.getTranslation( firstLang );
            Translation testLangTrans = newTerm.getTranslation( testLang );
            term->addTranslation( firstLangTrans );
            term->addTranslation( testLangTrans );
            BilingualKey commentKey( firstLang, testLang );
            term->addComment( commentKey, newTerm.getComment( commentKey ) );
            term->setImagePath( newTerm.getImagePath() );
            vocab->setModificationDate( QDateTime::currentDateTime() );
            vocab->setDirty( true );
            setTerm( newTerm );
        }
    }
}
