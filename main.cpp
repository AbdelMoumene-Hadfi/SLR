#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <map>
#include <utility>


struct state {
  int number;
  std::vector<std::string> rules;
}state;
std::vector<struct state> states;
std::map<std::pair<struct state,char> ,struct state> table_goto;
std::vector<char> terminal;
std::vector<char> noterminal;
std::vector<std::string> rules;
std::ifstream file;
int getTerminal() {
  std::string line;
  std::getline(file,line);
  if(line.length()==0) {
    return -1;
  }
  for(int i=0;i<line.length();i++) {
    if(line[i]==' ') {
      continue;
    }
    terminal.push_back(line[i]);
  }
  return 0;
}
void showTerminal() {
  for(int i=0;i<terminal.size();i++) {
      std::cout << terminal.at(i) << " ";
  }
  std::cout << std::endl;
}
int getNoterminal() {
  std::string line;
  std::getline(file,line);
  if(line.length()==0) {
    return -1;
  }
  for(int i=0;i<line.length();i++) {
    if(line[i]==' ') {
      continue;
    }
    noterminal.push_back(line[i]);
  }
  return 0;
}
void showNoterminal() {
  for(int i=0;i<noterminal.size();i++) {
      std::cout << noterminal.at(i) << " ";
  }
  std::cout <<  std::endl;
}
int getRules() {
  std::string line;
  std::getline(file,line);
  if(line.length()==0) {
    return -1;
  }
  while(line.length()!=0){
    noterminal.push_back(line[0]);
    std::string rule(line.substr(0,3));
    for(int i=3;i<line.length();i++) {
      if(line[i] == '|') {
        rules.push_back(rule);
        rule.assign(line.substr(0,3));
        continue;
      }
      rule.push_back(line[i]);
      if(i == line.length()-1) {
        rules.push_back(rule);
      }
    }
    std::getline(file,line);
  }
  return 0;
}
void showRules() {
  for(int i=0;i<rules.size();i++) {
      std::cout << rules.at(i) << std::endl;
  }
}
bool isTerminal(char element) {
  for(int i=0;i<terminal.size();i++) {
      if(terminal.at(i) == element) {
        return true;
      }
  }
  return false;
}
bool sameState() {return true;}
int main() {
  file.open("grammar");

  std::cout << "[INFO] : getting terminal symbol .." << std::endl;
  if(getTerminal() == -1) {
    std::cout << "[ERROR] : unable to get terminal" << std::endl;
  }
  std::cout << "[INFO] : terminal symbol" << std::endl;
  showTerminal();

  std::cout << "[INFO] : getting no-terminal symbol .." << std::endl;
  if(getNoterminal() == -1) {
    std::cout << "[ERROR] : unable to get no-terminal" << std::endl;
  }
  std::cout << "[INFO] : noterminal symbol" << std::endl;
  showNoterminal();

  std::cout << "[INFO] : getting Rules .." << std::endl;
  if(getRules() == -1) {
    std::cout << "[ERROR] : unable to get Rules" << std::endl;
  }
  std::cout << "[INFO] : Rules" << std::endl;
  showRules();

  struct state i {
  .number = (int)states.size(),
  .rules = rules,};

}
