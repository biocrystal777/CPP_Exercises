#include <iostream>
//#include <cstdlib>
#include <omp.h>
#include <random>
using namespace std;
typedef unsigned int uint;

uint countEdges(bool adjMat[], const uint nodes);
uint printAdjMat(bool adjMat[], const uint nodes);
void initGraph(bool adjMat[], const uint nodes, const double p);
inline bool isConnected(bool adjMat[], const uint nodes);
bool randomBool(const double p);
bool pushToStack (uint stackData[], uint &last, const uint limit, uint value);
uint popFromStack(uint stackData[], uint &last, const uint limit, bool &ok);
bool stackIsEmpty(uint stackData[], const uint last);


int main()
{
  const uint i_nodes = 50;
  const uint i_pGridSize = 100;
  const uint i_n = 10;
  //  const uint i_messageForEach = 200;
  double p[i_pGridSize];
  double q[i_pGridSize];
  
  for(uint  i = 0; i < i_pGridSize; ++i)
    p[i] = static_cast<double>(i) / static_cast<double>(i_pGridSize);
    #pragma omp parallel
  {
        #pragma omp for 
    for(uint i = 0; i < i_pGridSize; ++i)
      {
        const uint nodes     = i_nodes;
        const uint pGridSize = i_pGridSize;
        const uint n         = i_n;
        uint j = 0;
        //        const uint messageForEach = i_messageForEach;
        double pThread = p[i];
        uint countConnected = 0;
        bool graph[nodes*nodes];
        for( j = 0; j < n ; ++j){          //        srand(j+1);          
          initGraph(graph, nodes, pThread);          
          if (isConnected(graph, nodes)) ++countConnected;                             
        }
        q[i] = static_cast<double>(countConnected) / static_cast<double>(n);
      }    
  }
  cout << "\n\np" << "q" << endl;
  for(uint i = 0; i < i_pGridSize; ++i)
    cout << p[i] << " " << q[i] << endl;   
}  

uint countEdges(bool adjMat[], const uint nodes){
  uint count=0;
  for(uint i=0; i < nodes; ++i)
    for(uint j=0; j < nodes; ++j)
      if(adjMat[i*nodes + j]) ++count;
  return count;
}

void initGraph(bool adjMat[], const uint nodes, const double p){  
  for(uint i=0; i < nodes; ++i)
    for(uint j=i; j < nodes; ++j)
      adjMat[j*nodes + i] = adjMat[i*nodes + j] = randomBool(p);   
  return;
}

bool isConnected(bool adjMat[], const uint nodes){  
  if(countEdges(adjMat, nodes) < nodes-1) return false;

  // depth search;
  uint nodeStack[nodes] = {nodes+100};
  uint nodeStackLast = 0;
  bool markedNodes[nodes] = {false};
  // visit first node;
  markedNodes[0] = true;
  pushToStack(nodeStack, nodeStackLast, nodes, 0);
  //check all connected nodesl
  while(!stackIsEmpty(nodeStack, nodeStackLast)){
    bool ok = false;
    uint nextNode = popFromStack(nodeStack, nodeStackLast, nodes, ok);
    if(!ok){
      cout << "ERROR XXXX" << endl;
      return false;        
    }
    // visit the connected nodes (always take last on the stack) not marked yet and put them on the stack
    uint row = nodeStackLast*nodes;
    for(uint i=0; i < nodes; ++i){
      if( adjMat[row+i] && !(markedNodes[i]) ){
        //        cout << "Visit node " << i << "."<< endl;
        markedNodes[i] = true;
        bool ok = pushToStack(nodeStack, nodeStackLast, nodes, i);
        if(!ok){
          cout << "ERROR XXXX" << endl;
          return false;        
        }
      }
    }      
  }
  // check if all nodes were visited;
  bool result = true;
  for(uint i = 0; i < nodes; ++i ){
    if(!markedNodes[i]) {
      result = false;
      break;
    }    
  }
  return result;
}

bool randomBool(const double p, uint seed){

  return (static_cast<double>(e1()% 10000) / 10000.0 ) < p;
}

bool pushToStack (uint stackData[], uint &last, const uint limit, uint value){
  if( (last + 1) >= limit ) return false;  
  ++last;
  stackData[last] = value;
  return true;
}
  
uint popFromStack(uint stackData[], uint &last, const uint limit, bool &ok){
  if( stackIsEmpty(stackData, last) || ( last >= limit ) ) {
    ok = false;
    return 0;
  } else {
    ok = true;
    return stackData[last--];
  }
}

uint printAdjMat(bool adjMat[], const uint nodes){
  cout << "Adjacency matrix\n\n" << endl;
  for(uint i = 0; i < nodes; ++i){    
    uint row = i*nodes;
    for(uint j = 0; j < nodes; ++j)
      cout << adjMat[row+j];
    cout << endl;
  }
  cout << "\n\n" << endl;  
}

bool stackIsEmpty(uint stackData[], const uint last){
  return !static_cast<bool>(last); 
}
