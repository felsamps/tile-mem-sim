#include "../inc/TraceFileHandler.h"

TraceFileHandler::TraceFileHandler(string name) {
	this->fp.open(name.c_str(), fstream::in);		
	this->xParseHeader();
}

void TraceFileHandler::xParseHeader() {
	this->fp >> this->wFrame;
	this->fp >> this->hFrame;
	this->fp >> this->numTileColumns;
	this->fp >> this->numTileRows;
	this->fp >> this->searchRange;
		
	this->numOfTiles = this->numTileColumns * this->numTileRows;
	this->numVerTilesBoundaries = this->numTileColumns - 1;
	this->numHorTilesBoundaries = this->numTileRows - 1;
}

MotionEstimationData* TraceFileHandler::parseNextFrame() {
	MotionEstimationData* meData;
	TileData* tileData;
	CTUData* ctuData;
	CUData* cuData;
	PUData* puData;
	Entry* entry;
	
	Int idCurrFrame, idTile;
	Int xCTU, yCTU;
	Int xCU, yCU, idDepth;
	Int idPart, sizePart, idRefFrame;
	
	Int xFS, yFS;
	Int xCand, yCand;
	Int xLeft, xRight, yTop, yBottom;
	
	char command;
	
	while(!this->fp.eof()) {
		this->fp >> command;
		
		switch(command) {
			case 'I':	/*Begining of the frame (image)*/
				this->fp >> idCurrFrame;
				meData = new MotionEstimationData(idCurrFrame, this->numOfTiles);
				break;
			case 'i':	/*Ending of the frame (image)*/
				return meData;
				break;
			case 'L':	/*Begining of CTU*/
				this->fp >> xCTU >> yCTU >> idTile;
				tileData = meData->getTile(idTile);				
				ctuData = new CTUData(xCTU, yCTU);
				break;
			case 'E':	/*Ending of CTU*/
				tileData->insertCTU(ctuData);
				break;
				
			case 'U':	/*Begining of CU*/
				this->fp >> xCU >> yCU >> idDepth;
				cuData = new CUData(xCU, yCU, idDepth);
				
				break;
			case 'u':
				ctuData->insertCU(cuData);
				break;
				
			case 'P':	/*Begining of PU*/
				this->fp >> idPart >> sizePart >> idRefFrame;
				puData = new PUData(idPart, sizePart, idRefFrame);
				meData->insertRefFrame(idRefFrame);
				break;
				
			case 'p':
				if(sizePart == 0) {
					cuData->insertPU(puData);
				}
				break;
				
			case 'F':	/*TZ First Search*/
				fp >> xFS >> yFS;
				if( sizePart == 0 ) {
					entry = new Entry();
					entry->opcode = command;
					entry->xFS = xCU + xFS;
					entry->yFS = yCU + yFS;
					puData->insertEntry(entry);
				}
				break;
			case 'C':	/*TZ Candidate Search*/
				fp >> xCand >> yCand;
				if( sizePart == 0 ) {
					entry = new Entry();
					entry->opcode = command;
					entry->xCand = xCU + xCand;
					entry->yCand = yCU + yCand;
					puData->insertEntry(entry);
				}
				break;
			case 'R':	/*TZ Raster Search*/
				fp >> xLeft >> xRight >> yTop >> yBottom;
				if( sizePart == 0 ) {
					entry = new Entry();
					entry->opcode = command;
					entry->xLeft = xCU + xLeft;
					entry->xRight = xCU + xRight;
					entry->yTop = yCU + yTop;
					entry->yBottom = yCU + yBottom;
					puData->insertEntry(entry);
				}
				break;
		}
		
	}
	return NULL;
	
}

Int TraceFileHandler::getNumHorTilesBoundaries() const {
	return numHorTilesBoundaries;
}

Int TraceFileHandler::getNumVerTilesBoundaries() const {
	return numVerTilesBoundaries;
}

Int TraceFileHandler::getNumOfTiles() const {
	return numOfTiles;
}

Int TraceFileHandler::getSearchRange() const {
	return searchRange;
}

Int TraceFileHandler::getNumTileRows() const {
	return numTileRows;
}

Int TraceFileHandler::getNumTileColumns() const {
	return numTileColumns;
}

Int TraceFileHandler::getHFrame() const {
	return hFrame;
}

Int TraceFileHandler::getWFrame() const {
	return wFrame;
}