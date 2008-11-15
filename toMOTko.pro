######################################################################
# Automatically generated by qmake (1.07a) Sat Nov 11 11:30:37 2006
######################################################################

CONFIG      +=  console zlib
#CONFIG     +=  debug
TEMPLATE    =   app
DEPENDPATH  +=  src
INCLUDEPATH +=  . src
LIBS        +=  -lstdc++
QT          +=  xml
RESOURCES   =   toMOTko.qrc

HEADERS     +=  src/Base.h \
                src/Util.h \
                src/ImageBox.h \
                src/BilingualKey.h \
                src/DigraphLineEdit.h \
                src/HelpBrowser.h \
#               src/ScrollableLineEdit.h \
#               src/ScrollableMultiLineEdit.h \
                src/DigraphMultiLineEdit.h \
                src/Sequence.h \
                src/Preferences.h \
#               src/MarkedItemsParser.h \
                src/PropertiesPanel.h \
                src/TermDialog.h \
                src/TranslationSelectionDialog.h \
                src/ResultListItem.h \
                src/HistoryField.h \
                src/SearchDialog.h \
                src/KeyActionListViewItem.h \
                src/SequenceDialog.h \
                src/SequenceListItem.h \
                src/PreferencesDialog.h \
                src/Controller.h \
                src/MainWindow.h \
                src/PixMap.h \
                src/QuizFrame.h \
                src/VocabularyManagerFrame.h \
                src/Term.h \
                src/TermKey.h \
                src/Folder.h \
                src/TermListItem.h \
                src/StandbyTerm.h \
                src/TermScheduler.h \
                src/Translation.h \
                src/SmartListView.h \
#               src/ContentListView.h \
                src/VocabTreeView.h \
                src/TreeItem.h \
                src/VocabTreeItem.h \
                src/FolderTreeItem.h \
                src/FolderParser.h \
                src/VocabParser.h \
                src/Vocabulary.h \
                src/minizip/crypt.h \
                src/minizip/ioapi.h \
                src/minizip/zip.h \
                src/minizip/unzip.h \
                src/zconf.h \
                src/zlib.h

SOURCES     +=  src/Util.cpp \ 
                src/ImageBox.cpp \
                src/BilingualKey.cpp \
                src/DigraphLineEdit.cpp \
                src/HelpBrowser.cpp \
                #src/ScrollableLineEdit.cpp \
                #src/ScrollableMultiLineEdit.cpp \
                src/DigraphMultiLineEdit.cpp \
                src/Sequence.cpp \
                src/Preferences.cpp \
                #src/MarkedItemsParser.cpp \
                src/PropertiesPanel.cpp \
                src/TermDialog.cpp \
                src/TranslationSelectionDialog.cpp \
                src/ResultListItem.cpp \
                src/HistoryField.cpp \
                src/SearchDialog.cpp \
                src/SequenceDialog.cpp \
                src/SequenceListItem.cpp \
                src/PreferencesDialog.cpp \
                src/Controller.cpp \
                src/main.cpp \
                src/MainWindow.cpp \
                src/PixMap.cpp \
                src/QuizFrame.cpp \
                src/VocabularyManagerFrame.cpp \
                src/Term.cpp \
                src/TermKey.cpp \
                src/Folder.cpp \
                src/TermListItem.cpp \
                src/StandbyTerm.cpp \
                src/TermScheduler.cpp \
                src/Translation.cpp \
                src/SmartListView.cpp \
                #src/ContentListView.cpp \
                src/VocabTreeView.cpp \
                src/TreeItem.cpp \
                src/VocabTreeItem.cpp \
                src/FolderTreeItem.cpp \
                src/FolderParser.cpp \
                src/VocabParser.cpp \
                src/Vocabulary.cpp \
                src/minizip/ioapi.c \
                src/minizip/zip.c \
                src/minizip/unzip.c

TRANSLATIONS += i18n/en/toMOTko.ts \
                i18n/fr/toMOTko.ts \
                i18n/ja/toMOTko.ts \
                i18n/es/toMOTko.ts \
                i18n/zh/toMOTko.ts
