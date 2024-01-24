#include "stream_formatter.hpp"

#include <gtest/gtest.h>

TEST(Stream_Formatter, basic) {

  {
    xuzy::Stream_Formatter log;
    log << xuzy::L_time << xuzy::L_endl << "Oh, no mistake!" << std::endl;
  }

  SUCCEED();
}

TEST(Stream_Formatter, output_to_ostream) {
  std::stringstream out;

  {
    xuzy::Stream_Formatter log(out);
    log << "Oh, no mistake!";
  }

  // Expect equality.
  EXPECT_EQ(out.str(), "Oh, no mistake!");
}

TEST(Stream_Formatter, flag_std_hex) {
  std::stringstream out;

  {
    xuzy::Stream_Formatter log(out);
    log << std::hex << 42;
  }

  // Expect equality.
  EXPECT_EQ(out.str(), "2a");
}

TEST(Stream_Formatter, flag_std_oct) {
  std::stringstream out;

  {
    xuzy::Stream_Formatter log(out);
    log << std::oct << 42;
  }

  // Expect equality.
  EXPECT_EQ(out.str(), "52");
}