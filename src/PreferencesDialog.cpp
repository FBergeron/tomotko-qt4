#include "PreferencesDialog.h"

const QString PreferencesDialog::fontSizeNameList[] = { 
    QT_TR_NOOP( "VerySmall" ), QT_TR_NOOP( "Small" ), QT_TR_NOOP( "Medium" ), QT_TR_NOOP( "Large" ), QT_TR_NOOP( "VeryLarge" ) 
};

const QString PreferencesDialog::firstLanguageList[] = { 
    QString( "en" ), QString( "fr" ), QString( "es" ), QString( "ja" ) 
};

const QString PreferencesDialog::studyLanguageList[] = { 
    QString( "en" ), QString( "fr" ), QString( "es" ), QString( "ja" ), QString( "de" ), QString( "ar" ), QString( "bg" ), QString( "el" ),
    QString( "it" ), QString( "ko" ), QString( "nl" ), QString( "pt" ), QString( "ru" ), QString( "sw" ), QString( "th" ), QString( "vi" ),
    QString( "zh" )
};

PreferencesDialog::PreferencesDialog( QWidget* parent, Preferences* prefs ) 
    : QDialog( parent/*, 0, true*/ ), prefs( prefs ), grabAccelKeyFor( NULL ), ignoreReturn( false ), keyboardAccelModified( false )  {
    init();
}

PreferencesDialog::~PreferencesDialog() {
    delete languageUpdateNotifier;
}

