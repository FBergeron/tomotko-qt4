#include "MainWindow.h"
#include "icons/about.xpm"
#include "icons/help.xpm"
#include "icons/startQuiz.xpm"
#include "icons/glossaryManager.xpm"
#include "icons/import.xpm"
#include "icons/export.xpm"
#include "icons/preferences.xpm"
#include "icons/quit.xpm"
#include "icons/editcut.xpm"
#include "icons/editcopy.xpm"
#include "icons/editpaste.xpm"
#include "icons/blueArrow.xpm"
#include "icons/eye.xpm"
#include "icons/goodAns.xpm"
#include "icons/badAns.xpm"
#include "icons/addTerm.xpm"
#include "icons/editTerm.xpm"
#include "icons/maximize.xpm"
#include "icons/search.xpm"
#include "icons/removeTerm.xpm"
#include "icons/addFolder.xpm"
#include "icons/addVocab.xpm"
#include "icons/removeItem.xpm"
#include "icons/checkAllTerms.xpm"
#include "icons/inverseCheckedTerms.xpm"
#include "icons/flag_en.xpm"
#include "icons/flag_es.xpm"
#include "icons/flag_fr.xpm"
#include "icons/flag_ja.xpm"
#include "icons/flag_zh.xpm"
#include "icons/flag_de.xpm"

