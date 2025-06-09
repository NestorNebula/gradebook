#include <stdlib.h>
#include <stdio.h>
#include "db.h"
#include "input.h"
#include "methods.h"

struct option_type {
  const char *description;
  int (*function)(void);
};

struct methods 
  class_methods = { c_class, p_class, p_classes, u_class, d_class },
  student_methods = { c_student, p_student, p_students, u_student, d_student },
  test_methods = { .create = c_test, .update = u_test, .del = d_test },
  grade_methods = { .create = c_grade, .update = u_grade, .del = d_grade };

int main(void) {
  printf("\nWelcome to your gradebook!\n");

  // Associate each option with its function in a method structure
  struct option_type options[] = {
    { "Print all classes", class_methods.print_all },
    { "Print a class' detailed gradebook", class_methods.print },
    { "Print all students", student_methods.print_all },
    { "Print a student's detailed gradebook", student_methods.print },
    { "Create a class", class_methods.create },
    { "Create a student", student_methods.create },
    { "Create a test", test_methods.create },
    { "Create a grade", grade_methods.create },
    { "Update a class", class_methods.update },
    { "Update a student", student_methods.update },
    { "Update a test", test_methods.update },
    { "Update a grade", grade_methods.update },
    { "Delete a class", class_methods.del },
    { "Delete a student", student_methods.del },
    { "Delete a test", test_methods.del },
    { "Delete a grade", grade_methods.del }
  };

  int options_count = sizeof(options) / sizeof(struct option_type);

  open_db();

  for (;;) {
    printf("\nPlease choose an option among:\n\n");

    for (int i = 0; i < options_count; i++) {
      printf("%2d: %s\n", i + 1, options[i].description);
    }

    // Max choice is two digits long, giving the string a length of 3
    char choice_str[3];
    printf("\nEnter your choice: ");
    read_string(choice_str, 3, stdin);
    int choice = atoi(choice_str);

    // Stop loop if choice is empty or not corresponding to any option
    if (!choice) break;
    if (choice < 0 || choice > options_count) continue;
    choice--;
    
    // Call function associated to option
    int error = options[choice].function();
    if (error) {
      printf("\nAn error occurred during the execution of your request. "
             "Terminating program\n");
    }

    printf("\nContinue (y/n) ? ");
    if (!read_answer(stdin)) break;
  }

  close_db();
}