void PreferencesDialog::init() {
    tab = new QTabWidget();

    quizPage = new QWidget();

    quizLengthOptionsPanel = new QGroupBox( tr( "Length" ) );
    quizLengthOptionsPanelLayout = new QVBoxLayout();
    quizLengthOptionsPanel->setLayout( quizLengthOptionsPanelLayout );

    revealingOptionsPanel = new QGroupBox( tr( "RevealingOrders" ) );
    //revealingOptionsPanel->setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Minimum ) );
    revealingOptionsPanelLayout = new QHBoxLayout();
    revealingOptionsPanel->setLayout( revealingOptionsPanelLayout );
  
    sequencesViewPanel = new QWidget(); 
    sequencesViewPanelLayout = new QVBoxLayout();
    sequencesViewPanel->setLayout( sequencesViewPanelLayout );
    revealingOptionsPanelLayout->addWidget( sequencesViewPanel );
    //sequencesViewPanelLayout->setSpacing( 2 );
    sequencesView = new QTreeWidget();
    sequencesViewPanelLayout->addWidget( sequencesView );
    //sequencesView->addColumn( QString::null );
    //sequencesView->header()->hide();
    sequencesViewButtons = new QWidget();
    sequencesViewButtonsLayout = new QHBoxLayout();
    sequencesViewButtons->setLayout( sequencesViewButtonsLayout );
    sequencesViewPanelLayout->addWidget( sequencesViewButtons );
    //sequencesViewButtonsLayout->setSpacing( 2 );
    //sequencesViewButtonsFiller = new QWidget( sequencesViewButtons, "SequenceViewButtonsFiller" ); 
    addSequenceButton = new QPushButton( "+" );
    sequencesViewButtonsLayout->addWidget( addSequenceButton );
    addSequenceButton->setToolTip( tr( "Add revealing sequence" ) );
    connect( addSequenceButton, SIGNAL( clicked() ), this, SLOT( addSequence() ) );
    removeSequenceButton = new QPushButton( "-" );
    sequencesViewButtonsLayout->addWidget( removeSequenceButton );
    removeSequenceButton->setToolTip( tr( "Remove revealing sequence" ) );
    connect( removeSequenceButton, SIGNAL( clicked() ), this, SLOT( removeSequence() ) );
    //sequencesViewButtons->setStretchFactor( sequencesViewButtonsFiller, 1 );

    initSequences();
    connect( sequencesView, SIGNAL( itemSelectionChanged() ), this, SLOT( updateUi() ) );

    //revealingOptionsPanel->addSpace( 6 );

    sequencesLabelBox = new QGroupBox( tr( "QuizWindow" ) );
    sequencesLabelBoxLayout = new QVBoxLayout();
    sequencesLabelBox->setLayout( sequencesLabelBoxLayout );
    revealingOptionsPanelLayout->addWidget( sequencesLabelBox );
    sequencesLabel = new QLabel();
    sequencesLabelBoxLayout->addWidget( sequencesLabel );
    sequencesLabel->setPixmap( QPixmap( ":/pics/SequenceMapLandscape.png" ) );

    //sequencesViewPanel->setMaximumHeight( sequencesLabelBox->sizeHint().height() );

    quizLengthSlider = new QSlider( Qt::Horizontal );
    quizLengthSlider->setMinimum( 0 );
    quizLengthSlider->setMaximum( 4/*TermScheduler::poolCount - 1*/ );
    quizLengthSlider->setTickInterval( 1 );
    quizLengthSlider->setTickPosition( QSlider::TicksBothSides );
    quizLengthSlider->setFocusPolicy( Qt::StrongFocus );

    quizLengthOptionsPanelLayout->addWidget( quizLengthSlider );
    quizLengthLabelsPanel = new QWidget();
    quizLengthLabelsPanelLayout = new QHBoxLayout();
    quizLengthLabelsPanel->setLayout( quizLengthLabelsPanelLayout );
    quizLengthOptionsPanelLayout->addWidget( quizLengthLabelsPanel );

    quizLengthShortestLabel = new QLabel( tr( "QuizLengthShortest" ) );
    quizLengthMediumLabel = new QLabel( tr( "QuizLengthMedium" ) );
    quizLengthMediumLabel->setAlignment( Qt::AlignCenter );
    quizLengthLongestLabel = new QLabel( tr( "QuizLengthLongest" ) );
    quizLengthLongestLabel->setAlignment( Qt::AlignRight );
    quizLengthLabelsPanelLayout->addWidget( quizLengthShortestLabel );
    quizLengthLabelsPanelLayout->addWidget( quizLengthMediumLabel );
    quizLengthLabelsPanelLayout->addWidget( quizLengthLongestLabel );

    quizLengthSlider->setValue( prefs->getQuizLength() ); 

    quizLayout = new QVBoxLayout();
    quizPage->setLayout( quizLayout );
    quizLayout->addWidget( quizLengthOptionsPanel );
    quizLayout->addWidget( revealingOptionsPanel );
    quizLayout->addStretch();

    fontPage = new QWidget();

    fontsPanel = new QWidget();
    fontsPanelLayout = new QVBoxLayout();
    fontsPanel->setLayout( fontsPanelLayout );
    //fontsPanelLayout->setSpacing( 2 );

    labelsFontPanel = new QWidget();
    labelsFontPanelLayout = new QHBoxLayout();
    labelsFontPanel->setLayout( labelsFontPanelLayout );
    fontsPanelLayout->addWidget( labelsFontPanel );
    //labelsFontPanelLayout->setSpacing( 2 );
    labelsFontLabel = new QLabel( tr( "LabelsFont" ) );
    labelsFontFamilyComboBox = new QComboBox();
    labelsFontSizeComboBox = new QComboBox();
    resetDefaultLabelsFontButton = new QPushButton( tr( "ResetDefaultValue" )/*, labelsFontPanel, "ResetDefaultLabelsFontButton"*/ );
    connect( resetDefaultLabelsFontButton, SIGNAL( clicked() ), this, SLOT( resetDefaultLabelsFont() ) );
    labelsFontPanelLayout->addWidget( labelsFontLabel );
    labelsFontPanelLayout->addWidget( labelsFontFamilyComboBox );
    labelsFontPanelLayout->addWidget( labelsFontSizeComboBox );
    labelsFontPanelLayout->addWidget( resetDefaultLabelsFontButton );

    initFontFamilyValues( labelsFontFamilyComboBox );
    selectFontFamily( labelsFontFamilyComboBox, prefs->getLabelsFontFamily() );
    initFontSizeValues( labelsFontSizeComboBox );
    selectFontSize( labelsFontSizeComboBox, prefs->getLabelsFontSizeModifier() );

    fontPanel = new QWidget();
    fontPanelLayout = new QHBoxLayout();
    fontPanel->setLayout( fontPanelLayout );
    fontsPanelLayout->addWidget( fontPanel );
    //fontPanelLayout->setSpacing( 2 );
    fontLabel = new QLabel( tr( "FieldsFont" ) );
    fontFamilyComboBox = new QComboBox();
    fontSizeComboBox = new QComboBox();
    resetDefaultFontButton = new QPushButton( tr( "ResetDefaultValue" ) );
    connect( resetDefaultFontButton, SIGNAL( clicked() ), this, SLOT( resetDefaultFont() ) );
    fontPanelLayout->addWidget( fontLabel );
    fontPanelLayout->addWidget( fontFamilyComboBox);
    fontPanelLayout->addWidget( fontSizeComboBox);
    fontPanelLayout->addWidget( resetDefaultFontButton );


    initFontFamilyValues( fontFamilyComboBox );
    selectFontFamily( fontFamilyComboBox, prefs->getFontFamily() );
    initFontSizeValues( fontSizeComboBox );
    selectFontSize( fontSizeComboBox, prefs->getFontSizeModifier() );

    fontOverridesPanel = new QGroupBox( tr( "LanguageFontOverrides" ) );
    fontOverridesPanelLayout = new QVBoxLayout();
    fontOverridesPanel->setLayout( fontOverridesPanelLayout );
    fontsPanelLayout->addWidget( fontOverridesPanel );
    fontOverridesScrollView = new QScrollArea();//fontOverridesScrollView = new QScrollView( fontOverridesPanel, "FontOverridesScrollView" );
    fontOverridesPanelLayout->addWidget( fontOverridesScrollView );
    //fontOverridesScrollView->setResizePolicy( QScrollView::AutoOneFit );
    fontOverridesBox = new QWidget();
    fontOverridesBoxLayout = new QVBoxLayout();// fontOverridesBox );
    fontOverridesBox->setLayout( fontOverridesBoxLayout );
    initFontOverrides();
    fontOverridesScrollView->setWidget( fontOverridesBox );

    fontPageLayout = new QVBoxLayout();
    //fontPageLayoutLayout->setSpacing( 2 );
    fontPageLayout->addWidget( fontsPanel );
    fontPageLayout->addStretch();
    fontPage->setLayout( fontPageLayout );

    interfacePage = new QWidget();

    miscInterfaceOptionsPanel = new QWidget();
    miscInterfaceOptionsPanelLayout = new QGridLayout();
    miscInterfaceOptionsPanel->setLayout( miscInterfaceOptionsPanelLayout );

    digraphPanel = new QGroupBox( tr( "BuiltInSupportForAccents" ) );
    digraphPanelLayout = new QVBoxLayout();
    digraphPanel->setLayout( digraphPanelLayout );
    digraphCheckBox = new QCheckBox( tr( "DigraphesEnabled" ) );//, digraphPanel, "DigraphCheckBox" );
    digraphCheckBox->setCheckState( prefs->isDigraphEnabled() ? Qt::Checked : Qt::Unchecked );
    digraphPanelLayout->addWidget( digraphCheckBox );

    hideQuizButtonPanel = new QGroupBox( tr( "QuizOptions" ) );
    hideQuizButtonPanelLayout = new QVBoxLayout();
    hideQuizButtonPanel->setLayout( hideQuizButtonPanelLayout );
    hideQuizButtonCheckBox = new QCheckBox( tr( "hideQuizButtons" ) );
    hideQuizButtonCheckBox->setCheckState( prefs->areQuizButtonsHidden() ? Qt::Checked : Qt::Unchecked );
    hideQuizButtonPanelLayout->addWidget( hideQuizButtonCheckBox );

    showAltTextInTermListPanel = new QGroupBox( tr( "GlossaryManagerOptions" ) );
    showAltTextInTermListPanelLayout = new QVBoxLayout();
    showAltTextInTermListPanel->setLayout( showAltTextInTermListPanelLayout );
    showAltTextInTermListCheckBox = new QCheckBox( tr( "ShowAltInTermList" ) );
    showAltTextInTermListCheckBox->setChecked( prefs->isAltInTermListShown() ? Qt::Checked : Qt::Unchecked );
    showAltTextInTermListPanelLayout->addWidget( showAltTextInTermListCheckBox );

    miscInterfaceOptionsPanelLayout->addWidget( digraphPanel, 0, 0 );
    miscInterfaceOptionsPanelLayout->addWidget( hideQuizButtonPanel, 1, 0 );
    miscInterfaceOptionsPanelLayout->addWidget( showAltTextInTermListPanel, 2, 0 );

    keyboardAccelPanel = new QGroupBox( tr( "Keyboard Accelerators" ) );
    keyboardAccelPanelLayout = new QVBoxLayout();
    keyboardAccelPanel->setLayout( keyboardAccelPanelLayout );

    keyboardAccelListView = new QTreeWidget();
    keyboardAccelListView->setColumnCount( 2 );
    QStringList headerLabels;
    headerLabels << tr( "Action" ) << tr( "Key" ); 
    //keyboardAccelListView->addColumn( tr( "Action" ), 280 );
    //keyboardAccelListView->addColumn( tr( "Key" ), 140 );
    //keyboardAccelListView->setSorting( -1 );
    //keyboardAccelListView->setAllColumnsShowFocus( true );
    //keyboardAccelListView->setStretchColumn( 0 );
    keyboardAccelListView->setHeaderLabels( headerLabels );
    keyboardAccelListView->header()->setResizeMode( 0, QHeaderView::Stretch );
    //keyboardAccelListView->header()->setResizeMode( 1, QHeaderView::Stretch );
    int actionCount = sizeof( action ) / sizeof( QAction* );
    for( int i = actionCount - 1; i >= 0; i-- ) {
        KeyActionListViewItem* actionItem = new KeyActionListViewItem( keyboardAccelListView, action[ i ], (Action)i );
        actionItem->setText( 0, action[ i ]->text() );
        actionItem->setIcon( 0, action[ i ]->icon() );
        keyboardAccelListView->addTopLevelItem( actionItem );
    }
    //connect( keyboardAccelListView, SIGNAL( returnPressed( QListViewItem* ) ), this, SLOT( keyActionClicked( QListViewItem* ) ) );
    connect( keyboardAccelListView, SIGNAL( currentItemChanged( QTreeWidgetItem*, QTreeWidgetItem* )/*currentChanged( QListViewItem* )*/ ), this, SLOT( cancelSetAccelKey() ) );
    connect( tab, SIGNAL( currentChanged( int )/*selected( const QString& )*/ ), this, SLOT( cancelSetAccelKey() ) );
    keyboardAccelPanelLayout->addWidget( keyboardAccelListView );

    keyboardAccelButtonPanel = new QWidget();
    keyboardAccelPanelLayout->addWidget( keyboardAccelButtonPanel );
    clearAccelKeyButton = new QPushButton( tr( "Clear key" ) );
    setAccelKeyButton = new QPushButton( tr( "Set key" ) );
    resetAccelKeyButton = new QPushButton( tr( "Reset key" ) );
    
    keyboardAccelButtonPanelLayout = new QHBoxLayout( keyboardAccelButtonPanel );
    //keyboardAccelButtonPanelLayoutLayout->setSpacing( 2 );
    keyboardAccelButtonPanelLayout->addStretch();
    keyboardAccelButtonPanelLayout->addWidget( clearAccelKeyButton );
    keyboardAccelButtonPanelLayout->addWidget( setAccelKeyButton );
    keyboardAccelButtonPanelLayout->addWidget( resetAccelKeyButton );

    connect( clearAccelKeyButton, SIGNAL( clicked() ), this, SLOT( clearAccelKey() ) );
    connect( setAccelKeyButton, SIGNAL( clicked() ), this, SLOT( setAccelKey() ) );
    connect( resetAccelKeyButton, SIGNAL( clicked() ), this, SLOT( resetAccelKey() ) );

    //interfacePageFiller = new QWidget();

    interfacePageLayout = new QVBoxLayout( interfacePage );
    //interfacePageLayout->setSpacing( 2 );
    interfacePageLayout->addWidget( miscInterfaceOptionsPanel );
    interfacePageLayout->addWidget( keyboardAccelPanel, 1 );
    //interfacePageLayout->addWidget( interfacePageFiller, 1 );

    languagePage = new QWidget();

    languagesPanel = new QWidget();
    languagesPanelLayout = new QVBoxLayout();
    languagesPanel->setLayout( languagesPanelLayout );
    //languagesPanelLayout->setSpacing( 2 );

    studyLanguagesListView = new QTreeWidget();
    studyLanguagesListView->setColumnCount( 1 );
    languagesPanelLayout->addWidget( studyLanguagesListView );
    //studyLanguagesListView->setAllColumnsShowFocus( true );
    studyLanguagesListView->setHeaderLabel( tr( "StudyLanguages" ) );
    //studyLanguagesListView->setStretchColumn( 0 );
    //languagesPanel->setStretchFactor( studyLanguagesListView, 1 );
   
    languageUpdateNotifier = new LanguageUpdateNotifier();
    connect( languageUpdateNotifier, SIGNAL( languageAdded( const QString& ) ), this, SLOT( addFontOverride( const QString& ) ) );
    connect( languageUpdateNotifier, SIGNAL( languageRemoved( const QString& ) ), this, SLOT( removeFontOverride( const QString& ) ) );

    initStudyLanguageValues();

    //languagesRightPanel = new QWidget();
    //languagesRightPanelLayout = new QVBoxLayout();
    //languagesRightPanel->setLayout( languagesRightPanelLayout );
    //languagesPanelLayout->addWidget( languagesRightPanel );

    //languagesRightPanelFiller = new QWidget();

    //languagesRightPanelLayout->addWidget( languagesRightPanelFiller, 1 );

    //languagePageFiller = new QWidget();

    languageLayout = new QVBoxLayout();
    languagePage->setLayout( languageLayout );
    //languageLayout->setSpacing( 2 );
    languageLayout->addWidget( languagesPanel, 1 );
    //languageLayout->addWidget( languagePageFiller, 1 );
    tab->addTab( quizPage, tr( "Quiz" ) );
    tab->addTab( languagePage, tr( "Languages" ) );
    tab->addTab( fontPage, tr( "Fonts" ) );
    tab->addTab( interfacePage, tr( "Interface" ) );

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

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget( tab, 1 );
    mainLayout->addWidget( bottomButtonsPanel );
    setLayout( mainLayout );

    setWindowTitle( tr( "Preferences..." ) );

    updateUi();
}

