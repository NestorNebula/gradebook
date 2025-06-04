#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#define ML_CLASS_NAME 20
#define L_CLASS_YEAR 5
#define ML_STUDENT_NAME 50
#define ML_TEST_NAME 30
#define ML_TEST_DESCRIPTION 100
#define L_TEST_DATE 10

typedef unsigned int Id;

typedef struct grade_type {
  int *score;
  Id student_id;
  Id test_id;
} *Grade_p;

typedef struct test_type {
  Id id;
  char name[ML_TEST_NAME+1];
  char description[ML_TEST_DESCRIPTION+1];
  int max_score;
  char date[L_TEST_DATE+1];
  Id class_id;
  Grade_p *grades;
} *Test_p;

typedef struct student_type {
  Id id;
  char first_name[ML_STUDENT_NAME+1];
  char last_name[ML_STUDENT_NAME+1];
  Id class_id;
  Grade_p *grades;
} *Student_p;

typedef struct class_type {
  Id id;
  char name[ML_CLASS_NAME+1];
  char year[L_CLASS_YEAR+1];
  Student_p *students;
  Test_p *tests;
} *Class_p;

struct methods {
  int (*create)(void);
  int (*print)(void);
  int (*print_all)(void);
  int (*update)(void);
  int (*delete)(void);
};

#endif
