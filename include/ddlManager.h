#pragma once

#include <filesystem>
#include <string>
#include <vector>

enum class AddStatus { SUCCEED, FAIL };
enum class FileStatus { EMPTY, NO, LF, CRLF };

struct DDL {
  DDL() = delete;
  DDL(int id, std::string title, std::vector<int> deadline, bool tag,
      std::string tagTitle)
      : title_(title), deadline_(deadline), tag_(tag), tagTitle_(tagTitle) {}
  DDL(int id, std::string title, std::vector<int> deadline)
      :id_(id), title_(title), deadline_(deadline), tag_(0), tagTitle_("") {}
  int id_;
  std::string title_;
  std::vector<int> deadline_;
  bool tag_;
  std::string tagTitle_;
};

class ddlManager {
 public:
  ddlManager() = default;
  AddStatus addDDL(const std::vector<std::string>& strs);

 private:
  std::vector<DDL> DDLs;
  std::filesystem::path getFilePath() const;
  bool checkFilePath(const std::string& path);
  int loadFromFile(const std::string& path, bool& reused);
  bool append(const int id, const std::string& path, const std::string& title,
              const std::string& date, const std::string& tag);
  FileStatus checkFileEnd(const std::string& path);
};