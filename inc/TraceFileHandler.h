#ifndef TRACEFILEHANDLER_H
#define	TRACEFILEHANDLER_H

#include <fstream>
#include <vector>
#include <map>
#include <cmath>

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
	
	Int numOfFrames;
	Int wFrame, wFrameInCTU, wFrameInCU32;
	Int hFrame, hFrameInCTU, hFrameInCU32;
	Int numTileColumns;
	Int numTileRows;
	Int searchRange;
	Int numOfTiles;
	Int numVerTilesBoundaries;
	Int numHorTilesBoundaries;

	void xParseHeader();
	MotionEstimationData* xGetMotionEstimationDataEntry(Int idRefFrame);
    
public:
	TraceFileHandler(string name, Int numOfFrames);
	MotionEstimationData* parseNextFrame();
	
	Int getNumHorTilesBoundaries() const;
    Int getNumVerTilesBoundaries() const;
    Int getNumOfTiles() const;
    Int getSearchRange() const;
    Int getNumTileRows() const;
    Int getNumTileColumns() const;
    Int getHFrame() const;
    Int getWFrame() const;
    void setHFrameInCU32(Int hFrameInCU32);
    Int getHFrameInCU32() const;
    Int getWFrameInCU32() const;
    Int getWFrameInCTU() const;
    Int getHFrameInCTU() const;
    Int getNumOfFrames() const;
};

#endif	/* TRACEFILEHANDLER_H */

