README
======

Windows
-------

To build the Makefile :

> qmake toMOTko.pro

To build the executable :

> make 

To extract localized string :

> lupdate toMOTko.pro

To generate the string files :

> lrelease toMOTko.pro

To deploy the required resources into debug and release directories :

> bin\deploy.rb

To run the executable :

> debug\toMOTko.exe


Linux
-----
Todo


MacOS
-----
Todo



What to do before a release
---------------------------
- Update version number in the About dialog script.
- Update online documentation if necessary (screenshots, new features, etc.)
- Remove traces if any.
- Build latest setup.exe file.
- Test it.
- Update version number in the website (version.txt), Doxygen.
- Update website if necessary (screenshots, new features, download link, etc.)
- Run doxygen to update the documentation.
- Upload latest setup.exe file: cadaver https://frs.sourceforge.net/f/fb/fbergeron/uploads and then: put toMOTko-x.y.z-Setup.exe
- Update the change log file and its date (changes.log + www/en/history.shtml).
- Create the new release on SourceForge (hide it first, then use preformatted format and insert enter in the text).
- Upload the doxygen doc : cd $TOMOTKO_HOME/doc;rsync -ruv --exclude=.svn * fbergeron@web.sourceforge.net:tomotko_html/doc
- Upload the online doc : cd $TOMOTKO_HOME/help;rsync -ruv --exclude=.svn * fbergeron@web.sourceforge.net:tomotko_html/help
- Upload the latest screenshots.
- Write release notes and changes.
- Check in all changes.
- Upload new website : cd $TOMOTKO_HOME/www;rsync -ruv --exclude=.svn * fbergeron@web.sourceforge.net:tomotko_html
- Post news to announce the release.
- Close bugs in bug tracker if any.
- Do the same thing for Freshmeat.
- Tag the new version in SVN : svn copy https://tomotko.svn.sourceforge.net/svnroot/tomotko/qt4/trunk https://tomotko.svn.sourceforge.net/svnroot/tomotko/qt4/tags/release-0.X.0 -m "Tagging the 0.X.0 release."


Learned lessons and/or interesting things to know
=================================================
For a QListView that allows multiple selection, selectedItem() cannot be used.  We have to iterate through all the QListItem to find the first selected one even though we know that there is only one selected.

When changing a font of a widget, it's important to call updateGeometry() and ->layout()->invalidate() on the parent panel.  Curiously, the call order doesn't seem important...  Is it true with Qt4?

To find the UTF-8 character number for a specific kanji, use this :

http://www.fileformat.info/info/unicode/char/search.htm
Enter the kanji and look for UTF-8 (hex)

To generate thumbnails for screenshots, we can use a command like this one :

# cd img
> for img in *.png
> do
> convert $img -resize 50% ${img%%.png}_small.png
> done

About QT 4
==========
There is 2 ways to use the console :

1) Using qDebug()                  (*) Doesn't work (on Windows, at least, for now)

Add this line in the toMOTko.pro file:

CONFIG += debug in the .pro file

Add these in a source code file wherever you want to print a trace:

#include <QtDebug>
qDebug() << "myTrace";


2) Using cout and cerr

Add this line in the toMOTko.pro file:

CONFIG += console in the .pro file

Add these in a source code file wherever you want to print a trace:

#include <iostream>
cout << "myTrace" << endl;


To print an instance of QString into cout or cerr, 2 solutions : 


Either: cout << qPrintable( myStr ) << endl;

Or: cout << myStr.toLatin1().data() << endl;

Or:

#include <QTextStream>
#include <stdio.h>

QTextStream cin(stdin, QIODevice::ReadOnly);
QTextStream cout(stdout, QIODevice::WriteOnly);
QTextStream cerr(stderr, QIODevice::WriteOnly);

cout << QString("bla bla");

Concerning imagePath for terms.  We must always consider the cases whether the path is absolute and inside toMOTko's internal directory.
