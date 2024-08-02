#ifndef UTILS_H
#define UTILS_H
// test of github commit

/*
* Utils.h
*
*  Created on: Aug 28, 2018
*      Author: esimanto
*/
#ifndef UTILS_H_
#define UTILS_H_
#include <QString>
#include <QDir>
#include <QDebug>
#include "sys/sysinfo.h"
#include "sys/types.h"
#include <algorithm>
#include <cstring>
#include <ctime>
#include <fstream>
#include <glob.h>
#include <iostream>
#include <random>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <time.h>
#include <vector>
#include <unistd.h>
#include <glob.h>

using namespace std;

typedef struct {
  uint32_t virtualMem;
  uint32_t physicalMem;
} processMem_t;

class Utils {
public:
  Utils();
  virtual ~Utils();
  FILE *fp;

  processMem_t GetProcessMemory();
  int getmem();
  char *exec(const char *command);
  vector<string> split_stringm(const string &str,
                                         const string &delimiter);
  vector<string> split_string(const string &str,
                                        const string &delimiter);
  vector<string> getDirectoryFiles(const string &pattern);
  string now(const char *format);
  string now();
  string nowISO();
  int getDayOfYear(string ISODateTime);
  int getYear(string ISODateTime);
  int getDayOfYearFromTLE(string TLEString);
  int getYearFromTLE(string TLEString);
  int getSecondsSinceEpoch(string ISODateTime);
  unsigned int random_char();
  string generate_hex(const unsigned int len);
  bool dirExists(const char *path);
  int removeDirectory(string directoryToRemove);
  int copyDirectory(string directoryToCopy, string copyDestination);
  int copyFilesToDirectory(vector<string> filesToCopy,
                           string copyDestination);
  int removeFilesInDirectory(string directoryToRemoveFilesFrom);
  string trim(const string &str);
  string removeSpaces(string str);

  int getFreeDiskSpaceInGB(string dirPath);
  string changeISODate2XMLDate(string ISODate);
  string yesterday(const char *format);
  string StringToUpper(string strToConvert);

  tm getTimeFromISODateTime(string ISODateTime);
  string addDay(string ISODateTime);
  vector<string> getListOfISODays(string ISODateTime, int numDays);
  void cancelExec();
  QStringList traverseDirectory(const QString &pattern, const QString &dirname, int level);
private:
  int parseLine(char *line);
  processMem_t processMemory;
};


#endif /* UTILS_H_ */

#endif // UTILS_H
