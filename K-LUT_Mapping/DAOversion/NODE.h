#ifndef NODE_H
#define NODE_H

#include <vector>
class NODE{
public:
    NODE(int);
    NODE(int, int);
    int getID();
    int getInSize();
    int getOutSize();
    int getKind();
    int getOrder();
    bool getDone();
    NODE* getFanin(int);
    NODE* getFanout(int);

    void setInSize(int);
    void setOrder(int);
    void setDone(bool);
    void setFanin(int, NODE*);

    void addFanout(NODE*);
    void print();
private:
    int ID;
    int FaninSize, FanoutSize;
    int kind;   //0: PI, 1: PO, 2: gate
    int order;
    bool done;
    NODE* fanin[2];
    std::vector<NODE *> fanout;
};

#endif