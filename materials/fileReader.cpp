#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <sstream>

using std::string;
using std::stringstream;
using std::ifstream;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;

typedef vector<vector<double>*> vectorMatrix;
/**************************************************************
***
***   csvParser
***
**************************************************************/

/*! \class csvParser
*\brief This parser can read a csv file and creates data structures
* on heap (=> have to be destroyed externally as soon as they are not needed anymore)
* \author Benedikt Haeusele
* \version 1.0
* \date March 2013
* \copyright GNU General Public License version 3.0
*/


class CsvParser
{

public:
  /*!
  * \brief CsvParser
  * \param filePath absolute path of the file to be read
  * \param sep seperator char that indicates the seperator p.e. ',' ';' '\t' ':'  etc
  * \param dec for doubles: '.' or 'm,'
  */
  explicit CsvParser(string filePath, char sep = '.', char dec = ',');  
  
  /*!
  * \brief ~CsvParser Default Destructor
  */
  ~CsvParser();


  /*!
  * \brief parseFile is used to the parse the file and create the data structures
  * \brief errorInLine indicates the line in the file where a possible error occured
  * \return integer indicates errorvalue
  */
  int parseFile(uint *errorInLine = NULL);
  
  /*!
  * \brief getHeadLines returns a vector with the entries of the headline of the csv-file.
  *        Has
  * \return Pointer to the data on heap. Use delete on this address
  *         as soon as you don't need data are not need anymore!
  */
  std::vector<std::string>* getHeadLines();

  /*!
  * \brief getHeadLines returns a vector with data columns, which are stored in a vector
  *        themselves each. Access: data->at(column)->at(row)
  * \return Pointer to the data on heap. Use delete on this address
  *         as soon as you don't need data are not need anymore!
  */
  vectorMatrix* getData();

  void clearMemory();

protected:

  /////////////
  // Members //
  /////////////

  std::string filePath;
  char sep;
  char dec;

  std::vector<std::string>* headLineEntries;
  vectorMatrix* dataVectorList;


  /////////////////////////
  // Protected functions //
  /////////////////////////

  bool isDigit(char character);
  /*!
  * \brief intToString converts an int to a std::string using a stringstream
  * \param number integer to be converted
  * \return std::string
  */
  std::string intToString(int number);

  /*!
  * \brief doubleToString converts a double to a std::string using a stringstream
  * \param number
  * \return std::string
  */
  std::string doubleToString(double number);

  std::string charToString(char character);
};









///////////////////////////////////////////////////////////////////////////////////////////
//
// Example how to use the csvParser class
//
//////////////////////////////////////////////////////////////////////////////////////////




int main(){
  

  string filePath = string("/home/bluemage/testFile.csv");

  CsvParser *parser = new CsvParser(filePath, ',' , '.');
  uint errorLine;
  parser->parseFile();

  vector<string>* headLines = parser->getHeadLines();
  vectorMatrix* data = parser->getData();

  
  ///////////////////////////////////////
  // From here you can do anything with the parsed data



  

  cout << headLines->at(0) << endl;
  cout << headLines->at(1) << endl;
  
  cout << data->at(0)->at(0) <<  endl;
  cout << data->at(0)->at(1) <<  endl;
  




  // until here
  ///////////////////////////////////////
  
  parser->clearMemory(); // clear heap by deleting data from heap
  delete parser; // delete parser

  
}


//////////////////////////////////////////////////////////////////////////////////////////

















CsvParser::CsvParser(string filePath, char sep, char dec)
{
  this->filePath = filePath;
  this->sep = sep;
  this->dec = dec;
}

CsvParser::~CsvParser()
{
}

