#include <gtest/gtest.h>
#include "types.h"

struct class_type class_test = { 1, "Class test", "YY/YY", NULL, NULL };
Class_p class_t = &class_test;

struct student_type student_test = { 1, "John", "Doe", 1, NULL };
Student_p student_t = &student_test;

struct test_type test_test = { 1, "Test test", "", 100, "2024-10-01", 1, NULL };
Test_p test_t = &test_test;

struct grade_type grade_test = { 0, 1, 1 };
Grade_p grade_t = &grade_test;

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
