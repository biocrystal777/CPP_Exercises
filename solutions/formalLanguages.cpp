#include <iostream>
using namespace std;
typedef unsigned int uint;
const uint strSize = 100;

bool contains(char term[], char c, uint &pos);
bool containsOnlyValidLetters(char term[]);
bool substring(char input[], char result[], const uint first, const uint last);
bool isValidTerm(char term[]);
bool isAtomicTerm(char term[]);

int main()
{
  char express[strSize] = "***********A+B+*C+A+A+A+A+A+A+A+A+A+************B";

  if(containsOnlyValidLetters(express) && isValidTerm(express))
    cout << "\"" << express << "\" is a valid term in S." << endl;  
  else
    cout << express << " is not a valid term in S." << endl;
  
  return 0;
}

bool contains(char term[], char c, uint &pos)
{
  pos = 0;
  //  cout << "contains" << c << endl;
  while(pos < strSize){
    if(term[pos] == c) return true;    
    else if( c == '\0' || pos == strSize-1) return false;    
    ++pos;
  }
  return false;
}

bool containsOnlyValidLetters(char term[]){
  uint i = 0;
  while(i < strSize){
    if(term[i] == '\0' || i == strSize-1)  return true;
    else if(!(term[i] == 'A'
              || term[i] == 'B'
              || term[i] == 'C'
              || term[i] == '+'
              || term[i] == '*'
              ))
      return false;
    ++i;
  }    
}
    
// recursive function

bool isValidTerm(char term[]){

  // first rule: atomic terms
  if(isAtomicTerm(term)) return true;
  
  // second rule valid term "*x"
  {    
    uint substrXStart = 1;
    uint substrXEnd = 1;
    while(term[substrXEnd] != '\0') ++substrXEnd;  
    char substrX[strSize];
    substring(term, substrX, substrXStart, substrXEnd);
    if(term[0] == '*' && isValidTerm(substrX)) return true;
  }

  // third rule: valid term "y+z"
  {
    uint posPlus;
    if(contains(term, '+', posPlus)){
      uint substrYStart = 0;
      uint substrYEnd = posPlus - 1;
      uint substrZStart = posPlus + 1;      
      uint substrZEnd = substrZStart;
      while(term[substrZEnd] != '\0') ++substrZEnd;
      char substrY[strSize];
      char substrZ[strSize];
      substring(term, substrY, substrYStart, substrYEnd);
      substring(term, substrZ, substrZStart, substrZEnd);
      if(isValidTerm(substrY) && isValidTerm(substrZ)) return true;
    }    
  }
  return false;
}
    
bool isAtomicTerm(char term[]){
  if(term[1] == '\0' && 
     (term[0] == 'A' || term[0] == 'B' || term[0] == 'C')
     )
    return true;
  else return false;
}

bool substring(char input[], char result[], const uint first, const uint last){
  if(last < first) return false;
  if(last >= strSize) return false;  
  for(uint i = first; i <= last; ++i){
    result[i-first] = input[i];
  }
  result[last-first+1] = '\0';
  return true;
}
