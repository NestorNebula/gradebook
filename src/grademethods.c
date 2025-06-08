#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "dbqueries.h"
#include "input.h"
#include "methods.h"

/*
 * read_score:
 * Prompts the user for a score and casts it between 0 and max_score.
 * Returns a pointer to the number, that is null if no number was provided.
 */
static Score *read_score(Score max_score);

int c_grade(void) {
  Class_p *classes = find_classes(true, true);
  if (classes == NULL) return 1;
  if (*classes == NULL) {
    printf("\nNo existing class. "
           "Create at least a class, a student and a test "
           "before creating a grade.\n");
    clear_classes(classes);
    return 0;
  }

  printf("\nClasses (id):\n");
  for (Class_p *p = classes; *p != NULL; p++) {
    printf("%s (%d)\n", (*p)->name, (*p)->id);
  }

  printf("\nEnter the id of the class concerned by the grade: ");
  bool success = false;
  Id class_id = read_number(&success, stdin);
  if (!success) {
    clear_classes(classes);
    return 0;
  }

  Class_p class_p = search_class(classes, class_id);
  if (class_p == NULL) {
    clear_classes(classes);
    return 0;
  }

  if (class_p->students == NULL || class_p->tests == NULL) {
    clear_classes(classes);
    return 1;
  }
  if (class_p->students[0] == NULL || class_p->tests[0] == NULL) {
    printf("\nNo existing student and/or test. "
           "Create at least a student and a test before creating a grade.\n");
    clear_classes(classes);
    return 0;
  }

  printf("\nTests (id):\n");
  for (Test_p *p = class_p->tests; *p != NULL; p++) {
    printf("%s (%d)\n", (*p)->name, (*p)->id);
  }

  printf("\nEnter grade's test id: ");
  Id test_id = read_number(&success, stdin);
  if (!success) {
    clear_classes(classes);
    return 0;
  }

  Test_p test_p = search_test(class_p->tests, test_id);
  if (test_p == NULL) {
    clear_classes(classes);
    return 0;
  }

  for (;;) {
    printf("\nStudents (id):\n");
    for (Student_p *p = class_p->students; *p != NULL; p++) {
      printf("%s %s (%d)\n", (*p)->first_name, (*p)->last_name, (*p)->id);
    }

    printf("\nEnter grade's student id: ");
    Id student_id = read_number(&success, stdin);
    if (!success) {
      clear_classes(classes);
      return 0;
    }

    Student_p student_p = search_student(class_p->students, student_id);
    if (student_p == NULL) {
      clear_classes(classes);
      return 0;
    }

    Grade_p grade_p = malloc(sizeof(struct grade_type));
    if (grade_p == NULL) {
      clear_classes(classes);
      return 1;
    }
    grade_p->student_id = student_id;
    grade_p->test_id = test_id;
    printf("Enter score (optional): ");
    grade_p->score = read_score(test_p->max_score);

    int rc = create_grade(grade_p);
    clear_grade(grade_p);
    if (rc != SQLITE_OK) {
      clear_classes(classes);
      return 1;
    }
    printf("\nGrade created\n");

    printf("\nDo you want to enter another grade for this test (y/n) ? ");
    if (!read_answer(stdin)) break;
    printf("\n");
  }
  clear_classes(classes);
  return 0;
}

int u_grade(void) {
  Class_p *classes = find_classes(true, true);
  if (classes == NULL) return 1;
  if (*classes == NULL) {
    printf("\nNo existing grade\n");
    clear_classes(classes);
    return 0;
  }

  printf("\nClasses (id):\n");
  for (Class_p *p = classes; *p != NULL; p++) {
    printf("%s (%d)\n", (*p)->name, (*p)->id);
  }

  printf("\nEnter class' id: ");
  bool success = false;
  Id class_id = read_number(&success, stdin);
  if (!success) {
    clear_classes(classes);
    return 0;
  }

  Class_p class_p = search_class(classes, class_id);
  if (class_p == NULL) {
    clear_classes(classes);
    return 0;
  }

  if (class_p->students == NULL || class_p->tests == NULL) {
    clear_classes(classes);
    return 1;
  }

  if (*class_p->students == NULL || *class_p->tests == NULL) {
    printf("\nNo existing grade in \"%s\" class\n", class_p->name);
    return 0;
  }

  printf("\nStudents (id):\n");
  for (Student_p *p = class_p->students; *p != NULL; p++) {
    printf("%s %s (%d)\n", (*p)->last_name, (*p)->first_name, (*p)->id);
  }

  printf("\nEnter grade's student id: ");
  Id student_id = read_number(&success, stdin);
  if (!success) {
    clear_classes(classes);
    return 0;
  }

  Student_p student_p = search_student(class_p->students, student_id);
  if (student_p == NULL) {
    clear_classes(classes);
    return 0;
  }

  printf("\nTests (id):\n");
  for (Test_p *p = class_p->tests; *p != NULL; p++) {
    printf("%s (%d)\n", (*p)->name, (*p)->id);
  }

  printf("\nEnter grade's test id: ");
  Id test_id = read_number(&success, stdin);
  if (!success) {
    clear_classes(classes);
    return 0;
  }

  Test_p test_p = search_test(class_p->tests, test_id);
  if (test_p == NULL) {
    clear_classes(classes);
    return 0;
  }

  Grade_p *grades = find_grades(&student_id, &test_id);
  if (grades == NULL) {
    clear_classes(classes);
    return 1;
  }
  Grade_p grade_p = search_grade(grades, student_id, test_id);
  if (grade_p == NULL) {
    printf("\nNo existing grade for \"%s %s\" for test \"%s\"\n",
           student_p->last_name, student_p->first_name, test_p->name);
    clear_classes(classes);
    clear_grades(grades);
  }

  printf("Enter grade's score: ");
  if (grade_p->score != NULL) {
    printf("(%g) ", *grade_p->score);
  }
  Score *new_score = read_score(test_p->max_score);
  if (new_score == NULL && grade_p->score != NULL) {
    printf("Clear previous score (y/n) ? ");
    if (read_answer(stdin)) {
      grade_p->score = NULL;
    }
  } else grade_p->score = new_score;

  int rc = update_grade(grade_p);
  clear_classes(classes);
  clear_grades(grades);
  if (rc != SQLITE_OK) return 1;
  printf("\nGrade updated\n");
  return 0;
}

