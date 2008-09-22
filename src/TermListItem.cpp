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
        }
        else
            setText( 0, QApplication::translate( "QObject", "Undefined" ) );

        if( term->isTranslationExists( testLanguage ) ) {
            const Translation& testLangTranslation = term->getTranslation( testLanguage );
            QString testLangStr = testLangTranslation.getWord(); 
            if( altShown && testLangTranslation.getAlt().length() > 0 )
                testLangStr += " [" + testLangTranslation.getAlt() + "]";
            setText( 1, testLangStr );
        }
        else
            setText( 1, QApplication::translate( "QObject", "Undefined" ) );
    }
}

void TermListItem::setOn( bool isOn ) {
    //setCheckState( 0, isOn ? Qt::Checked : Qt::Unchecked );
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

//void TermListItem::paintCell( QPainter* p, const QColorGroup& cg, int column, int width, int align ) {
//    if( !p )
//        return;
//
//    QFont oldFont = p->font();
//    p->setFont( font( column ) );
//
//    QTreeWidgetItem::paintCell( p, cg, column, width, align );
//
//    QColorGroup colorGroup( cg );
//    switch( column ) {
//        case 0 : 
//            if( term && !term->isTranslationExists( firstLanguage ) )
//                colorGroup.setColor( QColorGroup::Text, listView()->palette().color( QPalette::Disabled, QColorGroup::Text ) );
//            break;
//
//        case 1 :
//            if( term && !term->isTranslationExists( testLanguage ) )
//                colorGroup.setColor( QColorGroup::Text, listView()->palette().color( QPalette::Disabled, QColorGroup::Text ) );
//            break;
//    }
//
//    QListViewItem::paintCell( p, colorGroup, column, width, align );
//
//    p->setFont( oldFont );
//}
