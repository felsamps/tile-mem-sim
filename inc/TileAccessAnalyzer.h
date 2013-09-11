#ifndef TILEACCESSANALYZER_H
#define	TILEACCESSANALYZER_H

#include "TraceFileHandler.h"

using namespace std;

class TileAccessAnalyzer {
private:
	TraceFileHandler* tfh;
	
	/*memory access counters*/
	long long int accAccum;	
	
	void xAnalyzePUAccess(CUData* cu, PUData* pu);
	Int xGetNumOfAcc(CUData* cu, PUData* pu);
	void xReportAndResetCounters();
public:
	TileAccessAnalyzer(TraceFileHandler* tfh);
	
	void analyze();
};

#endif	/* TILEACCESSANALYZER_H */

