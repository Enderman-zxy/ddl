#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <filesystem>

std::filesystem::path dataDir() {
  const char* home = std::getenv("HOME");
  if (!home) home = std::getenv("USERPROFILE");
  return std::filesystem::path(home) / ".ddl";
}

std::filesystem::path dataFile() {
  std::filesystem::create_directories(dataDir());
  return dataDir() / "data.tsv";
}



int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Usage: ddl <add|list|done>\n";
    return 1;
  }
  std::cout << "todo: " << argv[1] << "\n";
  std::ofstream out(dataFile(), std::ios::app);
  out << "1\ttest\t2026-12-01\tLab\t0\n";
  std::cout << "success\n";
  
  return 0;
}