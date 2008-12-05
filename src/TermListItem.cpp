#include "TermListItem.h"

TermListItem::TermListItem( QTreeWidget* parent, Term* term, const QString& firstLanguage, const QString& testLanguage, bool isAltShown /*= true*/ )
    : QTreeWidgetItem( parent ), term( term ), firstLanguage( firstLanguage ), testLanguage( testLanguage ), 
        altShown( isAltShown ) {
    updateUi();
}

TermListItem::~TermListItem() {
}

void TermListItem::updateUi() {
    if( term ) {
        // It's important to setCheckState() first.  Otherwise, if a change is made too soon, 
        // it may reset the markedForStudy property by an event.
        setCheckState( 0, term->isMarkedForStudy() ? Qt::Checked : Qt::Unchecked );
        if( term->isTranslationExists( firstLanguage ) ) {
            const Translation& firstLangTranslation = term->getTranslation( firstLanguage );
            setText( 0, firstLangTranslation.getWord() );
            setForeground( 0, QBrush( treeWidget()->palette().color( QPalette::Active, QPalette::WindowText ) ) );
        }
        else {
            setText( 0, QApplication::translate( "QObject", "Undefined" ) );
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
            setText( 1, QApplication::translate( "QObject", "Undefined" ) );
            setForeground( 1, QBrush( treeWidget()->palette().color( QPalette::Disabled, QPalette::WindowText ) ) );
        }
    }
}

void TermListItem::setOn( bool isOn ) {
    term->setMarkedForStudy( isOn );
}

Term* TermListItem::getTerm() {
    return( term );
}

void TermListItem::setFont( int column, const QFont& font ) {
    fonts[ column ] = font;
}

QFont TermListItem::font( int column ) const {
    if( fonts.contains( column ) )
        return( fonts[ column ] );
    else
        return( treeWidget()->font() );
}
