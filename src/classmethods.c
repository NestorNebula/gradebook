#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "dbqueries.h"
#include "input.h"
#include "printers.h"
#include "methods.h"

int p_classes(void) {
  Class_p *classes = find_classes(false, false);
  if (classes == NULL) return 1;
  if (*classes == NULL) {
    printf("\nNo existing class\n");
  } else {
    printf("\nClasses:\n");
    for (Class_p *p = classes; *p != NULL; p++) {
      puts((*p)->name);
    }
  }
  clear_classes(classes);
  return 0;
}

int p_class(void) {
  Class_p *classes = find_classes(true, true);
  if (classes == NULL) return 1;
  if (*classes == NULL) {
    printf("\nNo existing class\n");
  } else {
    printf("\nClasses (id):\n");
    for (Class_p *p = classes; *p != NULL; p++) {
      printf("%s (%d)\n", (*p)->name, (*p)->id);
    }

    printf("\nEnter class' id: ");
    bool success = false;
    Id id = read_number(&success, stdin);
    if (!success) {
      clear_classes(classes);
      return 0;
    }
    Class_p class_p = search_class(classes, id);
    if (class_p == NULL) {
      clear_classes(classes);
      return 0;
    }
    if (class_p->students == NULL || class_p->tests == NULL) {
      clear_classes(classes);
      return 1;
    }

    printf("\n");
    print_table_header(class_p->name, class_p->tests);
    
    for (Student_p *p = class_p->students; *p != NULL; p++) {
      (*p)->grades = find_grades(&(*p)->id, NULL);
      if ((*p)->grades == NULL) {
        print_table_footer(class_p->tests);
        clear_classes(classes);
        return 1;
      }
      print_student_name(*p);
      print_tests_grades(class_p->tests, (*p)->grades);
      printf("\n");
    }
    print_table_footer(class_p->tests);
  }
  clear_classes(classes);
  return 0;
}

int c_class(void) {
  Class_p class_p = malloc(sizeof(struct class_type));
  if (class_p == NULL) return 1;
  do {
    printf("\nEnter the name of the class: ");
  } while (!read_string(class_p->name, ML_CLASS_NAME+1, stdin));
  for (;;) {
    printf("Enter the school year (format yy/yy): ");
    read_string(class_p->year, L_CLASS_YEAR+1, stdin);
    unsigned int year1, year2;
    if (sscanf(class_p->year, "%2u/%2u", &year1, &year2) == 2
        && (year1 + 1 == year2 || (year1 == 99 && year2 == 0))) break;
  }
  int rc = create_class(class_p);
  clear_class(class_p);
  if (rc != SQLITE_OK) return 1;
  printf("\nClass created\n");
  return 0;
}

int u_class(void) {
  Class_p *classes = find_classes(false, false);
  if (classes == NULL) return 1;
  if (*classes == NULL) {
    printf("\nNo existing class\n");
    clear_classes(classes);
    return 0;
  }

  printf("\nClasses (id):\n");
  for (Class_p *p = classes; *p != NULL; p++) {
    printf("%s (%d)\n", (*p)->name, (*p)->id);
  }

  printf("\nEnter class' id: ");
  bool success = false;
  Id id = read_number(&success, stdin);
  if (!success) {
    clear_classes(classes);
    return 0;
  }

  Class_p class_p = search_class(classes, id);
  if (class_p == NULL) {
    clear_classes(classes);
    return 0;
  }

  char new_name[ML_CLASS_NAME+1];
  printf("Enter class' name: (%s) ", class_p->name);
  if (read_string(new_name, ML_CLASS_NAME+1, stdin)) {
    strcpy(class_p->name, new_name);
  }

  int rc = update_class(class_p);
  clear_classes(classes);
  if (rc != SQLITE_OK) return 1;
  printf("\nClass updated\n");
  return 0;
}

int d_class(void) {
  Class_p *classes = find_classes(false, false);
  if (classes == NULL) return 1;
  if (*classes == NULL) {
    printf("\nNo existing class\n");
    clear_classes(classes);
    return 0;
  }

  printf("\nClasses (id):\n");
  for (Class_p *p = classes; *p != NULL; p++) {
    printf("%s (%d)\n", (*p)->name, (*p)->id);
  }

  printf("\nPlease note that deleting a class will delete all associated data "
         "(students/tests/grades). This action can't be undone.\n");
  printf("Enter class' id: ");
  bool success = false;
  Id id = read_number(&success, stdin);
  if (!success) {
    clear_classes(classes);
    return 0;
  }

  Class_p class_p = search_class(classes, id);
  if (class_p == NULL) {
    clear_classes(classes);
    return 0;
  }

  int rc = delete_class(class_p);
  clear_classes(classes);
  if (rc != SQLITE_OK) return 1;
  printf("\nClass deleted\n");
  return 0;
}

Class_p search_class(Class_p *classes, Id id) {
  for (Class_p *p = classes; *p != NULL; p++) {
    if ((*p)->id == id) return *p;
  }
  return NULL;
}

void clear_classes(Class_p *classes) {
  for (Class_p *p = classes; *p != NULL; p++) {
    clear_class(*p);
  }
  free(classes);
}

void clear_class(Class_p class_p) {
  if (class_p->students != NULL) clear_students(class_p->students);
  if (class_p->tests != NULL) clear_tests(class_p->tests);
  free(class_p);
}
