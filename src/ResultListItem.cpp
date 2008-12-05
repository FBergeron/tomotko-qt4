#include "ResultListItem.h"

ResultListItem::ResultListItem( QTreeWidget* parent, Term* term, const QString& firstLanguage, const QString& testLanguage, const QString& vocabTitle, const QString& location, bool isAltShown /* = true */ )
    : QTreeWidgetItem( parent ), term( term ), firstLanguage( firstLanguage ), testLanguage( testLanguage ), 
        vocabTitle( vocabTitle ), location( location ), altShown( isAltShown ) {
    updateUi();
}

ResultListItem::~ResultListItem() {
}

void ResultListItem::updateUi() {
    if( term ) {
        if( term->isTranslationExists( firstLanguage ) ) {
            const Translation& firstLangTranslation = term->getTranslation( firstLanguage );
            setText( 0, firstLangTranslation.getWord() );
            setForeground( 0, QBrush( treeWidget()->palette().color( QPalette::Active, QPalette::WindowText ) ) );
        }
        else {
            setText( 0, QObject::tr( "Undefined" ) );
            setForeground( 0, QBrush( treeWidget()->palette().color( QPalette::Disabled, QPalette::WindowText ) ) );
        }

        if( term->isTranslationExists( testLanguage ) ) {
            const Translation& testLangTranslation = term->getTranslation( testLanguage );
            QString testLangStr = testLangTranslation.getWord(); 
            if( altShown && testLangTranslation.getAlt().length() > 0 )
                testLangStr += " [" + testLangTranslation.getAlt() + "]";
            setText( 1, testLangStr );
            setForeground( 1, QBrush( treeWidget()->palette().color( QPalette::Active, QPalette::WindowText ) ) );
        }
        else {
            setText( 1, QObject::tr( "Undefined" ) );
            setForeground( 1, QBrush( treeWidget()->palette().color( QPalette::Disabled, QPalette::WindowText ) ) );
        }

        setText( 2, vocabTitle );
        setText( 3, location );
    }
}

Term* ResultListItem::getTerm() {
    return( term );
}

void ResultListItem::setFont( int column, const QFont& font ) {
    fonts[ column ] = font;
}

QFont ResultListItem::font( int column ) const {
    if( fonts.contains( column ) )
        return( fonts[ column ] );
    else
        return( treeWidget()->font() );
}