MainWindow::MainWindow( QApplication& app, Controller* controller )
    : QMainWindow( 0/*, WDestructiveClose*/ ), app( app ), control( controller ) {
    setAttribute( Qt::WA_DeleteOnClose );
    Preferences& prefs( controller->getPreferences() );

    QString transDir = ":/i18n/" + prefs.getInterfaceLanguage();
    translator = new QTranslator( this );
    if( translator->load( "toMOTko", transDir ) )
        app.installTranslator( translator );
    else
        cerr << "Could not load translation files in directory " << qPrintable( transDir ) << endl;

    toolBar = new QToolBar();
    toolBar->setMovable( false );
    addToolBar( toolBar );

    languageSelectorPanel = new QWidget();
    languageSelectorPanelLayout = new QHBoxLayout();
    languageSelectorPanel->setLayout( languageSelectorPanelLayout );

    firstLanguageComboBox = new QComboBox();
    languageSelectorLabel = new QLabel();
    languageSelectorLabel->setPixmap( QPixmap( blueArrow_xpm ) );
    testLanguageComboBox = new QComboBox();

    languageSelectorPanelLayout->addWidget( firstLanguageComboBox );
    languageSelectorPanelLayout->addWidget( languageSelectorLabel );
    languageSelectorPanelLayout->addWidget( testLanguageComboBox );

    updateFirstLanguageValues();
    updateTestLanguageValues();
    connect( firstLanguageComboBox, SIGNAL( activated( const QString& ) ), this, SLOT( setFirstLanguage( const QString& ) ) );
    connect( testLanguageComboBox, SIGNAL( activated( const QString& ) ), this, SLOT( setTestLanguage( const QString& ) ) );

    copyAction = Util::createAction( QApplication::translate( "QObject", "Copy" ), editcopy_xpm, this, SLOT( copy() ), QKeySequence( Qt::CTRL + Qt::Key_C ) );
    cutAction = Util::createAction( QApplication::translate( "QObject", "Cut" ), editcut_xpm, this, SLOT( cut() ), QKeySequence( Qt::CTRL + Qt::Key_X ) );
    pasteAction = Util::createAction( QApplication::translate( "QObject", "Paste" ), editpaste_xpm, this, SLOT( paste() ), QKeySequence( Qt::CTRL + Qt::Key_V ) );

    progressBar = new QProgressBar();
    progressBar->setMaximumWidth( 160 );
    connect( controller, SIGNAL( progressChanged( int ) ), progressBar, SLOT( setValue( int ) ) );
    progressBar->setVisible( false );

    languageSelectorAction = toolBar->addWidget( languageSelectorPanel );
    toolBar->addSeparator();
    toolBar->addAction( copyAction );
    toolBar->addAction( cutAction );
    toolBar->addAction( pasteAction );
    toolBar->addSeparator();

    statusBar()->addPermanentWidget( progressBar );

    mainPanel = new QStackedWidget();

    quizFrame = new QuizFrame( control );

    vocabManagerFrame = new VocabularyManagerFrame( control );
    vocabManagerFrame->setDigraphEnabled( prefs.isDigraphEnabled() ); 

    action[ ACTION_REVEAL ] = Util::createAction( tr( "Reveal" ), eye_xpm, 
        quizFrame, SLOT( reveal() ), prefs.getAccelerator( ACTION_REVEAL ) );
    action[ ACTION_RIGHT_ANSWER ] = Util::createAction( tr( "RightAnswer" ), goodAns_xpm, 
        quizFrame, SLOT( rightAnswer() ), prefs.getAccelerator( ACTION_RIGHT_ANSWER ) );
    action[ ACTION_WRONG_ANSWER ] = Util::createAction( tr( "WrongAnswer" ), badAns_xpm, 
        quizFrame, SLOT( wrongAnswer() ), prefs.getAccelerator( ACTION_WRONG_ANSWER ) );
    action[ ACTION_EDIT_QUIZ_TERM ] = Util::createAction( tr( "EditQuizTerm" ), editTerm_xpm, 
        quizFrame, SLOT( editCurrentTerm() ), prefs.getAccelerator( ACTION_EDIT_QUIZ_TERM ) );
    action[ ACTION_START_QUIZ ] = Util::createAction( tr( "StartQuiz" ), startQuiz_xpm, 
        this, SLOT( startQuiz() ), prefs.getAccelerator( ACTION_START_QUIZ ) );
    action[ ACTION_MANAGE_GLOSSARIES ] = Util::createAction( tr( "ManageGlossaries" ), glossaryManager_xpm, 
        this, SLOT( invokeVocabularyManager() ), prefs.getAccelerator( ACTION_MANAGE_GLOSSARIES ) );
    action[ ACTION_IMPORT ] = Util::createAction( tr( "Import..." ), import_xpm, 
        this, SLOT( importData() ), prefs.getAccelerator( ACTION_IMPORT ) );
    action[ ACTION_EXPORT ] = Util::createAction( tr( "Export..." ), export_xpm, 
        this, SLOT( exportData() ), prefs.getAccelerator( ACTION_EXPORT ) );
    action[ ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ] = Util::createAction( tr( "ShowAllGlossariesAndTerms" ), NULL, 
        this, SLOT( toggleLanguageFilter() ), prefs.getAccelerator( ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ), true );
    action[ ACTION_PREFERENCES ] = Util::createAction( tr( "Preferences..." ), preferences_xpm, 
        this, SLOT( preferences() ), prefs.getAccelerator( ACTION_PREFERENCES ) );
    action[ ACTION_QUIT ] = Util::createAction( tr( "Quit" ), quit_xpm, 
        this, SLOT( quit() ), prefs.getAccelerator( ACTION_QUIT ) );
    action[ ACTION_ADD_FOLDER ] = Util::createAction( tr( "AddFolder" ), addFolder_xpm, 
        vocabManagerFrame, SLOT( addFolder() ), prefs.getAccelerator( ACTION_ADD_FOLDER ) );
    action[ ACTION_ADD_GLOSSARY ] = Util::createAction( tr( "AddGlossary" ), addVocab_xpm, 
        vocabManagerFrame, SLOT( addVocab() ), prefs.getAccelerator( ACTION_ADD_GLOSSARY ) );
    action[ ACTION_REMOVE_ITEM ] = Util::createAction( tr( "RemoveItem" ), removeItem_xpm, 
        vocabManagerFrame, SLOT( removeItem() ), prefs.getAccelerator( ACTION_REMOVE_ITEM ) );
    action[ ACTION_ADD_TERM ] = Util::createAction( tr( "AddTerm" ), addTerm_xpm, 
        vocabManagerFrame, SLOT( addTerm() ), prefs.getAccelerator( ACTION_ADD_TERM ) );
    action[ ACTION_EDIT_TERM ] = Util::createAction( tr( "EditTerm" ), editTerm_xpm, 
        vocabManagerFrame, SLOT( editTerm() ), prefs.getAccelerator( ACTION_EDIT_TERM ) );
    action[ ACTION_REMOVE_TERMS ] = Util::createAction( tr( "RemoveTerm" ), removeTerm_xpm, 
        vocabManagerFrame, SLOT( removeTerms() ), prefs.getAccelerator( ACTION_REMOVE_TERMS ) );
    action[ ACTION_CHECK_ALL_TERMS ] = Util::createAction( tr( "CheckAllTerms" ), checkAllTerms_xpm, 
        vocabManagerFrame, SLOT( checkAllTerms() ), prefs.getAccelerator( ACTION_CHECK_ALL_TERMS ) );
    action[ ACTION_INVERSE_CHECKED_TERMS ] = Util::createAction( tr( "InverseCheckedTerms" ), 
        inverseCheckedTerms_xpm, vocabManagerFrame, SLOT( inverseCheckedTerms() ), prefs.getAccelerator( ACTION_INVERSE_CHECKED_TERMS ) );
    action[ ACTION_MAXIMIZE ] = Util::createAction( tr( "Maximize" ), maximize_xpm, 
        this, SLOT( toggleMaximize( bool ) ), prefs.getAccelerator( ACTION_MAXIMIZE ), true );
    action[ ACTION_SEARCH ] = Util::createAction( tr( "Search..." ), search_xpm, 
        this, SLOT( search() ), prefs.getAccelerator( ACTION_SEARCH ) );

    // Accelerators that are not driven by menus/actions must be added to their widgets to work.
    vocabManagerFrame->addAction( action[ ACTION_CHECK_ALL_TERMS ] );
    vocabManagerFrame->addAction( action[ ACTION_INVERSE_CHECKED_TERMS ] );
    vocabManagerFrame->addAction( action[ ACTION_MAXIMIZE ] );
    vocabManagerFrame->addAction( action[ ACTION_ADD_TERM ] );
    vocabManagerFrame->addAction( action[ ACTION_REMOVE_TERMS ] );
    vocabManagerFrame->addAction( action[ ACTION_EDIT_TERM ] );
    vocabManagerFrame->addAction( action[ ACTION_ADD_FOLDER ] );
    vocabManagerFrame->addAction( action[ ACTION_ADD_GLOSSARY ] );
    vocabManagerFrame->addAction( action[ ACTION_REMOVE_ITEM ] );
    quizFrame->addAction( action[ ACTION_RIGHT_ANSWER ] );
    quizFrame->addAction( action[ ACTION_WRONG_ANSWER ] );
    quizFrame->addAction( action[ ACTION_REVEAL ] );
    quizFrame->addAction( action[ ACTION_EDIT_QUIZ_TERM ] );
    quizFrame->addAction( action[ ACTION_MAXIMIZE ] );

    actionsMenu = new QMenu( tr( "Actions" ), this );
    actionsMenuAction = menuBar()->addMenu( actionsMenu );
   
    actionsMenu->addAction( action[ ACTION_START_QUIZ ] );

    actionsMenu->addAction( action[ ACTION_MANAGE_GLOSSARIES ] );

    actionsMenu->addSeparator();
    actionsMenu->addAction( action[ ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ] );

    actionsMenu->addSeparator();
    actionsMenu->addAction( action[ ACTION_IMPORT ] );
    actionsMenu->addAction( action[ ACTION_EXPORT ] );

    actionsMenu->addSeparator();

    actionsMenu->addAction( action[ ACTION_PREFERENCES ] );

    actionsMenu->addSeparator();

    actionsMenu->addAction( action[ ACTION_QUIT ] );

    editionMenu = new QMenu( tr( "Edition" ), this );
    editionMenuAction = menuBar()->addMenu( editionMenu );

    editionMenu->addAction( cutAction );
    editionMenu->addAction( copyAction );
    editionMenu->addAction( pasteAction );

    editionMenu->addSeparator();

    editionMenu->addAction( action[ ACTION_SEARCH ] );

    helpMenu = new QMenu( tr( "?" ), this );
    helpMenuAction = menuBar()->addMenu( helpMenu );
    helpAction = Util::createAction( tr( "Help..." ), help_xpm, this, SLOT( help() ) );
    helpMenu->addAction( helpAction );
    helpMenu->addSeparator();
    
    languageActionGroup = new QActionGroup( this ); 
    QDir i18nDir( ":/i18n" );
    QStringList langDirs = i18nDir.entryList();
    QStringList sortedLanguages;
    for( int i = 0; i < langDirs.count(); i++ ) {
        if( langDirs[ i ] == QString( "." ) || langDirs[ i ] == QString( ".." ) )
            continue;
        QString locale = langDirs[ i ];
        QString langDirStr = ":/i18n/" + QString( locale );
        QDir langDir( langDirStr );
        if( langDir.exists( QString( "toMOTko.qm" ) ) ) {
            QTranslator translator( this );
            translator.load( QString( "toMOTko.qm" ), langDirStr );
            QString language = translator.translate( "QObject", locale.toLatin1().data() );
            sortedLanguages.append( language );
            availableLanguages[ language ] = locale;
        }
    }
    sortedLanguages.sort();
    for( int i = 0; i < sortedLanguages.count(); i++ ) {
        QAction* langAction = new QAction( this );
        langAction->setCheckable( true );
        langAction->setText( sortedLanguages[ i ] );
        
        // Refactor this if later.  Look at createAction() method for hint (maybe).
        QString langCode = availableLanguages[ sortedLanguages[ i ] ];
        if( langCode == QString( "en" ) )
            langAction->setIcon( QIcon( QPixmap( flag_en_xpm ) ) );
        else if( langCode == QString( "es" ) )
            langAction->setIcon( QIcon( QPixmap( flag_es_xpm ) ) );
        else if( langCode == QString( "fr" ) )
            langAction->setIcon( QIcon( QPixmap( flag_fr_xpm ) ) );
        else if( langCode == QString( "ja" ) )
            langAction->setIcon( QIcon( QPixmap( flag_ja_xpm ) ) );
        else if( langCode == QString( "zh" ) )
            langAction->setIcon( QIcon( QPixmap( flag_zh_xpm ) ) );
        else if( langCode == QString( "de" ) )
            langAction->setIcon( QIcon( QPixmap( flag_de_xpm ) ) );

        helpMenu->addAction( langAction );
        languageActionGroup->addAction( langAction );
        if( controller->getPreferences().getInterfaceLanguage() == availableLanguages[ sortedLanguages[ i ] ] )
            langAction->setChecked( true );
    }
    
    helpMenu->addSeparator();
    aboutAction = Util::createAction( tr( "About..." ), about_xpm, this, SLOT( about() ) );
    helpMenu->addAction( aboutAction );

    connect( quizFrame, SIGNAL( quizHidden() ), control, SLOT( concludeQuiz() ) );

    mainPanel->addWidget( quizFrame );
    mainPanel->addWidget( vocabManagerFrame );

    connect( vocabManagerFrame, SIGNAL( selectionChanged( QTreeWidgetItem* ) ), this, SLOT( updateMenus( QTreeWidgetItem* ) ) );
    setCentralWidget( mainPanel );
    setLanguageFilterEnabled( controller->getPreferences().isLanguageFilterEnabled() );
    invokeVocabularyManager();

    setWindowTitle( "toMOTko" );

    updateFonts();

    connect( languageActionGroup, SIGNAL( triggered( QAction* ) ), this, SLOT( switchLanguage( QAction* ) ) );
}

