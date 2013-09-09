#include "../inc/TileData.h"

TileData::TileData() {
	listCTU.clear();
}

void TileData::insertCTU(CTUData* ctu) {
	listCTU.push_back(ctu);
}

CTUData* TileData::getCTU(Int id) {
	if(id >= this->listCTU.size()) {
		return NULL;
	}
	return this->listCTU[id];
}

Int TileData::getNumOfCTU() {
	return this->listCTU.size();
}

void TileData::report() {
	cout << "CTUS " << this->listCTU.size() << endl;
	for(vector<CTUData*>::iterator it = listCTU.begin(); it != listCTU.end(); it++) {
		(*it)->report();
	}
}