#ifndef CTUDATA_H
#define	CTUDATA_H

#include <iostream>
#include <vector>
#include "CUData.h"

using namespace std;

class CTUData {
private:
	vector<CUData*> listCU;
    Int x, y;
public:
    CTUData(Int x, Int y);
    
    void insertCU(CUData* cu);
    
    CUData* getCU(Int id);
	Int getNumOfCU();
    
    void setY(Int y);
    Int getY() const;
    void setX(Int x);
    Int getX() const;
    
    void report();
    
};

#endif	/* CTUDATA_H */

