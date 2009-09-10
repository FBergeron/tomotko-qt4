#include "HelpBrowser.h"

HelpBrowser::HelpBrowser( const QString& path, const QString& page, QWidget* parent /* = 0 */ ) : QWidget( parent ) {
    setAttribute( Qt::WA_DeleteOnClose );
    setAttribute( Qt::WA_GroupLeader );

    textBrowser = new QTextBrowser();
    textBrowser->setOpenLinks( false );
    QString stylesheet( "body { margin: 10px; }" );
    textBrowser->document()->setDefaultStyleSheet( stylesheet );
    homeButton = new QPushButton( tr( "Home" ) );
    backButton = new QPushButton( tr( "Back" ) );
    closeButton = new QPushButton( tr( "Close" ) );
    //closeButton->setShortcut( tr( "Esc" ) );

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget( homeButton );
    buttonLayout->addWidget( backButton );
    buttonLayout->addStretch();
    buttonLayout->addWidget( closeButton );

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout( buttonLayout );
    mainLayout->addWidget( textBrowser );
    setLayout( mainLayout );

    connect( homeButton, SIGNAL( clicked() ), textBrowser, SLOT( home() ) );
    connect( backButton, SIGNAL( clicked() ), textBrowser, SLOT( backward() ) );
    connect( closeButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( textBrowser, SIGNAL( sourceChanged( const QUrl& ) ), this, SLOT( updateWindowTitle() ) );
    connect( textBrowser, SIGNAL( anchorClicked( const QUrl& ) ), this, SLOT( openLink( const QUrl& ) ) );

    textBrowser->setSearchPaths( QStringList() << path << ":/img" );
    textBrowser->setSource( page );
}

void HelpBrowser::updateWindowTitle() {
    setWindowTitle( tr( "Help: %1" ).arg( textBrowser->documentTitle() ) );
}

void HelpBrowser::openLink( const QUrl& url ) {
    if( url.toString().indexOf( "http://" ) == 0 )
        QDesktopServices::openUrl( url );
    else
        textBrowser->setSource( url );
}

void HelpBrowser::showPage( const QString& page ) {
    QString path = ":/help/en/html";
    HelpBrowser* browser = new HelpBrowser( path, page );
    browser->resize( 600, 600 );
#if defined(WINCE)
    browser->showMaximized();
#else
    browser->show();
#endif
}

