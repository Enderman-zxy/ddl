#pragma once

#include <string>
#include <vector>

enum class AddStatus { SUCCEED, FAIL };

struct DDL {
  DDL() = delete;
  DDL(std::string title, std::vector<int> deadline, bool tag,
      std::string tagTitle)
      : title_(title), deadline_(deadline), tag_(tag), tagTitle_(tagTitle) {}
  DDL(std::string title, std::vector<int> deadline)
      : title_(title), deadline_(deadline), tag_(0), tagTitle_("") {}
  std::string title_;
  std::vector<int> deadline_;
  bool tag_;
  std::string tagTitle_;
};

class ddlManager {
 public:
  ddlManager() = default;
  AddStatus addDDL(std::vector<std::string>& strs);

 private:
  std::vector<DDL> DDLs;
};