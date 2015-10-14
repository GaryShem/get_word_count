#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <windows.h>

namespace patch
{
template < typename T > std::string to_string( const T& n )
{
  std::ostringstream stm ;
  stm << n ;
  return stm.str() ;
}
}

int main()
{
  HANDLE inputFile = CreateFile("README.txt", GENERIC_READ, 0,
                                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  HANDLE outputFile = CreateFile("Result.txt", GENERIC_WRITE, 0,
                                  NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  std::map<std::string, int> wordCount;
  std::string delims = (char*)" ,.\n\r:\\\"()='-/;";
  int sizeOfBuffer = 255;
  char* buffer = new char[sizeOfBuffer + 1];
  DWORD bytesRead;
  char* current = new char[sizeOfBuffer + 1];
  current[0] = 0;
  char* next = new char[sizeOfBuffer + 1];
  ReadFile(inputFile, buffer, sizeOfBuffer, &bytesRead, NULL);
  buffer[bytesRead] = '\0';
  bool isTruncated = false;
  while (bytesRead != 0) //пока есть файл
  {
    buffer[bytesRead] = 0;
    if (bytesRead != 0) //пока остался фрагмент
    {
      if (isTruncated)
        strcat(current, strtok(buffer, delims.c_str()));
      else
        strcpy(current, strtok(buffer, delims.c_str()));

      if (strchr(delims.c_str(), buffer[bytesRead - 1]))
        isTruncated = true;
      else
        isTruncated = false;

      char* temp = strtok(NULL, delims.c_str());
      if (temp != NULL)
        strcpy(next, temp);
      while (temp != NULL)
      {
        wordCount[std::string(current)]++;
        strcpy(current, next);
        temp = strtok(NULL, delims.c_str());
        if (temp != NULL)
          strcpy(next, temp);
      }
    }

    ReadFile(inputFile, buffer, sizeOfBuffer, &bytesRead, NULL);
    buffer[bytesRead] = '\0';
  }

  for (auto x: wordCount)
  {
    std::string dictionaryLine = x.first + " - " + patch::to_string(x.second) + "\n\r";
    std::cout << dictionaryLine;
    DWORD bytesWritten;
    WriteFile(outputFile, dictionaryLine.c_str(), strlen(dictionaryLine.c_str()), &bytesWritten, NULL);
  }


  CloseHandle(inputFile);
  CloseHandle(outputFile);
  return 0;
}