MainWindow::~MainWindow() {
    delete( control );
}

QSize MainWindow::sizeHint() const {
    return( QSize( 800, 600 ) );
}

Controller* MainWindow::controller() {
    return( control );
}

void MainWindow::updateMenus( QTreeWidgetItem* /* currItem */ ) {
    action[ ACTION_START_QUIZ ]->setText( mainPanel->currentWidget() == quizFrame ? tr( "RestartQuiz" ) : tr( "StartQuiz" ) );
    action[ ACTION_MANAGE_GLOSSARIES ]->setEnabled( mainPanel->currentWidget() != vocabManagerFrame ); 
    if( mainPanel->currentWidget() == vocabManagerFrame ) {
        if( !menuBar()->actions().contains( editionMenuAction ) )
            menuBar()->insertAction( helpMenuAction, editionMenuAction );
        action[ ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ]->setEnabled( true );
        action[ ACTION_IMPORT ]->setEnabled( vocabManagerFrame->isImportAllowed() );
        action[ ACTION_EXPORT ]->setEnabled( vocabManagerFrame->isExportAllowed() );
    }
    else {
        menuBar()->removeAction( editionMenuAction );
        action[ ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ]->setEnabled( false );
        action[ ACTION_IMPORT ]->setEnabled( false );
        action[ ACTION_EXPORT ]->setEnabled( false );
    }
}

