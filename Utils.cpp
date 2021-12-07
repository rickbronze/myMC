
/*
* Utils.cpp
*
*  Created on: Aug 28, 2018
*      Author: esimanto
*/

#include "Utils.h"
#include <stdio.h>
using namespace std;

Utils::Utils() {}

Utils::~Utils() {}

QStringList Utils::traverseDirectory(const QString &pattern, const QString &dirname, int level)
{
    QDir dir(dirname);
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoSymLinks | QDir::NoDot | QDir::NoDotDot);

    static const QStringList stringList = pattern.split('/');
    foreach (QFileInfo fileInfo, dir.entryInfoList(stringList.mid(level, 1))) {
        if (fileInfo.isDir() && fileInfo.isReadable())
            traverseDirectory(pattern, fileInfo.filePath(), level+1);
        else if (stringList.size() == (level + 1))
            qDebug() << fileInfo.filePath();
    }
}
int Utils::getFreeDiskSpaceInGB(string dirPath) {
  struct statvfs diskData;
  statvfs(dirPath.c_str(), &diskData);
  unsigned long available =
      ((diskData.f_bavail * diskData.f_bsize) / (1024 * 1024 * 1024));
  return (int)available;
}

bool Utils::dirExists(const char *path) {
  struct stat info;

  if (stat(path, &info) != 0)
    return false;
  else if (info.st_mode & S_IFDIR)
    return true;
  else
    return false;
}

int Utils::removeDirectory(string directoryToRemove) {
  string commandString("rm -r ");
  commandString.append(directoryToRemove);
  exec(commandString.c_str());
  return 0;
}

int Utils::copyFilesToDirectory(vector<string> filesToCopy,
                                string copyDestination) {
  for (unsigned int i = 0; i < filesToCopy.size(); i++) {
    string commandString("cp -f ");
    commandString.append(filesToCopy.at(i));
    commandString.append(" ");
    commandString.append(copyDestination);
    exec(commandString.c_str());
    return 0;
  }
}

int Utils::removeFilesInDirectory(string directoryToRemoveFilesFrom) {
  string commandString("rm -f ");
  commandString.append(directoryToRemoveFilesFrom);
  commandString.append("*");
  exec(commandString.c_str());
  return 0;
}

unsigned int Utils::random_char() {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, 255);
  return dis(gen);
}

string Utils::generate_hex(const unsigned int len) {
  stringstream ss;
  for (auto i = 0; i < len; i++) {
    const auto rc = random_char();
    stringstream hexstream;
    hexstream << hex << rc;
    auto hex = hexstream.str();
    ss << (hex.length() < 2 ? '0' + hex : hex);
  }
  return ss.str();
}

vector<string> Utils::getDirectoryFiles(const string &pattern) {
  glob_t glob_result;
  glob(pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
  vector<string> files;
  for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
    if (!dirExists(string(glob_result.gl_pathv[i]).c_str()))
      files.push_back(string(glob_result.gl_pathv[i]));
  }

  globfree(&glob_result);
  return files;
}

string Utils::trim(const string &str) {
  size_t first = str.find_first_not_of(' ');
  if (string::npos == first) {
    return str;
  }
  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last - first + 1));
}

string Utils::removeSpaces(string str) {
  str.erase(remove_if(str.begin(), str.end(),
                      static_cast<int (*)(int)>(&isspace)),
            str.end());
  str.erase(remove_if(str.begin(), str.end(),
                      static_cast<int (*)(int)>(&ispunct)),
            str.end());
  return str;
}

int Utils::getDayOfYear(string ISODateTime) {
  int year = atoi(ISODateTime.substr(0, 4).c_str());
 int month = atoi(ISODateTime.substr(4, 2).c_str());
  int day = atoi(ISODateTime.substr(6, 2).c_str());

  tm timeinfo = {};
  timeinfo.tm_year = year - 1900;
  timeinfo.tm_mon = month - 1;
  timeinfo.tm_mday = day;
  mktime(&timeinfo);
  return timeinfo.tm_yday + 1;
}

int Utils::getYear(string ISODateTime) {
  int year = atoi(ISODateTime.substr(0, 4).c_str());
  return year;
}

