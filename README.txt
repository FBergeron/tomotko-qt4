README
======

Prerequisites
-------------
To build the application, you will need the open-source version of QT4.4 (or better).
You can download it from TrollTech's website or if you're using Linux or MacOS, you 
probably can get it using the package manager of your operating system.

You will also need a C++ compiler and some other miscellaneous development tools.


Windows XP/2000/Vista
---------------------
To extract localized string :

> lupdate toMOTko.pro

To generate the string files :

> lrelease toMOTko.pro

These 2 operations should be done at least once before building the Makefile‥
Otherwise, you will get errors like these when running qmake:

RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/en/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/fr/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/es/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/ja/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/zh/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/de/toMOTko.qm'

To build the Makefile :
> toMOTko.pro

To build the executable (the debuggable version) :

> make clean
> make 

To build the executable (the release version) :

> make clean
> make release 

To deploy the required resources into debug and release directories :

> bin\deploy.rb

To run the executable (the debuggable version) :

> debug\toMOTko.exe

To run the executable (the release version) :

> release\toMOTko.exe

To make the installer setup.exe file :

- Launch InstallJammer
- Open etc/InstallJammer/toMOTko/toMOTko.mpi
- Select Build (the file should be located in the output directory.)


Linux
-----
To extract localized string :

> lupdate toMOTko.pro

To generate the string files :

> lrelease toMOTko.pro

These 2 operations should be done at least once before building the Makefile‥
Otherwise, you will get errors like these when running qmake:

RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/en/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/fr/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/es/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/ja/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/zh/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/de/toMOTko.qm'

To build the Makefile :

> qmake toMOTko.pro

To build the executable :

> make clean; 
> make

To run the executable :

> ./toMOTko

To make the deb file :

> bin/makeDebFile.rb

To make the rpm file :

> bin/makeRpmFile.rb


Windows CE 
----------
To build toMOTko for Windows CE, the process is a little bit more complicated.  Here are the operations I did.  Hopefully, I haven't forgotten anything...

First, we need to build Qt for the desired target platform.  In my case, I was interested to build for Windows Mobile 6.0.  I downloaded the Qt source code: qt-embedded-wince-opensource-4.4.3.


MacOS
-----
I build the SDK from source.  I decompressed and extracted the archive in /Developer.  I use a screen session because it takes about 2 hours to build the SDK.

screen
sudo -s
cd /Developer/qt-mac-opensource-src-4.4.3
make clean
make confclean
./configure -universal -sdk /Developer/SDKs/MacOSX10.4u.sdk
make
make install
exit

That's it.  At this point, the SDK is built and installed.

This is not needed but, just in case, if you want to make a static version of Qt library, here is how to do:

screen
sudo -s
cd /Developer/qt-mac-opensource-src-4.4.3
make clean
make confclean
./configure -universal -static -sdk /Developer/SDKs/MacOSX10.4u.sdk
make sub-src
exit

At the end of this process, the file libQtGui.a is built and can be used to build the application.  I will not use it though.


To extract localized string :

> lupdate toMOTko.pro

To generate the string files :

> lrelease toMOTko.pro

These 2 operations should be done at least once before building the Makefile.
Otherwise, you will get errors like these when running qmake:

RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/en/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/fr/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/es/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/ja/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/zh/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/de/toMOTko.qm'

To build the Makefile :

> qmake -macx -config release toMOTko.pro

To build the executable :

> make clean; 
> make

To run the executable :

> ./toMOTko.app/Contents/MacOS/tomotko

To make the application package that it can be deployed later:

> bin/makeDmgFile.rb


Maemo
-----
I'm using Ubuntu 9.04 running inside VirtualBox.

Before compiling and building the application, it's required to install scratchbox, the Maemo SDK, and Xephyr X11-Server.  
This is out of scope of this document.  For more details, read installation documentation on Maemo's website.

To start scratchbox :

> sudo -s
> echo 0 > /proc/sys/vm/vdso_enabled
> exit
> /scratchbox/login

To select the architecture (either i386 or ARMEL), in scratchbox :

> sb-menu
* KillAll | 15 | OK
* Select | <arch> | OK

To launch the Xephyr X11-Server (from a new terminal window):

> Xephyr :2 -host-cursor -screen 800x480x16 -dpi 96 -ac -extension Composite

Once that Xephyr is running properly, set up the DISPLAY variable into scratchbox:

