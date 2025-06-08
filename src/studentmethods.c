#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "dbqueries.h"
#include "input.h"
#include "printers.h"
#include "methods.h"

int p_students(void) {
  Student_p *students = find_students(NULL, false);
  if (students == NULL) return 1;
  if (*students == NULL) {
    printf("\nNo existing student\n");
  } else {
    printf("\nStudents:\n\n");
    for (Student_p *p = students; *p != NULL; p++) {
      printf("%s %s\n", (*p)->last_name, (*p)->first_name);
    }
  }
  clear_students(students);
  return 0;
}

int p_student(void) {
  Class_p *classes = find_classes(true, true);
  if (classes == NULL) return 1;
  if (*classes == NULL) {
    printf("\nNo existing student\n");
  } else {
    printf("\nClasses (id):\n");
    for (Class_p *p = classes; *p != NULL; p++) {
      printf("%s (%d)\n", (*p)->name, (*p)->id);
    }
    printf("\nEnter the student's class id: ");
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

    if (*class_p->students == NULL) {
      printf("\nNo existing student in \"%s\" class\n", class_p->name);
      clear_classes(classes);
      return 0;
    }

    printf("\n%s students (id):\n", class_p->name);
    for (Student_p *p = class_p->students; *p != NULL; p++) {
      printf("%s %s (%d)\n", (*p)->last_name, (*p)->first_name, (*p)->id);
    }

    printf("\nEnter the student's id: ");
    Id id = read_number(&success, stdin);
    if (!success) {
      clear_classes(classes);
      return 0;
    }

    Student_p student_p = search_student(class_p->students, id);
    if (student_p == NULL) {
      clear_classes(classes);
      return 0;
    }

    student_p->grades = find_grades(&id, NULL);
    if (student_p->grades == NULL) {
      clear_classes(classes);
      return 1;
    }

    printf("\n");
    print_table_header("Grades", class_p->tests);
    print_student_name(student_p);
    print_tests_grades(class_p->tests, student_p->grades);
    printf("\n");
    print_table_footer(class_p->tests);
  }
  clear_classes(classes);
  return 0;
}

int c_student(void) {
  Class_p *classes = find_classes(false, false);
  if (classes == NULL) return 1;

  if (*classes == NULL) {
    printf("\nNo existing class. Create a class before creating a student.\n");
    clear_classes(classes);
    return 0;
  }

  printf("\nClasses (id):\n");
  for (Class_p *p = classes; *p != NULL; p++) {
    printf("%s (%d)\n", (*p)->name, (*p)->id);
  }
  printf("\nEnter student's class id: ");
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
  
  Student_p student_p = malloc(sizeof(struct student_type));
  if (student_p == NULL) {
    clear_classes(classes);
    return 1;
  }
  student_p->class_id = class_id;
  do {
    printf("Enter student's first name: ");
  } while (!read_string(student_p->first_name, ML_STUDENT_NAME+1, stdin));
  do {
    printf("Enter student's last name: ");
  } while (!read_string(student_p->last_name, ML_STUDENT_NAME+1, stdin));

  int rc = create_student(student_p);
  clear_classes(classes);
  clear_student(student_p);
  if (rc != SQLITE_OK) return 1;
  printf("\nStudent created\n");
  return 0;
}

int u_student(void) {
  Class_p *classes = find_classes(true, false);
  if (classes == NULL) return 1;
  if (*classes == NULL) {
    printf("\nNo existing student\n");
    clear_classes(classes);
    return 0;
  }

  printf("\nClasses (id):\n");
  for (Class_p *p = classes; *p != NULL; p++) {
    printf("%s (%d)\n", (*p)->name, (*p)->id);
  }

  printf("\nEnter the student's class id: ");
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

  if (class_p->students == NULL) {
    clear_classes(classes);
    return 1;
  }
  if (*class_p->students == NULL) {
    printf("\nNo student in \"%s\" class\n", class_p->name);
    clear_classes(classes);
    return 0;
  }

  printf("\nStudents (id):\n");
  for (Student_p *p = class_p->students; *p != NULL; p++) {
    printf("%s %s (%d)\n", (*p)->last_name, (*p)->first_name, (*p)->id);
  }

  printf("\nEnter student's id: ");
  Id id = read_number(&success, stdin);
  if (!success) {
    clear_classes(classes);
    return 0;
  }

  Student_p student_p = search_student(class_p->students, id);
  if (student_p == NULL) {
    clear_classes(classes);
    return 0;
  }

  printf("Enter student's first name: (%s) ", student_p->first_name);
  char new_name[ML_STUDENT_NAME+1];
  if (read_string(new_name, ML_STUDENT_NAME+1, stdin)) {
    strcpy(student_p->first_name, new_name);
  }

  printf("Enter student's last name: (%s) ", student_p->last_name);
  if (read_string(new_name, ML_STUDENT_NAME+1, stdin)) {
    strcpy(student_p->last_name, new_name);
  }

  int rc = update_student(student_p);
  clear_classes(classes);
  if (rc != SQLITE_OK) return 1;
  printf("\nStudent updated\n");
  return 0;
}

int d_student(void) {
  Class_p *classes = find_classes(true, false);
  if (classes == NULL) return 1;
  if (*classes == NULL) {
    printf("\nNo existing student\n");
    clear_classes(classes);
    return 0;
  }

  printf("\nClasses (id):\n");
  for (Class_p *p = classes; *p != NULL; p++) {
    printf("%s (%d)\n", (*p)->name, (*p)->id);
  }

  printf("\nEnter student's class id: ");
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

  if (class_p->students == NULL) {
    clear_classes(classes);
    return 1;
  }
  if (*class_p->students == NULL) {
    printf("\nNo existing student in \"%s\" class\n", class_p->name);
    clear_classes(classes);
    return 0;
  }

  printf("\nStudents (id):\n");
  for (Student_p *p = class_p->students; *p != NULL; p++) {
    printf("%s %s (%d)\n", (*p)->last_name, (*p)->first_name, (*p)->id);
  }

  printf("\nPlease note that deleting a student will delete "
         "all its associated grades. This action can't be undone.\n");
  printf("Enter student's id: ");
  Id id = read_number(&success, stdin);
  if (!success) {
    clear_classes(classes);
    return 0;
  }

  Student_p student_p = search_student(class_p->students, id);
  if (student_p == NULL) {
    clear_classes(classes);
    return 0;
  }

  int rc = delete_student(student_p);
  clear_classes(classes);
  if (rc != SQLITE_OK) return 1;
  printf("\nStudent deleted\n");
  return 0;
}

Student_p search_student(Student_p *students, Id id) {
  for (Student_p *p = students; *p != NULL; p++) {
    if ((*p)->id == id) return *p;
  }
  return NULL;
}

void clear_students(Student_p *students) {
  for (Student_p *p = students; *p != NULL; p++) {
    clear_student(*p);
  }
  free(students);
}

void clear_student(Student_p student_p) {
  if (student_p->grades != NULL) clear_grades(student_p->grades);
  free(student_p);
}