vector<string> Utils::getListOfISODays(string ISODateTime, int numDays) {
    vector<string> days;
    days.push_back(ISODateTime);
//    cout << "day in list " << ISODateTime << endl;
    for (unsigned int i=1;i<numDays;i++){
        ISODateTime = addDay(ISODateTime);
//        cout << "day in list " << ISODateTime << endl;
        days.push_back(ISODateTime);
    }
    return days;
}

string Utils::addDay(string ISODateTime) {
    time_t timeBase;
    tm timeinfo = getTimeFromISODateTime(ISODateTime);
    timeBase =   mktime(&timeinfo);
    timeBase += 86400;
    char cstr[20];
    strftime(cstr, sizeof(cstr), "%Y%m%dT%H%M%S", localtime(&timeBase));
    cstr[128] = ' ';
    return cstr;
}
tm Utils::getTimeFromISODateTime(string ISODateTime) {
  int year = atoi(ISODateTime.substr(0, 4).c_str());
  int month = atoi(ISODateTime.substr(4, 2).c_str());
  int day = atoi(ISODateTime.substr(6, 2).c_str());
  int hour = atoi(ISODateTime.substr(9,2).c_str());
  int minute = atoi(ISODateTime.substr(11,2).c_str());
  int second = atoi(ISODateTime.substr(13,2).c_str());
  tm timeinfo = {};
  timeinfo.tm_year = year - 1900;
  timeinfo.tm_mon = month - 1;
  timeinfo.tm_mday = day;
  timeinfo.tm_hour = hour;
  timeinfo.tm_min = minute;
  timeinfo.tm_sec = second;
  mktime(&timeinfo);
  return timeinfo;
}


int Utils::getDayOfYearFromTLE(string TLEString) {
  return atoi(TLEString.substr(20, 3).c_str());
}

int Utils::getYearFromTLE(string TLEString) {
  return atoi(TLEString.substr(18, 2).c_str()) + 2000;
}

int Utils::getSecondsSinceEpoch(string ISODateTime) {
  struct tm tm;
  time_t t;

  if (strptime(ISODateTime.c_str(), "%Y%m%dT%H%M%S", &tm) == NULL)
    cout << "error getSecondsSinceEpoch for epoch " << ISODateTime
              << endl;

  //    printf("year: %d; month: %d; day: %d;\n",
  //            tm.tm_year, tm.tm_mon, tm.tm_mday);
  //    printf("hour: %d; minute: %d; second: %d\n",
  //            tm.tm_hour, tm.tm_min, tm.tm_sec);
  //    printf("week day: %d; year day: %d\n", tm.tm_wday, tm.tm_yday);

  tm.tm_isdst = -1; /* Not set by strptime(); tells mktime()
                       to determine whether daylight saving time
                       is in effect */
  t = mktime(&tm);
  if (t == -1)
    /* Handle error */;
  //    printf("seconds since the Epoch: %ld\n", (long) t);
  return t;
}

int Utils::parseLine(char *line) {
  // This assumes that a digit will be found and the line ends in " Kb".
  int i = strlen(line);
  const char *p = line;
  while (*p < '0' || *p > '9')
    p++;
  line[i - 3] = '\0';
  i = atoi(p);
  return i;
}

string Utils::changeISODate2XMLDate(string ISODate) {
  ISODate.insert(4, "-");
  ISODate.insert(7, "-");
  ISODate.insert(13, ":");
  ISODate.insert(16, ":");
  ISODate.append(".0");
  return ISODate;
}

processMem_t Utils::GetProcessMemory() {
  pid_t pid = getpid();
  char cmd[64];
  snprintf(cmd, 64, "/proc/%d/status", pid);

  FILE *file = fopen(cmd, "r");
  char line[128];
  processMem_t processMem;

  while (fgets(line, 128, file) != NULL) {
    if (strncmp(line, "VmSize:", 7) == 0) {
      processMem.virtualMem = parseLine(line);
      break;
    }

    if (strncmp(line, "VmRSS:", 6) == 0) {
      processMem.physicalMem = parseLine(line);
      break;
    }
  }
  fclose(file);
  return processMem;
}

