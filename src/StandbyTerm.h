#ifndef STANDBY_TERM_H
#define STANDBY_TERM_H

#include <QDataStream>
#include "TermKey.h"


class StandbyTerm {

public:

    StandbyTerm();
    StandbyTerm( const TermKey& termKey, uint pool );
    StandbyTerm( const StandbyTerm& standbyTerm );
    ~StandbyTerm();

    TermKey getKey() const;
    int getPool() const;
    
    int operator==( const StandbyTerm& term ) const;

private:

    TermKey key;
    int pool;

    friend QDataStream& operator<<( QDataStream& out, const StandbyTerm& term );
    friend QDataStream& operator>>( QDataStream& in, StandbyTerm& term );

};

#endif

