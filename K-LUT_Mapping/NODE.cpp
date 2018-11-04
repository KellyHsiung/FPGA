#include <iostream>
#include "NODE.h"

NODE::NODE(int id, int kind){
    this->ID = id;
    this->size = 0;
    this->kind = kind;
    this->done = false;
    this->fanin[0] = NULL;
    this->fanin[1] = NULL;
}

NODE::NODE(int id){
    this->ID = id;
    this->size = 0;
    this->kind = -1;
    this->done = false;
    this->fanin[0] = NULL;
    this->fanin[1] = NULL;
}

int NODE::getID(){ return this->ID; }
int NODE::getSize(){ return this->size; }
int NODE::getKind(){ return this->kind; }
bool NODE::getDone(){ return this->done; }
NODE* NODE::getFanin(int i){ return this->fanin[i]; }

void NODE::setSize(int s){ this->size = s; }
void NODE::setDone(bool d){ this->done = d; }
void NODE::setFanin(int i, NODE* f){ this->fanin[i] = f; }
void NODE::print(){
    std::cout << "ID: " << this->ID << std::endl;
    std::cout << "Size: " << this->size << std::endl;
    std::cout << "Kind: " << this->kind << std::endl;
    std::cout << "Done: " << this->done << std::endl;
    if(this->size == 0) return;
    std::cout << "Fanin 1: " << this->fanin[0]->getID();
    if(this->size == 2) std::cout << "\tFanin 2: " << this->fanin[1]->getID();
    std::cout << std::endl << std::endl;
}