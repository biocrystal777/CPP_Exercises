#include <iostream>
#include "shellcolordefs.h"

using namespace std; 


int main(){
  cout << BLACK   << "black"   << RESET << endl;
  cout << GREEN   << "green"   << RESET << endl;
  cout << RED     << "red"     << RESET << endl;
  cout << BLUE    << "blue"    << RESET << endl;
  cout << CYAN    << "cyan"    << RESET << endl;
  cout << YELLOW  << "yellow"  << RESET << endl;
  cout << MAGENTA << "magenta" << RESET << endl;
  cout << WHITE   << "white"   << RESET << endl;

  cout << BOLD_BLACK   << "boldBlack"   << RESET << endl;
  cout << BOLD_GREEN   << "boldGreen"   << RESET << endl;
  cout << BOLD_RED     << "boldRed"     << RESET << endl;
  cout << BOLD_BLUE    << "boldBlue"    << RESET << endl;
  cout << BOLD_CYAN    << "boldCyan"    << RESET << endl;
  cout << BOLD_MAGENTA << "boldMagenta" << RESET << endl;
  cout << BOLD_YELLOW  << "boldYellow"  << RESET << endl;
  cout << BOLD_WHITE   << "boldWhite"   << RESET << endl;

  return 0;
}
