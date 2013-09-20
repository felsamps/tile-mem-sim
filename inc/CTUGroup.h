#ifndef CTUGROUP_H
#define	CTUGROUP_H

#include <cstdio>
#include "CTUData.h"
#include "TraceFileHandler.h"
#include "MemPredictionHandler.h"

using namespace std;

class CTUGroup {
private:
	
	
	Int xInit;
	Int yInit; 
	Int wInCTU;
	Int hInCTU;
	
	Int currX;
	Int currY;
	
	LLInt accAccum;
	
	CTUData*** ctuGroup;
	
	Int searchRange;
	set<Int> refs;
	Int idCurrFrame;
	
	Int xGetNumOfAcc(CUData* cu, PUData* pu);
	void xSearchPU(CUData* cu, PUData* pu);
	void xSearchCTU(CTUData* ctu);
	
	void xCalcPredMemStatistics();
public:
	
	Int id;
	double mean, stdDev;
	
	CTUGroup(Int id, CTUData*** ctuTile, Int xInit, Int yInit, Int wInCTU, Int hInCTU, set<Int> refs, Int idCurrFrame, bool isFirstFrame);
	
	void search();
	
	LLInt codeNextCTU(TraceFileHandler* tfh);
	bool hasAnyCTUToBeCoded();
	
	bool operator<(const CTUGroup *rhs) const { return mean < rhs->mean; }
	
};

#endif	/* CTUGROUP_H */

