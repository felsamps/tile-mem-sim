#include "../inc/PUData.h"

PUData::PUData(Int idPart, Int sizePart, Int idRefFrame){
	this->idPart = idPart;
	this->sizePart = sizePart;
	this->idRefFrame = idRefFrame;
	this->entries.clear();
}

void PUData::insertEntry(Entry* e) {
	if(e->opcode == 'F') {
		this->xCenter = e->xFS;
		this->yCenter = e->yFS;
	}
	entries.push_back(e);
}

Entry* PUData::getEntry() {
	if(this->entries.empty()) {
		return NULL;
	}
	Entry* returnable = this->entries.front();
	this->entries.pop_front();
	return returnable;
}

void PUData::setIdRefFrame(Int idRefFrame) {
	this->idRefFrame = idRefFrame;
}

Int PUData::getIdRefFrame() const {
	return idRefFrame;
}

void PUData::setSizePart(Int sizePart) {
	this->sizePart = sizePart;
}

Int PUData::getSizePart() const {
	return sizePart;
}

void PUData::setIdPart(Int idPart) {
	this->idPart = idPart;
}

Int PUData::getIdPart() const {
	return idPart;
}

Int PUData::getYCenter() const {
	return yCenter;
}

Int PUData::getXCenter() const {
	return xCenter;
}

