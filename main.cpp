#include <iostream>
#include <fstream>
#include <tuple>
#include <functional>
#include <vector>
#include <string>

namespace parsec {

  template <typename A>
  using ResultTuple = std::tuple<A, std::string>;

  template <typename A>
  using ResultVector = std::vector<ResultTuple<A>>;

  // Parser :: a -> [(a, String)]
  template<typename A>
  using Parser = std::function< ResultVector<A> (std::string)>;


  template <typename A>
  Parser<A> result(A a) {
    return std::bind([](A& a, std::string str) {
	ResultVector<A> v;
	v.emplace_back(std::move(a), std::move(str));
	return v;
      }, std::move(a), std::placeholders::_1);
  }


  //bindParsers :: Parser a -> (a -> Parser b) -> Parser b
  template <typename A, typename B>
  Parser<B> bindParsers(Parser<A> p, std::function<Parser<B> (A)> f) {
    return [=] (std::string inp) {
      ResultVector<B> retList {};
      ResultVector<A> pResult = p(inp);

      for(auto pResultItem : pResult) {
	A v = std::get<0>(pResultItem);
	std::string inpPrime = std::get<1>(pResultItem);
	Parser<B> newParser = f(v);
	ResultVector<B> innerResultVector = newParser(inpPrime);

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
      if (r1.size() == 0) {
	auto r2 = p2(str);
	return r2;
      }
      return r1;
    };
  }


  template<typename A>
  Parser<A> zero() {
    return [=] (std::string str) {
      ResultVector<A> v {};
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

  template <typename A>
  Parser<A> sat(std::function<bool(A)> p) {
    std::function<Parser<A>(A)> f = [=] (A c) {
      if(p(c)) {
	return result(c);
      } else {
	return zero<A>();
      }
    };
    return bindParsers(item(), f);
  }


//  template <typename A>
//  Parser <std::vector<A>> many (Parser<A> p) {
//    std::function<Parser<std::vector<A>>(A)> f = [=] (A c) {
//      std::function<Parser<std::vector<A>>(std::vector<A>)> g = [=] (std::vector<A> list) {
//	std::vector<A> r {c};
//	r.insert(r.end(), list.begin(), list.end());
//	return result(std::move(r));
//      };
//      return bindParsers(many(p), g);
//    };
//    std::vector<A> v {};
//    return plus(bindParsers(p, f), result(v));
//  }


  template <typename A>
  Parser <std::vector<A>> many1 (Parser<A> p);

  template <typename A>
  Parser <std::vector<A>> many (Parser<A> p) {
    Parser<std::vector<A>> pp = many1(p);
    return plus(pp, zero<std::vector<A>>());
  }

  template <typename A>
  Parser <std::vector<A>> many1 (Parser<A> p) {
    std::function<Parser<std::vector<A>>(A)> f = [=] (A c) {
      std::function<Parser<std::vector<A>>(std::vector<A>)> g = [=] (std::vector<A> list) -> Parser<std::vector<A>> {
	std::vector<A> r {c};
	r.insert(r.end(), list.begin(), list.end());
	return result(std::move(r));
      };
      return bindParsers(many(p), g);
    };
    std::vector<A> v {};
    return plus(bindParsers(p, f), result(v));
  }

  Parser<char> particularChar(char v) {
    return sat<char>([=] (char c) { return v==c; });
  }

  Parser<char> digit() {
    return sat<char>([] (char c) { return '0' <= c && '9' >=  c; });
  }
  Parser<char> lower() {
    return sat<char>([] (char c) { return 'a' <= c && 'z' >=  c; });
  }
  Parser<char> upper() {
    return sat<char>([] (char c) { return 'A' <= c && 'Z' >=  c; });
  }
  Parser<char> letter() {
    return plus(lower(), upper());
  }
  Parser<char> alphanum() {
    return plus(digit(), letter());
  }

}



int main(int argc, char *argv[]) {

  using namespace parsec;
  int i {10};
  auto pA = sat<char>([] (char c) { return c == 'A'; });
  auto pB = sat<char>([] (char c) { return c == 'B'; });
  Parser<char> pAorB = alphanum();
  Parser<std::vector<char>> pl = many(pAorB);

  //  auto p = seq1(item(), item());//result(i);
  auto x = pl("ABC1232,");
  std::cout << x.size() << std::endl;
  if (x.size() > 0) {
    for(auto v: x) {
      auto vv = std::get<0>(v);
      auto rem = std::get<1>(v);
      std::cout << "Result = ";
      for (auto vvv: vv){
	std::cout << vvv;
      }
      std::cout << " (remaining = " << rem << ")" << std::endl;
    }

  }else {
    std::cout << "No match" << std::endl;
  }

  return 0;
}
