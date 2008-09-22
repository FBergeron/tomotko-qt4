#ifndef STUDY_LANGUAGE_ITEM_H
#define STUDY_LANGUAGE_ITEM_H

#include <QString>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "LanguageUpdateNotifier.h"
#include "Util.h"

class StudyLanguageItem : public QTreeWidgetItem {

public:

    StudyLanguageItem( QTreeWidget* parent, LanguageUpdateNotifier* languageUpdateNotifier );
    ~StudyLanguageItem();

protected:

    void stateChange( bool isOn );

private:

    LanguageUpdateNotifier* languageUpdateNotifier;

};

#endif