void MainWindow::updateFonts() {
    QFont labelsFont( control->getPreferences().getLabelsFont() ); 

    qApp->setFont( labelsFont );
    firstLanguageComboBox->setFont( labelsFont );
    testLanguageComboBox->setFont( labelsFont );
    progressBar->setFont( labelsFont );

    quizFrame->updateFonts();
    vocabManagerFrame->updateFonts();
}

bool MainWindow::isDigraphEnabled() const {
    return( vocabManagerFrame->isDigraphEnabled() );
}

void MainWindow::setDigraphEnabled( bool isEnabled ) {
    vocabManagerFrame->setDigraphEnabled( isEnabled );
}

void MainWindow::retranslateUi() {
    actionsMenuAction->setText( tr( "Actions" ) );
    editionMenuAction->setText( tr( "Edition" ) );

    // Could use a loop here.
    action[ ACTION_REVEAL ]->setText( tr( "Reveal" ) );
    action[ ACTION_RIGHT_ANSWER ]->setText( tr( "RightAnswer" ) );
    action[ ACTION_WRONG_ANSWER ]->setText( tr( "WrongAnswer" ) );
    action[ ACTION_EDIT_QUIZ_TERM ]->setText( tr( "EditQuizTerm" ) );
    action[ ACTION_START_QUIZ ]->setText( tr( "StartQuiz" ) );
    action[ ACTION_MANAGE_GLOSSARIES ]->setText( tr( "ManageGlossaries" ) );
    action[ ACTION_IMPORT ]->setText( tr( "Import..." ) );
    action[ ACTION_EXPORT ]->setText( tr( "Export..." ) );
    action[ ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ]->setText( tr( "ShowAllGlossariesAndTerms" ) );
    action[ ACTION_PREFERENCES ]->setText( tr( "Preferences..." ) );
    action[ ACTION_QUIT ]->setText( tr( "Quit" ) );
    action[ ACTION_ADD_FOLDER ]->setText( tr( "AddFolder" ) );
    action[ ACTION_ADD_GLOSSARY ]->setText( tr( "AddGlossary" ) );
    action[ ACTION_REMOVE_ITEM ]->setText( tr( "RemoveItem" ) );
    action[ ACTION_ADD_TERM ]->setText( tr( "AddTerm" ) );
    action[ ACTION_EDIT_TERM ]->setText( tr( "EditTerm" ) );
    action[ ACTION_REMOVE_TERMS ]->setText( tr( "RemoveTerm" ) );
    action[ ACTION_CHECK_ALL_TERMS ]->setText( tr( "CheckAllTerms" ) );
    action[ ACTION_INVERSE_CHECKED_TERMS ]->setText( tr( "InverseCheckedTerms" ) );
    action[ ACTION_MAXIMIZE ]->setText( tr( "Maximize" ) );
    action[ ACTION_SEARCH ]->setText( tr( "Search..." ) );

    cutAction->setText( QObject::tr( "Cut" ) );
    copyAction->setText( QObject::tr( "Copy" ) );
    pasteAction->setText( QObject::tr( "Paste" ) );

    helpAction->setText( tr( "Help..." ) );
    aboutAction->setText( tr( "About..." ) );
    
    updateFirstLanguageValues();
    updateTestLanguageValues();

    quizFrame->retranslateUi();
    vocabManagerFrame->retranslateUi();
}