void PreferencesDialog::initFontFamilyValues( QComboBox* comboBox, bool withEmptyEntry /*= false*/ ) const {
    QFontDatabase fontDatabase;
    QStringList families = fontDatabase.families();
    if( withEmptyEntry ) 
        comboBox->addItem( QString::null );
    comboBox->addItems( families );
}

void PreferencesDialog::initFontSizeValues( QComboBox* comboBox, bool withEmptyEntry /*= false*/ ) const {
    int fontSizeNameListLength = sizeof( fontSizeNameList ) / sizeof( QString );
    if( withEmptyEntry ) 
        comboBox->addItem( QString::null );
    for( int i = 0; i < fontSizeNameListLength; i++ )
        comboBox->addItem( tr( fontSizeNameList[ i ].toLatin1().data() ) );
}

void PreferencesDialog::initFontOverrides() {
    QList<QString> lang( prefs->getStudyLanguages() );
    for( int i = 0; i < lang.count(); i++ ) {
        QString language = lang.at( i );
        QWidget* fontOverrideBox = new QWidget();//HBox( fontOverridesBox );
        QHBoxLayout* fontOverrideBoxLayout = new QHBoxLayout();//( fontOverridesBox );
        fontOverrideBox->setLayout( fontOverrideBoxLayout );
        //fontOverridesBoxLayout->addWidget( fontOverrideBox );
        //    fontOverrideBoxLayout->setSpacing( 2 );
        //    fontOverrideBox->setMargin( 2 );
        QLabel* fontOverrideLabel = new QLabel( QApplication::translate( "QObject", language.toLatin1().data() ) );//, fontOverrideBox );
        QComboBox* fontOverrideFamilyComboBox = new QComboBox();// fontOverrideBox );
        initFontFamilyValues( fontOverrideFamilyComboBox, true );
        if( prefs->isFontOverrideFamilyDefined( language ) ) 
            selectFontFamily( fontOverrideFamilyComboBox, prefs->getFontOverrideFamily( language ) );
        QComboBox* fontOverrideSizeComboBox = new QComboBox();// fontOverrideBox );
        initFontSizeValues( fontOverrideSizeComboBox, true );
        if( prefs->isFontOverrideSizeDefined( language ) ) 
            selectFontSize( fontOverrideSizeComboBox, prefs->getFontOverrideSize( language ), true );
    //    fontOverrideBox->setStretchFactor( fontOverrideLabel, 1 );
        fontOverrideBoxLayout->addWidget( fontOverrideLabel );
        fontOverrideBoxLayout->addWidget( fontOverrideFamilyComboBox );
        fontOverrideBoxLayout->addWidget( fontOverrideSizeComboBox );

        fontOverrideBoxes.append( fontOverrideBox );
        fontOverrideLabels.append( fontOverrideLabel );
        fontOverrideFamilyComboBoxes.append( fontOverrideFamilyComboBox );
        fontOverrideSizeComboBoxes.append( fontOverrideSizeComboBox );

        fontOverridesBoxLayout->addWidget( fontOverrideBox, 0 );
    }
    fontOverridesBoxFiller = new QWidget();// fontOverridesBox );
    fontOverridesBoxLayout->addWidget( fontOverridesBoxFiller, 1 );
}

