#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "dbqueries.h"
#include "input.h"
#include "methods.h"

int c_test(void) {
  Class_p *classes = find_classes(false, false);
  if (classes == NULL) return 1;

  if (*classes == NULL) {
    printf("\nNo existing class. Create a class before creating a test\n");
    clear_classes(classes);
    return 0;
  }

  printf("\nClasses (id):\n");
  for (Class_p *p = classes; *p != NULL; p++) {
    printf("%s (%d)\n", (*p)->name, (*p)->id);
  }
  printf("Enter the test's class id: ");
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

  Test_p test_p = malloc(sizeof(struct test_type));
  if (test_p == NULL) {
    clear_classes(classes);
    return 1;
  }

  test_p->class_id = class_id;
  do {
    printf("Enter test's name: ");
  } while (!read_string(test_p->name, ML_TEST_NAME+1, stdin));
  printf("Enter test's description (optional): ");
  read_string(test_p->description, ML_TEST_DESCRIPTION+1, stdin);
  do {
    printf("Enter test's max score: ");
    test_p->max_score = read_number(&success, stdin);
  } while (!success);
  for (;;) {
    printf("Enter test's date (format: yyyy-mm-dd): ");
    read_string(test_p->date, L_TEST_DATE+1, stdin);
    unsigned int year, month, day;
    if (sscanf(test_p->date, "%4u-%2u-%2u", &year, &month, &day) == 3) break;
  }

  int rc = create_test(test_p);
  clear_classes(classes);
  clear_test(test_p);
  if (rc != SQLITE_OK) return 1;
  printf("\nTest created\n");
  return 0;
}

int u_test(void) {
  Class_p *classes = find_classes(false, true);
  if (classes == NULL) return 1;
  if (*classes == NULL) {
    printf("\nNo existing test\n");
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

  if (class_p->tests == NULL) {
    clear_classes(classes);
    return 1;
  }
  if (*class_p->tests == NULL) {
    printf("\nNo existing test\n");
    clear_classes(classes);
    return 0;
  }

  printf("\nTests (id):\n");
  for (Test_p *p = class_p->tests; *p != NULL; p++) {
    printf("%s (%d)\n", (*p)->name, (*p)->id);
  }

  printf("\nEnter test's id: ");
  Id id = read_number(&success, stdin);
  if (!success) {
    clear_classes(classes);
    return 0;
  }

  Test_p test_p = search_test(class_p->tests, id);
  if (test_p == NULL) {
    clear_classes(classes);
    return 0;
  }

  char new_name[ML_TEST_NAME+1], new_description[ML_TEST_DESCRIPTION+1];
  printf("Enter test's name: (%s) ", test_p->name);
  if (read_string(new_name, ML_TEST_NAME+1, stdin)) {
    strcpy(test_p->name, new_name);
  }

  printf("Enter test's description: ");
  if (test_p->description[0] != '\0') printf("(%s) ", test_p->description);
  if (read_string(new_description, ML_TEST_DESCRIPTION+1, stdin)) {
    strcpy(test_p->description, new_description);
  } else if (test_p->description[0] != '\0') {
    printf("Clear previous description (y/n): ");
    if (read_answer(stdin)) {
      strcpy(test_p->description, "");
    }
  }

  int rc = update_test(test_p);
  clear_classes(classes);
  if (rc != SQLITE_OK) return 1;
  printf("\nTest updated\n");
  return 0;
}

int d_test(void) {
  Class_p *classes = find_classes(false, true);
  if (classes == NULL) return 1;
  if (*classes == NULL) {
    printf("\nNo existing test\n");
    clear_classes(classes);
    return 0;
  }

  printf("\nClasses (id):\n");
  for (Class_p *p = classes; *p != NULL; p++) {
    printf("%s (%d)\n", (*p)->name, (*p)->id);
  }

  bool success = false;
  printf("\nEnter test's class id: ");
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

  if (class_p->tests == NULL) {
    clear_classes(classes);
    return 1;
  }
  if (*class_p->tests == NULL) {
    printf("\nNo existing test for \"%s\" class\n", class_p->name);
    clear_classes(classes);
    return 0;
  }

  printf("\nTests (id):\n");
  for (Test_p *p = class_p->tests; *p != NULL; p++) {
    printf("%s (%d)\n", (*p)->name, (*p)->id);
  }

  printf("\nPlease note that deleting a test will delete "
         "all associated grades. This action can't be undone.\n");
  printf("Enter test's id: ");
  Id id = read_number(&success, stdin);
  if (!success) {
    clear_classes(classes);
    return 0;
  }

  Test_p test_p = search_test(class_p->tests, id);
  if (test_p == NULL) {
    clear_classes(classes);
    return 0;
  }

  int rc = delete_test(test_p);
  clear_classes(classes);
  if (rc != SQLITE_OK) return 1;
  printf("\nTest deleted\n");
  return 0;
}

Test_p search_test(Test_p *tests, Id id) {
  for (Test_p *p = tests; *p != NULL; p++) {
    if ((*p)->id == id) return *p;
  }
  return NULL;
}

void clear_tests(Test_p *tests_arr) {
  for (Test_p *p = tests_arr; *p != NULL; p++) {
    clear_test(*p);
  }
  free(tests_arr);
}

void clear_test(Test_p test_p) {
  if (test_p->grades != NULL) clear_grades(test_p->grades);
  free(test_p);
}
