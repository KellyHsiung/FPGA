#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <stack>
#include <queue>
#include <set>
#include "NODE.h"
#include "LUT.h"

#define PI 0
#define PO 1
#define Gate 2

template <class T>
bool compare(T* const&one, T* const&two){
    return one->getID() < two->getID();
}
template <class T>
bool compareOrder(T* const&one, T* const&two){
    return one->getOrder() < two->getOrder();
}

bool find(std::vector<NODE *> &n, int &id){
    for(auto it = n.begin(); it != n.end(); ++it){
        if( (*it)->getID() == id ) return true;
    }
    return false;
}
void topologicalSort(std::vector<NODE *> &nodes);
void label(std::vector<NODE *> &nodes, const int &K);
void mapping(std::vector<NODE *> &nodes, std::set<LUT *> &luts);






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
    if(text != "agg"){
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
        nodes.push_back( new NODE(element, PI) );
    }
    //read POs
    for(int i=0; i<PONum; ++i){
        file >> element;
        nodes.push_back( new NODE(element, PO) );
    }

    //std::sort(nodes.begin(), nodes.end(), compare<NODE>);

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

        if( !find(nodes, target) ){
            NODE *tempNode = new NODE(target, Gate);
            nodes.push_back(tempNode);
        }
    }
    file.close();

    std::sort(nodes.begin(), nodes.end(), compare<NODE>);
    for(auto FanIt = fanin.begin(); FanIt != fanin.end(); ++FanIt){
        NODE *temp = new NODE((*FanIt)[0]);
        auto tempIter = lower_bound(nodes.begin(), nodes.end(), temp, compare<NODE>);
        
        NODE *fanTemp = new NODE((*FanIt)[1]);
        auto fanIter = lower_bound(nodes.begin(), nodes.end(), fanTemp, compare<NODE>);

        (*tempIter)->setFanin(0, (*fanIter) );
        (*fanIter)->addFanout( (*tempIter) );
        delete fanTemp;

        if( (*FanIt).size() == 3 ){
            fanTemp = new NODE((*FanIt)[2]);
            fanIter = lower_bound(nodes.begin(), nodes.end(), fanTemp, compare<NODE>);
            (*tempIter)->setFanin(1, (*fanIter) );
            (*fanIter)->addFanout( (*tempIter) );
            delete fanTemp;
            (*tempIter)->setInSize(2);
        }else (*tempIter)->setInSize(1);

        delete temp;
    }

    //sort the nodes by topological order
    topologicalSort(nodes);
    
    int K = atoi(argv[2]);
    label(nodes, K);
    
    std::set<LUT *> luts;
    mapping(nodes, luts);

    file.open(argv[3], std::ifstream::out);
    for(auto it = luts.begin(); it != luts.end(); ++it){
        file << (*it)->getID() << " ";
        for(int i = 0; i<(*it)->getSize(); ++i)
            file << (*it)->getFanin(i)<<" ";
        file<<std::endl;
    }
    file.close();

    return 0;
}


/* sort nodes by topological order */
void topologicalSort(std::vector<NODE *> &nodes){
    
    std::stack<NODE *> nodeStack;
    //push PIs into stack nodeStack
    for(auto it = nodes.begin(); it != nodes.end(); ++it){
        if((*it)->getKind() == PI) nodeStack.push( (*it) );
    }
    //use DFS to travel all nodes
    std::stack<NODE *> orderStack;
    while( !nodeStack.empty() ){
        NODE *temp = nodeStack.top();
        nodeStack.pop();

        std::stack<NODE *> tempStack;
        tempStack.push(temp);

        while( !tempStack.empty() ){
            temp = tempStack.top();
            NODE *out;
            bool flag = true;
            //push a fanout of temp which hasn't been visited into tempStack
            for(int i=0; i<temp->getOutSize(); ++i){
                out = temp->getFanout(i);
                if( !out->getVisited() ){
                    tempStack.push(out);
                    flag = false;
                    break;
                }
            }
            //all the fanouts of temp have already been visited
            if(flag){
                temp->setVisited(true);
                orderStack.push(temp);
                tempStack.pop();
            //out does not have any fanout
            }else if(out->getOutSize() == 0){
                out->setVisited(true);
                orderStack.push(out);
                tempStack.pop();
            }
        }
    }
    int order = 0;
    while( !orderStack.empty() ){
        NODE *temp = orderStack.top();
        temp->setOrder(order);
        ++order;
        orderStack.pop();
    }

    std::sort(nodes.begin(), nodes.end(), compareOrder<NODE>);
}

