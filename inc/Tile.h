#ifndef TILE_H
#define	TILE_H

#include <list>
#include <algorithm>
#include "MotionEstimationData.h"
#include "CTUGroup.h"

using namespace std;

class Tile {
private:
	Int xInitCTU;
	Int yInitCTU;
	Int wInCTU;
	Int hInCTU;
	Int id;
	
	Int ng;
	
	CTUData*** ctuTile;
	list<CTUGroup*> groupList;
	
	set<Int> refs;
	Int idCurrFrame;
	
	list<LLInt> memAccList;
	
	char classTile;
	
	void xAllocateTileData(TileData* data);
	void xParseCTU(TileData* data);
	void xCalcPredMemStatistics();
public:
	
	double mean, stdDev;
	
	Tile(MotionEstimationData* me, Int idTile, bool isFirstFrame);
	
	void setNumOfGroups(Int ng, double meanFrame, double stdDevFrame, bool isFirstFrame);
	CTUGroup* getNextGroup(bool isFirstFrame);
	bool hasAnyGroupToBeCoded();
	
	void classify(double meanFrame, double stdDevFrame);
	
	void test();
    list<LLInt> getMemAccList() const;
};

#endif	/* TILE_H */

