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
#include "icons/toMOTko.xpm"

using namespace std;

QAction* action[ ACTION_COUNT ];

int main( int argc, char **argv ) {
    QApplication app( argc, argv );
    app.setWindowIcon( QIcon( QPixmap( toMOTko_xpm ) ) );
    Controller* controller = new Controller();
    if( !controller->init() ) {
        cerr << "Cannot initialize controller.  Check disk space and file permissions." << endl;
        exit( 2 );
    }

    MainWindow* mainWindow = new MainWindow( app, controller );
    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    mainWindow->show();

    return( app.exec() );
}
