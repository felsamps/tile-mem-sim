#ifndef ENTRY_H
#define	ENTRY_H

class Entry {
public: 
    char opcode;
    Int xFS, yFS;
    Int xCand, yCand;
    Int xLeft, xRight, yTop, yBottom;
    
    Entry() {};
};


#endif	/* ENTRY_H */

