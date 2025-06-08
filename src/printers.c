#include <stdio.h>
#include <string.h>
#include "printers.h"

#define L_TEST_COL 10
#define L_NAME_COL (ML_STUDENT_NAME / 2 + 1)
#define ML_NAME ((L_NAME_COL - 1) / 2)

/*
 * print_dividing_line: 
 * Prints a dividing line. Uses separator at the beginning and end of line
 * as well as between tests.
 */
static void print_dividing_line(Test_p *tests, char separator);

void print_table_header(const char *title, Test_p *tests) {
  print_dividing_line(tests, '-');
  printf("|%*s |", L_NAME_COL - 1, title);
  for (Test_p *p = tests; *p != NULL; p++) {
    printf("%*s|", L_TEST_COL, 
           strlen((*p)->name) > L_TEST_COL ? (*p)->date : (*p)->name);
  }
  printf("\n");
  print_dividing_line(tests, '|');
}

void print_student_name(Student_p student) {
  printf("|%.*s %.*s", 
         ML_NAME, student->last_name,
         ML_NAME, student->first_name);
  int first_len = strlen(student->first_name),
    last_len = strlen(student->last_name);
  int remaining_len = L_NAME_COL
    - (first_len > ML_NAME ? ML_NAME : first_len)
    - (last_len > ML_NAME ? ML_NAME : last_len) 
    - 1;
  while (remaining_len-- > 0) printf(" ");
  printf("|");
}

void print_tests_grades(Test_p* tests, Grade_p *grades) {
  int test_count = 0, grade_index = 0;
  for (Test_p *p = tests; *p != NULL; p++, test_count++);
  for (int i = 0; i < test_count; i++) {
    if (grades[grade_index] == NULL) {
      printf("%*s|", L_TEST_COL, "");
    } else if 
      (tests[i]->id != grades[grade_index]->test_id) {
      printf("%*s|", L_TEST_COL, "");
    } else if (grades[grade_index]->score == NULL) {
      printf("%*s|", L_TEST_COL, "");
      grade_index++;
    } else {
      printf("%*g|", L_TEST_COL, *grades[grade_index++]->score);
    }
  }
}

void print_table_footer(Test_p *tests) {
  int test_count = 0;
  for (Test_p *p = tests; *p != NULL; p++, test_count++);
  print_dividing_line(tests, '|');
  printf("|%*s|", L_NAME_COL, "Max Score");
  for (int i = 0; i < test_count; i++) {
    printf("%*d|", L_TEST_COL, tests[i]->max_score);
  }
  printf("\n");
  print_dividing_line(tests, '-');
  printf("\n");
}

static void print_dividing_line(Test_p *tests, char separator) {
  putchar(separator);
  for (int i = 0; i < L_NAME_COL; i++) printf("-");
  putchar(separator);
  for (Test_p *p = tests; *p != NULL; p++) {
    for (int i = 0; i < L_TEST_COL; i++) {
      printf("-");
    }
    putchar(separator);
  }
  printf("\n");
}
