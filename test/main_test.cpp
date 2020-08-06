#include "gtest/gtest.h"
#include "../main.cpp"

TEST(ReadFile,Axiom)  {
  Mygrammer.getAxiom();
  EXPECT_EQ('E',Mygrammer.axiom);
};

TEST(ReadFile,Terminal)  {
  EXPECT_EQ(0,Mygrammer.readTerminal());
};

int main(int argc,char**argv) {
    Mygrammer.file.open("grammar");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
