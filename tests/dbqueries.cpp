#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fff.h>

using testing::IsNull;
using testing::NotNull;

extern "C" {
#include <sqlite3.h>
#include "dbqueries.h"
}

#define ROWS_NUMBER 3
int rows[] = { SQLITE_ROW, SQLITE_ROW, SQLITE_ROW, SQLITE_DONE};

extern Class_p class_t;
extern Student_p student_t;
extern Test_p test_t;
extern Grade_p grade_t;

typedef int (*callback)(void *, int, char **, char **);

FAKE_VALUE_FUNC(int, sqlite3_exec, sqlite3 *, const char *, 
  callback, void *, char **);
FAKE_VALUE_FUNC(int, sqlite3_prepare_v2, sqlite3 *, const char *,
  int, sqlite3_stmt **, const char **);
FAKE_VALUE_FUNC(int, sqlite3_step, sqlite3_stmt *);
FAKE_VALUE_FUNC(int, sqlite3_column_int, sqlite3_stmt *, int);
FAKE_VALUE_FUNC(const unsigned char *, sqlite3_column_text, 
  sqlite3_stmt *, int);
FAKE_VALUE_FUNC(int, sqlite3_finalize, sqlite3_stmt *);


TEST(create_class, returns_sqlite3_exec_return_value) {
  sqlite3_exec_fake.return_val = SQLITE_OK;
  EXPECT_EQ(create_class(class_t), SQLITE_OK);
  RESET_FAKE(sqlite3_exec);
}

TEST(find_classes, returns_every_classes_without_students_and_tests) {
  SET_RETURN_SEQ(sqlite3_step, rows, ROWS_NUMBER);
  Class_p *classes = find_classes(false, false);
  int i = 0;
  while (classes[i] != NULL) i++;
  EXPECT_EQ(i, ROWS_NUMBER);
  ASSERT_NE(i, 0);
  EXPECT_THAT(classes[0]->students, IsNull());
  EXPECT_THAT(classes[0]->tests, IsNull());
  RESET_FAKE(sqlite3_step);
}

TEST(find_classes, returns_classes_with_students_and_tests) {
  SET_RETURN_SEQ(sqlite3_step, rows, ROWS_NUMBER);
  Class_p *classes = find_classes(true, true);
  ASSERT_THAT(classes[0], NotNull());
  EXPECT_THAT(classes[0]->students, NotNull());
  EXPECT_THAT(classes[0]->tests, NotNull());
  RESET_FAKE(sqlite3_step);
}

TEST(update_class, returns_sqlite3_exec_return_value) {
  sqlite3_exec_fake.return_val = SQLITE_OK;
  EXPECT_EQ(update_class(class_t), SQLITE_OK);
  RESET_FAKE(sqlite3_exec);
}

TEST(delete_class, returns_sqlite3_exec_return_value) {
  sqlite3_exec_fake.return_val = SQLITE_OK;
  EXPECT_EQ(delete_class(class_t), SQLITE_OK);
  RESET_FAKE(sqlite3_exec);
}

TEST(create_student, returns_sqlite3_exec_return_value) {
  sqlite3_exec_fake.return_val = SQLITE_OK;
  EXPECT_EQ(create_student(student_t), SQLITE_OK);
  RESET_FAKE(sqlite3_exec);
}

TEST(find_students, returns_every_student_without_grades) {
  SET_RETURN_SEQ(sqlite3_step, rows, ROWS_NUMBER);
  Student_p *students = find_students(NULL, false);
  int i = 0;
  while (students[i] != NULL) i++;
  EXPECT_EQ(i, ROWS_NUMBER);
  ASSERT_NE(i, 0);
  EXPECT_THAT(students[0]->grades, IsNull());
  RESET_FAKE(sqlite3_step);
}

TEST(find_students, returns_students_with_grades) {
  SET_RETURN_SEQ(sqlite3_step, rows, ROWS_NUMBER);
  Student_p *students = find_students(NULL, true);
  ASSERT_THAT(students[0], NotNull());
  EXPECT_THAT(students[0]->grades, NotNull());
  RESET_FAKE(sqlite3_step);
}

TEST(find_students, only_returns_students_with_matching_class_id) {
  SET_RETURN_SEQ(sqlite3_step, rows, ROWS_NUMBER);
  sqlite3_column_int_fake.return_val = class_t->id + 1;
  EXPECT_THAT(find_students(&class_t->id, false)[0], IsNull());
  RESET_FAKE(sqlite3_column_int);
  RESET_FAKE(sqlite3_step);
}