void MainWindow::switchLanguage( const QString& language ) {
    qApp->removeTranslator( translator );
    QString transDir = ":/i18n/" + language;
    if( translator->load( "toMOTko", transDir ) )
        qApp->installTranslator( translator );
    else
        cerr << "Could not load translation files in directory " << qPrintable( transDir ) << endl;
    retranslateUi();
}

void MainWindow::toggleMaximize( bool isOn ) {
    if( mainPanel->currentWidget() == quizFrame )
        quizFrame->toggleMaximizeComment( isOn );
    else if( mainPanel->currentWidget() == vocabManagerFrame )
        vocabManagerFrame->toggleMaximizeDetails( isOn );
}

void MainWindow::search() {
    if( mainPanel->currentWidget() == vocabManagerFrame )
        vocabManagerFrame->search();
}

void MainWindow::switchLanguage( QAction* langAction ) {
    QString language = availableLanguages[ langAction->text() ];
    switchLanguage( language );
    control->getPreferences().setInterfaceLanguage( language );
}

void MainWindow::closeEvent( QCloseEvent* ce ) {
    bool isOk = vocabManagerFrame->saveData();
    if( !isOk )
        QMessageBox::warning( this, QObject::tr( "Error" ), QObject::tr( "ErrorSavingData" ) );
    ce->accept();
}