int d_grade(void) {
  Class_p *classes = find_classes(true, true);
  if (classes == NULL) return 1;
  if (*classes == NULL) {
    printf("\nNo existing grade\n");
    clear_classes(classes);
    return 0;
  }

  printf("\nClasses (id):\n");
  for (Class_p *p = classes; *p != NULL; p++) {
    printf("%s (%d)\n", (*p)->name, (*p)->id);
  }

  bool success = false;
  printf("\nEnter grade's class id: ");
  Id class_id = read_number(&success, stdin);
  if (!success) {
    clear_classes(classes);
    return 0;
  }

  Class_p class_p = search_class(classes, class_id);
  if (class_p == NULL) {
    clear_classes(classes);
    return 0;
  }

  if (class_p->students == NULL || class_p->tests == NULL) {
    clear_classes(classes);
    return 1;
  }
  if (*class_p->students == NULL || *class_p->tests == NULL) {
    printf("\nNo existing grade for \"%s\" class\n", class_p->name);
    clear_classes(classes);
    return 0;
  }

  printf("\nStudents (id):\n");
  for (Student_p *p = class_p->students; *p != NULL; p++) {
    printf("%s %s (%d)\n", (*p)->last_name, (*p)->first_name, (*p)->id);
  }

  printf("\nEnter grade's student id: ");
  Id student_id = read_number(&success, stdin);
  if (!success) {
    clear_classes(classes);
    return 0;
  }

  Student_p student_p = search_student(class_p->students, student_id);
  if (student_p == NULL) {
    clear_classes(classes);
    return 0;
  }

  printf("\nTests (id):\n");
  for (Test_p *p = class_p->tests; *p != NULL; p++) {
    printf("%s (%d)\n", (*p)->name, (*p)->id);
  }

  printf("\nPlease note that deleting a grade can't be undone.\n");
  printf("Enter grade's test id: ");
  Id test_id = read_number(&success, stdin);
  if (!success) {
    clear_classes(classes);
    return 0;
  }

  Test_p test_p = search_test(class_p->tests, test_id);
  if (test_p == NULL) {
    clear_classes(classes);
    return 0;
  }

  Grade_p *grades = find_grades(&student_id, &test_id);
  if (grades == NULL) {
    clear_classes(classes);
    return 1;
  }
  Grade_p grade_p = search_grade(grades, student_id, test_id);
  if (*grades == NULL || grade_p == NULL) {
    printf("\nNo existing grade for %s %s on test \"%s\"\n",
           student_p->last_name, student_p->first_name, test_p->name);
    clear_classes(classes);
    clear_grades(grades);
    return 0;
  }

  int rc = delete_grade(grade_p);
  clear_classes(classes);
  clear_grades(grades);
  if (rc != SQLITE_OK) return 1;
  printf("\nGrade deleted\n");
  return 0;
}

static Score *read_score(Score max_score) {
  char score_str[L_TEST_DATE+1];
  read_string(score_str, L_TEST_DATE+1, stdin);
  float score;
  if (sscanf(score_str, "%f", &score) != 1) return NULL;
  Score *score_ptr = malloc(sizeof(Score));
  if (score_ptr == NULL) return score_ptr;
  *score_ptr = score < 0 ? 0 : score > max_score ? max_score : score;
  return score_ptr;
}

Grade_p search_grade(Grade_p *grades, Id student_id, Id test_id) {
  for (Grade_p *p = grades; *p != NULL; p++) {
    if ((*p)->student_id == student_id && (*p)->test_id == test_id) return *p;
  }
  return NULL;
}

void clear_grades(Grade_p *grades) {
  for (Grade_p *p = grades; *p != NULL; p++) {
    clear_grade(*p);
  }
  free(grades);
}

void clear_grade(Grade_p grade_p) {
  if (grade_p->score != NULL) free(grade_p->score);
  free(grade_p);
}
