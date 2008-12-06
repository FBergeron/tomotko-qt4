#include "SequenceDialog.h"

SequenceDialog::SequenceDialog( Preferences* prefs, QWidget* parent ) 
    : QDialog( parent ), prefs( prefs ) {
    init();
}

SequenceDialog::~SequenceDialog() {
}

void SequenceDialog::init() {
    setWindowFlags( Qt::Window );
    setModal( true );

    sequencePanel = new QWidget();
    sequencePanelLayout = new QHBoxLayout();
    sequencePanel->setLayout( sequencePanelLayout );
    sequencePanelLayout->setSpacing( 0 );

    quizPanel = new QGroupBox( tr( "Quiz" ) );
    quizPanelLayout = new QVBoxLayout();
    quizPanel->setLayout( quizPanelLayout );
    sequencePanelLayout->addWidget( quizPanel, 1 );
    
    quizTopPanel = new QWidget();
    quizTopPanelLayout = new QHBoxLayout();
    quizTopPanel->setLayout( quizTopPanelLayout );
    quizPanelLayout->addWidget( quizTopPanel );

    quizTopLeftPanel = new QWidget();
    quizTopLeftPanelLayout = new QVBoxLayout();
    quizTopLeftPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    quizTopLeftPanel->setLayout( quizTopLeftPanelLayout );
    quizTopPanelLayout->addWidget( quizTopLeftPanel );
    quizTopLeftPanel->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred ) );

    quizFirstLangPanel = new QGroupBox( tr( "FirstLanguage" ) );
    quizFirstLangPanelLayout = new QHBoxLayout();
    quizFirstLangPanelLayout->setContentsMargins( 0, 0, 0, 0 );

    quizFirstLangPanel->setLayout( quizFirstLangPanelLayout );
    quizTopLeftPanelLayout->addWidget( quizFirstLangPanel );

    quizFirstLangPanelWrapper = new QWidget();
    quizFirstLangPanelWrapperLayout = new QHBoxLayout();
    quizFirstLangPanelWrapper->setLayout( quizFirstLangPanelWrapperLayout );
    quizFirstLangPanelLayout->addWidget( quizFirstLangPanelWrapper );
    quizFirstLangLabel = new QLabel( tr( "Word/Expr." ) );
    quizFirstLangPanelWrapperLayout->addWidget( quizFirstLangLabel );
    quizFirstLangTermButton = new QPushButton( "a" );
    quizFirstLangTermButton->setMinimumWidth( 150 );
    quizFirstLangPanelWrapperLayout->addWidget( quizFirstLangTermButton, 1 );
    quizFirstLangTermButton->installEventFilter( this );
    quizFirstLangPanel->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );

    quizTestLangPanel = new QGroupBox( tr( "TestLanguage" ) );
    quizTestLangPanelLayout = new QHBoxLayout();
    quizTestLangPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    quizTestLangPanel->setLayout( quizTestLangPanelLayout );
    quizTopLeftPanelLayout->addWidget( quizTestLangPanel );

    quizTestLangPanelWrapper = new QWidget();
    quizTestLangPanelWrapperLayout = new QHBoxLayout();
    quizTestLangPanelWrapper->setLayout( quizTestLangPanelWrapperLayout );
    quizTestLangPanelLayout->addWidget( quizTestLangPanelWrapper );
    quizTestLangLabelsPanel = new QWidget();
    quizTestLangLabelsPanelLayout = new QVBoxLayout();
    quizTestLangLabelsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    quizTestLangLabelsPanel->setLayout( quizTestLangLabelsPanelLayout );
    quizTestLangPanelWrapperLayout->addWidget( quizTestLangLabelsPanel );
    quizTestLangButtonsPanel = new QWidget();
    quizTestLangButtonsPanelLayout = new QVBoxLayout();
    quizTestLangButtonsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    quizTestLangButtonsPanel->setLayout( quizTestLangButtonsPanelLayout );
    quizTestLangPanelWrapperLayout->addWidget( quizTestLangButtonsPanel, 1 );
    quizTestLangAltLabel = new QLabel( tr( "Alt./Phon." ) );
    quizTestLangLabelsPanelLayout->addWidget( quizTestLangAltLabel );
    quizTestLangTermLabel = new QLabel( tr( "Word/Expr." ) );
    quizTestLangLabelsPanelLayout->addWidget( quizTestLangTermLabel );
    quizTestLangAltButton = new QPushButton( "b" );
    quizTestLangButtonsPanelLayout->addWidget( quizTestLangAltButton );
    quizTestLangAltButton->installEventFilter( this );
    quizTestLangTermButton = new QPushButton( "c" );
    quizTestLangButtonsPanelLayout->addWidget( quizTestLangTermButton );
    quizTestLangTermButton->installEventFilter( this );

    quizImagePanel = new QGroupBox( tr( "Image" ) );
    quizImagePanelLayout = new QVBoxLayout();
    quizImagePanel->setLayout( quizImagePanelLayout );
    quizTopPanelLayout->addWidget( quizImagePanel );
    quizImagePanel->setFixedWidth( 130 );
    quizImageButton = new QPushButton( "e" );
    quizImagePanelLayout->addWidget( quizImageButton );
    quizImageButton->installEventFilter( this );
    quizImageButton->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );

    quizCommentBox = new QWidget();
    quizCommentBox->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
    quizCommentBoxLayout = new QVBoxLayout();
    quizCommentBox->setLayout( quizCommentBoxLayout );
    quizPanelLayout->addWidget( quizCommentBox, 1 );

    quizCommentLabel = new QLabel( tr( "CommentLabelPanel" ) );
    quizCommentBoxLayout->addWidget( quizCommentLabel );
    quizCommentButton = new QPushButton( "d" );
    quizCommentButton->setMinimumHeight( 100 );
    quizCommentButton->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
    quizCommentBoxLayout->addWidget( quizCommentButton, 1 );
    quizCommentButton->installEventFilter( this );

    sequencePanelButtons = new QWidget();
    sequencePanelButtonsLayout = new QVBoxLayout();
    sequencePanelButtons->setLayout( sequencePanelButtonsLayout );
    sequencePanelLayout->addWidget( sequencePanelButtons );

    addSequenceMarkButton = new QPushButton( ">" );
    sequencePanelButtonsLayout->addWidget( addSequenceMarkButton );
    addSequenceMarkButton->setEnabled( false );
    addSequenceMarkButton->setMinimumWidth( 50 );
    connect( addSequenceMarkButton, SIGNAL( clicked() ), this, SLOT( addSequenceMark() ) );
    addGroupMarkButton = new QPushButton( "+" );
    sequencePanelButtonsLayout->addWidget( addGroupMarkButton );
    addGroupMarkButton->setEnabled( false );
    addGroupMarkButton->setMinimumWidth( addSequenceMarkButton->sizeHint().width() );
    connect( addGroupMarkButton, SIGNAL( clicked() ), this, SLOT( addGroupMark() ) );
    removeLastMarkButton = new QPushButton( "X" );
    sequencePanelButtonsLayout->addWidget( removeLastMarkButton );
    removeLastMarkButton->setEnabled( false );
    removeLastMarkButton->setMinimumWidth( addSequenceMarkButton->sizeHint().width() );
    connect( removeLastMarkButton, SIGNAL( clicked() ), this, SLOT( removeLastToken() ) );

    sequenceLinePanel = new QWidget();
    sequenceLinePanelLayout = new QHBoxLayout();
    sequenceLinePanel->setLayout( sequenceLinePanelLayout );
    sequenceLineLabel = new QLabel( tr( "Sequence" ) );
    sequenceLinePanelLayout->addWidget( sequenceLineLabel );
    sequenceLineLineEdit = new QLineEdit();
    sequenceLinePanelLayout->addWidget( sequenceLineLineEdit );
    sequenceLineLineEdit->setReadOnly( true );

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
    mainLayout->addWidget( sequenceLinePanel );
    mainLayout->addWidget( sequencePanel, 1 );
    mainLayout->addWidget( bottomButtonsPanel );
    mainLayout->activate();

    setLayout( mainLayout );

    setWindowTitle( tr( "AddRevealingSequence" ) );
}

