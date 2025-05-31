#ifndef TYPES_H
#define TYPES_H

#define ML_CLASS_NAME 20
#define L_CLASS_YEAR 5
#define ML_STUDENT_NAME 50
#define ML_TEST_NAME 30
#define ML_TEST_DESCRIPTION 100
#define L_TEST_DATE 10

typedef struct class_type {
  int id;
  char name[ML_CLASS_NAME+1];
  char year[L_CLASS_YEAR+1];
} *Class;

typedef struct student_type {
  int id;
  char first_name[ML_STUDENT_NAME+1];
  char last_name[ML_STUDENT_NAME+1];
  int class_id;
} *Student;

typedef struct test_type {
  int id;
  char name[ML_TEST_NAME+1];
  char description[ML_TEST_DESCRIPTION+1];
  int max_score;
  char date[L_TEST_DATE+1];
} *Test;

typedef struct grade_type {
  int score;
  int student_id;
  int test_id;
} *Grade;

#endif
