#include "StringFormatter.hpp"

#include <algorithm>
#include <cstddef>

void StringFormatter::Format() {
  std::string temp = std::move(input_);
  auto new_end = std::remove(temp.begin(), temp.end(), '-');
  temp.erase(new_end, temp.end());
  new_end = std::remove(temp.begin(), temp.end(), ' ');
  temp.erase(new_end, temp.end());

  int column_length = 3;
  std::size_t characters_left = temp.size();
  int character_in_column_count = 0;
  for (const auto character : temp) {
    if ((characters_left == 4 && temp.size() % 3 == 1) || (characters_left == 2 && temp.size() % 3 == 2)) {
      column_length = 2;
    }
    formatted_.push_back(character);
    character_in_column_count++;
    characters_left--;
    if (character_in_column_count == column_length && characters_left > 0) {
      formatted_.push_back(' ');
      character_in_column_count = 0;
    }
  }
  DecorateResult();
}

void StringFormatter::DecorateResult() { formatted_.push_back(';'); }