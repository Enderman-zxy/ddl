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
  std::vector<std::string> args;
  for (int i = 0; i < argc; i++) {
    args.push_back(argv[i]);
  }
  if (args.size() < 2) {
    std::cout << "Usage: ddl <add|list|done>\n";
    return 1;
  }
  if (!vertifyFirstParam(args[1])) {
    std::cout << "Usage: ddl <add|list|done>\n";
    return 1;
  }
  ddlManager manageer;
  if (args[1] == "add") {
    std::vector<std::string> strs;
    for (int i = 2; i < args.size(); i++) {
      strs.push_back(args[i]);
    }
    manageer.addDDL(strs);
  }
  if (args[1] == "list") {
    
  }
  return 0;
}