Sequence SequenceDialog::getSequence() const {
    return( sequence );
}

void SequenceDialog::accept() {
    if( sequenceLineLineEdit->text().isEmpty() ) {
        QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "SequenceIsEmpty" ) );
        return;
    }

    QString lastChar = sequenceLineLineEdit->text().right( 1 );
    if( lastChar == " " || lastChar == "+" ) {
        QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "SequenceNotTerminated" ) );
        return;
    }

    currGroup.append( items.top() );
    sequence.addGroup( currGroup );
    currGroup.clear();

    //trace();

    QDialog::accept();
}

bool SequenceDialog::eventFilter( QObject* obj, QEvent* evt ) {
    if( evt->type() == QEvent::MouseButtonRelease ) {
        QMouseEvent* mouseEvt = (QMouseEvent*)evt;
        if( mouseEvt->button() == Qt::LeftButton ) {
            if( obj && obj->inherits( "QPushButton" ) ) {
                QPushButton* button = (QPushButton*)obj;
                addSequenceItem( button->text() );
            }
        }
    }
    return( false );
}

void SequenceDialog::addSequenceItem( const QString& itemStr ) {
    sequenceLineLineEdit->setText( items.count() == 0 ? itemStr : sequenceLineLineEdit->text() + itemStr );
    Sequence::Item item = Sequence::stringToItem( itemStr );
    items.push( item );
    setItemButtonsEnabled( false );
    setUnionButtonsEnabled( true );
    removeLastMarkButton->setEnabled( true );
    //trace();
}

