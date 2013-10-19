#include "parsec.h"


Parser<char> zeroChar() {
  return [=] (std::string str) {
    ResultVector<char> v {};
    return v;
  };
}


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

Parser<char> sat(std::function<bool(char)> p) {
  std::function<Parser<char>(char)> f = [=] (char c) {
    if(p(c)) {
      return result(c);
    } else {
      return zeroChar();
    }
  };
  return bindParsers(item(), f);
}


using Dingo = char; //std::tuple<char,char>;
//
Parser<Dingo> seq1(Parser<char> p1, Parser<char> p2) {
  std::function<Parser<Dingo>(char)> g = [=] (char d) {
    std::cout << d << std::endl;
    Dingo v {};
    return result(v);
  };


  std::function<Parser<char>(char)> f = [=] (char c) {
    std::cout << c << std::endl;
    return bindParsers(p2, g);
  };
  return bindParsers(p1, f);
}


int main(int argc, char *argv[]) {

  int i {10};
  auto p = sat([] (char c) { return c == 'A'; });
  //  auto p = seq1(item(), item());//result(i);
  auto x = p("ABC");
  std::cout << x.size() << std::endl;
  if (x.size() > 0) {
    auto z = std::get<0>(x[0]);
    std::cout << z << std::endl;
  }

  return 0;
}