void MainWindow::about() {
#if defined(Q_WS_HILDON)
    QMessageBox::about( this, tr( "AboutToMOTko..." ), tr( "AboutMessageShort" ) );
#else
    QMessageBox::about( this, tr( "AboutToMOTko..." ), tr( "AboutMessage" ) );
#endif
}

void MainWindow::help() {
    HelpBrowser::showPage( "toMOTko.html" );
}

void MainWindow::startQuiz() {
    if( mainPanel->currentWidget() == quizFrame ) 
        quizFrame->restartQuiz();
    else {
        bool resumeQuiz = false;
        if( control->isResumableQuizAvailable() ) {
            QMessageBox msgBox( QObject::tr( "Information" ), tr( "ConfirmResumeQuiz" ),
                QMessageBox::Warning,
                QMessageBox::Yes | QMessageBox::Default | QMessageBox::Escape,
                QMessageBox::No,
                QMessageBox::NoButton,
                this );
            msgBox.setButtonText( QMessageBox::Yes, tr( "Yes" ) );
            msgBox.setButtonText( QMessageBox::No, tr( "No" ) );
        
            int response = msgBox.exec();
            resumeQuiz = ( response == QMessageBox::Yes );
        }
        showQuiz();
        if( resumeQuiz )
            quizFrame->resumeQuiz();
        else
            quizFrame->startQuiz();
    }
}

void MainWindow::showQuiz() {
    mainPanel->setCurrentIndex( frameQuizIndex );
    updateMenus( NULL );
    toolBar->setVisible( false );
    progressBar->setVisible( true );
    languageSelectorAction->setVisible( false );
}

void MainWindow::invokeVocabularyManager() {
    mainPanel->setCurrentIndex( frameVocabManagerIndex );
    updateMenus( NULL );
    toolBar->setVisible( true );
    progressBar->setVisible( false );
    languageSelectorAction->setVisible( true );
}

void MainWindow::importData() {
    vocabManagerFrame->importData();
    // Some study languages in the preferences may have been added after the import
    // so we need to update the language selectors.
    updateFirstLanguageValues();
    updateTestLanguageValues();
}

void MainWindow::exportData() {
    vocabManagerFrame->exportData();
}

void MainWindow::preferences() {
    PreferencesDialog dialog( this, &(control->getPreferences()) );
#if defined(WINCE)
    dialog.showMaximized();
#elif defined(Q_WS_HILDON)
    dialog.showFullScreen();
#else
    dialog.show();
#endif
    int result = dialog.exec();
    if( result ) {
        // Check if different from previous value before updating.
        updateFonts();
        switchLanguage( control->getPreferences().getInterfaceLanguage() );
        //if( mainPanel->currentWidget() == quizFrame ) {
        //    // Update the quiz.  May be tricky if the current term has no data for the current first/test languages.
        //}
        //else if( mainPanel->currentWidget() == vocabManagerFrame )
        updateFirstLanguageValues();
        updateTestLanguageValues();
        setDigraphEnabled( control->getPreferences().isDigraphEnabled() );
        quizFrame->setButtonsHidden( control->getPreferences().areQuizButtonsHidden() );
        vocabManagerFrame->updateShownItems();
    }
}

void MainWindow::cut() {
    QWidget* widget = qApp->focusWidget();
    if( widget ) {
        if( widget->inherits( "QLineEdit" ) )
            ((QLineEdit*)widget)->cut();
        else if( widget->inherits( "QTextEdit" ) )
            ((QTextEdit*)widget)->cut();
        else if( mainPanel->currentWidget() == vocabManagerFrame )
            vocabManagerFrame->cut();
    }
}

