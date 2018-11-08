#ifndef NODE_H
#define NODE_H

#include <vector>
class NODE{
public:
    NODE(int);
    NODE(int, int);
    int getID();
    int getSize();
    int getKind();
    bool getDone();
    NODE* getFanin(int);

    void setSize(int);
    //void setKind(int);
    void setDone(bool);
    void setFanin(int, NODE*);
    void print();
private:
    int ID;
    int size;
    int kind;   //0: PI, 1: PO, 2: gate
    bool done;
    NODE* fanin[2];
};

#endif