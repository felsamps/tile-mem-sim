#include "../inc/Tile.h"


Int NG_LIST[22][2] = {{0,0},{1,1},{2,1},{0,0},{2,2},{0,0},{3,2},{0,0},{4,2},{0,0},{5,2},{0,0},{4,3},{0,0},{7,2},{0,0},{4,4},{0,0},{6,3},{0,0},{5,4}};

Tile::Tile(MotionEstimationData* me, Int idTile, bool isFirstFrame) {
	this->id = idTile;
	this->refs = me->getRefFrames();
	this->idCurrFrame = me->getIdCurrFrame();
	this->memAccList.clear();
	
	TileData* data = me->getTile(this->id);
	
	xAllocateTileData(data);
	xParseCTU(data);
	if(isFirstFrame) {
		this->mean = 0;
		this->stdDev = 0;
	}
	else {
		xCalcPredMemStatistics();
	}
}

void Tile::xCalcPredMemStatistics() {
	LLInt accum = 0;
	for (int y = 0; y < this->hInCTU; y++) {
		for (int x = 0; x < this->wInCTU; x++) {
			Int xPos = this->ctuTile[x][y]->getX() / CTU_SIZE;
			Int yPos = this->ctuTile[x][y]->getY() / CTU_SIZE;
			LLInt mem = MemPredictionHandler::getCTUPredMem(this->idCurrFrame, xPos, yPos);
			this->memAccList.push_back(mem);
			accum += mem;
			//cout << "(" << mem << ") ";
		}
	}
	this->mean = accum / (this->hInCTU*this->wInCTU);
	
	double dAccum = 0.0;
	for (int y = 0; y < this->hInCTU; y++) {
		for (int x = 0; x < this->wInCTU; x++) {
			Int xPos = this->ctuTile[x][y]->getX() / CTU_SIZE;
			Int yPos = this->ctuTile[x][y]->getY() / CTU_SIZE;
			LLInt mem = MemPredictionHandler::getCTUPredMem(this->idCurrFrame, xPos, yPos);
			dAccum += pow(mem - this->mean, 2);
		}
	}
	this->stdDev = 	pow(dAccum / (this->hInCTU*this->wInCTU), 0.5);
	//cout << "TILE STATS " << (LLInt)this->mean << " " <<  (LLInt)this->stdDev << endl;
	//getchar();
}

void Tile::xAllocateTileData(TileData* data) {
	//scan for the tile properties
	Int xMin=999, xMax=0, yMin=999, yMax=0;
	for (int idCTU = 0; idCTU < data->getNumOfCTU(); idCTU++) {
		CTUData* ctu = data->getCTU(idCTU);
		Int xCTU = ctu->getX() / CTU_SIZE;
		Int yCTU = ctu->getY() / CTU_SIZE;
		
		xMin = (xCTU < xMin) ? xCTU : xMin ;		
		yMin = (yCTU < yMin) ? yCTU : yMin ;
		xMax = (xCTU > xMax) ? xCTU : xMax ;		
		yMax = (yCTU > yMax) ? yCTU : yMax;		
	}
	this->xInitCTU = xMin;
	this->yInitCTU = yMin;
	this->wInCTU = xMax - xMin + 1;
	this->hInCTU = yMax - yMin + 1;
	
	//cout << this->xInitCTU << " ";
	//cout << this->yInitCTU << " ";
	//cout << this->wInCTU << " ";
	//cout << this->hInCTU << endl;
	
	this->ctuTile = new CTUData** [this->wInCTU];
	for (int x = 0; x < this->wInCTU; x++) {
		this->ctuTile[x] = new CTUData* [this->hInCTU];
	}

}

void Tile::xParseCTU(TileData* data) {
	for (int idCTU = 0; idCTU < data->getNumOfCTU(); idCTU++) {
		CTUData* ctu = data->getCTU(idCTU);
		Int xCTU = (ctu->getX() / CTU_SIZE) - this->xInitCTU;
		Int yCTU = (ctu->getY() / CTU_SIZE) - this->yInitCTU;
		
		this->ctuTile[xCTU][yCTU] = ctu;
	}
}

bool compare(CTUGroup* first, CTUGroup* second) {
	return first->mean < second->mean;
}

void Tile::classify(double meanFrame, double stdDevFrame) {
	classTile = 
			(mean > (meanFrame + stdDevFrame * TILE_CLASS_FACTOR)) ? 'H' :
			((meanFrame + stdDevFrame * TILE_CLASS_FACTOR > mean) and (mean > meanFrame - stdDevFrame * TILE_CLASS_FACTOR)) ? 'M' :
          /*(meanFrame - stdDevFrame * TILE_CLASS_FACTOR > mean												 )*/ 'L' ;
	//cout << "TILE CLASSIFICATION " << this->id << " " << classTile << endl;
}

void Tile::setNumOfGroups(Int ng, double meanFrame, double stdDevFrame, bool isFirstFrame) {
	
	/*if(!isFirstFrame) {
		double normStdDev = this->stdDev / this->mean;
		double normStdDevFrame = stdDevFrame / meanFrame;
		double factor = (normStdDevFrame - normStdDev) / 5;
		Int factorInt = (this->wInCTU*this->hInCTU) * factor;

		ng += (this->stdDev > stdDevFrame) ? factorInt : -factorInt;
		ng = (ng/2) * 2;

		cout << "TILE " << this->id << " NG EQUALS TO " << factorInt << " " << factor << " " << ng << endl;
		
		//getchar();
	}*/
	
	Int xNg = NG_LIST[ng][0];
	Int yNg = NG_LIST[ng][1];
	

	Int wSize = this->wInCTU / xNg;
	Int hSize = this->hInCTU / yNg;
	
	Int id = 0;

	for (int y = 0; y < yNg; y++) {
		for (int x = 0; x < xNg; x++) {
			Int wActualSize = wSize;
			Int hActualSize = hSize;
			if(x == xNg-1) {
				wActualSize = this->wInCTU - x*wSize;
			}
			if(y == yNg-1) {
				hActualSize = this->hInCTU - y*hSize;
			}
			CTUGroup* group = new CTUGroup(id, this->ctuTile, x*wSize, y*hSize, wActualSize, hActualSize, this->refs, this->idCurrFrame, isFirstFrame);
			this->groupList.push_back(group);
			id ++;
		}
	}
	
	this->groupList.sort(compare);
	
		
}

CTUGroup* Tile::getNextGroup(bool isFirstFrame) {
	CTUGroup* returnable;
	
	if(isFirstFrame) {
		returnable = this->groupList.front();
		this->groupList.pop_front();
	}
	else {
		switch (classTile) {
			case 'H':
				returnable = this->groupList.back();
				this->groupList.pop_back();
				break;
			case 'M':
				break;
			case 'L':
				returnable = this->groupList.front();
				this->groupList.pop_front();
				break;
		}
	}
	
	//cout << "NEW GROUP IS DISPATCHED " << returnable->id << endl;
	//getchar();
	return returnable;
}

bool Tile::hasAnyGroupToBeCoded() {
	return (this->groupList.size() > 0);
}

void Tile::test() {
	for (int y = 0; y < this->hInCTU; y++) {
		for (int x = 0; x < this->wInCTU; x++) {
			CTUData* ctu = this->ctuTile[x][y];
			cout << ctu->getX() << " " << ctu->getY() << endl;
		}
	}

}

list<LLInt> Tile::getMemAccList() const {
	return memAccList;
}