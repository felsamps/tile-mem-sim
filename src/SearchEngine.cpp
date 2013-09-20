#include "../inc/SearchEngine.h"

SearchEngine::SearchEngine(TraceFileHandler* tfh) {
	this->tfh = tfh;
	this->accumMemPressVec.clear();
}

vector<Tile*> SearchEngine::xParseTiles(MotionEstimationData* me, bool isFirstFrame) {
	vector<Tile*> returnable;	
	for (int t = 0; t < this->tfh->getNumOfTiles(); t++) {
		Tile *tile = new Tile(me, t, isFirstFrame);
		returnable.push_back(tile);
	}
	return returnable;
}

void SearchEngine::xCalcPredMemStatisticsFrame(vector<Tile*> tiles, bool isFirstFrame) {
	
	if(!isFirstFrame) {
		list<LLInt> accList;

		for (int i = 0; i < tiles.size(); i++) {
			list<LLInt> tileList = tiles[i]->getMemAccList();
			for(list<LLInt>::iterator it = tileList.begin(); it != tileList.end(); it++) {
				accList.push_back(*it);
			}
		}

		double accum = 0;
		for (list<LLInt>::iterator it = accList.begin(); it != accList.end(); it++) {
			accum += (*it);
		}
		this->meanFrame = accum / (accList.size());
		double dAccum = 0.0;
		for (list<LLInt>::iterator it = accList.begin(); it != accList.end(); it++) {		
				dAccum += pow((*it) - this->meanFrame, 2);
		}
	
		this->stdDevFrame = pow(dAccum/accList.size(), 0.5);
		this->normStdDevFrame = this->stdDevFrame / this->meanFrame;
	}
}

Int SearchEngine::xCalcNBase() {
	double factor = (this->stdDevFrame / this->meanFrame) / N_BASE_CONST;
	Int ctuPerTile = (this->tfh->getHFrameInCTU() * this->tfh->getWFrameInCTU()) / this->tfh->getNumOfTiles();
	Int factorInt = ((Int)(factor * ctuPerTile) / 2) * 2;
	return factorInt;
}

void SearchEngine::search() {
	MotionEstimationData* me;
	bool isFirstFrame;
	while(me = this->tfh->parseNextFrame()) {
		isFirstFrame = (me->getIdCurrFrame() == 8) ? true : false;
		//cout << "INIT FRAME " << me->getIdCurrFrame() << endl;
		
		if(!isFirstFrame) {
			MemPredictionHandler::predictCTUMem(me->getIdCurrFrame(), me->getRefFrames());
		}
				
		vector<Tile*> tilesToBeProcessed = xParseTiles(me, isFirstFrame);
		xCalcPredMemStatisticsFrame(tilesToBeProcessed, isFirstFrame);
		
		if(isFirstFrame) { //first frame
			/*TODO raster order*/
			for (int t = 0; t < tilesToBeProcessed.size(); t++) {
				tilesToBeProcessed[t]->setNumOfGroups(BASE_NUMBER_OF_GROUPS, this->meanFrame, this->stdDevFrame, isFirstFrame);
			}
		}
		else {
			/*TODO memory pressure prediction*/
			for (int t = 0; t < tilesToBeProcessed.size(); t++) {
				tilesToBeProcessed[t]->classify(this->meanFrame, this->stdDevFrame);
				tilesToBeProcessed[t]->setNumOfGroups(BASE_NUMBER_OF_GROUPS, this->meanFrame, this->stdDevFrame, isFirstFrame);
			}
			/*TODO CTU-groups*/
		}	
		xSearchFrame(tilesToBeProcessed, isFirstFrame);
		//MemPredictionHandler::reportActualCTU(me->getIdCurrFrame());
		xReportStatistics(me->getIdCurrFrame());
		
	}
	
}

void SearchEngine::xSearchFrame(vector<Tile*> tiles, bool isFirstFrame) {
	vector<CTUGroup*> groups(tiles.size());
	bool initFlag = true;
	while(1) {

		/*TODO test if there is any assigned group or if there are remaining CTUs to be coded*/
		for (int t = 0; t < tiles.size(); t++) {
			if(initFlag) {
				groups[t] = tiles[t]->getNextGroup(isFirstFrame);
			}
			else {
				if(groups[t] != NULL) {
					if(!groups[t]->hasAnyCTUToBeCoded()) {
						if(tiles[t]->hasAnyGroupToBeCoded()) {
							groups[t] = tiles[t]->getNextGroup(isFirstFrame);
						}
						else {
							groups[t] = NULL;
						}
					}
				}
			}
			
		}
		initFlag = false;
		
		vector<LLInt> memPress;
		for (int t = 0; t < groups.size(); t++) {
			if(groups[t] != NULL) {
				memPress.push_back(groups[t]->codeNextCTU(this->tfh));
			}
			else {
				memPress.push_back(0);
			}
		}
		
		LLInt accumPress = 0;
		bool insertionFlag = true;
		for (int i = 0; i < memPress.size(); i++) {
			if(memPress[i] == 0) {
				insertionFlag = false;
				break;
			}
			accumPress += memPress[i];
			MemPredictionHandler::fp << memPress[i] << ";" ;
		}
		if(insertionFlag) {
			this->accumMemPressVec.push_back(accumPress);
		}

		MemPredictionHandler::fp << endl;
		
		/*TODO break condition*/
		Int terminate = 0;
		for (int t = 0; t < tiles.size(); t++) {
			terminate += (groups[t] == NULL) ? 1 : 0;
		}
		if(terminate == tiles.size()) {
			break;
		}
	}
}

void SearchEngine::xReportStatistics(Int idCurrFrame) {
	double accum = 0;
	for (int i = 0; i < this->accumMemPressVec.size(); i++) {
		accum += this->accumMemPressVec[i];
	}
	double average = accum / this->accumMemPressVec.size();
	
	double dAccum = 0;
	for (int i = 0; i < this->accumMemPressVec.size(); i++) {
		dAccum += pow(average - this->accumMemPressVec[i], 2);
	}
	double stdDev = (dAccum / this->accumMemPressVec.size());
	
	MemPredictionHandler::fp1 << idCurrFrame << ";" << (average/(1024*1024)) << ";" << (LLInt)(stdDev/(1024*1024)) << endl;
	
	this->accumMemPressVec.clear();
}