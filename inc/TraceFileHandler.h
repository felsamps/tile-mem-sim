#ifndef TRACEFILEHANDLER_H
#define	TRACEFILEHANDLER_H

#include <fstream>
#include <vector>
#include <map>

#include "MotionEstimationData.h"
#include "TypeDefs.h"
#include "CTUData.h"
#include "CUData.h"
#include "TileData.h"
#include "PUData.h"

using namespace std;

class TraceFileHandler {
private:
	fstream fp;
	
	Int wFrame;
	Int hFrame;
	Int numTileColumns;
	Int numTileRows;
	Int searchRange;
	Int numOfTiles;
	Int numVerTilesBoundaries;
	Int numHorTilesBoundaries;

	void xParseHeader();
	MotionEstimationData* xGetMotionEstimationDataEntry(Int idRefFrame);
    
public:
	TraceFileHandler(string name);
	MotionEstimationData* parseNextFrame();
    
	Int getNumHorTilesBoundaries() const;
    Int getNumVerTilesBoundaries() const;
    Int getNumOfTiles() const;
    Int getSearchRange() const;
    Int getNumTileRows() const;
    Int getNumTileColumns() const;
    Int getHFrame() const;
    Int getWFrame() const;
};

#endif	/* TRACEFILEHANDLER_H */

