#ifndef MEMPREDICTIONHANDLER_H
#define	MEMPREDICTIONHANDLER_H

#include "TypeDefs.h"
#include "TraceFileHandler.h"

using namespace std;

class MemPredictionHandler {
private:
	
	static TraceFileHandler* tfh;
	
	static LLInt*** ctuPredMemPressure;
	static LLInt*** cuPredMemPressure;		//32x32
	static LLInt*** ctuActualMemPressure;
	static LLInt*** cuActualMemPressure;	//32x32
	
	static double*** cuPredSearchWindowUsage;	//32x32
	static double*** cuActualSearchWindowUsage;	//32x32
	
	
public:
	
	static fstream fp;
	static fstream fp1;
	
	MemPredictionHandler();
	
	static void init(TraceFileHandler* t);
	static void insertCTUPredMem(Int f, Int x, Int y, LLInt value) { ctuPredMemPressure[f][x][y] = value; }
	static void insertCTUActualMem(Int f, Int x, Int y, LLInt value) { ctuActualMemPressure[f][x][y] = value; }
	
	static void insertCUPredMem(Int f, Int x, Int y, LLInt value) { cuPredMemPressure[f][x][y] = value; }
	static void insertCUActualMem(Int f, Int x, Int y, LLInt value) { cuActualMemPressure[f][x][y] = value; }
	
	static void insertCUPredSWUsage(Int f, Int x, Int y, double value) { cuPredSearchWindowUsage[f][x][y] = value; }
	static void insertCUActualSWUsage(Int f, Int x, Int y, double value) { cuActualSearchWindowUsage[f][x][y] = value; }
	
	static LLInt getCTUPredMem(Int f, Int x, Int y) { return ctuPredMemPressure[f][x][y]; }
	static LLInt getCTUActualMem(Int f, Int x, Int y) { return ctuActualMemPressure[f][x][y]; }

	static LLInt getCUPredMem(Int f, Int x, Int y) { return cuPredMemPressure[f][x][y]; }
	static LLInt getCUActualMem(Int f, Int x, Int y) { return cuActualMemPressure[f][x][y]; }
	
	static double getCUPredSWUsage(Int f, Int x, Int y) { return cuPredSearchWindowUsage[f][x][y]; }
	static double getCUActualSWUsage(Int f, Int x, Int y) { return cuActualSearchWindowUsage[f][x][y]; }
	
	static void predictCTUMem(Int f, set<Int> refs);
	static void reportActualCTU(Int f);
	
};

#endif	/* MEMPREDICTIONHANDLER_H */