void PreferencesDialog::initSequences() {
    int seqCount = prefs->getRevealingSequenceCount();
    for( int i = 0; i < seqCount; i++ ) {
        Sequence seq = prefs->getRevealingSequenceAt( i );
        SequenceListItem* item = new SequenceListItem( sequencesView, seq.toHumanReadableString(), seq ); 
        item->setOn( seq.isEnabled() );
    }
}

void PreferencesDialog::initStudyLanguageValues() const {
    int studyLanguageListLength = sizeof( studyLanguageList ) / sizeof( QString );
    for( int i = 0; i < studyLanguageListLength; i++ ) {
        bool isStudied( prefs->isStudyLanguage( studyLanguageList[ i ] ) );
        StudyLanguageItem* lang = new StudyLanguageItem( studyLanguagesListView, languageUpdateNotifier );
        lang->setCheckState( 0, isStudied ? Qt::Checked : Qt::Unchecked );//lang->setOn( isStudied );
        lang->setText( 0, QApplication::translate( "QObject", studyLanguageList[ i ].toLatin1().data() ) );
        studyLanguagesListView->addTopLevelItem( lang );
    }
}

void PreferencesDialog::selectFontFamily( QComboBox* comboBox, const QString& fontFamily ) {
    for( int i = 0; i < comboBox->count(); i++ ) {
        if( comboBox->itemText( i ) == fontFamily )
            comboBox->setCurrentIndex( i );
    }
}

