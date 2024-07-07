#include "mastermind.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <set>
#include <thread>
#include <vector>

using namespace std;

bool Game::Play() {
  system("reset");
  char users_answer = PrintWelcomMsg();
  switch (users_answer) {
    case 'n':
      NewGame();
      break;
    case 'q':
      cout << "Goodbye.." << endl;
      return false;
    default:
      break;
  }
  return true;
}

char Game::PrintWelcomMsg() {
  PrintOnScreen(welcome_msg);

  char users_answer;

  cin >> users_answer;
  while (cin && isalpha(users_answer) && (users_answer != 'n' && users_answer != 'q')) {
    cout << "Choose option (ctrl+c to kill): ";
    cin >> users_answer;
  }

  return users_answer;
}

void Game::NewGame() {
    GenerateCode();
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

    for (int i = kNoOfGuesses; i > 0; i--) {
      system("clear");
      PrintOnScreen(game_screen);
      string input;
      char users_answer[4];
      cin >> input;
      while (cin && !ValidateUsersAnswer(input)) {
        cout << "Incorrect format of the answer, provide again: ";
        cin >> input;
      }
      PutToUsersAnswer(input, users_answer);
      vector<char> feedback;
      if (CompareWithCode(users_answer, feedback)) {
        // user wins
        PrintOnScreen(win_msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        return;
      }
      UpdateScreenMsg(game_screen, users_answer, feedback, i + 15);  // 15 is added to modify proper line
    }
    PrintOnScreen(loose_msg);
    loose_msg.erase(loose_msg.begin() + kCodeInfoInLostGameMsg);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

void Game::PrintOnScreen(const vector<string>& msg) {
  for (auto line : msg) cout << line;
}

void Game::UpdateScreenMsg(vector<string>& screen_msg, char* users_answer, vector<char>& feedback, int guess) {
  // add user's answer to screen msg without ">"
  screen_msg[guess] = "|   ";
  for (int i = 0; i < kSize; i++) {
    screen_msg[guess] += users_answer[i];
  }
  screen_msg[guess] += "   | ";

  // add feedback info for the user to screen msg
  int feedback_size = feedback.size();
  if (feedback_size < kMaxFeedbackSize) {
    for (int i = feedback_size; i < kMaxFeedbackSize; i++) {
      feedback.push_back(kSpaceAsciiCode);  // ASCI code for space
    }
  }

  for (long unsigned int i = 0; i < feedback.size(); i++)  // feedback.length() won't be longer than 4
  {
    screen_msg[guess] += feedback[i];
  }
  screen_msg[guess] += " |\n";

  // add ">" to next guess' line
  if (guess != 1) screen_msg[guess - 1][kGreaterThanSignIndex] = '>';

  // clear the feedback before next guess
  feedback.clear();
}

bool Game::ValidateUsersAnswer(string input) {
  for (int i = 0; i < kSize; i++) {
    if (isdigit(input[i]) == false || input[i] < '1' || input[i] > '6') return false;
  }
  return true;
}

void Game::PutToUsersAnswer(string input, char* users_answer) {
  for (int i = 0; i < kSize; i++) {
    users_answer[i] = input[i];
  }
}

bool Game::CompareWithCode(char* users_answer, vector<char>& feedback) {
  bool result = false;
  set<char> already_white_or_black;
  for (int i = 0; i < kSize; i++) {
    if (code[i] == users_answer[i])  // kBlack
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
      feedback.push_back(kBlack);
      already_white_or_black.insert(users_answer[i]);
    } else  // kWhite
    {
      if (already_white_or_black.find(users_answer[i]) == already_white_or_black.end()) {
        if (count(code, code + kSize, users_answer[i])) {
          feedback.push_back(kWhite);
          already_white_or_black.insert(users_answer[i]);
        }
      }
    }
  }
  sort(feedback.begin(), feedback.end());

  if (feedback.size() == kMaxFeedbackSize && count(feedback.begin(), feedback.end(), 'b') == kMaxFeedbackSize)
    result = true;

  return result;
}

void Game::GenerateCode() {
  random_device os_seed;
  const uint_least32_t seed = os_seed();

  mt19937 generator(seed);
  uniform_int_distribution<uint_least32_t> distribution(49, 54);

  for (int i = 0; i < kSize; i++) {
    code[i] = distribution(generator);
    cout << code[i];
  }
  cout << endl;
  UpdateLooseMsg();
}

void Game::UpdateLooseMsg() {
  string code_info = "|       Code: ";
  for (int i = 0; i < kSize; i++) code_info += code[i];
  code_info += "       |\n";
  loose_msg.insert(loose_msg.begin() + kCodeInfoInLostGameMsg, code_info);
}