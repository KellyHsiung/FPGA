#ifndef LUT_H
#define LUT_H

#include <vector>
class LUT{
public:
    LUT(int);
    int getID();
    int getSize();
    int getFanin(int);

    void addMember(int);
    void print();
private:
    int PO_ID;
    std::vector<int> fanin;
};

#endif