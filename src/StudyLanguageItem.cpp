#include "StudyLanguageItem.h"

StudyLanguageItem::StudyLanguageItem( QTreeWidget* parent, LanguageUpdateNotifier* languageUpdateNotifier )
    : QTreeWidgetItem( parent/*, "StudyLanguageItem", CheckBox*/ ), languageUpdateNotifier( languageUpdateNotifier ) {
    //setSelectable( false );
}

StudyLanguageItem::~StudyLanguageItem() {
}

void StudyLanguageItem::stateChange( bool isOn ) {
    //QCheckListItem::stateChange( isOn );
    //if( isOn )
    //    languageUpdateNotifier->addLanguage( Util::getLanguageCode( text() ) );
    //else
    //    languageUpdateNotifier->removeLanguage( Util::getLanguageCode( text() ) );
}