void PreferencesDialog::selectFontSize( QComboBox* comboBox, int fontSizeModifier, bool withEmptyEntry /*= false*/ ) {
    int fontSizeNameListLength = sizeof( fontSizeNameList ) / sizeof( QString );
    int sizeIndex = fontSizeModifier + ( fontSizeNameListLength - 1 ) / 2;
    if( withEmptyEntry )
        sizeIndex++;
    comboBox->setCurrentIndex( sizeIndex );
}

void PreferencesDialog::selectLanguage( QComboBox* comboBox, const QString& langCode ) {
    int itemCount = comboBox->count();
    for( int i = 0; i < itemCount; i++ ) {
        if( comboBox->itemText( i ) == QApplication::translate( "QQObject", langCode.toLatin1().data() ) ) { 
            comboBox->setCurrentIndex( i );
            return;
        }
    }
}

void PreferencesDialog::accept() {
    cancelSetAccelKey();

    if( !isRevealingSequenceSelectionValid() ) {
        tab->setCurrentWidget( quizPage ); 
        QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "RevealingOrderMandatory" ) );
        return;
    }

    if( !isStudyLanguageSelectionValid() ) {
        tab->setCurrentWidget( languagePage ); 
        QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "StudyLanguagesMandatory" ) );
        return;
    }

    prefs->setQuizLength( quizLengthSlider->value() );

    prefs->clearRevealingSequences();
    for( int i = 0; i < sequencesView->topLevelItemCount(); i++ ) {
        SequenceListItem* item = (SequenceListItem*)sequencesView->topLevelItem( i );
        prefs->addRevealingSequence( item->getSequence() );
    }

    prefs->setLabelsFontFamily( labelsFontFamilyComboBox->currentText() );//labelsFontFamilyComboBox->text( labelsFontFamilyComboBox->currentItem() ) );
    int fontSizeNameListLength = sizeof( fontSizeNameList ) / sizeof( QString );
    int labelsFontSizeModifier =  labelsFontSizeComboBox->currentIndex() - ( fontSizeNameListLength - 1 ) / 2;
    prefs->setLabelsFontSizeModifier( labelsFontSizeModifier );

    prefs->setFontFamily( fontFamilyComboBox->currentText() );//fontFamilyComboBox->text( fontFamilyComboBox->currentItem() ) );
    int fontSizeModifier =  fontSizeComboBox->currentIndex() - ( fontSizeNameListLength - 1 ) / 2;
    prefs->setFontSizeModifier( fontSizeModifier );

    prefs->clearFontOverrideFamilies();
    prefs->clearFontOverrideSizes();
    int fontOverrideCount = fontOverrideLabels.count();
    for( int i = 0; i < fontOverrideCount; i++ ) {
        const QString& language = Util::getLanguageCode( fontOverrideLabels.at( i )->text() );
        if( fontOverrideFamilyComboBoxes.at( i )->currentIndex() > 0 )
            prefs->setFontOverrideFamily( language, fontOverrideFamilyComboBoxes.at( i )->currentText() );
        if( fontOverrideSizeComboBoxes.at( i )->currentIndex() > 0 ) {
            int fontSizeModifier =  ( fontOverrideSizeComboBoxes.at( i )->currentIndex() - 1 ) - ( fontSizeNameListLength - 1 ) / 2;
            prefs->setFontOverrideSize( language, fontSizeModifier );
        }
    }

    bool firstLanguageExists = false;
    bool testLanguageExists = false;
    prefs->clearStudyLanguages();
    for( int i = 0; i < studyLanguagesListView->topLevelItemCount(); i++ ) {
        StudyLanguageItem* item = (StudyLanguageItem*)studyLanguagesListView->topLevelItem( i );
        bool isChecked = ( item->checkState( 0 ) != Qt::Unchecked );
        if( isChecked ) {
            QString langCode( Util::getLanguageCode( item->text( 0 ) ) );
            prefs->addStudyLanguage( langCode );
            if( !firstLanguageExists )
                firstLanguageExists = ( prefs->getFirstLanguage() == langCode );
            if( !testLanguageExists )
                testLanguageExists = (prefs->getTestLanguage() == langCode );
        }
    }
    if( !firstLanguageExists )
        prefs->setFirstLanguage( QString::null );
    if( !testLanguageExists )
        prefs->setTestLanguage( QString::null );

    bool isDigraphEnabled = ( digraphCheckBox->checkState() != Qt::Unchecked );
    prefs->setDigraphEnabled( isDigraphEnabled );

    bool isQuizButtonHiddenChecked = ( hideQuizButtonCheckBox->checkState() != Qt::Unchecked );
    prefs->setQuizButtonsHidden( isQuizButtonHiddenChecked );

    bool isAltInTermListShownChecked = ( showAltTextInTermListCheckBox->checkState() != Qt::Unchecked );
    prefs->setAltInTermListShown( isAltInTermListShownChecked );

    if( keyboardAccelModified ) {
        for( int i = 0; i < keyboardAccelListView->topLevelItemCount(); i++ ) {
            KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->topLevelItem( i );
            QAction *action = item->getAction();
            action->setShortcut( item->getKey() );
            prefs->setAccelerator( item->getActionIndex(), item->getKey() );
        }
    }

    QDialog::accept();
}

