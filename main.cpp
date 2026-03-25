#include "emulator.hpp"

#include "gtest/gtest.h"

TEST(EmuTest, TestFactorial) {
  std::string factorial = R"(
    Mov R0 5
    Mov R1 1
    Jmpz 6

    Mul R1 R0
    Sub R0 1
    Jmp 2

    Mov R0 R1
  )";

  ASSERT_EQ(120, Emulator::emulate(factorial));
}

TEST(EmuTest, TestDivide) {
  std::string program = R"(
    Mov R0 5
    Mov R1 2
    Div R0 R1
  )";

  ASSERT_EQ(2, Emulator::emulate(program));
}

TEST(EmuTest, TestLoadStore) {
  std::string program = R"(
    Mov R1 5
    Store R1 2
    Load R0 2
  )";

  ASSERT_EQ(5, Emulator::emulate(program));
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
