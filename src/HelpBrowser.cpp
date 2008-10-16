#include "HelpBrowser.h"

HelpBrowser::HelpBrowser( const QString& path, const QString& page, QWidget* parent /* = 0 */ ) : QWidget( parent ) {
    setAttribute( Qt::WA_DeleteOnClose );
    setAttribute( Qt::WA_GroupLeader );

    textBrowser = new QTextBrowser();
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

    textBrowser->setSearchPaths( QStringList() << path << ":/img" );
    textBrowser->setSource( page );
}

void HelpBrowser::updateWindowTitle() {
    setWindowTitle( tr( "Help: %1" ).arg( textBrowser->documentTitle() ) );
}

void HelpBrowser::showPage( const QString& page ) {
    //QString path = QApplication::applicationDirPath() + "/doc";
    QString path = ":/help/en/html";
    HelpBrowser* browser = new HelpBrowser( path, page );
    browser->resize( 500, 400 );
    browser->show();
}