void PreferencesDialog::updateUi() {
    removeSequenceButton->setEnabled( sequencesView->currentItem() ); 
}

bool PreferencesDialog::isRevealingSequenceSelectionValid() const {
    for( int i = 0; i < sequencesView->topLevelItemCount(); i++ ) {
        SequenceListItem* item = (SequenceListItem*)sequencesView->topLevelItem( i );
        bool isChecked = ( item->checkState( 0 ) != Qt::Unchecked );
        if( isChecked )
            return( true );
    }

    return( false );
}

bool PreferencesDialog::isRevealingSequenceDefined( const QString& seqStr ) const {
    for( int i = 0; i < sequencesView->topLevelItemCount(); i++ ) {
        SequenceListItem* item = (SequenceListItem*)sequencesView->topLevelItem( i );
        if( item->getSequence().toHumanReadableString() == seqStr )
            return( true );
    }

    return( false );
}

bool PreferencesDialog::isStudyLanguageSelectionValid() const {
    int checkedLangCount = 0;
    for( int i = 0; i < studyLanguagesListView->topLevelItemCount(); i++ ) {
        StudyLanguageItem* item = (StudyLanguageItem*)studyLanguagesListView->topLevelItem( i );
        bool isChecked = ( item->checkState( 0 ) != Qt::Unchecked );
        if( isChecked )
            checkedLangCount++;
    }
    return( checkedLangCount >= 2 );
}

