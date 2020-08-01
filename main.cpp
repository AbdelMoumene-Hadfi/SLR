#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <map>
#include <utility>

void processState(struct state& st,const char c,const struct state stSource);
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
  std::vector<std::string>  getRulesStart(char element) {
    std::vector<std::string> rulesFrom;
    for(int i=0;i<rules.size();i++) {
      if(rules.at(i)[0]==element) {
        rulesFrom.push_back(rules.at(i));
      }
    }
    return rulesFrom;
  }
}grammar;
struct grammar Mygrammer;
struct state {
  int number;
  std::vector<std::string> rules;
}state;
std::vector<struct state> states;
std::vector<int> statesNotProcessed;
std::map<std::pair<struct state,char> ,struct state> table_goto;
void showRules(std::vector<std::string> rules) {
  for(int i=0;i<rules.size();i++) {
      std::cout << rules.at(i) << std::endl;
  }
}
bool sameState(const struct state st,const struct state st2) {
  bool same = true;
  if(st.rules.size() != st2.rules.size()) {
    same = false ;
  }
  else {
    for(int i=0;i<st.rules.size();i++) {
      if(st.rules.at(i).compare(st2.rules.at(i)) != 0) {
          same = false;
          break;
      }
    }
  }
  return same;
}
int addState(const struct state st) {
  for(int i=0;i<states.size();i++) {
    if(sameState(st,states.at(i))) {
      return states.at(i).number;
    }
  }
  states.push_back(st);
  return -1;
}
void putDot(std::vector<std::string>& rules) {
  std::string rule;
  std::string dot(".");
  for(int i=0;i<rules.size();i++) {
    rule = rules.at(i).substr(0,3)+dot+rules.at(i).substr(3);
    rules.at(i)=rule;
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
    insert = true;
    afterDot = getElementAfterDot(st.rules.at(i));

    if(afterDot=='\0') {
      continue;
    }
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
bool ruleExist(const std::vector<std::string> rules,const std::string rule) {
  bool exist = false ;
  for(int i=0;i<rules.size();i++) {
    if(rule.compare(rules.at(i)) == 0) {
      exist = true ;
      break;
    }
  }
  return exist;
}
void extendRules(struct state& st) {
  bool recall = false;
  std::vector<std::string> rules;
  std::string rule;
  std::vector<char> CharToRead = getElementToRead(st);
  for(int i=0;i<CharToRead.size();i++) {
    rules = Mygrammer.getRulesStart(CharToRead.at(i));
    for(int j=0;j<rules.size();j++) {
      rule = rules.at(j);
      if(!ruleExist(st.rules,rule)) {
        if(!Mygrammer.isTerminal(rule[0])) {
          recall = true ;
        }
        st.rules.push_back(rule) ;
      }
    }
  }
  if(recall) {
      extendRules(st);
  }
}
struct state generateFirstState() {
  std::vector<std::string> rules;
  std::string rule("S->");
  rule.push_back(Mygrammer.axiom);
  rules.push_back(rule);
  struct state st {
    .number = (int)states.size(),
    .rules = rules,};
  putDot(st.rules);
  return st;
}
void gotoRules(const struct state st) {
  std::vector<char> CharToRead = getElementToRead(st);
  std::vector<std::string> rules2;
  struct state stGenerated;
  int ret ;
  for(int i=0;i<CharToRead.size();i++) {
    rules2 = getrules(st,CharToRead.at(i));
    for(int j=0;j<rules2.size();j++) {
      ret=moveDot(rules2.at(j));
      if(ret == -1 || ret == -2) {
        std::cout << "[ERROR] : Unable to move dot " << CharToRead.at(i) << std::endl;
      }
    }
    stGenerated.number = states.size();
    stGenerated.rules = rules2 ;
    processState(stGenerated,CharToRead.at(i),st);
  }
}
void processState(struct state& st,const char c,const struct state stSource) {
  int ret_add;
  extendRules(st);
  ret_add = addState(st);
  if(ret_add == -1) {
    if(st.number == 0) {
      std::cout << "[INFO] : "<< st.number << " state" << std::endl;
      showRules(st.rules);
      gotoRules(st);
    }
    else {
      statesNotProcessed.push_back(st.number);
      std::cout << "[INFO] :  FROM " << stSource.number << " state WITH " << c << " GOTO " <<  st.number << " state" << std::endl;
      showRules(st.rules);
    }
  }
  else {
        std::cout << "[INFO] :  FROM " << stSource.number << " state WITH " << c << " GOTO " <<  ret_add << " state" << std::endl;
  }
}
struct state getStateNumber(const int number) {
  int i;
  for(i=0;i<states.size();i++) {
    if(states.at(i).number == number) {
      break;
    }
  }
  return states.at(i);
}
void generateState() {
  struct state st = getStateNumber(statesNotProcessed.at(0));
  gotoRules(st);
  statesNotProcessed.erase(statesNotProcessed.begin());
}
int main() {
  Mygrammer.file.open("grammar0");

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
  std::cout << "[INFO] : Grammar Rules" << std::endl;
  showRules(Mygrammer.rules);
  std::cout << "[INFO] : Adding '.' to Grammar Rules .." << std::endl;
  putDot(Mygrammer.rules);
  std::cout << "[INFO] : Grammar Rules" << std::endl;
  showRules(Mygrammer.rules);

  struct state i = generateFirstState();
  processState(i,'\0',i);
  while(statesNotProcessed.size()!=0) {
    generateState();
  }
}
