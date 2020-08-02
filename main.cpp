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
  std::map<char,std::string> first;
  std::map<char,std::string> follow;
  void showTerminal() {
    std::cout << "[INFO] : Terminal Symbol" << std::endl;
    for(int i=0;i<terminal.size();i++) {
        std::cout << terminal.at(i) << " ";
    }
    std::cout << std::endl;
  }
  void showNoterminal() {
    std::cout << "[INFO] : No-Terminal Symbol" << std::endl;
    for(int i=0;i<noterminal.size();i++) {
        std::cout << noterminal.at(i) << " ";
    }
    std::cout <<  std::endl;
  }
  int readTerminal() {
    std::cout << "[INFO] : Read Terminal Symbol .." << std::endl;
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
  int readNoTerminal() {
    std::cout << "[INFO] : Read No-Terminal Symbol .." << std::endl;
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
  int readRules() {
    std::cout << "[INFO] : Reading Rules .." << std::endl;
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
    std::cout << "[INFO] : Read axiom .." << std::endl;
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
  void showFirst() {
    std::cout << "[INFO] First Table  " << std::endl;
    for (auto it=first.begin(); it!=first.end(); ++it)
      std::cout <<  it->first << " : " << it->second << std::endl;
  }
}grammar;
struct grammar Mygrammer;
struct state {
  int number;
  std::vector<std::string> rules;
}state;
std::vector<struct state> states;
std::vector<int> statesNotProcessed;
std::map<std::pair<int,char> ,int> automate;
std::map<std::pair<int,char> ,std::string> action;
std::string generateFirst(const char c) {
  std::map<char,std::string>::iterator retFind = Mygrammer.first.find(c);
  if(retFind != Mygrammer.first.end() ) {
    return retFind->second;
  }
  else {
    std::string First,RetFirst;
    std::vector<std::string> rulesStart = Mygrammer.getRulesStart(c);
    std::string rule;
    int index_epsilon=0;
    for(int i=0;i<rulesStart.size();i++) {
      rule = rulesStart.at(i).substr(3);
      if(Mygrammer.isTerminal(rule[0]) || rule[0]=='@') {
        if(First.find(rule[0])==std::string::npos) {
          First.push_back(rule[0]);
        }
      }
      else if(rule[0] != c) {
        do {
          if(index_epsilon == rule.length()) {
            break;
          }
          if(First.find("@")!=std::string::npos) {
            First.erase(First.find("@"));
          }
          RetFirst=generateFirst(rule[index_epsilon]);
          index_epsilon++;
          First += RetFirst;
        }while((RetFirst.find("@"))!=std::string::npos);
      }
    }
    Mygrammer.first.insert(std::pair<char,std::string>(c,First));
    return First;
  }
}
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
  std::pair<int,char> key(stSource.number,c);
  int ret_add;
  extendRules(st);
  ret_add = addState(st);
  if(ret_add == -1) {
    if(st.number == 0) {
      std::cout << "[INFO] : I"<< st.number << " state" << std::endl;
      showRules(st.rules);
      gotoRules(st);
    }
    else {
      automate.insert(std::pair<std::pair<int,char> ,int>(key,st.number));
      statesNotProcessed.push_back(st.number);
      std::cout << "[INFO] :  FROM I" << stSource.number << " state WITH " << c << " GOTO I" <<  st.number << " state" << std::endl;
      showRules(st.rules);
    }
  }
  else {
      automate.insert(std::pair<std::pair<int,char> ,int>(key,ret_add));
      std::cout << "[INFO] :  FROM I" << stSource.number << " state WITH " << c << " GOTO I" <<  ret_add << " state" << std::endl;
  }
}
struct state getStateByNumber(const int number) {
  int i;
  for(i=0;i<states.size();i++) {
    if(states.at(i).number == number) {
      break;
    }
  }
  return states.at(i);
}
void generateState() {
  struct state st = getStateByNumber(statesNotProcessed.at(0));
  gotoRules(st);
  statesNotProcessed.erase(statesNotProcessed.begin());
}
void showAutomate() {
  std::cout << "Grammar Automate : " << std::endl;
  for (auto it=automate.begin(); it!=automate.end(); ++it)
    std::cout << "FROM I"<< it->first.first << " WITH " << it->first.second << " GOTO I" << it->second << std::endl;
}
void generateAction() {
  std::vector<char> CharToRead;
  for(int i=0;i<states.size();i++) {
    CharToRead = getElementToRead(states.at(i));
    for(int j=0;j<CharToRead.size();j++) {
      if(Mygrammer.isTerminal(CharToRead.at(j))) {
        std::cout << states.at(i).number << " : " << CharToRead.at(j) << " -> S "<< automate.at(std::pair<int,char>(states.at(i).number,CharToRead.at(j))) << std::endl;
      }
    }
  }
}
int main() {
  Mygrammer.file.open("grammar0");

  if(Mygrammer.getAxiom() == -1) {
    std::cout << "[ERROR] : unable to get axiom" << std::endl;
  }
  std::cout << "[INFO] : Axiom " << std::endl;
  //std::cout << Mygrammer.axiom << std::endl;

  if(Mygrammer.readTerminal() == -1) {
    std::cout << "[ERROR] : unable to get terminal" << std::endl;
  }
  //Mygrammer.showTerminal();

  if(Mygrammer.readNoTerminal() == -1) {
    std::cout << "[ERROR] : unable to get no-terminal" << std::endl;
  }
  //Mygrammer.showNoterminal();

  if(Mygrammer.readRules() == -1) {
    std::cout << "[ERROR] : unable to get Rules" << std::endl;
  }
  std::cout << "[INFO] : Grammar Rules" << std::endl;
  //showRules(Mygrammer.rules);

  generateFirst(Mygrammer.axiom);
  Mygrammer.showFirst();

  std::cout << "[INFO] : Adding '.' to Grammar Rules .." << std::endl;
  putDot(Mygrammer.rules);
  std::cout << "[INFO] : Grammar Rules" << std::endl;
  showRules(Mygrammer.rules);
  /*
  struct state i = generateFirstState();
  processState(i,'\0',i);
  while(statesNotProcessed.size()!=0) {
    generateState();
  }

  showAutomate();

  generateAction();
  */



}
