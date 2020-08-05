#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <map>
#include <utility>

/*
  Define grammar struct
*/
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
    std::string firstRule("S->");
    firstRule.push_back(axiom);
    rules.push_back(firstRule);
    std::getline(file,line);
    if(line.length()==0) {
      return -1;
    }
    while(line.length()!=0){
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
  void showFirst() {
    std::cout << "[INFO] First Table  " << std::endl;
    for (auto it=first.begin(); it!=first.end(); ++it)
      std::cout <<  it->first << " : " << it->second << std::endl;
  }
  void showFollow() {
    std::cout << "[INFO] Follow Table  " << std::endl;
    for (auto it=follow.begin(); it!=follow.end(); ++it)
      std::cout <<  it->first << " : " << it->second << std::endl;
  }
  bool isTerminal(const char element) {
    for(int i=0;i<terminal.size();i++) {
        if(terminal.at(i) == element) {
          return true;
        }
    }
    return false;
  }
  /*
    @@ ruleNumber() : get the position of rule in my grammer.
  */
  int ruleNumber(std::string rule) {
    std::string thisrule;
    rule.erase(rule.begin()+rule.find("."));
    for(int i=0;i<rules.size();i++) {
      thisrule = rules.at(i);
      thisrule.erase(thisrule.begin()+thisrule.find("."));
      if(rule.compare(thisrule) == 0) {
        return i;
      }
    }
    return -1;
  }
  /*
    @@ getRulesStart() : get all rules of my grammar that left term equal to 'element' .
    @@@ element : the left term .
  */
  std::vector<std::string>  getRulesStart(const char element) {
    std::vector<std::string> rulesFrom;
    for(int i=0;i<rules.size();i++) {
      if(rules.at(i)[0]==element) {
        rulesFrom.push_back(rules.at(i));
      }
    }
    return rulesFrom;
  }
  /*
    @@ getPositionSecond() : get a vector op position of an element in right term in a specific rule .
  */
  std::vector<int>  getPositionSecond(const std::string rule,const char element) {
    std::vector<int> Pos;
    for(int i=3;i<rule.length();i++) {
      if(rule[i]==element) {
        Pos.push_back(i);
      }
    }
    return Pos;
  }
}grammar;
struct grammar Mygrammer;
/*
  Define state struct .
  @@@ number : is the number of state .
  @@@ rules : rules that i have in this state.
*/
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
            First.erase(First.begin()+First.find("@"));
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
std::string generateFollow(const char c) {
  std::map<char,std::string>::iterator retFind = Mygrammer.follow.find(c);
  if(retFind != Mygrammer.follow.end() ) {
    return retFind->second;
  }
  else {
    std::string Follow,RetFollow;
    std::vector<int> pos;
    int index;
    for(int i=0;i<Mygrammer.rules.size();i++) {
      pos=Mygrammer.getPositionSecond(Mygrammer.rules.at(i),c);
      for(int j=0;j<pos.size();j++) {
        if(pos.at(j)==Mygrammer.rules.at(i).length()-1) {
          if(Mygrammer.rules.at(i)[0]!=c) {
            RetFollow=generateFollow(Mygrammer.rules.at(i)[0]);
            for(int k=0;k<RetFollow.size();k++) {
              if(Follow.find(RetFollow.at(k))==std::string::npos) {
                Follow.push_back(RetFollow.at(k));
              }
            }
          }
        }
        else {
          index = pos.at(j)+1;
          do {
            if(Mygrammer.isTerminal(Mygrammer.rules.at(i)[index])) {
              if(Follow.find(Mygrammer.rules.at(i)[index])==std::string::npos) {
                  Follow.push_back(Mygrammer.rules.at(i)[index]);
              }
              break;
            }
            else {
              RetFollow = generateFirst(Mygrammer.rules.at(i)[index]);
              for(int k=0;k<RetFollow.size();k++) {
                if(Follow.find(RetFollow.at(k))==std::string::npos) {
                  Follow.push_back(RetFollow.at(k));
                }
              }
              index++;
            }
          }while((RetFollow.find("@"))!=std::string::npos && index<Mygrammer.rules.at(i).length());
          if(index>Mygrammer.rules.at(i).length()) {
            if((RetFollow.find("@"))!=std::string::npos) {
              RetFollow=generateFollow(Mygrammer.rules.at(i)[0]);
              for(int k=0;k<RetFollow.size();k++) {
                if(Follow.find(RetFollow.at(k))==std::string::npos) {
                  Follow.push_back(RetFollow.at(k));
                }
              }
            }
          }
        }
      }
    }
    Mygrammer.follow.insert(std::pair<char,std::string>(c,Follow));
    return Follow;
  }
}
void generateFollows() {
  Mygrammer.follow.insert(std::pair<char,std::string>('S',"$"));
  for(int i=0;i<Mygrammer.noterminal.size();i++) {
    generateFollow(Mygrammer.noterminal.at(i));
  }
}
void showRules(std::vector<std::string> rules) {
  for(int i=0;i<rules.size();i++) {
      std::cout << rules.at(i) << std::endl;
  }
}
/*
  @@ sameState : check if the two state is the same .
*/
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
/*
  @@ addState : add state to 'states' .
*/
int addState(const struct state st) {
  for(int i=0;i<states.size();i++) {
    if(sameState(st,states.at(i))) {
      return states.at(i).number;
    }
  }
  states.push_back(st);
  return -1;
}
/*
  @@ putDot : add Dote in the first place in second term of rules .
*/
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
  rules.push_back(Mygrammer.rules.at(0));
  struct state st {
    .number = (int)states.size(),
    .rules = rules,};
  return st;
}
void processState(struct state& st,const char c,const struct state stSource) ;
void gotoRules(const struct state st) {
  std::vector<char> CharToRead = getElementToRead(st);
  std::vector<std::string> rules2;
  struct state stGenerated;
  int ret ;
  for(int i=0;i<CharToRead.size();i++) {
    if(CharToRead.at(i)=='@') {
      continue;
    }
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
  std::string rule,Follow,shift("S"),reduce("R");
  int rule_number;


  for(int i=0;i<states.size();i++) {
    //
    CharToRead = getElementToRead(states.at(i));
    for(int j=0;j<CharToRead.size();j++) {
      if(Mygrammer.isTerminal(CharToRead.at(j))) {
        shift.append(std::to_string(automate.at(std::pair<int,char>(states.at(i).number,CharToRead.at(j)))));
        action.insert(std::pair<std::pair<int,char> ,std::string>(std::pair<int,char>(states.at(i).number,CharToRead.at(j)),shift));
        shift.erase(shift.begin()+1,shift.end());
      }
    }
    //
    for(int j=0;j<states.at(i).rules.size();j++) {
      rule = states.at(i).rules.at(j);
      if(rule.find(".")==(rule.length()-1)) {
        if(rule[0]=='S') {
          action.insert(std::pair<std::pair<int,char> ,std::string>(std::pair<int,char>(states.at(i).number,'$'),"ACC"));
        }
        else {
          rule_number = Mygrammer.ruleNumber(rule);
          Follow = Mygrammer.follow.at(rule[0]);
          for(int k=0;k<Follow.length();k++) {
            reduce.append(std::to_string(rule_number));
            action.insert(std::pair<std::pair<int,char> ,std::string>(std::pair<int,char>(states.at(i).number,Follow.at(k)),reduce));
            reduce.erase(reduce.begin()+1,reduce.end());
          }
        }
      }
    }
  }
}
void showAction() {
  std::map<std::pair<int,char> ,std::string>::iterator retFind;
  std::cout << "Action Table: " << std::endl << "State" << "\t";
  for(int i=0;i<Mygrammer.terminal.size();i++) {
    std::cout << Mygrammer.terminal.at(i) << "\t";
  }
  std::cout << '$' << "\t" << std::endl;
  for (int i=0;i<states.size();i++) {
    std::cout << i << "\t";
    for(int j=0;j<Mygrammer.terminal.size();j++) {
      retFind = action.find(std::pair<int,char>(i,Mygrammer.terminal.at(j)));
      if(retFind != action.end() ) {
         std::cout << retFind->second << "\t";
      }
      else {
        std::cout <<  "\t";
      }
    }
    retFind = action.find(std::pair<int,char>(i,'$'));
    if(retFind != action.end() ) {
       std::cout << retFind->second << "\t";
    }
    else {
      std::cout <<  "\t";
    }
    std::cout << std::endl;
  }
}
void showGoto() {
  std::map<std::pair<int,char> ,int>::iterator retFind;
  std::cout << "[INFO] : GOTO Table " << std::endl << "State" << "\t";
  for(int i=0;i<Mygrammer.noterminal.size();i++) {
    std::cout << Mygrammer.noterminal.at(i) << "\t";
  }
  std::cout << std::endl;
  for (int i=0;i<states.size();i++) {
    std::cout << i << "\t";
    for(int j=0;j<Mygrammer.noterminal.size();j++) {
      retFind = automate.find(std::pair<int,char>(i,Mygrammer.noterminal.at(j)));
      if(retFind != automate.end() ) {
         std::cout << retFind->second << "\t";
      }
      else {
        std::cout <<  "\t";
      }
    }
    std::cout << std::endl;
  }
}
int main() {
  Mygrammer.file.open("grammar");

  if(Mygrammer.getAxiom() == -1) {
    std::cout << "[ERROR] : unable to get axiom" << std::endl;
  }
  std::cout << "[INFO] : Axiom " << std::endl;
  std::cout << Mygrammer.axiom << std::endl;

  if(Mygrammer.readTerminal() == -1) {
    std::cout << "[ERROR] : unable to get terminal" << std::endl;
  }
  Mygrammer.showTerminal();

  if(Mygrammer.readNoTerminal() == -1) {
    std::cout << "[ERROR] : unable to get no-terminal" << std::endl;
  }
  Mygrammer.showNoterminal();

  if(Mygrammer.readRules() == -1) {
    std::cout << "[ERROR] : unable to get Rules" << std::endl;
  }
  std::cout << "[INFO] : Grammar Rules" << std::endl;
  showRules(Mygrammer.rules);

  generateFirst('S');
  Mygrammer.showFirst();
  generateFollows();
  Mygrammer.showFollow();

  std::cout << "[INFO] : Adding '.' to Grammar Rules .." << std::endl;
  putDot(Mygrammer.rules);
  std::cout << "[INFO] : Grammar Rules" << std::endl;
  showRules(Mygrammer.rules);

  struct state i = generateFirstState();
  processState(i,'\0',i);
  while(statesNotProcessed.size()!=0) {
    generateState();
  }

  showAutomate();

  generateAction();

  showAction();

  showGoto();



}
