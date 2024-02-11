#include "tools/utils/stream_formatter.hpp"

#include <gtest/gtest.h>

// Tests that a Message object doesn't take up too much stack space.
TEST(Stream_Formatter, DoesNotTakeUpMuchStackSpace) {
  EXPECT_LE(sizeof(xuzy::Stream_Formatter), 500U);
}

// Tests the default constructor.
TEST(Stream_Formatter, default_constructor) {
  // steam output to std::cout
  xuzy::Stream_Formatter msg;
  EXPECT_EQ("", msg.get_string());
}

// Tests Stream_Formatter::get_string()
TEST(Stream_Formatter, get_string) {
  // steam output to std::cout
  xuzy::Stream_Formatter msg;
  msg << 1 << " lamb";
  EXPECT_EQ("1 lamb", msg.get_string());
}

// Tests streaming a Stream_Formatter object to an ostream.
TEST(Stream_Formatter, streams_to_ostream) {
  // steam output to std::stringstream
  std::stringstream out;
  {
    xuzy::Stream_Formatter log(out);
    log << "Oh, no mistake!";
  }

  // Expect equality.
  EXPECT_EQ("Oh, no mistake!", out.str());
}

// Tests streaming a C string
TEST(Stream_Formatter, streams_c_string) {
  xuzy::Stream_Formatter msg;
  EXPECT_EQ("Oh, no mistake!", (msg << "Oh, no mistake!").get_string());
}

// Tests streaming a NULL C string.
TEST(Stream_Formatter, streams_null_c_string) {
  char* p = nullptr;
  xuzy::Stream_Formatter msg;
  EXPECT_EQ("(null)", (msg << p).get_string());
}

// Tests streaming std::string.
TEST(Stream_Formatter, streams_std_string) {
  const std::string str("Hello");
  EXPECT_EQ("Hello", (xuzy::Stream_Formatter() << str).get_string());
}

// Tests that we can output strings containing embedded NULs.
TEST(Stream_Formatter, streams_string_with_embedded_NUL) {
  const char char_array_with_nul[] = "Here's a NUL\0 and some more string";
  const std::string string_with_nul(char_array_with_nul,
                                    sizeof(char_array_with_nul) - 1);
  // steam output to std::cout
  xuzy::Stream_Formatter msg;
  EXPECT_EQ("Here's a NUL\\0 and some more string",
            (msg << string_with_nul).get_string());

  // steam output to std::stringstream
  std::stringstream out;
  {
    xuzy::Stream_Formatter log(out);
    log << string_with_nul;
  }
  EXPECT_EQ("Here's a NUL\\0 and some more string", out.str());
}

// Tests streaming a NUL char.
TEST(Stream_Formatter, streams_NUL_char) {
  EXPECT_EQ("\\0", (xuzy::Stream_Formatter() << '\0').get_string());
}

// Tests streaming int.
TEST(Stream_Formatter, streams_int) {
  EXPECT_EQ("123", (xuzy::Stream_Formatter() << 123).get_string());
}

// Tests streaming a non-char pointer.
TEST(Stream_Formatter, streams_pointer) {
  int n = 0;
  int* p = &n;
  // steam output to std::cout
  xuzy::Stream_Formatter msg;
  EXPECT_NE("(null)", (msg << p).get_string());

  // steam output to std::stringstream
  std::stringstream out;
  {
    xuzy::Stream_Formatter log(out);

    int m = 0;
    int* ptr = &m;
    log << ptr;
  }
  EXPECT_NE("(null)", out.str());
}

// Tests streaming a NULL non-char pointer.
TEST(Stream_Formatter, streams_null_pointer) {
  int* p = nullptr;
  // steam output to std::cout
  xuzy::Stream_Formatter msg;
  EXPECT_EQ("(null)", (msg << p).get_string());

  // steam output to std::stringstream
  std::stringstream out;
  {
    xuzy::Stream_Formatter log(out);

    int* ptr = nullptr;
    log << ptr;
  }
  EXPECT_EQ("(null)", out.str());
}

// Tests that basic IO manipulators (endl, ends, and flush) can be
// streamed to Stream_Formatter.
TEST(Stream_Formatter, streams_basic_io_manip) {
  EXPECT_EQ(
      "Line 1.\nA NUL char \n in line 2.",
      (xuzy::Stream_Formatter() << "Line 1." << std::endl
                                << "A NUL char " << std::ends << " in line 2.")
          .get_string());

  EXPECT_EQ("Line 1.\nA NUL char \n\n in line 2.",
            (xuzy::Stream_Formatter()
             << "Line 1." << std::endl
             << "A NUL char " << std::ends << std::flush << " in line 2.")
                .get_string());
}

TEST(Stream_Formatter, streams_flag_allwaysFlush) {
  // steam output to std::stringstream
  std::stringstream out;

  xuzy::Stream_Formatter log(out);
  EXPECT_FALSE(log.is_alwaysflush());

  log << xuzy::L_allwaysFlush;
  EXPECT_TRUE(log.is_alwaysflush());

  log << xuzy::L_clearFlags;
  EXPECT_FALSE(log.is_alwaysflush());
}

TEST(Stream_Formatter, streams_flag_tabs_concat) {
  // steam output to std::stringstream
  std::stringstream out;

  {
    xuzy::Stream_Formatter log(out);
    EXPECT_FALSE(log.is_tabs());

    log << 1 << 2 << xuzy::L_tabs << 3 << 4;
    EXPECT_TRUE(log.is_tabs());

    log << xuzy::L_concat;
    EXPECT_FALSE(log.is_tabs());

    log << 5 << 6;
  }

  // Expect equality.
  EXPECT_EQ("12\t3\t456", out.str());
}

TEST(Stream_Formatter, streams_flag_tabs_clearFlags) {
  // steam output to std::stringstream
  std::stringstream out;

  {
    xuzy::Stream_Formatter log(out);
    EXPECT_FALSE(log.is_tabs());

    log << 1 << 2 << xuzy::L_tabs << 3 << 4;
    EXPECT_TRUE(log.is_tabs());

    log << xuzy::L_clearFlags;
    EXPECT_FALSE(log.is_tabs());

    log << 5 << 6;
  }

  // Expect equality.
  EXPECT_EQ("12\t3\t4\n56", out.str());
}

TEST(Stream_Formatter, streams_flag_std_setw) {
  // steam output to std::stringstream
  std::stringstream out;

  {
    xuzy::Stream_Formatter log(out);
    log << 1 << 2 << std::setw(5) << 3 << 4;
  }

  // Expect equality.
  EXPECT_EQ("12    34", out.str());
}

TEST(Stream_Formatter, streams_lag_std_hex) {
  // steam output to std::stringstream
  std::stringstream out;

  {
    xuzy::Stream_Formatter log(out);
    log << std::hex << 42;
  }

  // Expect equality.
  EXPECT_EQ("2a", out.str());
}

TEST(Stream_Formatter, streams_flag_std_oct) {
  // steam output to std::stringstream
  std::stringstream out;

  {
    xuzy::Stream_Formatter log(out);
    log << std::oct << 42;
  }

  // Expect equality.
  EXPECT_EQ("52", out.str());
}

TEST(Stream_Formatter, streams_glag_flush) {
  EXPECT_EQ(" in line 2.",
            (xuzy::Stream_Formatter()
             << "Line 1." << std::endl
             << "A NUL char " << std::ends << xuzy::L_flush << " in line 2.")
                .get_string());
}