int CsvParser::parseFile(uint *errorInLine)
{

  ifstream inpFile;
  inpFile.open(filePath.c_str());
  if(!inpFile.is_open()){
    cerr << "File could not be opened." << endl;
    return 1;
  }


  uint counterHead = 0;
  uint counterData = 0;
  char c = inpFile.get();
  while(!((c == '\n') || (c == '\r'))){
    c = inpFile.get();
    counterHead++;
  }

 
  for(uint i=0; i<5; i++)
    inpFile.get();
  c = inpFile.get();
  while(!((c == '\n') || (c == '\r'))){

    c = inpFile.get();
    counterData++;
  }
  counterHead += 30000;
  counterData += 30000;


  char headLine[counterHead];
  memset(headLine, 0x00, counterHead * sizeof(char));
  char dataLine[counterData];
  memset(dataLine, 0x00, counterData * sizeof(char));
  inpFile.seekg(0);

  ////////////////////
  // parse headLine //
  ////////////////////

  uint lineCounter = 1;
  headLineEntries = new vector<string>();
  inpFile.getline(headLine, counterHead);
  uint entryBufferLength = 2048;
  char entryBuffer[entryBufferLength];
  memset(entryBuffer, 0x00, entryBufferLength * sizeof(char));
  uint entryBufferPos = 0;
  string newEntry;

  for(uint i = 0; i < counterHead; i++){
    c = headLine[i];

    if((c == '\r') || (c == '\n') || (c == '\0')){
      newEntry = string(entryBuffer);
      memset(entryBuffer, 0x00, entryBufferLength * sizeof(char));
      entryBufferPos = 0;
      headLineEntries->push_back(newEntry);
      break;
    } else if (c == sep){
      newEntry = string(entryBuffer);
      memset(entryBuffer, 0x00, entryBufferLength * sizeof(char));
      entryBufferPos = 0;
      headLineEntries->push_back(newEntry);
    } else {
      entryBuffer[entryBufferPos] = c;
      entryBufferPos++;
    }
  }

  ////////////////////////////////////////////
  // Check if numbers begin in second line ///
  ////////////////////////////////////////////

  inpFile.getline(dataLine, counterHead);
  ++lineCounter;

  if(! ( isDigit(dataLine[0])
         || (dataLine[0] == '-')
         || (dataLine[0] == '+')
         || (dataLine[0] == '.')
         )){
    *errorInLine = lineCounter;
    cerr << "Input file may have only a single headline!" << endl;
    delete headLineEntries;
    return 2;
  }

  /////////////////////////
  // create data vectors //
  /////////////////////////

  uint numberOfColumns = headLineEntries->size();
  dataVectorList = new vector<vector<double>*>();
  vector<double> *newColumn;
  for(uint i = 0; i < numberOfColumns; i++){
    newColumn = new vector<double>();
    dataVectorList->push_back(newColumn);
  }


   
  ////////////////
  // parse Data //
  ////////////////

  double convDouble;
  uint column = 0;

  // begin with second Line parsed for checking the numbers => do while loop, which begins to read the third line
  do {
    memset(entryBuffer, 0x00, entryBufferLength * sizeof(char));
    column = 0;
    entryBufferPos = 0;

    for(uint i = 0; i < counterData; i++){
      c = dataLine[i];
      if((c == '\r') || (c == '\n') || (c == '\0')){ // end of line reached
        // Check the number of entries in the line .csv-Files
        // and abort if the file structur is corrupted
        if (column + 1 < numberOfColumns){
          cerr <<  ("Input file has too many entries in line ") << lineCounter;
          cerr << "Has " << column + 1 <<  ", should have " << numberOfColumns;
          *errorInLine = lineCounter;
          delete headLineEntries;
          for(uint vectors = 0; vectors < dataVectorList->size(); ++vectors){
            delete dataVectorList->at(vectors);
          }
          delete dataVectorList;
          return 3;
        }
        if(column + 1 > numberOfColumns){
          cerr << "Input file has not enough entries in line " << lineCounter;
          cerr << "Has " << column + 1 << ", should have " << numberOfColumns;
          *errorInLine = lineCounter;
          delete headLineEntries;
          for(uint vectors = 0; vectors < dataVectorList->size(); ++vectors){
            delete dataVectorList->at(vectors);
          }
          delete dataVectorList;
          return 4;
        }
         
        entryBufferPos = 0;
        convDouble = atof(entryBuffer);
        dataVectorList->at(column)->push_back(convDouble);
        memset(entryBuffer, 0x00, entryBufferLength * sizeof(char));
        ++lineCounter;
        break;
      } else if (c == sep){
        //cCounter = 0;
        /*if (lineCounter < 3) FFFLog::logText(string(entryBuffer).append(" ; ")
        .append(intToString(entryBufferPos))
        .append("!! SEPERATOR !!"));
        */
        convDouble = atof(entryBuffer);
        dataVectorList->at(column)->push_back(convDouble);
        memset(entryBuffer, 0x00, entryBufferLength * sizeof(char));
        entryBufferPos = 0;
        column++;
      } else {
        entryBuffer[entryBufferPos] = c;
 
        ++entryBufferPos;
      }
    }
  }
  while (inpFile.getline(dataLine, counterHead));

  return 0;
}

vector<string>* CsvParser::getHeadLines()
{
  return headLineEntries;
}

vectorMatrix* CsvParser::getData()
{
  return dataVectorList;
}

bool CsvParser::isDigit(char character)
{
  if((character == '0')
     || (character == '1')
     || (character == '2')
     || (character == '3')
     || (character == '4')
     || (character == '5')
     || (character == '6')
     || (character == '7')
     || (character == '8')
     || (character == '9')
     )
    return true;
  else
    return false;
}

void CsvParser::clearMemory(){
  if(dataVectorList){
    for (uint i = 0; i < dataVectorList->size(); ++i){
      if(dataVectorList->at(i)) delete dataVectorList->at(i);
    }
    delete dataVectorList;
  }
  
  if(headLineEntries)
    delete headLineEntries;
}


string CsvParser::intToString(int number)
{
  stringstream ss;//create a stringstream
  ss << number;//add number to the stream
  return ss.str();//return a string with the contents of the stream
}

string CsvParser::charToString(char character)
{
  stringstream ss;//create a stringstream
  ss << character;//add number to the stream
  return ss.str();//return a string with the contents of the stream
}

string CsvParser::doubleToString(double number)
{
  stringstream ss;//create a stringstream
  ss << number;//add number to the stream
  return ss.str();//return a string with the contents of the stream
}

