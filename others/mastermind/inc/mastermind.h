#ifndef MASTERMIND_H_
#define MASTERMIND_H_

#include <string>
#include <vector>

#define SIZE 4
#define MAX_FEEDBACK_SIZE 4
#define BLACK 'b'
#define WHITE 'w'
#define NO_OF_GUESSES 10
#define GREATER_THAN_SIGN_INDEX 2
#define CODE_INFO_IN_LOST_GAME_MSG 2
#define SPACE_ASCII_CODE 32

class Game { // TODO: it would be good to have it singleton
  char code[SIZE];
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
  char print_welcome_msg();
  void new_game();
  void print_on_screen(const std::vector<std::string>&);
  bool validate_users_answer(std::string);
  void put_to_users_answer(std::string, char*);
  bool compare_with_code(char*, std::vector<char>&);
  void generate_code();
  void update_screen_msg(std::vector<std::string>&, char*, std::vector<char>&, int);
  void update_loose_msg();

 public:
  bool play();
};

#endif  // MASTERMIND_H_