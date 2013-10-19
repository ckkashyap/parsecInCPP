#include "parsec.h"

Parser<char> item() {
  return [=] (std::string str) {
    if(str.size() == 0) {
      ResultVector<char> v {};
      return v;
    }else{
      char c = str[0];
      ResultVector<char> v {ResultTuple<char>{c, str.substr(1)}};
      return v;
    }
  };
}


//using Dingo = std::tuple<char,char>;
//
Parser<char> seq1(Parser<char> p1, Parser<char> p2) {
  std::function<Parser<char>(char)> f = [=] (char c) {
    std::function<Parser<char>(char)> g = [=] (char d) {
      std::cout << "got " << c << d << std::endl;
      return item();
    };
    return bindParsers(p2, g);
  };
  return bindParsers(p1, f);
}


int main(int argc, char *argv[]) {

  int i {10};
  auto p = seq1(item(), item());//result(i);
  auto x = p("hello");
  auto z = std::get<0>(x[0]);
  std::cout << z << std::endl;


  return 0;
}
