/****************************************************************
**
** toMOTko
** 
** Small application to learn vocabulary on my Zaurus.
**
** Author : Frederic Bergeron (c) 2006-209
**
****************************************************************/

#include <QApplication>
#include <QLabel>
#include <QtDebug>
#include "Controller.h"
#include "MainWindow.h"
#ifdef Q_WS_MAC
#include "icons/toMOTko_MacOS.xpm"
#else
#include "icons/toMOTko.xpm"
#endif

using namespace std;

QAction* action[ ACTION_COUNT ];

int main( int argc, char **argv ) {
    QApplication app( argc, argv );
#ifdef Q_WS_MAC
    app.setWindowIcon( QIcon( QPixmap( toMOTko_MacOS_xpm ) ) );
#else
    app.setWindowIcon( QIcon( QPixmap( toMOTko_xpm ) ) );
#endif
    Controller* controller = new Controller();
    if( !controller->init() ) {
        cerr << "Cannot initialize controller.  Check disk space and file permissions." << endl;
        exit( 2 );
    }

    MainWindow* mainWindow = new MainWindow( app, controller );
    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
#if defined(WINCE)
    mainWindow->showMaximized();
#else
    mainWindow->show();
#endif

    return( app.exec() );
}