void PreferencesDialog::resizeEvent( QResizeEvent* evt ) {
    QString pictFilename( ":/pics/SequenceMap" + (QString)( evt->size().width() > evt->size().height() ? "Landscape" : "Portrait" ) + ".png" );
    sequencesLabel->setPixmap( QPixmap( pictFilename ) );
    sequencesViewPanel->setMaximumHeight( sequencesLabelBox->sizeHint().height() );
    QDialog::resizeEvent( evt );
}

void PreferencesDialog::resetDefaultLabelsFont() {
    selectFontFamily( labelsFontFamilyComboBox, prefs->getDefaultLabelsFontFamily() );
    selectFontSize( labelsFontSizeComboBox, prefs->getDefaultLabelsFontSizeModifier() );
}

void PreferencesDialog::resetDefaultFont() {
    selectFontFamily( fontFamilyComboBox, prefs->getDefaultFontFamily() );
    selectFontSize( fontSizeComboBox, prefs->getDefaultFontSizeModifier() );
}

void PreferencesDialog::addFontOverride( const QString& language ) {
    //// Do nothing if the font override already exists.
    //int fontOverrideCount = fontOverrideLabels.count();
    //for( int i = 0; i < fontOverrideCount; i++ ) {
    //    if( fontOverrideLabels.at( i )->text() == QObject::tr( language ) )
    //        return;
    //}
    //
    //QHBox* fontOverrideBox = new QHBox( fontOverridesBox );
    //fontOverrideBoxLayout->setSpacing( 2 );
    //fontOverrideBox->setMargin( 2 );
    //QLabel* fontOverrideLabel = new QLabel( QObject::tr( language ), fontOverrideBox );
    //QComboBox* fontOverrideFamilyComboBox = new QComboBox( fontOverrideBox );
    //initFontFamilyValues( fontOverrideFamilyComboBox, true );
    //if( prefs->isFontOverrideFamilyDefined( language ) ) 
    //    selectFontFamily( fontOverrideFamilyComboBox, prefs->getFontOverrideFamily( language ) );
    //QComboBox* fontOverrideSizeComboBox = new QComboBox( fontOverrideBox );
    //initFontSizeValues( fontOverrideSizeComboBox, true );
    //if( prefs->isFontOverrideSizeDefined( language ) ) 
    //    selectFontSize( fontOverrideSizeComboBox, prefs->getFontOverrideSize( language ), true );
    //fontOverrideBox->setStretchFactor( fontOverrideLabel, 1 );

    //fontOverrideBoxes.append( fontOverrideBox );
    //fontOverrideLabels.append( fontOverrideLabel );
    //fontOverrideFamilyComboBoxes.append( fontOverrideFamilyComboBox );
    //fontOverrideSizeComboBoxes.append( fontOverrideSizeComboBox );

    //int indexOfFiller = fontOverridesBoxLayout->findWidget( fontOverridesBoxFiller );
    //fontOverridesBoxLayout->insertWidget( indexOfFiller, fontOverrideBox ); 
}

void PreferencesDialog::removeFontOverride( const QString& language ) {
    //int fontOverrideCount = fontOverrideLabels.count();
    //for( int i = 0; i < fontOverrideCount; i++ ) {
    //    if( fontOverrideLabels.at( i )->text() == QObject::tr( language ) ) {
    //        QHBox* fontOverrideBox = fontOverrideBoxes.at( i );

    //        fontOverrideLabels.remove( i );
    //        fontOverrideFamilyComboBoxes.remove( i );
    //        fontOverrideSizeComboBoxes.remove( i );
    //        fontOverrideBoxes.remove( i );
    //        
    //        fontOverridesBox->removeChild( fontOverrideBox );

    //        break;
    //    }
    //}
}