void Utils::cancelExec(){
    cout << "got CancelExec" << endl;
    fflush(NULL);
    fflush(fp);
    if (pclose(fp) != 0) {
      perror("Cannot close stream.\n");
    }
}
//    else {
//        cout << "file pointer is null already" << endl;
//    }
//}

// Calling function must free the returned result.
char *Utils::exec(const char *command) {

  char *line = NULL;
  // Following initialization is equivalent to char* result = ""; and just
  // initializes result to an empty string, only it works with
  // -Werror=write-strings and is so much less clear.
  char *result = (char *)calloc(1, 1);
  size_t len = 0;

  //          fflush(NULL);
  fp = popen(command, "r");
  if (fp == NULL) {
    printf("Cannot execute command:\n%s\n", command);
    return NULL;
  }

  while (getline(&line, &len, fp) != -1) {
    // +1 below to allow room for null terminator.
    result = (char *)realloc(result, strlen(result) + strlen(line) + 1);
    // +1 below so we copy the final null terminator.
    strncpy(result + strlen(result), line, strlen(line) + 1);
    free(line);
    line = NULL;
  }

  fflush(fp);
  if (pclose(fp) != 0) {
    perror("Cannot close stream.\n");
  }
  return result;
}

int Utils::getmem() {
  pid_t pid = getpid();
  char cmd[64];
  snprintf(cmd, 64, "/bin/ps -p %d -o size", pid);
  char *result = exec(cmd);
  if (!result) {
    return 0;
  }
  // Find first newline.
  int pos = 0;
  while (result[pos] != '\n') {
    pos++;
  }
  // Remove the final newline.
  result[strlen(result) - 1] = '\0';
  // Convert to integer.
  int size = atoi(result + pos + 1);
  free(result);
  return size;
}
vector<string> Utils::split_string(const string &str,
                                             const string &delimiter) {
  vector<string> strings;

  string::size_type pos = 0;
  string::size_type prev = 0;
  while ((pos = str.find(delimiter, prev)) != string::npos) {
    strings.push_back(str.substr(prev, pos - prev));
    prev = pos + 1;
  }

  // To get the last substring (or only, if delimiter is not found)
  strings.push_back(str.substr(prev));
  return strings;
}

vector<string> Utils::split_stringm(const string &str,
                                              const string &delimiter) {
  vector<string> strings;

  string::size_type pos = 0;
  string::size_type prev = 0;
  while ((pos = str.find(delimiter, prev)) != string::npos) {
    strings.push_back(str.substr(prev, pos - prev));
    prev = pos + 1;
  }

  // To get the last substring (or only, if delimiter is not found)
  strings.push_back(str.substr(prev));

  return strings;
}

string Utils::now(const char *format = "%Y-%m-%dT%H:%M:%S") {
  time_t t = time(0);

  char cstr[129];
  strftime(cstr, sizeof(cstr), format, localtime(&t));
  cstr[128] = ' ';
  return cstr;
}

string Utils::yesterday(const char *format = "%y%m%d") {
  time_t t = time(0);
  t = t - 86400;
  char cstr[129];
  strftime(cstr, sizeof(cstr), format, localtime(&t));
  cstr[128] = ' ';
  return cstr;
}

//  Function used to timetag cout statements - ISO format
string Utils::nowISO() {
  time_t t = time(0);
  char cstr[129];
  strftime(cstr, sizeof(cstr), "%Y%m%dT%H%M%S", localtime(&t));
  cstr[128] = ' ';
  return cstr;
}
//  Function used to timetag cout statements - ISO format
string Utils::now() {
  time_t t = time(0);
  char cstr[129];
  strftime(cstr, sizeof(cstr), "%Y-%m-%dT%H:%M:%S", localtime(&t));
  cstr[128] = ' ';
  return cstr;
}
string Utils::StringToUpper(string strToConvert) {
  for (unsigned int i = 0; i < strToConvert.length(); i++) {
    strToConvert[i] = toupper(strToConvert[i]);
  }
  return strToConvert; // return the converted string
}

