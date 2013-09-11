#ifndef CUDATA_H
#define	CUDATA_H

#include <iostream>
#include <list>
#include <map>

#include "TypeDefs.h"
#include "PUData.h"

using namespace std;

class CUData {
private:
    map<Int, list<PUData*> > refsPU;
    Int size, depth, x, y;
public:
    CUData(Int x, Int y, Int depth);
    
    void insertPU(PUData* pu);
	PUData* getPU(Int idRefFrame);
	
    void setY(Int y);
    Int getY() const;
    void setX(Int x);
    Int getX() const;
    void setDepth(Int depth);
    Int getDepth() const;
    Int getSize() const;    
    
	void report();
	
};

#endif	/* CUDATA_H */

