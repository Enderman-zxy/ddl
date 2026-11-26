#include <iostream>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Usage: ddl <add|list|done>\n";
    return 1;
  }
  std::cout << "todo: " << argv[1] << "\n";
  return 0;
}