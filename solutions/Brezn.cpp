#include <iostream>

using namespace std;

bool contains7(unsigned int number);
bool isRepDigit1(unsigned int number);
bool isRepDigit2(unsigned int number);

int main()
{
  cout << "Enter a number:\n";
  unsigned int n;  
  cin >> n;

  for (unsigned int i = 0; i <= n; i++){

    bool caseTriggered = false;

    if(i % 7 == 0){
      cout << "Brezn ";
      caseTriggered = true;
    }

    if(contains7(i)){
      cout << "Bia ";
      caseTriggered = true;
    }

    if(isRepDigit1(i) && isRepDigit2(i)){
      cout << "Obazda";
      caseTriggered =  true;
    }
    
    if(!caseTriggered) cout << i;
    cout << "\n";                         
  }
}


bool contains7(unsigned int number)
{
  bool result = false;
  while(number != 0 ){    
    if(number % 10 == 7){
      result = true;
    }
    number /= 10;
  }  
  return result;
}

// NOT RECOMMENDED!
bool isRepDigit1(unsigned int number)
{
  if ( ((10 < number) && (number < 1e2) && (number % 11 == 0))
       || ((1e2 < number) && (number < 1e3) && (number % 111 == 0))
       || ((1e3 < number) && (number < 1e4) && (number % 1111 == 0))
       || ((1e4 < number) && (number < 1e5) && (number % 11111 == 0))
       || ((1e5 < number) && (number < 1e6) && (number % 111111 == 0))
       || ((1e6 < number) && (number < 1e7) && (number % 1111111 == 0))
       || ((1e7 < number) && (number < 1e8) && (number % 11111111 == 0))
       || ((1e8 < number) && (number < 1e9) && (number % 111111111 == 0))  
       || ((1e9 < number) && (number < 1e10) && (number % 1111111111 == 0))
       )
    return true;
  else 
    return false;      
}

bool isRepDigit2(unsigned int number)
{
  unsigned int lastDigit = number % 10;    
  bool result = true;

  while(number != 0){
    unsigned int nextDigit = number % 10;
    number /= 10;                           // equivalent to: number = number / 10;}
    if(lastDigit != nextDigit){
      result = false;
      break;
    }       
  }
  return result;
}


