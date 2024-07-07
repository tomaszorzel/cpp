#ifndef MASTERMIND_H_
#define MASTERMIND_H_

#include <sys/types.h>

#include <cstdint>
#include <string>
#include <vector>

class Game {  // TODO: it would be good to have it singleton
 public:
  bool Play();

 private:
  char PrintWelcomMsg();
  void NewGame();
  void PrintOnScreen(const std::vector<std::string>&);
  bool ValidateUsersAnswer(std::string);
  void PutToUsersAnswer(std::string, char*);
  bool CompareWithCode(char*, std::vector<char>&);
  void GenerateCode();
  void UpdateScreenMsg(std::vector<std::string>&, char*, std::vector<char>&, int);
  void UpdateLooseMsg();

  static constexpr uint8_t kSize = 4;
  static constexpr uint8_t kMaxFeedbackSize = 4;
  static constexpr char kBlack = 'b';
  static constexpr char kWhite = 'w';
  static constexpr uint8_t kNoOfGuesses = 10;
  static constexpr uint8_t kGreaterThanSignIndex = 2;
  static constexpr uint8_t kCodeInfoInLostGameMsg = 2;
  static constexpr uint8_t kSpaceAsciiCode = 32;

  char code[kSize];
  std::vector<std::string> welcome_msg = {
      "Welcome in Mastermind v1!\n"
      "   Options:\n"
      "   n - new game\n",
      "   q - quit\n", "Choose option (ctrl+c to kill): "};
  std::vector<std::string> win_msg = {
      "\n|------------------------|\n",
      "|   Congrats, you win!   |\n",
      "|------------------------|\n\n",
  };
  std::vector<std::string> loose_msg = {
      "\n|------------------------|\n",
      "|       You loose!       |\n",
      "|------------------------|\n\n",
  };
};

#endif  // MASTERMIND_H_