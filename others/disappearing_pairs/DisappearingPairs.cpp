#include "DisappearingPairs.hpp"
#include <algorithm>
#include <stack>

std::string DisappearingPairs::Solution(std::string& S) {
  while (S.contains("AA") || S.contains("BB") || S.contains("CC")) {
    RemovePairFrom(S);
  }
  return S;
}

bool DisappearingPairs::UpdateIfPolindrome(std::string& test_string) {
  if (test_string.size()%2 == 0) {
    std::string rev = test_string;
    std::ranges::reverse(rev.begin(), rev.end());
    if (rev == test_string) {
      test_string = "";
      return true;
    }
  }
  return false;
}


void DisappearingPairs::RemovePairFrom(std::string& test_string) {
  if (test_string.empty() || test_string.size() == 1) {
    return;
  } else if (test_string.size() == 2 && test_string.at(0) == test_string.at(1)) {
    test_string = "";
    return;
  } else {
    if (UpdateIfPolindrome(test_string)) {
      return;
    } else {
      std::stack<char> sequence;
      sequence.push(test_string.at(0));
      for (auto it = test_string.begin() + 1; it != test_string.end(); it++) {
        if (sequence.empty()) {
          sequence.push(*it);
        } else if (sequence.top() != *it) {
          sequence.push(*it);
        } else {
          sequence.pop();
        }
      }
      test_string.resize(sequence.size());
      for (auto it = test_string.rbegin(); it != test_string.rend(); it++) {
        *it = sequence.top();
        sequence.pop();
      }
    }
  }
}
