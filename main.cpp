#include "parsec.h"

Parser<char> item() {
  return [=] (std::string str) {
    char c = str[0];
    auto t = std::tuple<char, std::string> {c, str};
    auto v = std::vector<std::tuple<char, std::string>> {t};
    return v;
  };
}


int main(int argc, char *argv[]) {
  std::regex regex {argv[1]};
  std::ifstream fs {argv[2]};


  int i {10};
  auto p = bind(item);//result(i);
  auto x = p()("hello");
  auto z = std::get<0>(x[0]);
  std::cout << z << std::endl;

  bind(item);



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
