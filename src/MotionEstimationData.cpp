#include "../inc/MotionEstimationData.h"

MotionEstimationData::MotionEstimationData(Int idCurrFrame, Int numOfTiles) {
	this->idCurrFrame = idCurrFrame;
	this->numOfTiles = numOfTiles;
	this->refFrames.clear();
	
	tiles.resize(numOfTiles);
	for (int i = 0; i < numOfTiles; i++) {
		tiles[i] = NULL;
	}	
}

TileData* MotionEstimationData::getTile(Int idTile) {
	if( tiles[idTile] == NULL ) {
		tiles[idTile] = new TileData();
	}
	return tiles[idTile];
}

void MotionEstimationData::insertRefFrame(Int idRefFrame) {
    this->refFrames.insert(idRefFrame);
	
}

void MotionEstimationData::report() {
	for (int i = 0; i < this->tiles.size(); i++) {
        cout << "TILE " << i << endl;
		this->tiles[i]->report();
    }
}

void MotionEstimationData::setRefFrames(set<Int> refFrames) {
	this->refFrames = refFrames;
}

set<Int> MotionEstimationData::getRefFrames() const {
	return refFrames;
}

Int MotionEstimationData::getIdCurrFrame() const {
	return idCurrFrame;
}