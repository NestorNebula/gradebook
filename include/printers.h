#ifndef PRINTERS_H
#define PRINTERS_H

#include "types.h"

/* 
 * print_table_header:
 * Prints table's header with a title of title and a column for every test.
 */
void print_table_header(const char *title, Test_p *tests);

/*
 * print_student_name:
 * Prints student's name in a table format.
 */
void print_student_name(Student_p student);

/*
 * print_tests_grades:
 * Print a grade in table format for each test in tests.
 * If a grade exist in grades for a test in tests, prints it,
 * else prints '/'.
 */
void print_tests_grades(Test_p* tests, Grade_p *grades);

/*
 * print_table_footer:
 * Prints table's footer with the maximum note of each test.
 */
void print_table_footer(Test_p *tests);

#endif
