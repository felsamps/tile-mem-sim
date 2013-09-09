#ifndef PUDATA_H
#define	PUDATA_H

#include <list>
#include <cstdlib>

#include "TypeDefs.h"
#include "Entry.h"

using namespace std;

class PUData {
private:
    Int idPart, sizePart, idRefFrame;
	Int xCenter, yCenter;
    list<Entry*> entries;
public:
    PUData(Int idPart, Int sizePart, Int idRefFrame);
    
    void insertEntry(Entry* e);
    Entry* getEntry();
    
    void setIdRefFrame(Int idRefFrame);
    Int getIdRefFrame() const;
    void setSizePart(Int sizePart);
    Int getSizePart() const;
    void setIdPart(Int idPart);
    Int getIdPart() const;
    Int getYCenter() const;
    Int getXCenter() const;

};

#endif	/* PUDATA_H */

