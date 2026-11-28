#include "../include/ddlManager.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace {

enum class FileStatus { EMPTY, NO, LF, CRLF };

FileStatus check_file_end(const std::string& path) {
  std::ifstream in(path, std::ios::binary | std::ios::ate);

  std::streamoff size = in.tellg();
  if (size <= 0) return FileStatus::EMPTY;

  in.seekg(size - 1);
  char last = 0;

  if (!in.get(last)) return FileStatus::NO;

  if (last != '\n') return FileStatus::NO;

  if (size >= 2) {
    in.seekg(size - 2);
    char prev = 0;
    if (in.get(prev) && prev == '\r') return FileStatus::CRLF;
  }
  return FileStatus::LF;
}

/**
 * @brief 把日期由字符串转换为年月日
 * @param 日期字符串
 * @return vector<int>，包含年、月、日三个值
 */
std::vector<int> getNumberDate(const std::string& date) {
  int yyyy = 0;
  int mm = 0;
  int dd = 0;
  for (size_t i = 0; i < 4; i++) {
    yyyy *= 10;
    yyyy += (date[i] - '0');
  }
  for (size_t i = 5; i < 7; i++) {
    mm *= 10;
    mm += (date[i] - '0');
  }
  for (size_t i = 8; i < 10; i++) {
    dd *= 10;
    dd += (date[i] - '0');
  }
  return std::vector<int>({yyyy, mm, dd});
}

/**
 * @brief 检查日期是否合法
 * @param 日期字符串
 * @return 是则返回true，否则返回false
 */
bool checkDate(const std::string& date) {
  if (date.size() != 10) return false;
  if (date[4] != '-' || date[7] != '-') return false;
  for (size_t i = 0; i < date.size(); i++) {
    if (i != 4 && i != 7) {
      if (!(date[i] >= '0' && date[i] <= '9')) return false;
    }
  }
  std::vector<int> numberDate = getNumberDate(date);
  int yyyy = numberDate[0];
  int mm = numberDate[1];
  int dd = numberDate[2];
  if (mm < 1 || mm > 12 || dd < 1) return false;
  const bool leap_year = (yyyy % 4 == 0 && yyyy % 100 != 0) || yyyy % 400 == 0;
  const int days_in_month[] = {
      31, leap_year ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  return dd <= days_in_month[mm - 1];
}

bool hasTAB(const std::string& s) {
  for (size_t i = 0; i < s.size(); i++) {
    if (s[i] == '\t') {
      return true;
    }
  }
  return false;
}

// 检查文件是否存在
bool checkFilePath(const std::string& path) {
  std::filesystem::path filePath = static_cast<std::filesystem::path>(path);
  if (std::filesystem::exists(filePath)) {
    return true;
  }
  return false;
}

// 得到目标文件路径
std::filesystem::path getFilePath() {
  const char* home = std::getenv("HOME");
  if (!home) home = std::getenv("USERPROFILE");
  return std::filesystem::path(home) / ".ddl" / "data.tsv";
}

bool checkLine(const std::vector<std::string>& strs) {
  if (strs.size() != 5) {
    return false;
  }
  std::string id = strs[0];
  for (int i = 0; i < id.size(); i++) {
    if (!(id[i] >= '0' && id[i] <= '9')) {
      return false;
    }
  }
  std::string date = strs[2];
  if (!checkDate(date)) {
    return false;
  }
  std::string done = strs[4];
  if (done.size() != 1) {
    return false;
  }
  if (!(done[0] >= '0' && done[0] <= '9')) {
    return false;
  }
  return true;
}

std::vector<std::string> split_tsv(const std::string& line) {
  std::vector<std::string> fields;
  size_t start = 0;
  while (true) {
    size_t pos = line.find('\t', start);
    if (pos == std::string::npos) {
      fields.emplace_back(line.substr(start));
      break;
    }
    fields.emplace_back(line.substr(start, pos - start));
    start = pos + 1;
  }
  return fields;
}

/**
 * @brief 读取data.tsv
 * @return 下一个加入的ddl的id
 */
int readFile(const std::string& path) {
  std::fstream tsv_data(path, std::ios::in);
  std::string line;
  int id = 1;
  while (std::getline(tsv_data, line)) {
    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }
    if (line.empty()) continue;
    auto fields = split_tsv(line);
    if (checkLine(fields)) {
      id++;
    }
  }
  return id;
}

