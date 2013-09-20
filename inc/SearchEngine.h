#ifndef SEARCHENGINE_H
#define	SEARCHENGINE_H

#include "TraceFileHandler.h"
#include "MemPredictionHandler.h"
#include "TraceFileHandler.h"
#include "MemPredictionHandler.h"
#include "Tile.h"
#include "CTUGroup.h"


class SearchEngine {
private:
	TraceFileHandler* tfh;
	
	LLInt accAccum;
	vector<LLInt> accumMemPressVec;
	
	double meanFrame, stdDevFrame, normStdDevFrame;

	void xSearchFrame(vector<Tile*> tiles, bool isFirstFrame);
	void xSearchPU(CUData* cu, PUData* pu);
	Int xGetNumOfAcc(CUData* cu, PUData* pu);
	vector<Tile*> xParseTiles(MotionEstimationData* me, bool isFirstFrame);
	void xCalcPredMemStatisticsFrame(vector<Tile*> tiles, bool isFirstFrame);
	Int xCalcNBase();
	void xReportStatistics(Int idCurrFrame);
	
public:
	SearchEngine(TraceFileHandler* tfh);
	
	void search();

};

#endif	/* SEARCHENGINE_H */

