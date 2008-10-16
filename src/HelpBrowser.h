#ifndef HELP_BROWSER_H
#define HELP_BROWSER_H

#include <QApplication>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QWidget>

class HelpBrowser : public QWidget {

    Q_OBJECT

public:

    HelpBrowser( const QString& path, const QString& page, QWidget* parent = 0 );

    static void showPage( const QString& page );

private slots:

    void updateWindowTitle();

private:
    
    QTextBrowser*   textBrowser;
    QPushButton*    homeButton;
    QPushButton*    backButton;
    QPushButton*    closeButton;

};

#endif
