#include "../inc/CUData.h"

CUData::CUData(Int x, Int y, Int depth) {
	this->depth = depth;
	this->x = x;
	this->y = y;
	this->size = (depth == 0) ? 64 :
		(depth == 1) ? 32 :
		(depth == 2) ? 16 :
		8;
}

void CUData::insertPU(PUData* pu) {
	Int idRefFrame = pu->getIdRefFrame();
	this->refsPU[idRefFrame] = pu;	
}

PUData* CUData::getPU(Int idRefFrame) {
	if( this->refsPU.find(idRefFrame) != this->refsPU.end() ) {
		return this->refsPU[idRefFrame];
	}
	else {
		return NULL;
	}
	
}

void CUData::report() {
	cout << "PUs " << this->refsPU.size() << endl;
}

void CUData::setY(Int y) {
	this->y = y;
}

Int CUData::getY() const {
	return y;
}

void CUData::setX(Int x) {
	this->x = x;
}

Int CUData::getX() const {
	return x;
}

void CUData::setDepth(Int depth) {
	this->depth = depth;
}

Int CUData::getDepth() const {
	return depth;
}

Int CUData::getSize() const {
	return size;
}

