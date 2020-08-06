#include "gtest/gtest.h"
#include "../main.cpp"

TEST(ReadFile,Axiom)  {
  Mygrammer.getAxiom();
  EXPECT_EQ('E',Mygrammer.axiom);
};
TEST(ReadFile,Terminal)  {
  EXPECT_EQ(0,Mygrammer.readTerminal());
};
TEST(ReadFile,noTerminal)  {
  EXPECT_EQ(0,Mygrammer.readNoTerminal());
};
TEST(ReadFile,Rules)  {
  EXPECT_EQ(0,Mygrammer.readRules());
};
TEST(GrammarFunction,isTerminal)  {
  EXPECT_EQ(true,Mygrammer.isTerminal('+'));
};
TEST(GrammarFunction,ruleNumber)  {
  EXPECT_EQ(1,Mygrammer.ruleNumber(std::string("E->E+T")));
};

int main(int argc,char**argv) {
    Mygrammer.file.open("grammar");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