// 文件存在的情况下写入
void writeWhenExist(std::string& path, std::string& title, std::string& date) {
  std::fstream out(path, std::ios::app);
  FileStatus end_of_file = check_file_end(path);
  int id = 0;
  if (end_of_file == FileStatus::EMPTY) {
    id = 1;
    if (std::getenv("HOME")) {  // Linux
      out << "\n";
    } else {
      out << "\r\n";
    }
  } else {
    id = readFile(path);
  }
  out << id << "\t" << title << "\t" << date << "\t" << "Undetermined" << 0;
  if (end_of_file == FileStatus::LF) out << "\n";
  if (end_of_file == FileStatus::CRLF) out << "\r\n";
}

void writeWhenNotExist(std::string& title, std::string& date) {}

// 检查标题、指令、日期
bool checkNormal(std::vector<std::string>& strs) {
  if (strs.size() < 3) {
    std::cout << "Error: Lack Param\n";
    std::cout << "Usage: ./ddl add \"your title\" --due yyyy-mm-dd [--tag "
                 "\"tag title\"]\n";
    return false;
  }
  if (strs.size() == 4 && strs[3] == "--tag") {
    std::cout << "Error: Lack Param\n";
    std::cout << "Usage: ./ddl add \"your title\" --due yyyy-mm-dd [--tag "
                 "\"tag title\"]\n";
    return false;
  }
  if (strs.size() == 4 && strs[3] != "--tag") {
    std::cout << "Error: Invalid Param\n";
    std::cout << "Usage: ./ddl add \"your title\" --due yyyy-mm-dd [--tag "
                 "\"tag title\"]\n";
    return false;
  }
  if (strs.size() > 5) {
    std::cout << "Error: invalid Param\n";
    return false;
  }
  std::string title = strs[0];
  std::string option1 = strs[1];
  std::string date = strs[2];
  if (hasTAB(title)) {
    std::cout << "ERROR: " << title
              << " contains TAB, which is not a valid option\n";
    std::cout << "Usage: ./ddl add \"your title\" --due yyyy-mm-dd [--tag "
                 "\"tag title\"]\n";
    return false;
  }
  // if (title[0] != '\"') {
  //   std::cout << "Error: Invalid Title(without \"\")\n";
  //   std::cout << title[0] << "\n";
  //   std::cout << "Usage: ./ddl add \"your title\" --due yyyy-mm-dd [--tag "
  //                "\"tag title\"]\n";
  //   return false;
  // }
  if (option1 != "--due") {
    std::cout << "ERROR: " << option1 << " is not a valid option\n";
    std::cout << "Usage: ./ddl add \"your title\" --due yyyy-mm-dd [--tag "
                 "\"tag title\"]\n";
    return false;
  }
  if (!checkDate(date)) {
    std::cout << "ERROR: " << date << " is not a valid date\n";
    std::cout << "Usage: ./ddl add \"your title\" --due yyyy-mm-dd [--tag "
                 "\"tag title\"]\n";
    return false;
  }
  return true;
}

bool checkExtra(std::string option, std::string tag_title) {
  if (option != "--tag") {
    std::cout << "ERROR: " << option << " is not a valid option\n";
    std::cout << "Usage: ./ddl add \"your title\" --due yyyy-mm-dd [--tag "
                 "\"tag title\"]\n";
    return false;
  }
  if (hasTAB(tag_title)) {
    std::cout << "ERROR: " << tag_title
              << " contains TAB, which is not a valid option\n";
    std::cout << "Usage: ./ddl add \"your title\" --due yyyy-mm-dd [--tag "
                 "\"tag title\"]\n";
    return false;
  }
  // if (tag_title[0] != '\"' && tag_title[tag_title.size() - 1] != '\"') {
  //   std::cout << "Error: Invalid Title(without \"\")\n";
  //   std::cout << "Usage: ./ddl add \"your title\" --due yyyy-mm-dd [--tag "
  //                "\"tag title\"]\n";
  //   return false;
  // }
  return true;
}

}  // namespace

AddStatus ddlManager::addDDL(std::vector<std::string>& strs) {
  if (!checkNormal(strs)) {
    return AddStatus::FAIL;
  }
  std::string title = strs[0];
  std::string option1 = strs[1];
  std::string date = strs[2];
  if (strs.size() == 3) {
    std::cout << 1 << "\t" << title << "\t" << date << "\t"
              << "Underdetermined" << "\t" << 0 << "\n";
    return AddStatus::SUCCEED;
  } else {
    std::string option2 = strs[3];
    std::string tag_title = strs[4];
    if (checkExtra(option2, tag_title)) {
      std::cout << 1 << "\t" << title << "\t" << date << "\t"
                << tag_title << "\t" << 0 << "\n";
      return AddStatus::SUCCEED;
    } else {
      return AddStatus::FAIL;
    }
    
  }
}
