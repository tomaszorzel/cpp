#include <string>

class DisappearingPairs {
 public:
  std::string Solution(std::string& S);

 private:
  void RemovePairFrom(std::string& test_string);
  bool UpdateIfPolindrome(std::string& test_string);
};