void MainWindow::copy() {
    QWidget* widget = qApp->focusWidget();
    if( widget ) {
        if( widget->inherits( "QLineEdit" ) )
            ((QLineEdit*)widget)->copy();
        else if( widget->inherits( "QTextEdit" ) )
            ((QTextEdit*)widget)->copy();
        else if( mainPanel->currentWidget() == vocabManagerFrame )
                vocabManagerFrame->copy();
    }
}

void MainWindow::paste() {
    QWidget* widget = qApp->focusWidget();
    if( widget ) {
        if( widget->inherits( "QLineEdit" ) )
            ((QLineEdit*)widget)->paste();
        else if( widget->inherits( "QTextEdit" ) )
            ((QTextEdit*)widget)->paste();
        else if( mainPanel->currentWidget() == vocabManagerFrame )
            vocabManagerFrame->paste();
    }
}

void MainWindow::toggleLanguageFilter() {
    setLanguageFilterEnabled( !action[ ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ]->isChecked() );
    vocabManagerFrame->updateShownItems();
}

void MainWindow::quit() {
    close();
}

void MainWindow::setLanguageFilterEnabled( bool isEnabled ) {
    action[ ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ]->setChecked( !isEnabled );
    control->getPreferences().setLanguageFilterEnabled( isEnabled );
}

void MainWindow::updateFirstLanguageValues() {
    updateLanguageSelector( firstLanguageComboBox );
    selectLanguage( firstLanguageComboBox, control->getPreferences().getFirstLanguage() );
}

void MainWindow::updateTestLanguageValues() {
    updateLanguageSelector( testLanguageComboBox );
    selectLanguage( testLanguageComboBox, control->getPreferences().getTestLanguage() );
}

void MainWindow::selectLanguage( QComboBox* comboBox, const QString& langCode ) {
    int itemCount = comboBox->count();
    for( int i = 0; i < itemCount; i++ ) {
        if( comboBox->itemText( i ) == QApplication::translate( "QObject", langCode.toLatin1().data() ) ) {
            comboBox->setCurrentIndex( i );
            return;
        }
    }
}

void MainWindow::updateLanguageSelector( QComboBox* comboBox ) {
    QList<QString> studyLanguages = control->getPreferences().getStudyLanguages();
    QStringList sortedLanguages;
    for( int i = 0; i < studyLanguages.size(); i++ ) {
        QString lang( studyLanguages[ i ] );
        sortedLanguages.append( QApplication::translate( "QObject", lang.toLatin1().data() ) );
    }
    sortedLanguages.sort(); 

    comboBox->clear();
    comboBox->addItem( QString( "" ) );
    for( QStringList::ConstIterator it = sortedLanguages.begin(); it != sortedLanguages.end(); it++ ) {
        QString lang( *it );
        comboBox->addItem( lang );
    }
}

void MainWindow::setFirstLanguage( const QString& lang ) {
    if( lang == QApplication::translate( "QObject", control->getPreferences().getTestLanguage().toLatin1().data() ) && 
            control->getPreferences().getTestLanguage() != QString( "" ) )
        switchFirstAndTestLanguages();
    else
        control->getPreferences().setFirstLanguage( Util::getLanguageCode( lang ) );
    updateFonts();
    quizFrame->updateLanguageLabels();
    vocabManagerFrame->updateShownItems();
}

void MainWindow::setTestLanguage( const QString& lang ) {
    if( lang == QApplication::translate( "QObject", control->getPreferences().getFirstLanguage().toLatin1().data() ) && 
            control->getPreferences().getFirstLanguage() != QString( "" ) )
        switchFirstAndTestLanguages();
    else
        control->getPreferences().setTestLanguage( Util::getLanguageCode( lang ) );
    updateFonts();
    quizFrame->updateLanguageLabels();
    vocabManagerFrame->updateShownItems();
}

void MainWindow::switchFirstAndTestLanguages() {
    QString firstLang( control->getPreferences().getFirstLanguage() );
    QString testLang( control->getPreferences().getTestLanguage() );
    selectLanguage( firstLanguageComboBox, testLang );
    selectLanguage( testLanguageComboBox, firstLang );
    control->getPreferences().setFirstLanguage( testLang );
    control->getPreferences().setTestLanguage( firstLang );
}
