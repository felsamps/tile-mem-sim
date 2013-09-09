#include "../inc/CTUData.h"

CTUData::CTUData(Int x, Int y) {
	this->x = x;
	this->y = y;
	this->listCU.clear();
}

void CTUData::insertCU(CUData* cu) {
	Int depth = cu->getDepth();
	this->listCU.push_back(cu);
}

CUData* CTUData::getCU(Int id) {
	return this->listCU[id];
}

Int CTUData::getNumOfCU() {
	return this->listCU.size();
}

void CTUData::setY(Int y) {
	this->y = y;
}

Int CTUData::getY() const {
	return y;
}

void CTUData::setX(Int x) {
	this->x = x;
}

Int CTUData::getX() const {
	return x;
}


void CTUData::report() {
	for(vector<CUData*>::iterator it = this->listCU.begin(); it != this->listCU.end(); it++) {
		CUData* cu = (*it);
		cu->report();
	}
	
}