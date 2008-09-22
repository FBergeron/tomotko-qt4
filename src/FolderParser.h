#ifndef FOLDER_PARSER_H
#define FOLDER_PARSER_H

#include <QMessageBox>
#include <QXmlAttributes>
#include <QXmlParseException>
#include "Folder.h"

class FolderParser : public QXmlDefaultHandler {

public:

    FolderParser( Folder& vocabulary );
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString&, const QXmlAttributes& attribs );
    bool characters( const QString& characters );
    bool endElement( const QString&, const QString&, const QString& qname );
    bool endDocument();

    bool fatalError( const QXmlParseException& exception );

    bool isVocabularyFile();

private:

    QString desc;

    QString tempCh;
    bool mustKeepText;

    Folder& folder;
    bool isFolderFile;

};

#endif
