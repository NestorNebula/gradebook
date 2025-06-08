#include <gtest/gtest.h>
#include <fff.h>
#include "types.h"

extern "C" {
#include <sqlite3.h>
}

struct class_type class_test = { 1, "Class test", "YY/YY", NULL, NULL };
Class_p class_t = &class_test;

struct student_type student_test = { 1, "John", "Doe", 1, NULL };
Student_p student_t = &student_test;

struct test_type test_test = { 1, "Test test", "", 100, "2024-10-01", 1, NULL };
Test_p test_t = &test_test;

float score = 0;
struct grade_type grade_test = { &score, 1, 1 };
Grade_p grade_t = &grade_test;

typedef int (*callback)(void *, int, char **, char **);
FAKE_VALUE_FUNC(int, sqlite3_exec, sqlite3 *, const char *, 
  callback, void *, char **);
FAKE_VALUE_FUNC(int, sqlite3_prepare_v2, sqlite3 *, const char *,
  int, sqlite3_stmt **, const char **);
FAKE_VALUE_FUNC(int, sqlite3_column_int, sqlite3_stmt *, int);
FAKE_VALUE_FUNC(double, sqlite3_column_double, sqlite3_stmt *, int);
FAKE_VALUE_FUNC(const unsigned char *, sqlite3_column_text, 
  sqlite3_stmt *, int);
FAKE_VALUE_FUNC(int, sqlite3_column_count, sqlite3_stmt *);
FAKE_VALUE_FUNC(const char *, sqlite3_column_name, sqlite3_stmt *, int);
FAKE_VALUE_FUNC(int, sqlite3_column_type, sqlite3_stmt *, int);
FAKE_VALUE_FUNC(int, sqlite3_finalize, sqlite3_stmt *);

int main(int argc, char *argv[]) {
sqlite3_exec_fake.return_val = SQLITE_OK;
  sqlite3_column_text_fake.return_val = (const unsigned char *) "Test";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