void SequenceDialog::addSequenceMark() {
    currGroup.append( items.top() );
    sequence.addGroup( currGroup );
    currGroup.clear();
    sequenceLineLineEdit->setText( sequenceLineLineEdit->text() + " > " );
    setItemButtonsEnabled( true );
    setUnionButtonsEnabled( false );
    removeLastMarkButton->setEnabled( true );
    //trace();
}

void SequenceDialog::addGroupMark() {
    currGroup.append( items.top() );
    sequenceLineLineEdit->setText( sequenceLineLineEdit->text() + "+" );
    setItemButtonsEnabled( true );
    setUnionButtonsEnabled( false );
    removeLastMarkButton->setEnabled( true );
    //trace();
}

void SequenceDialog::removeLastToken() {
    QString seqStr = sequenceLineLineEdit->text();
    QString lastChar = seqStr.right( 1 );
    if( lastChar == "+" ) {
        int index = currGroup.indexOf( items.top() );
        currGroup.removeAt( index );
        setItemButtonsEnabled( false );
        setUnionButtonsEnabled( true );
        sequenceLineLineEdit->setText( seqStr.left( seqStr.length() - 1 ) );
    }
    else if( lastChar == " " ) { /* " > " */
        Sequence::ItemList lastGroup = sequence.getGroupAt( sequence.getGroupCount() - 1 );
        sequence.removeLastGroup();
        currGroup = lastGroup;
        if( !currGroup.isEmpty() )
            currGroup.removeAt( currGroup.count() - 1 );
        setItemButtonsEnabled( false );
        setUnionButtonsEnabled( true );
        sequenceLineLineEdit->setText( seqStr.left( seqStr.length() - 3 ) );
    }
    else { /* "a", "b", "c", "d" or "e" */
        items.pop();
        setItemButtonsEnabled( true );
        setUnionButtonsEnabled( false );
        sequenceLineLineEdit->setText( seqStr.left( seqStr.length() - 1 ) );
    }
    removeLastMarkButton->setEnabled( sequenceLineLineEdit->text().length() > 0 );
    //trace();
}

void SequenceDialog::setUnionButtonsEnabled( bool isEnabled ) {
    addSequenceMarkButton->setEnabled( isEnabled && areItemsLeft() );
    addGroupMarkButton->setEnabled( isEnabled && areItemsLeft() );
}

bool SequenceDialog::isItemUsed( const Sequence::Item& item ) const {
    return( items.contains( item ) );
}

bool SequenceDialog::areItemsLeft() const {
    return( !isItemUsed( Sequence::FIRST_LANG_TERM ) || !isItemUsed( Sequence::TEST_LANG_ALT ) || !isItemUsed( Sequence::TEST_LANG_TERM ) ||
        !isItemUsed( Sequence::COMMENT ) || !isItemUsed( Sequence::IMAGE ) );
}

void SequenceDialog::setItemButtonsEnabled( bool isEnabled ) {
    quizFirstLangTermButton->setEnabled( isEnabled && !isItemUsed( Sequence::FIRST_LANG_TERM ) );
    quizTestLangAltButton->setEnabled( isEnabled && !isItemUsed( Sequence::TEST_LANG_ALT ) );
    quizTestLangTermButton->setEnabled( isEnabled && !isItemUsed( Sequence::TEST_LANG_TERM ) );
    quizCommentButton->setEnabled( isEnabled && !isItemUsed( Sequence::COMMENT ) );
    quizImageButton->setEnabled( isEnabled && !isItemUsed( Sequence::IMAGE ) );
}

//void SequenceDialog::trace() const {
//    QString itemsDelim;
//    QString currGroupDelim;
//    cout << "debug items=[";
//    for( int i = 0; i < items.count(); i++ ) {
//        cout << qPrintable( itemsDelim ) << items.at( i );
//        itemsDelim = ",";
//    }
//    cout << "]";
//    cout << " currGroup=[";
//    for( int i = 0; i < currGroup.count(); i++ ) {
//        cout << qPrintable( currGroupDelim ) << currGroup.at( i );
//        currGroupDelim = ",";
//    }
//    cout << "] seq=" << qPrintable( sequence.toHumanReadableString() ) << endl;
//}
