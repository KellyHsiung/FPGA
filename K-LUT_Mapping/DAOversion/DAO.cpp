#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <stack>
#include "NODE.h"

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
void label(std::vector<NODE *> &nodes);


void label(std::vector<NODE *> &nodes){
    for(auto it = nodes.begin(); it != nodes.end(); ++it){
        if( (*it)->getKind() == 0 ){ 
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
        
        //get the fanin(s) with maximal label
        std::vector<NODE *> maxLabel;
        if( (*it)->getInSize() == 1 || (*it)->getFanin(0)->getLabel() > (*it)->getFanin(1)->getLabel() )
            maxLabel.push_back( (*it)->getFanin(0) );
        else if( (*it)->getFanin(0)->getLabel() < (*it)->getFanin(1)->getLabel() )
            maxLabel.push_back( (*it)->getFanin(1) );
        else{
            maxLabel.push_back( (*it)->getFanin(0) );
            maxLabel.push_back( (*it)->getFanin(1) );
        }
        
        
        //find label
            


    }
}

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
        nodes.push_back( new NODE(element, 0) );
    }
    //read POs
    for(int i=0; i<PONum; ++i){
        file >> element;
        nodes.push_back( new NODE(element, 1) );
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

        if(!find(nodes, target)){
            NODE *tempNode = new NODE(target, 2);
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
    // for(auto it = nodes.begin(); it != nodes.end(); ++it) 
    //     (*it)->print();

    //sort the nodes by topological order
    topologicalSort(nodes);
    std::sort(nodes.begin(), nodes.end(), compareOrder<NODE>);

    label(nodes);
    for(auto it = nodes.begin(); it != nodes.end(); ++it) 
        (*it)->print();
    return 0;
}


/* sort nodes by topological order */
void topologicalSort(std::vector<NODE *> &nodes){
    
    std::stack<NODE *> nodeStack;
    //push PIs into stack nodeStack
    for(auto it = nodes.begin(); it != nodes.end(); ++it){
        if((*it)->getKind() == 0) nodeStack.push( (*it) );
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
}