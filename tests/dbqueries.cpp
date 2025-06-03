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

FAKE_VALUE_FUNC(int, sqlite3_step, sqlite3_stmt *);

TEST(create_class, returns_sqlite3_exec_return_value) {
  EXPECT_EQ(create_class(class_t), SQLITE_OK);
}

TEST(find_classes, returns_every_classes_without_students_and_tests) {
  SET_RETURN_SEQ(sqlite3_step, rows, ROWS_NUMBER+1);
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
  SET_RETURN_SEQ(sqlite3_step, rows, ROWS_NUMBER+1);
  Class_p *classes = find_classes(true, true);
  ASSERT_THAT(classes[0], NotNull());
  EXPECT_THAT(classes[0]->students, NotNull());
  EXPECT_THAT(classes[0]->tests, NotNull());
  RESET_FAKE(sqlite3_step);
}

TEST(update_class, returns_sqlite3_exec_return_value) {
  EXPECT_EQ(update_class(class_t), SQLITE_OK);
}

TEST(delete_class, returns_sqlite3_exec_return_value) {
  EXPECT_EQ(delete_class(class_t), SQLITE_OK);
}

TEST(create_student, returns_sqlite3_exec_return_value) {
  EXPECT_EQ(create_student(student_t), SQLITE_OK);
}

TEST(find_students, returns_every_student_without_grades) {
  SET_RETURN_SEQ(sqlite3_step, rows, ROWS_NUMBER+1);
  Student_p *students = find_students(NULL, false);
  int i = 0;
  while (students[i] != NULL) i++;
  EXPECT_EQ(i, ROWS_NUMBER);
  ASSERT_NE(i, 0);
  EXPECT_THAT(students[0]->grades, IsNull());
  RESET_FAKE(sqlite3_step);
}

TEST(find_students, returns_students_with_grades) {
  SET_RETURN_SEQ(sqlite3_step, rows, ROWS_NUMBER+1);
  Student_p *students = find_students(NULL, true);
  ASSERT_THAT(students[0], NotNull());
  EXPECT_THAT(students[0]->grades, NotNull());
  RESET_FAKE(sqlite3_step);
}

TEST(update_student, returns_sqlite3_exec_return_value) {
  EXPECT_EQ(update_student(student_t), SQLITE_OK);
}

TEST(delete_student, returns_sqlite3_exec_return_value) {
  EXPECT_EQ(delete_student(student_t), SQLITE_OK);
}

TEST(create_test, returns_sqlite3_exec_return_value) {
  EXPECT_EQ(create_test(test_t), SQLITE_OK);
}

TEST(find_tests, returns_every_test_without_grades) {
  SET_RETURN_SEQ(sqlite3_step, rows, ROWS_NUMBER+1);
  Test_p *test_p = find_tests(NULL, false);
  int i = 0;
  while (test_p[i] != NULL) i++;
  EXPECT_EQ(i, ROWS_NUMBER);
  ASSERT_NE(i, 0);
  EXPECT_THAT(test_p[0]->grades, IsNull());
  RESET_FAKE(sqlite3_step);
}

TEST(find_tests, returns_tests_with_grades) {
  SET_RETURN_SEQ(sqlite3_step, rows, ROWS_NUMBER+1);
  Test_p *test_p = find_tests(NULL, true);
  ASSERT_THAT(test_p[0], NotNull());
  EXPECT_THAT(test_p[0]->grades, NotNull());
  RESET_FAKE(sqlite3_step);
}

TEST(update_test, returns_sqlite3_exec_return_value) {
  EXPECT_EQ(update_test(test_t), SQLITE_OK);
}

TEST(delete_test, returns_sqlite3_exec_return_value) {
  EXPECT_EQ(delete_test(test_t), SQLITE_OK);
}

TEST(create_grade, returns_sqlite3_exec_return_value) {
  EXPECT_EQ(create_grade(grade_t), SQLITE_OK);
}

TEST(find_grades, returns_every_grade) {
  SET_RETURN_SEQ(sqlite3_step, rows, ROWS_NUMBER+1);
  Grade_p *grades = find_grades(NULL, NULL);
  int i = 0;
  while (grades[i] != NULL) i++;
  EXPECT_EQ(i, ROWS_NUMBER);
  RESET_FAKE(sqlite3_step);
}

TEST(update_grade, returns_sqlite3_exec_return_value) {
  EXPECT_EQ(update_grade(grade_t), SQLITE_OK);
}

TEST(delete_grade, returns_sqlite3_exec_return_value) {
  EXPECT_EQ(delete_grade(grade_t), SQLITE_OK);
}
