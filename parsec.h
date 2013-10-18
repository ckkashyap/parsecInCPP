#include <iostream>
#include <fstream>
#include <regex>
#include <tuple>
#include <functional>

// Parser :: a -> [(a, String)]
template<typename A>
using Parser = std::function< std::vector<std::tuple<A, std::string>> (std::string)>;

template<typename A>
Parser<A> result(A a) {
  return [=] (std::string str) {
    auto t = std::tuple<A, std::string> {a, str};
    auto v = std::vector<std::tuple<A, std::string>> {t};
    return v;
  };
}

template<typename A>
Parser<A> zero(A a) {
  return [=] (std::string str) {
    auto v = std::vector<std::tuple<A, std::string>> {};
    return v;
  };
}


//template<typename A>
//Parser<A> bind(std::function< std::vector<std::tuple<A, std::string>> (std::string)> f) {
//  return f;
//}

//bind :: Parser a -> (a -> Parser b) -> Parser b
template <typename A, typename B>
Parser<B> bind(Parser<A> p, std::function<Parser<B> (A)> f) {
  return [=] (std::string str) {
    auto v = std::vector<std::tuple<B, std::string>> {};
    auto ra = p(str);
    for(auto t : ra) {
      auto raa  = f(t);
      for(auto tt: raa) {
	v.push_back(tt);
      }
    }    
    return v;    
  };
}