void PreferencesDialog::setAccelKey() {
    KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->currentItem();
    if( item == NULL ) 
        return;
    item->setText( 1, tr( "<press key>" ) ); 
    grabAccelKeyFor = item;
    grabKeyboard();  
}

void PreferencesDialog::clearAccelKey() {
    cancelSetAccelKey();

    KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->currentItem();
    if( item == NULL ) 
        return;
    item->setKey( 0 );
    keyboardAccelModified = true;
}

void PreferencesDialog::resetAccelKey() {
    cancelSetAccelKey();

    KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->currentItem();
    if( item == NULL ) 
        return;
    item->setKey( prefs->getDefaultAccelerator( item->getActionIndex() ) );
    keyboardAccelModified = true;
}

void PreferencesDialog::cancelSetAccelKey() {
    if( grabAccelKeyFor != NULL ) {
        KeyActionListViewItem* ka = (KeyActionListViewItem*)grabAccelKeyFor;
        ka->updateText();
        grabAccelKeyFor = NULL;
        releaseKeyboard();
    }
}

void PreferencesDialog::keyActionClicked( QTreeWidgetItem* ) {
    ignoreReturn = true;
    setAccelKey();
}

void PreferencesDialog::keyPressEvent( QKeyEvent* evt ) {
    if( grabAccelKeyFor != NULL ) {
        switch (evt->key()) {
            case Qt::Key_Shift:
            case Qt::Key_Control:
            case Qt::Key_Meta:
            case Qt::Key_Alt:
            case Qt::Key_CapsLock:
            case Qt::Key_NumLock:
            case Qt::Key_ScrollLock:
            case Qt::Key_F22:
                evt->ignore();
                break;
            default:
                if( ignoreReturn && evt->key() == Qt::Key_Return ) {
                    evt->ignore();
                    break;
                }
                //      printf( "key: %x modif: %x\n", evt->key(), evt->state() );

                KeyActionListViewItem *ka = (KeyActionListViewItem*)grabAccelKeyFor;

                // This part needs some adapdation for Qt4.  Look the code in the Zaurus version. - FB
                QKeySequence keyCode = evt->key();
                if( (evt->modifiers() & Qt::Key_Shift) != 0 )
                    //keyCode = |= SHIFT;
                    cerr << "Shift was pressed." << endl;
                if( (evt->modifiers() & Qt::Key_Alt) != 0 )
                    //keyCode |= ALT;
                    cerr << "Alt was pressed." << endl;
                if( (evt->modifiers() & Qt::Key_Control) != 0 )
                    //keyCode |= CTRL;      
                    cerr << "Ctrl was pressed." << endl;

                //    printf( "keycode: %x\n", keyCode );
                releaseKeyboard();

                // Check for duplicate key codes
                bool cancelBinding = false;
                for( int i = 0; i < keyboardAccelListView->topLevelItemCount(); i++ ) {
                    KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->topLevelItem( i ); 
                    if( item == ka ) 
                        continue;

                    if( item->getKey() == keyCode ) {
                        int sel = QMessageBox::warning( this, tr( "Duplicate key binding" ),
                        tr( "Key '" ) + keyCode.toString() + tr( "'\nis currently assigned to\n'" ) + item->getAction()->text() +
                            tr( "'.\nOverwrite the current assignment?" ), QMessageBox::No, QMessageBox::Yes );
                        if( sel == QMessageBox::Yes ) 
                            item->setKey( 0 );
                        else {
                            cancelBinding = true;
                            break;
                        }
                    }
                }

                if( cancelBinding ) 
                    ka->updateText();
                else {
                    ka->setKey( keyCode );
                    keyboardAccelModified = true;
                }

                grabAccelKeyFor = NULL;
                evt->accept();
                break;
        }
    }
    ignoreReturn = false;

    QDialog::keyPressEvent( evt );
}

void PreferencesDialog::addSequence() {
    //SequenceDialog dialog( prefs, this );
    //dialog.showMaximized();
    //int result = dialog.exec();
    //if( result ) {
    //    Sequence sequence = dialog.getSequence();
    //    // Just add new sequence.  Ignore duplicates.
    //    if( !isRevealingSequenceDefined( sequence.toHumanReadableString() ) ) {
    //        SequenceListItem* item = new SequenceListItem( sequencesView, sequence.toHumanReadableString(), sequence ); 
    //        item->setOn( true );
    //    }
    //}
}

void PreferencesDialog::removeSequence() {
    //QListViewItem* currSeqItem = sequencesView->currentItem();
    //if( currSeqItem ) {
    //    delete( currSeqItem );
    //    updateUi();
    //}
}
