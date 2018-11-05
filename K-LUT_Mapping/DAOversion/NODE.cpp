#include <iostream>
#include "NODE.h"

NODE::NODE(int id, int kind){
    this->ID = id;
    this->FaninSize = 0;
    this->FanoutSize = 0;
    this->order = -1;
    this->kind = kind;
    this->done = false;
    this->fanin[0] = NULL;
    this->fanin[1] = NULL;
}

NODE::NODE(int id){
    this->ID = id;
    this->FaninSize = 0;
    this->FanoutSize = 0;
    this->kind = -1;
    this->order = -1;
    this->done = false;
    this->fanin[0] = NULL;
    this->fanin[1] = NULL;
}

int NODE::getID(){ return this->ID; }
int NODE::getInSize(){ return this->FaninSize; }
int NODE::getOutSize(){ return this->fanout.size(); }
int NODE::getKind(){ return this->kind; }
int NODE::getOrder(){ return this->order; }
bool NODE::getDone(){ return this->done; }
NODE* NODE::getFanin(int i){ return this->fanin[i]; }
NODE* NODE::getFanout(int i){ return this->fanout[i]; }

void NODE::setInSize(int s){ this->FaninSize = s; }
void NODE::setOrder(int o){ this->order = o; }
void NODE::setDone(bool d){ this->done = d; }
void NODE::setFanin(int i, NODE *f){ this->fanin[i] = f; }

void NODE::addFanout(NODE *n){ this->fanout.push_back(n); }

void NODE::print(){
    std::cout << "ID: " << this->ID << std::endl;
    std::cout << "Order: " << this->order << std::endl;
    std::cout << "Kind: " << this->kind << std::endl;
    std::cout << "Done: " << this->done << std::endl;
    std::cout << "Fanin size: " << this->FaninSize << std::endl;
    if(this->FaninSize != 0) std::cout << "Fanin 1: " << this->fanin[0]->getID();
    if(this->FaninSize == 2) std::cout << "\tFanin 2: " << this->fanin[1]->getID();
    std::cout << std::endl;
    
    int i=1;
    std::cout << "Fanout size: " << this->fanout.size() << std::endl;
    for(auto it = this->fanout.begin(); it != this->fanout.end(); ++it, ++i){
        std::cout << "Fanout " << i << ": " << (*it)->getID() << "\t";
        if( i%5 == 0) std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}