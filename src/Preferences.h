#ifndef PREFERENCES_H
#define PREFERENCES_H 

#include <iostream>
#include <QFile>
#include <QFileInfo>
#include <QFont>
#include <QMap>
#include <QObject>
#include <QtGlobal>
#include <QList>
#include <QString>
#include "Sequence.h"
#include "Util.h"

using namespace std;

class Preferences : public QObject {

    Q_OBJECT

public:

    static const qint32 magicNumber;

    enum { SHORTEST, SHORT, MEDIUM, LONG, LONGEST };

    typedef QList<Sequence> SequenceList;

    static const uint fontSizeList[];

    Preferences();
    ~Preferences(); 

    bool load();
    bool save();

    uint getQuizLength() const;
    void setQuizLength( uint quizLength );

    void clearRevealingSequences();
    void addRevealingSequence( Sequence sequence );
    int getRevealingSequence( const QString& seqStr );
    void removeRevealingSequence( int index );
    void removeRevealingSequence( const QString& seqStr );
    int getActiveRevealingSequenceCount();
    int getRevealingSequenceCount() const;
    Sequence getRevealingSequenceAt( int index );

    QString getInterfaceLanguage() const;
    void setInterfaceLanguage( const QString& interfaceLanguage );

    bool isDigraphEnabled() const;
    void setDigraphEnabled( bool isEnabled );

    bool areQuizButtonsHidden() const;
    void setQuizButtonsHidden( bool areHidden );

    bool isAltInTermListShown() const;
    void setAltInTermListShown( bool isShown );

    QString getFirstLanguage() const;
    void setFirstLanguage( const QString& firstLanguage );

    QList<QString> getStudyLanguages();
    bool isStudyLanguage( const QString& language );
    void addStudyLanguage( const QString& language );
    void removeStudyLanguage( const QString& language );
    void clearStudyLanguages();

    QString getTestLanguage() const;
    void setTestLanguage( const QString& testLanguage );

    QString getFontFamily() const;
    void setFontFamily( const QString& fontFamily );
    QString getDefaultFontFamily() const;
    int getDefaultFontSizeModifier() const;
    int getFontSizeModifier() const;
    void setFontSizeModifier( int fontSizeModifier );

    QFont getLargeFont( const QString& language = QString::null ) const;
    QFont getMediumFont( const QString& language = QString::null ) const;
    QFont getSmallFont( const QString& language = QString::null ) const;
    QFont getBestFont( const QString& lang1, const QString& lang2 ) const;

    QString getLabelsFontFamily() const;
    void setLabelsFontFamily( const QString& labelsFontFamily );
    QString getDefaultLabelsFontFamily() const;
    int getDefaultLabelsFontSizeModifier() const;
    int getLabelsFontSizeModifier() const;
    void setLabelsFontSizeModifier( int labelsFontSizeModifier );
    QFont getLabelsFont() const;

    QString getFontOverrideFamily( const QString& language ) const;
    int getFontOverrideSize( const QString& language ) const;
    void setFontOverrideFamily( const QString& language, const QString& fontFamily );
    void setFontOverrideSize( const QString& language, int fontSize );
    bool isFontOverrideFamilyDefined( const QString& language ) const;
    bool isFontOverrideSizeDefined( const QString& language ) const;
    void clearFontOverrideFamilies();
    void clearFontOverrideSizes();

    bool isLanguageFilterEnabled() const;
    void setLanguageFilterEnabled( bool isEnabled );

    bool isFolderOpen( int folderId ) const;
    void setFolderOpen( int folderId, bool isOpen );

    void setApplicationDirName( const QString& applDir );

    int getDefaultAccelerator( Action action );
    void setAccelerator( Action action, int accel );
    int getAccelerator( Action action );

signals:

    void studyLanguageSelected( QString language );
    void studyLanguageDeselected( QString language );

private:

    void initDefaultKeyboardAccelerators();
    QFont getFont( const QString& fontFamily, uint size ) const;

    uint                    quizLength;
    SequenceList            sequences;

    QString                 interfaceLanguage;
    bool                    digraphEnabled;
    bool                    quizButtonsHidden;
    bool                    altInTermListShown;

    QString                 firstLanguage;
    QList<QString>          studyLanguages;
    QString                 testLanguage;

    QString                 labelsFontFamily;
    int                     labelsFontSizeModifier;

    QString                 fontFamily;
    int                     fontSizeModifier;

    QMap<QString,QString>   fontOverrideFamilies;
    QMap<QString,int>       fontOverrideSizes;

    bool                    languageFilterEnabled;

    QList<int>              closedFolders;

    QString                 prefsXmlFilename;
    QString                 prefsFilename;

    QMap<int,int>           defaultAccel;
    QMap<int,int>           accel;

};

#endif
