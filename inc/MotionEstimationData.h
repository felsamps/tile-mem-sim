#ifndef MOTIONESTIMATIONDATA_H
#define	MOTIONESTIMATIONDATA_H

#include <vector>
#include <cstdlib>
#include <set>
#include <iostream>
#include <cstdio>

#include "TypeDefs.h"
#include "TileData.h"


using namespace std;

class MotionEstimationData {
private:
    vector<TileData*> tiles;
    set<Int> refFrames;
    
    Int numOfTiles, idCurrFrame;
	    
public:
    MotionEstimationData(Int idCurrFrame, Int numOfTiles);
    
    TileData* getTile(Int idTile);
    void insertRefFrame(Int idRefFrame);
    
    void report();
    void setRefFrames(set<Int> refFrames);
    set<Int> getRefFrames() const;
    Int getIdCurrFrame() const;
    
};

#endif	/* MOTIONESTIMATIONDATA_H */

