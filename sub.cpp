#include "parsec.h"
int main123(int argc, char *argv[]) {
  std::regex regex {argv[1]};
  std::ifstream fs {argv[2]};


  int i {10};
  auto p = result(i);



  while(fs.good()) {

    std::string line;
    std::getline(fs, line);
    std::cout << line << std::endl;
    std::smatch matches;
    if(std::regex_search(line, matches, regex)) {
      std::cout << ">> " << line << std::endl;
    }
  }

  return 0;
}
