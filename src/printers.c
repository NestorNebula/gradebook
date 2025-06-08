#include <stdio.h>
#include <string.h>
#include "printers.h"

int name_column_length = ML_STUDENT_NAME + 1;

void print_table_header(const char *title, Test_p *tests) {
  printf("%*s|", name_column_length, title);
  for (Test_p *p = tests; *p != NULL; p++) {
    printf("%*s|", L_TEST_DATE, (*p)->date);
  }
  printf("\n");
}

void print_student_name(Student_p student) {
  int max_name_len = ML_STUDENT_NAME / 2;
  printf("\n%.*s %.*s", 
         max_name_len, student->last_name,
         max_name_len, student->first_name);
  int first_len = strlen(student->first_name),
    last_len = strlen(student->last_name);
  int remaining_len = name_column_length
    - (first_len > max_name_len ? max_name_len : first_len)
    - (last_len > max_name_len ? max_name_len : last_len) 
    - 1;
  while (remaining_len-- > 0) printf(" ");
  printf("|");
}

void print_tests_grades(Test_p* tests, Grade_p *grades) {
  int test_count = 0, grade_index = 0;
  for (Test_p *p = tests; *p != NULL; p++, test_count++);
  for (int i = 0; i < test_count; i++) {
    if (grades[grade_index] == NULL) {
      printf("%*s|", L_TEST_DATE, "/");
    } else if 
      (tests[i]->id != grades[grade_index]->test_id) {
      printf("%*s|", L_TEST_DATE, "/");
    } else if (grades[grade_index]->score == NULL) {
      printf("%*s|", L_TEST_DATE, "/");
      grade_index++;
    } else {
      printf("%*g|", L_TEST_DATE, *grades[grade_index++]->score);
    }
  }
}

void print_table_footer(Test_p *tests) {
  int test_count = 0;
  for (Test_p *p = tests; *p != NULL; p++, test_count++);
  printf("%*s|", name_column_length, "Max Score");
  for (int i = 0; i < test_count; i++) {
    printf("%*d|", L_TEST_DATE, tests[i]->max_score);
  }
  printf("\n");
}
