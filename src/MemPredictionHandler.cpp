#include "../inc/MemPredictionHandler.h"

TraceFileHandler* MemPredictionHandler::tfh;
	
LLInt*** MemPredictionHandler::ctuPredMemPressure;
LLInt*** MemPredictionHandler::cuPredMemPressure;		//32x32
LLInt*** MemPredictionHandler::ctuActualMemPressure;
LLInt*** MemPredictionHandler::cuActualMemPressure;	//32x32

double*** MemPredictionHandler::cuPredSearchWindowUsage;	//32x32
double*** MemPredictionHandler::cuActualSearchWindowUsage;	//32x32

fstream MemPredictionHandler::fp;
fstream MemPredictionHandler::fp1;

MemPredictionHandler::MemPredictionHandler() {
}

void MemPredictionHandler::init(TraceFileHandler* t) {
	tfh = t;
	ctuPredMemPressure = new LLInt** [tfh->getNumOfFrames()];
	ctuActualMemPressure = new LLInt** [tfh->getNumOfFrames()];
	cuPredMemPressure = new LLInt** [tfh->getNumOfFrames()];
	cuActualMemPressure = new LLInt** [tfh->getNumOfFrames()];
	cuPredSearchWindowUsage = new double** [tfh->getNumOfFrames()];
	cuActualSearchWindowUsage = new double** [tfh->getNumOfFrames()];
	for (int f = 0; f < tfh->getNumOfFrames(); f++) {
		ctuPredMemPressure[f] = new LLInt* [tfh->getWFrameInCTU()];
		ctuActualMemPressure[f] = new LLInt* [tfh->getWFrameInCTU()];
		for (int x = 0; x < tfh->getWFrameInCTU(); x++) {
			ctuPredMemPressure[f][x] = new LLInt [tfh->getHFrameInCTU()];
			ctuActualMemPressure[f][x] = new LLInt [tfh->getHFrameInCTU()];			
		}
		cuPredMemPressure[f] = new LLInt* [tfh->getWFrameInCU32()];
		cuActualMemPressure[f] = new LLInt* [tfh->getWFrameInCU32()];
		cuPredSearchWindowUsage[f] = new double*[tfh->getWFrameInCU32()];
		cuActualSearchWindowUsage[f] = new double*[tfh->getWFrameInCU32()];
		for (int x = 0; x < tfh->getWFrameInCU32(); x++) {
			cuPredMemPressure[f][x] = new LLInt [tfh->getHFrameInCU32()];
			cuActualMemPressure[f][x] = new LLInt [tfh->getHFrameInCU32()];
			cuPredSearchWindowUsage[f][x] = new double[tfh->getHFrameInCU32()];
			cuActualSearchWindowUsage[f][x] = new double[tfh->getHFrameInCU32()];
		}
	}
	
	fp.open("balanced_pressure.csv", fstream::out);
	fp1.open("summary.csv", fstream::out);
}

void MemPredictionHandler::predictCTUMem(Int f, set<Int> refs) {
	//cout << "PREDICTING FRAME " << f << " " << refs.size() << endl;
	
	refs.erase(0);
	for (int y = 0; y < tfh->getHFrameInCTU(); y++) {
		for (int x = 0; x < tfh->getWFrameInCTU(); x++) {
			double prediction = 0;
			
			for(set<Int>::iterator it = refs.begin(); it != refs.end(); it ++) {
				Int idRefFrame = (*it);
				prediction += ctuActualMemPressure[idRefFrame][x][y];
			}
			prediction /= refs.size();
			ctuPredMemPressure[f][x][y] = (LLInt) prediction;
		}
	}
	refs.insert(0);
	
}

void MemPredictionHandler::reportActualCTU(Int f) {
	for (int y = 0; y < tfh->getHFrameInCTU(); y++) {
		for (int x = 0; x < tfh->getWFrameInCTU(); x++) {
			cout << ctuActualMemPressure[f][x][y] << " ";
		}
		cout << endl;
	}
}


