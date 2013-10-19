#include <iostream>
#include <fstream>
#include <regex>
#include <tuple>
#include <functional>


template <typename A>
using ResultTuple = std::tuple<A, std::string>;

template <typename A>
using ResultVector = std::vector<ResultTuple<A>>;

// Parser :: a -> [(a, String)]
template<typename A>
using Parser = std::function< ResultVector<A> (std::string)>;

template<typename A>
Parser<A> result(A a) {
  return [=] (std::string str) {
    auto t = ResultTuple<A> {a, str};
    auto v = ResultVector<A> {t};
    return v;
  };
}

template<typename A>
Parser<A> zero() {
  return [=] (std::string str) {
    auto v = ResultVector<A> {};
    return v;
  };
}


//template<typename A>
//Parser<A> bind(std::function< std::vector<std::tuple<A, std::string>> (std::string)> f) {
//  return f;
//}



//bind :: Parser a -> (a -> Parser b) -> Parser b
template <typename A, typename B>
Parser<B> bindParsers(Parser<A> p, std::function<Parser<B> (A)> f) {
  std::cout << "bindParsers called" << std::endl;
  return [=] (std::string inp) {
    ResultVector<B> retList {};
    ResultVector<A> pResult = p(inp);
    std::cout << "Got " << pResult.size() << " results" << std::endl;
    for(auto pResultItem : pResult) {
      A v = std::get<0>(pResultItem);
      std::string inpPrime = std::get<1>(pResultItem);
      Parser<B> newParser = f(v);
      ResultVector<B> innerResultVector = newParser(inpPrime);
      std::cout << "Got " << innerResultVector.size() << " results from inner" << std::endl;
      for(auto pInnerResultItem: innerResultVector) {
    	retList.push_back(pInnerResultItem);
      }
    }
    return retList;    
  };
}


template<typename A>
Parser<A>plus(Parser<A> p1, Parser<A> p2) {
  return [=] (std::string str) {
    auto r1 = p1(str);
    auto r2 = p2(str);
    r1.insert(r1.end(), r2.begin(), r2.end());
    return r1;
  };
}


