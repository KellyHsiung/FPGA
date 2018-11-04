#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <queue>
#include "NODE.h"

template <class T>
bool compare(T* const&one, T* const&two){
    return one->getID() < two->getID();
}
enum KIND { PI, PO, GATE };

int main(int argc, char** argv){

    if(argc != 4){
        std::cout<<"Error: The number of arguments is wrong!\n";
        std::cout<<"A input file, LUT size and output file must be given!\n";
        return -1;
    }

    std::fstream file(argv[1], std::ifstream::in);
    if( !file.is_open() ){
        std::cout<<"Error: Failed to open input file!\n";
        return -1;
    }
    std::string text;
    file >> text;
    if(text != "aag"){
        std::cout<<"Error: The format of input file is wrong!\n";
        file.close();
        return -1;
    }
    //read the content of input file
    std::vector<NODE *> nodes;
    int PINum, PONum, nodeNum, element;
    file >> nodeNum >> PINum >> PONum;
    //read PIs
    for(int i=0; i<PINum; ++i){
        file >> element;
        nodes.push_back( new NODE(element, KIND::PI) );
    }
    //read POs
    for(int i=0; i<PONum; ++i){
        file >> element;
        nodes.push_back( new NODE(element, KIND::PO) );
    }

    //read connection between gates and PI PO
    std::vector< std::vector<int> > fanin;
    getline(file, text);
    while(getline(file, text)){
        std::istringstream iss(text);
        int target, source;
        iss >> target;
        std::vector<int> temp;
        temp.push_back(target);
        while( iss>>source ) temp.push_back(source);
        fanin.push_back(temp);

        if(target > (PINum+PONum)){
            NODE *tempNode = new NODE(target, KIND::GATE);
            nodes.push_back(tempNode);
        }
    }
    file.close();

    std::sort(nodes.begin(), nodes.end(), compare<NODE>);
    for(auto FanIt = fanin.begin(); FanIt != fanin.end(); ++FanIt){
        nodes[ (*FanIt)[0]-1 ]->setFanin(0, nodes[ (*FanIt)[1]-1 ] );
        if( (*FanIt).size() == 3 ){
            nodes[ (*FanIt)[0]-1 ]->setFanin(1, nodes[ (*FanIt)[2]-1 ] );
            nodes[ (*FanIt)[0]-1 ]->setSize(2);
        }else nodes[ (*FanIt)[0]-1 ]->setSize(1);
    }
    // for(auto it = nodes.begin(); it != nodes.end(); ++it) 
    //     (*it)->print();


    //greedy
    int K = atoi(argv[2]);
    std::queue<NODE*> nodeQueue;
    for(auto it = nodes.begin(); it != nodes.end(); ++it){
        if((*it)->getKind() == KIND::PO)
            nodeQueue.push( (*it) );
    }
    file.open(argv[3], std::ifstream::out);
    while( !nodeQueue.empty() ){
        NODE *temp = nodeQueue.front();
        nodeQueue.pop();
        if( temp->getDone() ) continue;

        int size = temp->getSize();
        temp->setDone(true);
        file << temp->getID() <<" ";

        std::queue<NODE *> tempQueue;
        tempQueue.push(temp);
        while(size < K ){
            tempQueue.pop();
            for(int i=0; i<temp->getSize(); ++i){
                NODE *faninTemp = temp->getFanin(i);
                size += faninTemp->getSize();
                if(size >= K){
                    nodeQueue.push(faninTemp);
                    size -= faninTemp->getSize();
                    continue;
                }
                else{
                    faninTemp->setDone(true);
                    file << faninTemp->getID() <<" ";
                    if( faninTemp->getSize()!=0 ) tempQueue.push(faninTemp->getFanin(0) );
                    if( faninTemp->getSize()==2 ) tempQueue.push(faninTemp->getFanin(1) );
                }
            }
            if( tempQueue.empty() ) break;
            else temp = tempQueue.front();
        }
        file << std::endl;
    }
    file.close();

    return 0;
}