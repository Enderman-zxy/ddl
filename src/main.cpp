#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../include/ddlManager.h"

/**
 * @brief 检查第一个参数是否为add/list/done
 * @param 第一个参数
 * @return 是则返回true，否则返回false
 */
bool vertifyFirstParam(const std::string& s) {
  if (s == "add" || s == "list" || s == "done") {
    return true;
  } else {
    return false;
  }
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Usage: ddl <add|list|done>\n";
    return 1;
  }
  if (!vertifyFirstParam(argv[1])) {
    std::cout << "Usage: ddl <add|list|done>\n";
    return 1;
  }
  ddlManager manageer;
  if (static_cast<std::string>(argv[1]) == "add") {
    std::vector<std::string> strs;
    for (int i = 2; i < argc; i++) {
      strs.push_back(argv[i]);
    }
    manageer.addDDL(strs);
  } else {
    std::cout << "Other commands haven't been finished...\n";
  }
  return 0;
}