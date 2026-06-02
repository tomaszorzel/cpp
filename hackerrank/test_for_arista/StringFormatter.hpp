#include "string"

class StringFormatter {
 public:
  StringFormatter() = delete;
  explicit StringFormatter(std::string input) : input_(std::move(input)) {}
  void Format();
  std::string GetFormatted() { return formatted_; }

 private:
  void DecorateResult();
  std::string input_;
  std::string formatted_;
};