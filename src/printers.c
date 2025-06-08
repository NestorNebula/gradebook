#include <stdio.h>
#include <string.h>
#include "printers.h"

#define L_TEST_COL 10

#define L_NAME_COL (ML_STUDENT_NAME / 2 + 1)

void print_table_header(const char *title, Test_p *tests) {
  printf("%*s |", L_NAME_COL - 1, title);
  for (Test_p *p = tests; *p != NULL; p++) {
    printf("%*s|", L_TEST_COL, (*p)->name);
  }
  printf("\n");
}

void print_student_name(Student_p student) {
  int max_name_len = (L_NAME_COL - 1) / 2;
  printf("%.*s %.*s", 
         max_name_len, student->last_name,
         max_name_len, student->first_name);
  int first_len = strlen(student->first_name),
    last_len = strlen(student->last_name);
  int remaining_len = L_NAME_COL
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
      printf("%*s|", L_TEST_COL, "/");
    } else if 
      (tests[i]->id != grades[grade_index]->test_id) {
      printf("%*s|", L_TEST_COL, "/");
    } else if (grades[grade_index]->score == NULL) {
      printf("%*s|", L_TEST_COL, "/");
      grade_index++;
    } else {
      printf("%*g|", L_TEST_COL, *grades[grade_index++]->score);
    }
  }
}

void print_table_footer(Test_p *tests) {
  int test_count = 0;
  for (Test_p *p = tests; *p != NULL; p++, test_count++);
  printf("%*s|", L_NAME_COL, "Max Score");
  for (int i = 0; i < test_count; i++) {
    printf("%*d|", L_TEST_COL, tests[i]->max_score);
  }
  printf("\n");
}