/* label all the nodes */
void label(std::vector<NODE *> &nodes, const int &K){
    for(auto it = nodes.begin(); it != nodes.end(); ++it){
        if( (*it)->getKind() == PI ){ 
            (*it)->setLabel(0);
            continue;
        }

        //It maximal label of the fanin(s) is 0, *it gets a label number as 1 
        if( (*it)->getInSize() == 1 ){
            if( (*it)->getFanin(0)->getLabel() == 0 ){
                (*it)->setLabel(1);
                continue;
            }
        }else if( (*it)->getFanin(0)->getLabel() == 0 && (*it)->getFanin(1)->getLabel() == 0 ){
            (*it)->setLabel(1);
            continue;
        }

        
        int size = 0;
        //get the fanin(s) with maximal label
        std::stack<NODE *> maxLabelStack;
        if( (*it)->getInSize() == 1 || (*it)->getFanin(0)->getLabel() > (*it)->getFanin(1)->getLabel() ){
            maxLabelStack.push( (*it)->getFanin(0) );
            size = 1;
        }else if( (*it)->getFanin(0)->getLabel() < (*it)->getFanin(1)->getLabel() ){
            size = 1;
            maxLabelStack.push( (*it)->getFanin(1) );
        }else{
            maxLabelStack.push( (*it)->getFanin(0) );
            maxLabelStack.push( (*it)->getFanin(1) );
        }
        
        
        //find label of (*it)
        NODE *temp = maxLabelStack.top();
        int maxLabel = temp->getLabel();
        std::set<NODE *> cutNode;
        while( !maxLabelStack.empty() ){
            temp = maxLabelStack.top();
            maxLabelStack.pop();
            
            for(int i=0; i<temp->getInSize(); ++i){
                if( temp->getFanin(i)->getLabel() == maxLabel )
                    maxLabelStack.push( temp->getFanin(i) );
                else cutNode.insert( temp->getFanin(i) );
            }
        }
        size += (int)cutNode.size();
        if( size <= K) (*it)->setLabel(maxLabel);
        else (*it)->setLabel( maxLabel+1 );
    }
}

/* mapping into LUT */
void mapping(std::vector<NODE *> &nodes, std::set<LUT *> &luts){
    
    std::vector<NODE *> nodeStack;
    for(auto it = nodes.begin(); it != nodes.end(); ++it){
        if( (*it)->getKind() == PO ) nodeStack.push_back( (*it) );
    }

    while( nodeStack.size() != 0 ){
        
        NODE *temp = nodeStack.back();
        nodeStack.pop_back();

        //find if this LUT with root temp has already existed, if yes, then skip
        if( temp->getDone() ) continue;

        int label = temp->getLabel();   //label of root
        LUT *lut = new LUT( temp->getID() );

        std::set<NODE *> faninSet;      //store fanin of LUT
        std::vector<NODE *> tempStack;  //find ancestor of node
        tempStack.push_back( temp );

        while( tempStack.size() != 0 ){

            NODE *top = tempStack.back();
            tempStack.pop_back();

            for(int i=0; i<top->getInSize(); ++i){
                if( top->getFanin(i)->getLabel() == label ){
                    tempStack.push_back( top->getFanin(i) );

                    //find if top is in the nodeStack, if yes, remove it from nodeStack
                    for(auto it = nodeStack.begin(); it != nodeStack.end(); ++it){
                        if( (*it) == top->getFanin(i) ){
                            it = nodeStack.erase(it);
                            break;
                        }
                    }
                }else faninSet.insert( top->getFanin(i) );
            }
        }

        tempStack.insert( tempStack.end(), faninSet.begin(), faninSet.end() );
        std::sort(tempStack.begin(), tempStack.end(), compareOrder<NODE>);
        for( auto it = tempStack.begin(); it != tempStack.end(); ++it){
            if( (*it)->getKind() != PI) nodeStack.push_back( *it );
            lut->addMember( (*it)->getID() );
        }
        luts.insert( lut );
        temp->setDone(true);
    }
}