#ifndef METHODS_H
#define METHODS_H

#include "types.h"

/*
 * Classic methods
 * These methods perform different tasks when called, always using
 * user interaction via the terminal.
 * The c_ functions prompts create new structures with information
 * entered by the user that are then saved in the database.
 * The p_ functions either print all classes/students or a table of
 * the grades of a specific class/student.
 * The u_ functions update data stored in the database with informations
 * entered by the user.
 * The d_ functions delete data from the database.
 */
Method c_class, p_class, p_classes, u_class, d_class,
       c_student, p_student, p_students, u_student, d_student,
       c_test, u_test, d_test,
       c_grade, u_grade, d_grade;

/*
 * Search methods:
 * These methods take an array of pointers to structure and the id of the
 * searched structure.
 * Each one returns a pointer to the structure having the id if found,
 * else the methods return a null pointer.
 * For search_grade it returns a structure (if found) matching
 * both student_id and test_id.
 */
Class_p search_class(Class_p *classes, Id id);
Student_p search_student(Student_p *students, Id id);
Test_p search_test(Test_p *tests, Id id);
Grade_p search_grade(Grade_p *grades, Id student_id, Id test_id);

/*
 * Clear methods:
 * These methods release memory taken by their arguments
 * and by their members if needs be.
 */
void clear_classes(Class_p *classes);
void clear_class(Class_p class_p);
void clear_students(Student_p *students);
void clear_student(Student_p student_p);
void clear_tests(Test_p *tests_arr);
void clear_test(Test_p test_p);
void clear_grades(Grade_p *grades);
void clear_grade(Grade_p grade_p);

#endif
