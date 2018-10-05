#include "./csvparser.h"

using std::string;
using std::stringstream;
using std::ifstream;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;

CsvParser::CsvParser(string filePath, char sep, char dec) :
  filePath(filePath), sep(sep), dec(dec)
{
  //  this->filePath = filePath;
  //this->sep = sep;
  //this->dec = dec;
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

