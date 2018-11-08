#include <iostream>
#include "LUT.h"


LUT::LUT(int id){ this->PO_ID = id; }

int LUT::getID(){ return this->PO_ID; }

void LUT::addMember(int m){ this->fanin.push_back(m); }


void LUT::print(){
    std::cout<< this->PO_ID << " ";
    for(auto it = this->fanin.begin(); it != this->fanin.end(); ++it, ++it){
        std::cout << (*it) << " ";
    }
    std::cout << std::endl;
}