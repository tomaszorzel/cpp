#include "../inc/mastermind.h"
#include <sys/types.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <random>
#include <set>
#include <thread>
#include <vector>

bool Game::Play() {
  system("reset");
  char users_answer = PrintWelcomMsg();
  switch (users_answer) {
    case 'n':
      NewGame();
      break;
    case 'q':
      std::cout << "Goodbye..\n";
      return false;
    default:
      break;
  }
  return true;
}

char Game::PrintWelcomMsg() {
  PrintOnScreen(welcome_msg);

  char users_answer = 0;

  std:: cin >> users_answer;
  while (std::cin && (isalpha(users_answer) != 0) && (users_answer != 'n' && users_answer != 'q')) {
    std::cout << "Choose option (ctrl+c to kill): ";
    std::cin >> users_answer;
  }

  return users_answer;
}

void Game::NewGame() {
    GenerateCode();
    std::vector<std::string> game_screen;
    game_screen.emplace_back("|-----------------|\n");
    game_screen.emplace_back("|-----------------|\n");
    game_screen.emplace_back("|  Mastermind v1  |\n");
    game_screen.emplace_back("|  Colors codes:  |\n");
    game_screen.emplace_back("|  1: blue        |\n");
    game_screen.emplace_back("|  2: green       |\n");
    game_screen.emplace_back("|  3: red         |\n");
    game_screen.emplace_back("|  4: yellow      |\n");
    game_screen.emplace_back("|  5: cyan        |\n");
    game_screen.emplace_back("|  6: orange      |\n");
    game_screen.emplace_back("|-----------------|\n");
    game_screen.emplace_back("|  Feedback:      |\n");
    game_screen.emplace_back("|  b: black       |\n");
    game_screen.emplace_back("|  w: white       |\n");
    game_screen.emplace_back("|-----------------|\n");
    game_screen.emplace_back("|   XXXX   |      |\n");
    game_screen.emplace_back("|-----------------|\n");  // 15th line
    game_screen.emplace_back("|          |      |\n");
    game_screen.emplace_back("|          |      |\n");
    game_screen.emplace_back("|          |      |\n");
    game_screen.emplace_back("|          |      |\n");
    game_screen.emplace_back("|          |      |\n");
    game_screen.emplace_back("|          |      |\n");
    game_screen.emplace_back("|          |      |\n");
    game_screen.emplace_back("|          |      |\n");
    game_screen.emplace_back("|          |      |\n");
    game_screen.emplace_back("| >        |      |\n");
    game_screen.emplace_back("|-----------------|\n");
    game_screen.emplace_back("Provide your answer (ctrl+c to kill): ");

    for (int i = kNoOfGuesses; i > 0; i--) {
      system("clear");
      PrintOnScreen(game_screen);
      std::string input;
      const u_int16_t answer_size = 4;
      std::array<char, answer_size> users_answer{};
      std::cin >> input;
      while (std::cin && !ValidateUsersAnswer(input)) {
        std::cout << "Incorrect format of the answer, provide again: ";
        std::cin >> input;
      }
      PutToUsersAnswer(input, users_answer.data());
      std::vector<char> feedback;
      if (CompareWithCode(users_answer.data(), feedback)) {
        // user wins
        PrintOnScreen(win_msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        return;
      }
      UpdateScreenMsg(game_screen, users_answer.data(), feedback, i + 15);  // 15 is added to modify proper line
    }
    PrintOnScreen(loose_msg);
    loose_msg.erase(loose_msg.begin() + kCodeInfoInLostGameMsg);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

void Game::PrintOnScreen(const std::vector<std::string>& msg) {
  for (auto line : msg) std::cout << line;
}

void Game::UpdateScreenMsg(std::vector<std::string>& screen_msg, char* users_answer, std::vector<char>& feedback, int guess) {
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

bool Game::ValidateUsersAnswer(std::string input) {
  for (int i = 0; i < kSize; i++) {
    if (isdigit(input[i]) == false || input[i] < '1' || input[i] > '6') return false;
  }
  return true;
}

void Game::PutToUsersAnswer(std::string input, char* users_answer) {
  for (int i = 0; i < kSize; i++) {
    users_answer[i] = input[i];
  }
}

bool Game::CompareWithCode(char* users_answer, std::vector<char>& feedback) {
  bool result = false;
  std::set<char> already_white_or_black;
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
        if (std::count(code, code + kSize, users_answer[i])) {
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
  std::random_device os_seed;
  const uint_least32_t seed = os_seed();

  std::mt19937 generator(seed);
  std::uniform_int_distribution<uint_least32_t> distribution(49, 54);

  for (int i = 0; i < kSize; i++) {
    code[i] = distribution(generator);
    std::cout << code[i];
  }
  std::cout << std::endl;
  UpdateLooseMsg();
}

void Game::UpdateLooseMsg() {
  std::string code_info = "|       Code: ";
  for (int i = 0; i < kSize; i++) code_info += code[i];
  code_info += "       |\n";
  loose_msg.insert(loose_msg.begin() + kCodeInfoInLostGameMsg, code_info);
}