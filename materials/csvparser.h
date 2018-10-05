#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <sstream>


typedef std::vector<std::vector<double>*> vectorMatrix;
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
  explicit CsvParser(std::string filePath, char sep = '.', char dec = ',');


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
  std::vector<std::string>* getHeadLines() const;

  /*!
  * \brief getHeadLines returns a vector with data columns, which are stored in a vector
  *        themselves each. Access: data->at(column)->at(row)
  * \return Pointer to the data on heap. Use delete on this address
  *         as soon as you don't need data are not need anymore!
  */
  vectorMatrix* getData() const;

  void clearMemory();

protected:

  /////////////
  // Members //
  /////////////

const std::string filePath;
const char sep;
const char dec;

  std::vector<std::string>* headLineEntries;
  vectorMatrix* dataVectorList;


  /////////////////////////
  // Protected functions //
  /////////////////////////

  bool isDigit(char character) const;
  /*!
  * \brief intToString converts an int to a std::string using a stringstream
  * \param number integer to be converted
  * \return std::string
  */
  std::string intToString(int number) const;

  /*!
  * \brief doubleToString converts a double to a std::string using a stringstream
  * \param number
  * \return std::string
  */
  std::string doubleToString(double number) const;

  std::string charToString(char character) const;
};

#endif // CSVPARSER_H
