#ifndef HELP_BROWSER_H
#define HELP_BROWSER_H

#include <iostream>
#include <QApplication>
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QWidget>

using namespace std;

class HelpBrowser : public QWidget {

    Q_OBJECT

public:

    HelpBrowser( const QString& path, const QString& page, QWidget* parent = 0 );

    static void showPage( const QString& page );

private slots:

    void updateWindowTitle();
    void openLink( const QUrl& url );

private:
    
    QTextBrowser*   textBrowser;
    QPushButton*    homeButton;
    QPushButton*    backButton;
    QPushButton*    closeButton;

};

#endif
