#include "../inc/CTUGroup.h"

#define NUM_TZ_FIRST_SEARCH_16 44
#define NUM_TZ_FIRST_SEARCH_32 60
#define NUM_TZ_FIRST_SEARCH_64 76
#define NUM_TZ_FIRST_SEARCH_96 76
#define NUM_TZ_FIRST_SEARCH_128 76

Int TZ_SEARCH_CANDIDATES[76][2] = {{0,-1}, {-1,0}, {1,0}, {0,1}, {0,-2}, {-1,-1}, {1,-1}, {-2,0}, {2,0}, {-1,1}, {1,1}, {0,2}, {0,-4}, {-2,-2}, {2,-2}, {-4,0}, {4,0}, {-2,2}, {2,2}, {0,4}, {0,-8}, {-4,-4}, {4,-4}, {-8,0}, {8,0}, {-4,4}, {4,4}, {0,8}, {0,-16}, {-16,0}, {16,0}, {0,16}, {-4,-12}, {4,-12}, {-4,12}, {4,12}, {-8,-8}, {8,-8}, {-8,8}, {8,8}, {-12,-4}, {12,-4}, {-12,4}, {12,4}, {0,-32}, {-32,0}, {32,0}, {0,32}, {-8,-24}, {8,-24}, {-8,24}, {8,24}, {-16,-16}, {16,-16}, {-16,16}, {16,16}, {-24,-8}, {24,-8}, {-24,8}, {24,8}, {0,-64}, {-64,0}, {64,0}, {0,64}, {-16,-48}, {16,-48}, {-16,48}, {16,48}, {-32,-32}, {32,-32}, {-32,32}, {32,32}, {-48,-16}, {48,-16}, {-48,16}, {48,16}};

CTUGroup::CTUGroup(Int id, CTUData*** ctuTile, Int xInit, Int yInit, Int wInCTU, Int hInCTU, set<Int> refs, Int idCurrFrame, bool isFirstFrame) {
	
	this->id = id;
	this->xInit = xInit;
	this->yInit = yInit;
	this->wInCTU = wInCTU;
	this->hInCTU = hInCTU;
	
	this->refs = refs;
	this->idCurrFrame = idCurrFrame;
		
	this->ctuGroup = new CTUData**[wInCTU];
	for (int x = 0; x < wInCTU; x++) {
		this->ctuGroup[x] = new CTUData*[this->hInCTU];
	}
	
	for (int y = 0; y < hInCTU; y++) {
		for (int x = 0; x < wInCTU; x++) {
			this->ctuGroup[x][y] = ctuTile[x+xInit][y+yInit];
		}
	}
	
	//cout << "NEW CTU GROUP ";
	//cout << this->xInit << " ";
	//cout << this->yInit << " ";
	//cout << this->wInCTU << " ";
	//cout << this->hInCTU << endl;
	
	//TODO create order list!! (1st solution: simpler approach)
	this->currX = 0;
	this->currY = 0;
	
	this->accAccum = 0;
	
	if(isFirstFrame) {
		this->mean = 0;
		this->stdDev = 0;
	}
	else {
		xCalcPredMemStatistics();
	}
}

void CTUGroup::xCalcPredMemStatistics() {
	LLInt accum = 0;
	for (int y = 0; y < this->hInCTU; y++) {
		for (int x = 0; x < this->wInCTU; x++) {
			Int xPos = this->ctuGroup[x][y]->getX() / CTU_SIZE;
			Int yPos = this->ctuGroup[x][y]->getY() / CTU_SIZE;
			LLInt mem = MemPredictionHandler::getCTUPredMem(this->idCurrFrame, xPos, yPos);
			accum += mem;
		}
	}
	this->mean = accum / (this->hInCTU*this->wInCTU);
	
	double dAccum = 0.0;
	for (int y = 0; y < this->hInCTU; y++) {
		for (int x = 0; x < this->wInCTU; x++) {
			Int xPos = this->ctuGroup[x][y]->getX() / CTU_SIZE;
			Int yPos = this->ctuGroup[x][y]->getY() / CTU_SIZE;
			LLInt mem = MemPredictionHandler::getCTUPredMem(this->idCurrFrame, xPos, yPos);
			dAccum += pow(mem - this->mean, 2);
		}
	}
	this->stdDev = 	pow(dAccum / (this->hInCTU*this->wInCTU), 0.5);
	//cout << "CTU-GROUP STATS " << (LLInt)this->mean << " " <<  (LLInt)this->stdDev << endl;
	//getchar();	
}



LLInt CTUGroup::codeNextCTU(TraceFileHandler* tfh) {
	/*TODO code next CTU*/
	//cout << "Coding (" << this->id << ")";
	CTUData* ctu = this->ctuGroup[this->currX][this->currY];
	//cout << " CTU " << ctu->getX() << " " << ctu->getY() << endl;
	//getchar();
	
	this->searchRange = tfh->getSearchRange();
	xSearchCTU(ctu);
	
	LLInt returnable = accAccum;
	MemPredictionHandler::insertCTUActualMem(this->idCurrFrame, ctu->getX()/CTU_SIZE, ctu->getY()/CTU_SIZE, this->accAccum);
	this->accAccum = 0;
	
	/*increase pointers*/
	this->currX = (this->currX == (this->wInCTU-1)) ? 0 : this->currX+1;
	this->currY += (this->currX == 0) ? 1 : 0;
	
	return returnable;

}

bool CTUGroup::hasAnyCTUToBeCoded() {
	return (this->currY != this->hInCTU);
}

void CTUGroup::xSearchCTU(CTUData* ctu) {
	for (int idCU = 0; idCU < ctu->getNumOfCU(); idCU++) {
		CUData* cu = ctu->getCU(idCU);
		for(set<Int>::iterator it = this->refs.begin(); it != this->refs.end(); it++) {
			Int idRefFrame = (*it);
			PUData* pu;
			while(pu = cu->getPU(idRefFrame)) {
				xSearchPU(cu, pu);
			}
		}		
	}
}

void CTUGroup::xSearchPU(CUData* cu, PUData* pu) {
	Entry* e;
	Int numOfCandidates;
	
	Int numOfAcc = xGetNumOfAcc(cu, pu);
	
	while(e = pu->getEntry()) {
		switch(e->opcode) {
			case 'F':
				numOfCandidates = (this->searchRange == 16) ? NUM_TZ_FIRST_SEARCH_16 :
						(this->searchRange == 32) ? NUM_TZ_FIRST_SEARCH_32 :
						(this->searchRange == 64) ? NUM_TZ_FIRST_SEARCH_64 :
						(this->searchRange == 96) ? NUM_TZ_FIRST_SEARCH_96 :
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
		
	}
}


Int CTUGroup::xGetNumOfAcc(CUData* cu, PUData* pu) {
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