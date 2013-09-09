#ifndef TILEDATA_H
#define	TILEDATA_H

#include <list>

#include "CTUData.h"

using namespace std;

class TileData {
private:
    vector<CTUData*> listCTU;
public:
    TileData();
    
    void insertCTU(CTUData* ctu);
    CTUData* getCTU(Int id);
	
	Int getNumOfCTU();
    
    void report();
};

#endif	/* TILEDATA_H */

