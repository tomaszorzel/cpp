#include "mastermind.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <set>
#include <thread>
#include <vector>

using namespace std;

bool Game::play() {
  system("reset");
  char users_answer = print_welcome_msg();
  switch (users_answer) {
    case 'n':
      new_game();
      break;
    case 'q':
      cout << "Goodbye.." << endl;
      return false;
    default:
      break;
  }
  return true;
}

char Game::print_welcome_msg() {
  print_on_screen(welcome_msg);

  char users_answer;

  cin >> users_answer;
  while (cin && isalpha(users_answer) && (users_answer != 'n' && users_answer != 'q')) {
    cout << "Choose option (ctrl+c to kill): ";
    cin >> users_answer;
  }

  return users_answer;
}

void Game::new_game() {
    generate_code();
    vector<string> game_screen = {
        "|-----------------|\n",
        "|  Mastermind v1  |\n",
        "|  Colors codes:  |\n",
        "|  1: blue        |\n",
        "|  2: green       |\n",
        "|  3: red         |\n",
        "|  4: yellow      |\n",
        "|  5: cyan        |\n",
        "|  6: orange      |\n",
        "|-----------------|\n",
        "|  Feedback:      |\n",
        "|  b: black       |\n",
        "|  w: white       |\n",
        "|-----------------|\n",
        "|   XXXX   |      |\n",
        "|-----------------|\n",//15th line
        "|          |      |\n",
        "|          |      |\n",
        "|          |      |\n",
        "|          |      |\n",
        "|          |      |\n",
        "|          |      |\n",
        "|          |      |\n",
        "|          |      |\n",
        "|          |      |\n",
        "| >        |      |\n",
        "|-----------------|\n",
        "Provide your answer (ctrl+c to kill): "
    };

    for (int i = NO_OF_GUESSES; i > 0; i--) {
      system("clear");
      print_on_screen(game_screen);
      string input;
      char users_answer[4];
      cin >> input;
      while (cin && !validate_users_answer(input)) {
        cout << "Incorrect format of the answer, provide again: ";
        cin >> input;
      }
      put_to_users_answer(input, users_answer);
      vector<char> feedback;
      if (compare_with_code(users_answer, feedback)) {
        // user wins
        print_on_screen(win_msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        return;
      }
      update_screen_msg(game_screen, users_answer, feedback, i + 15);  // 15 is added to modify proper line
    }
    print_on_screen(loose_msg);
    loose_msg.erase(loose_msg.begin() + CODE_INFO_IN_LOST_GAME_MSG);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

void Game::print_on_screen(const vector<string>& msg) {
  for (auto line : msg) cout << line;
}

void Game::update_screen_msg(vector<string>& screen_msg, char* users_answer, vector<char>& feedback, int guess) {
  // add user's answer to screen msg without ">"
  screen_msg[guess] = "|   ";
  for (int i = 0; i < SIZE; i++) {
    screen_msg[guess] += users_answer[i];
  }
  screen_msg[guess] += "   | ";

  // add feedback info for the user to screen msg
  int feedback_size = feedback.size();
  if (feedback_size < MAX_FEEDBACK_SIZE) {
    for (int i = feedback_size; i < MAX_FEEDBACK_SIZE; i++) {
      feedback.push_back(SPACE_ASCII_CODE);  // ASCI code for space
    }
  }

  for (long unsigned int i = 0; i < feedback.size(); i++)  // feedback.length() won't be longer than 4
  {
    screen_msg[guess] += feedback[i];
  }
  screen_msg[guess] += " |\n";

  // add ">" to next guess' line
  if (guess != 1) screen_msg[guess - 1][GREATER_THAN_SIGN_INDEX] = '>';

  // clear the feedback before next guess
  feedback.clear();
}

bool Game::validate_users_answer(string input) {
  for (int i = 0; i < SIZE; i++) {
    if (isdigit(input[i]) == false || input[i] < '1' || input[i] > '6') return false;
  }
  return true;
}

void Game::put_to_users_answer(string input, char* users_answer) {
  for (int i = 0; i < SIZE; i++) {
    users_answer[i] = input[i];
  }
}

bool Game::compare_with_code(char* users_answer, vector<char>& feedback) {
  bool result = false;
  set<char> already_white_or_black;
  for (int i = 0; i < SIZE; i++) {
    if (code[i] == users_answer[i])  // BLACK
    {
      // remove 'w' from feedback and insert 'b' instead
      if (already_white_or_black.find(users_answer[i]) != already_white_or_black.end()) {
        for (long unsigned int j = 0; j < feedback.size(); j++) {
          if (feedback[j] == 'w') {
            feedback.erase(feedback.begin() + j);
            break;
          }
        }
      }
      feedback.push_back(BLACK);
      already_white_or_black.insert(users_answer[i]);
    } else  // WHITE
    {
      if (already_white_or_black.find(users_answer[i]) == already_white_or_black.end()) {
        if (count(code, code + SIZE, users_answer[i])) {
          feedback.push_back(WHITE);
          already_white_or_black.insert(users_answer[i]);
        }
      }
    }
  }
  sort(feedback.begin(), feedback.end());

  if (feedback.size() == MAX_FEEDBACK_SIZE && count(feedback.begin(), feedback.end(), 'b') == MAX_FEEDBACK_SIZE)
    result = true;

  return result;
}

void Game::generate_code() {
  random_device os_seed;
  const uint_least32_t seed = os_seed();

  mt19937 generator(seed);
  uniform_int_distribution<uint_least32_t> distribution(49, 54);

  for (int i = 0; i < SIZE; i++) {
    code[i] = distribution(generator);
    cout << code[i];
  }
  cout << endl;
  update_loose_msg();
}

void Game::update_loose_msg() {
  string code_info = "|       Code: ";
  for (int i = 0; i < SIZE; i++) code_info += code[i];
  code_info += "       |\n";
  loose_msg.insert(loose_msg.begin() + CODE_INFO_IN_LOST_GAME_MSG, code_info);
}