> export DISPLAY=:2

To launch the Hildon Application Framework, in scratchbox :

> af-sb-init.sh start

To extract localized string, in scratchbox (1) :

> lupdate toMOTko.pro

To generate the string files, in scratchbox (1) :

> lrelease toMOTko.pro

These 2 operations should be done at least once before building the Makefile‥
Otherwise, you will get errors like these when running qmake:

RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/en/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/fr/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/es/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/ja/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/zh/toMOTko.qm'
RCC: Error in 'toMOTko.qrc': Cannot find file 'i18n/de/toMOTko.qm'

To build the Makefile, in scratchbox (1) :

> qmake toMOTko.pro

To build the executable, in scratchbox (1) :

> make clean
> make 

To run the application, in scratchbox :

> ./toMOTko

Or even better (so that it looks even more like a real Hildon application):

> run-standalone.sh ./toMOTko

When I run the application, on ARMEL, it doesn't work very well.  Many errors are issued concerning the pixmaps :

X Error: BadDrawable (invalid Pixmap or Window parameter) 9
  Major opcode: 62 (X_CopyArea)
  Resource id:  0x1f6fb0

I suspect that it's because I'm running Ubuntu under VirtualBox but it's not confirmed.

Also, the first time I invoked the QFileDialog, I got this error:

D-Bus library appears to be incorrectly set up; failed to open "/var/lib/dbus/machine-id": No such file or directory.

To fix this, in scratchbox:

dbus-uuidgen --ensure

To print QString in maemo to debug the application, I use this:

cerr << aString.toStdString() << endl;

To make the binary deb file for i386 architecture, first compile and build the executable for i386 architecture then, outside scratchbox :

> cd /scratchbox/users/fred/home/fred/tomotko
> /home/fred/tomotko/qt4/trunk/bin/makeMaemoDebFile.rb i386

To make the binary deb file for armel architecture, first compile and build the executable for armel architecture then, outside scratchbox :

> cd /scratchbox/users/fred/home/fred/tomotko
> /home/fred/tomotko/qt4/trunk/bin/makeMaemoDebFile.rb armel

To submit the application to Maemo's Extras repository, it must first be submitted to Extras-Devel repository.

To generate the required files to do so is not an easy task.  I managed to do it once but I'm not even sure I will be able to succeed again.  Here are the steps that are required to perform:

> cp -r trunk $SCRATCHBOX_HOME/tomotko-x.y.z

In Scratchbox (no matter the architecture), clean up the source directory.  It must be perfectly clean!!!  Even a hidden file could disturb the procedure.  Pay special attention to .swp files created by vim while editing files:

> cd ~/tomotko-x.y.z
> make clean
> rm toMOTko
> rm ...

Then, the debian directory for Maemo must be copied into the source directory and the files must be generated.

> cp -r etc/MaemoDeb/debian . 
> dpkg-buildpackage -rfakeroot -sa -S

3 files should be produced in $SCRATCHBOX_HOME:

tomotko_x.y.z-w_source.changes
tomotko_x.y.z-w.dsc
tomotko_x.y.z-w.tar.gz

These files must be uploaded and submitted to the Maemo Extras Assistant.  Other methods are available but have never been tried yet. 

If everything goes fine, the binary deb files will be built and made available through the extras-devel repository.

If it fails, fix something and repeat the procedure.

The next step is to promote the package to extras.  Look the Maemo documentation for this.


Notes
-----
(1) In ARMEL, if you get the error nmap: Permission denied when issuing this command, the following command is needed:

> sudo -s
> echo 4096 > /proc/sys/vm/mmap_min_addr
> exit



What to do before a release
---------------------------
- Update version number in the About dialog and scripts (makeDebFile.rb, makeMacOSFile.rb, and makeRpmFile.rb).
- Update online documentation if necessary (screenshots, new features, etc.)
- Remove traces if any.
- Build installers
    - Windows: make clean;make release;make clean; then run InstallJammer after updating the Install Version and Version String in Application Information to get setup.exe
    - MacOS: make clean;make;bin/makeDmgFile.rb to get toMOTko-x.y.z.dmg
    - Linux: make clean;make;bin/makeDevFile.rb to get tomotko-x.y.z_i386.deb
    - Test them!
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
- Add a group and/or category in the SF's Bug Tracker system.
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
