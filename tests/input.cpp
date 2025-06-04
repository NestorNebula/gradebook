#include <gtest/gtest.h>

extern "C" {
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "input.h"
}

#define NUM 3
#define STR "Test string"

TEST(read_number, returns_number_read_and_stores_true_in_success) {
  FILE *stream = tmpfile();
  fprintf(stream, "%d", NUM);
  rewind(stream);
  bool success = false;

  EXPECT_EQ(read_number(&success, stream), NUM);
  EXPECT_TRUE(success);
  fclose(stream);
}

TEST(read_number, returns_zero_and_stores_false_in_success) {
  FILE *stream = tmpfile();
  fprintf(stream, "%s%d", STR, NUM);
  rewind(stream);
  bool success = true;

  EXPECT_EQ(read_number(&success, stream), 0);
  EXPECT_FALSE(success);
  fclose(stream);
}

TEST(read_number, clears_following_content_until_new_line) {
  FILE *stream = tmpfile();
  fprintf(stream, "%d%s\n%d", NUM, STR, NUM + 1);
  rewind(stream);
  bool success = false;

  EXPECT_EQ(read_number(&success, stream), NUM);
  EXPECT_EQ(read_number(&success, stream), NUM + 1);
  fclose(stream);
}

TEST(read_string, returns_characters_read_and_stores_string_in_output) {
  FILE *stream = tmpfile();
  fprintf(stream, "%s", STR);
  rewind(stream);
  char output[strlen(STR) * 2];

  EXPECT_EQ(read_string(output, sizeof(output), stream), strlen(STR));
  EXPECT_EQ(strcmp(STR, output), 0);
  fclose(stream);
}

TEST(read_string, does_not_read_more_than_max_len_minus_one_character) {
  FILE *stream = tmpfile();
  fprintf(stream, "%s", STR);
  rewind(stream);
  char output[strlen(STR) + 1];

  EXPECT_EQ(read_string(output, 2, stream), 1);
  EXPECT_EQ(STR[0], output[0]);
  fclose(stream);
}


TEST(read_string, clears_following_content_until_new_line) {
  FILE *stream = tmpfile();
  fprintf(stream, "%s\n%s", STR, STR);
  rewind(stream);
  char output[strlen(STR) + 1];

  read_string(output, 1, stream);
  read_string(output, sizeof(output), stream);
  EXPECT_EQ(strcmp(STR, output), 0);
  fclose(stream);
}
