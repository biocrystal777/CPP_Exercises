#include "./csvparser.h"

using std::string;
using std::cout;
using std::vector;
using std::endl;

int main(){

  string filePath = string("./testFile.csv");

  CsvParser *parser = new CsvParser(filePath, ',' , '.');
  uint errorLine;
  parser->parseFile();

  vector<string>* headLines = parser->getHeadLines();
  vectorMatrix* data = parser->getData();

  
  ///////////////////////////////////////////////////////////////////
  // Beginning from here you can do anything with the parsed data
  

  cout << headLines->at(0) << endl;
  cout << headLines->at(1) << endl;
  
  cout << data->at(0)->at(0) << endl;
  cout << data->at(0)->at(1) << endl;
  
  // until here
  ///////////////////////////////////////
  
  parser->clearMemory(); // clear heap by deleting data from heap
  delete parser; // delete parser
}