TEST(update_student, returns_sqlite3_exec_return_value) {
  sqlite3_exec_fake.return_val = SQLITE_OK;
  EXPECT_EQ(update_student(student_t), SQLITE_OK);
  RESET_FAKE(sqlite3_exec);
}

TEST(delete_student, returns_sqlite3_exec_return_value) {
  sqlite3_exec_fake.return_val = SQLITE_OK;
  EXPECT_EQ(delete_student(student_t), SQLITE_OK);
  RESET_FAKE(sqlite3_exec);
}

TEST(create_test, returns_sqlite3_exec_return_value) {
  sqlite3_exec_fake.return_val = SQLITE_OK;
  EXPECT_EQ(create_test(test_t), SQLITE_OK);
  RESET_FAKE(sqlite3_exec);
}

TEST(find_tests, returns_every_test_without_grades) {
  SET_RETURN_SEQ(sqlite3_step, rows, ROWS_NUMBER);
  Test_p *test_p = find_tests(NULL, false);
  int i = 0;
  while (test_p[i] != NULL) i++;
  EXPECT_EQ(i, ROWS_NUMBER);
  ASSERT_NE(i, 0);
  EXPECT_THAT(test_p[0]->grades, IsNull());
  RESET_FAKE(sqlite3_step);
}

TEST(find_tests, returns_tests_with_grades) {
  SET_RETURN_SEQ(sqlite3_step, rows, ROWS_NUMBER);
  Test_p *test_p = find_tests(NULL, true);
  ASSERT_THAT(test_p[0], NotNull());
  EXPECT_THAT(test_p[0]->grades, NotNull());
  RESET_FAKE(sqlite3_step);
}

TEST(find_tests, only_returns_tests_with_matching_class_id) {
  SET_RETURN_SEQ(sqlite3_step, rows, ROWS_NUMBER);
  sqlite3_column_int_fake.return_val = class_t->id + 1;
  EXPECT_THAT(find_tests(&class_t->id, false)[0], IsNull());
  RESET_FAKE(sqlite3_column_int);
  RESET_FAKE(sqlite3_step);
}

TEST(update_test, returns_sqlite3_exec_return_value) {
  sqlite3_exec_fake.return_val = SQLITE_OK;
  EXPECT_EQ(update_test(test_t), SQLITE_OK);
  RESET_FAKE(sqlite3_exec);
}

TEST(delete_test, returns_sqlite3_exec_return_value) {
  sqlite3_exec_fake.return_val = SQLITE_OK;
  EXPECT_EQ(delete_test(test_t), SQLITE_OK);
  RESET_FAKE(sqlite3_exec);
}

TEST(create_grade, returns_sqlite3_exec_return_value) {
  sqlite3_exec_fake.return_val = SQLITE_OK;
  EXPECT_EQ(create_grade(grade_t), SQLITE_OK);
  RESET_FAKE(sqlite3_exec);
}

TEST(find_grades, returns_every_grade) {
  SET_RETURN_SEQ(sqlite3_step, rows, ROWS_NUMBER);
  Grade_p *grades = find_grades(NULL, NULL);
  int i = 0;
  while (grades[i] != NULL) i++;
  EXPECT_EQ(i, ROWS_NUMBER);
  RESET_FAKE(sqlite3_step);
}

TEST(find_grades, only_returns_grades_with_matching_student_id_and_test_id) {
  SET_RETURN_SEQ(sqlite3_step, rows, ROWS_NUMBER);
  sqlite3_column_int_fake.return_val = student_t->id + test_t->id;
  EXPECT_THAT(find_grades(&student_t->id, &test_t->id)[0], IsNull());
  RESET_FAKE(sqlite3_column_int);
  RESET_FAKE(sqlite3_step);
}

TEST(update_grade, returns_sqlite3_exec_return_value) {
  sqlite3_exec_fake.return_val = SQLITE_OK;
  EXPECT_EQ(update_grade(grade_t), SQLITE_OK);
  RESET_FAKE(sqlite3_exec);
}

TEST(delete_grade, returns_sqlite3_exec_return_value) {
  sqlite3_exec_fake.return_val = SQLITE_OK;
  EXPECT_EQ(delete_grade(grade_t), SQLITE_OK);
  RESET_FAKE(sqlite3_exec);
}
