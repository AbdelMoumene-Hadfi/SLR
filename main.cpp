#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <map>
#include <utility>

struct grammar {
  std::ifstream file;
  char axiom;
  std::vector<char> terminal;
  std::vector<char> noterminal;
  std::vector<std::string> rules;
  void showTerminal() {
    for(int i=0;i<terminal.size();i++) {
        std::cout << terminal.at(i) << " ";
    }
    std::cout << std::endl;
  }
  void showNoterminal() {
    for(int i=0;i<noterminal.size();i++) {
        std::cout << noterminal.at(i) << " ";
    }
    std::cout <<  std::endl;
  }
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
  int getAxiom() {
    std::string line;
    std::getline(file,line);
    if(line.length()==0) {
      return -1;
    }
    axiom = line[0];
    return 0;
  }
  bool isTerminal(char element) {
    for(int i=0;i<terminal.size();i++) {
        if(terminal.at(i) == element) {
          return true;
        }
    }
    return false;
  }
}grammar;
struct grammar Mygrammer;
struct state {
  int number;
  std::vector<std::string> rules;
}state;
std::vector<struct state> states;
std::map<std::pair<struct state,char> ,struct state> table_goto;
void showRules(std::vector<std::string> rules) {
  for(int i=0;i<rules.size();i++) {
      std::cout << rules.at(i) << std::endl;
  }
}
bool sameState() {return true;}
void putDot(struct state& st) {
  std::string rule;
  std::string dot(".");
  for(int i=0;i<st.rules.size();i++) {
    rule = st.rules.at(i).substr(0,3)+dot+st.rules.at(i).substr(3);
    st.rules.at(i)=rule;
  }
}
int moveDot(std::string& rule) {
  std::string rule2;
  std::string dot(".");
  std::size_t index = rule.find('.');
  if (index==std::string::npos) {
      return -1;
  }
  if (index==rule.length()-1) {
      return -2;
  }
  rule2 = rule.substr(0,index) + rule[index+1] + dot ;
  for(int i = index+2;i<rule.length();i++) {
    rule2.push_back(rule[i]);
  }
  rule=rule2;
  return 0;
}
char getElementAfterDot(const std::string rule) {
  std::size_t index = rule.find('.');
  if (index==std::string::npos || index == rule.length()) {
      return '\0';
  }
  return rule[index+1];
}
std::vector<char> getElementToRead(const struct state st) {
  std::vector<char> ArrayElement;
  char afterDot;
  bool insert = true;
  for(int i=0;i<st.rules.size();i++) {
    afterDot = getElementAfterDot(st.rules.at(i));
    for(int j=0;j<ArrayElement.size();j++) {
      if(ArrayElement.at(j) == afterDot) {
        insert = false;
        break;
      }
    }
    if(insert) {
      ArrayElement.push_back(afterDot);
    }
  }
  return ArrayElement;

}
std::vector<std::string>  getrules(const struct state st,char element) {
  std::vector<std::string> rules;
  char afterDot;
  for(int i=0;i<st.rules.size();i++) {
    afterDot = getElementAfterDot(st.rules.at(i));
    if(afterDot == element) {
      rules.push_back(st.rules.at(i));
    }
  }
  return rules;
}
void gotoRules(const struct state st) {
  std::vector<char> Charlist = getElementToRead(st);
  std::vector<std::string> rules2;
  for(int i=0;i<Charlist.size();i++) {
    rules2 = getrules(st,Charlist.at(i));
    std::cout << "[INFO] : Rules after " << Charlist.at(i) << std::endl;
    showRules(rules2);
  }

}
struct state generateFirstState() {
  std::vector<std::string> rules;
  std::string rule("S->");
  rule.push_back(Mygrammer.axiom);
  rules.push_back(rule);
  struct state i {
    .number = (int)states.size(),
    .rules = rules,};
  return i;
}
int main() {
  Mygrammer.file.open("grammar");

  std::cout << "[INFO] : getting axiom .." << std::endl;
  if(Mygrammer.getAxiom() == -1) {
    std::cout << "[ERROR] : unable to get axiom" << std::endl;
  }
  std::cout << "[INFO] : axiom " << std::endl;
  std::cout << Mygrammer.axiom << std::endl;

  std::cout << "[INFO] : getting terminal symbol .." << std::endl;
  if(Mygrammer.getTerminal() == -1) {
    std::cout << "[ERROR] : unable to get terminal" << std::endl;
  }
  std::cout << "[INFO] : terminal symbol" << std::endl;
  Mygrammer.showTerminal();

  std::cout << "[INFO] : getting no-terminal symbol .." << std::endl;
  if(Mygrammer.getNoterminal() == -1) {
    std::cout << "[ERROR] : unable to get no-terminal" << std::endl;
  }
  std::cout << "[INFO] : noterminal symbol" << std::endl;
  Mygrammer.showNoterminal();

  std::cout << "[INFO] : getting Rules .." << std::endl;
  if(Mygrammer.getRules() == -1) {
    std::cout << "[ERROR] : unable to get Rules" << std::endl;
  }
  std::cout << "[INFO] : Rules" << std::endl;
  showRules(Mygrammer.rules);

  struct state i = generateFirstState();
  std::cout << "[INFO] : First State rules" << std::endl;
  showRules(i.rules);

  std::cout << "[INFO] : Adding '.' to Rules .." << std::endl;
  putDot(i);
  std::cout << "[INFO] : Rules" << std::endl;
  showRules(i.rules);
  /*
  int ret ;
  std::cout << "[INFO] : Move Dot" << std::endl;
  ret=moveDot(i.rules.at(0));
  std::cout << ret << std::endl;


  gotoRules(i);
  */


}
