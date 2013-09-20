#include "../inc/TileAccessAnalyzer.h"

#define NUM_TZ_FIRST_SEARCH_16 44
#define NUM_TZ_FIRST_SEARCH_32 60
#define NUM_TZ_FIRST_SEARCH_64 76
#define NUM_TZ_FIRST_SEARCH_96 76
#define NUM_TZ_FIRST_SEARCH_128 76

extern Int TZ_SEARCH_CANDIDATES[76][2];

TileAccessAnalyzer::TileAccessAnalyzer(TraceFileHandler* tfh) {
	this->tfh = tfh;	
	this->accAccum = 0;
	accumMemPressVec.clear();
	accumMemPressVec.resize(tfh->getNumOfTiles());
	
}


Int TileAccessAnalyzer::xGetNumOfAcc(CUData* cu, PUData* pu) {
	Int sizeCU = cu->getSize() * cu->getSize();
	Int sizePU;
	switch(pu->getSizePart()) {
		case SIZE_2Nx2N:
			sizePU = sizeCU;
			break;
		case SIZE_Nx2N:
			sizePU = sizeCU/2;
			break;
		case SIZE_2NxN:
			sizePU = sizeCU/2;
			break;
		case SIZE_NxN:
			sizePU = sizeCU/4;
			break;
		case SIZE_2NxnU:
			sizePU = (pu->getIdPart() == 0) ? sizeCU*(1/4) : sizeCU*(3/4);
			break;
		case SIZE_2NxnD:
			sizePU = (pu->getIdPart() == 0) ? sizeCU*(3/4) : sizeCU*(1/4);
			break;
		case SIZE_nLx2N:
			sizePU = (pu->getIdPart() == 0) ? sizeCU*(1/4) : sizeCU*(3/4);
			break;
		case SIZE_nRx2N:
			sizePU = (pu->getIdPart() == 0) ? sizeCU*(3/4) : sizeCU*(1/4);
			break;
	}
	return sizePU;
}


void TileAccessAnalyzer::xAnalyzePUAccess(CUData* cu, PUData* pu) {
	Entry* e = pu->getEntry();
	Int numOfCandidates;
	
	Int numOfAcc = xGetNumOfAcc(cu, pu);
	
	while(e != NULL) {
		switch(e->opcode) {
			case 'F':
				numOfCandidates = (this->tfh->getSearchRange() == 16) ? NUM_TZ_FIRST_SEARCH_16 :
						(this->tfh->getSearchRange() == 32) ? NUM_TZ_FIRST_SEARCH_32 :
						(this->tfh->getSearchRange() == 64) ? NUM_TZ_FIRST_SEARCH_64 :
						(this->tfh->getSearchRange() == 96) ? NUM_TZ_FIRST_SEARCH_96 :
						NUM_TZ_FIRST_SEARCH_128;
				
				this->accAccum += (numOfCandidates * numOfAcc);

				break;
			case 'C':
				this->accAccum += numOfAcc;
				break;
			case 'R':
				Int hor = e->xRight - e->xLeft;
				Int ver = e->yBottom - e->yTop;
				this->accAccum += (hor*ver);				
				break;
		}
		e = pu->getEntry();
	}
}

void TileAccessAnalyzer::analyze() {
	MotionEstimationData* me;
	me = this->tfh->parseNextFrame();
	
	while(me != NULL) {
		for (int t = 0; t < this->tfh->getNumOfTiles(); t++) {
			TileData* tile = me->getTile(t);
			for (int idCTU = 0; idCTU < tile->getNumOfCTU(); idCTU++) {
				CTUData* ctu = tile->getCTU(idCTU);
				
				for (int idCU = 0; idCU < ctu->getNumOfCU(); idCU++) {
					CUData* cu = ctu->getCU(idCU);
					set<Int> refFrames = me->getRefFrames();
					for(set<Int>::iterator it = refFrames.begin(); it != refFrames.end(); it++) {
						Int idRefFrame = (*it);
						PUData* pu = cu->getPU(idRefFrame);
						while(pu != NULL) {
							xAnalyzePUAccess(cu, pu);
							pu = cu->getPU(idRefFrame);
						}
					}
				}
				this->accumMemPressVec[t].push_back(this->accAccum);
				this->accAccum = 0;
			}
			
		}
		xReportAndResetCounters(me->getIdCurrFrame());
		me = this->tfh->parseNextFrame();
	}
}


void TileAccessAnalyzer::xReportAndResetCounters(Int idCurrFrame) {
	Int maxSize = 0;
	vector<Int> accumPress;
	for (int i = 0; i < this->tfh->getNumOfTiles(); i++) {
		maxSize = (this->accumMemPressVec[i].size() > maxSize) ? this->accumMemPressVec[i].size() : maxSize;
	}
	for (int a = 0; a < maxSize; a++) {
		Int accum = 0;
		bool insertionFlag = true;
		for (int i = 0; i < this->tfh->getNumOfTiles(); i++) {
			if(this->accumMemPressVec[i].size() < a) {
				insertionFlag = false;
				break;
			}
			accum += this->accumMemPressVec[i][a];
		}
		//cout << endl;
		if(insertionFlag) {
			accumPress.push_back(accum);
		}
	}
	
	double accum = 0;
	for (int i = 0; i < accumPress.size(); i++) {
		accum += accumPress[i];
	}
	double average = accum / accumPress.size();
	
	double dAccum = 0;
	for (int i = 0; i < accumPress.size(); i++) {
		dAccum += pow(average - accumPress[i], 2);
	}
	double stdDev = (dAccum / accumPress.size());
	
	cout << idCurrFrame << ";" << (average/(1024*1024)) << ";" << (LLInt)(stdDev/(1024*1024)) << endl;
	
	for (int i = 0; i < this->tfh->getNumOfTiles(); i++) {
		this->accumMemPressVec[i].clear();
	}

	
}
