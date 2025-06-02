#ifndef DBQUERIES_H
#define DBQUERIES_H

#include "types.h"

/*
 * DB Queries:
 * All functions in this module make changes to the database.
 * The database must have been opened by a call of open_db (dbsetup module)
 * before using any function in this module.
 * Every function declared to return an int returns SQLITE_OK if no error
 * occurred, otherwise, the function will print an error and
 * return an error code.
 * The pointer to a structure pointer returned by the other functions is a
 * pointer to an array of structures pointers. Each array will be terminated
 * by a null pointer.
 */


/*
 * create_class:
 * Inserts a new class in the database using the informations 
 * stored in class.
 */
int create_class(Class cla);

/*
 * find_classes:
 * Selects all the classes from the database.
 * If with_students is true, include students in class structure.
 * If with_tests is true, includes tests in class structure.
 */
Class *find_classes(bool with_students, bool with_tests);

/*
 * update_class:
 * Updates the class stored in the database that has the same id as class
 * using the informations stored in class.
 */
int update_class(Class cla);

/*
 * delete_class:
 * Deletes the class stored in the database that has the same id as class.
 */
int delete_class(Class cla);

/*
 * create_student:
 * Inserts a new student in the database using the informations 
 * stored in student.
 */
int create_student(Student student);

/*
 * find_students:
 * Selects all the students from the database.
 * If class_id isn't null, limits the search to the students
 * whose class_id matches class_id.
 * If with_grades is true, includes grades in student structure.
 */
Student *find_students(Id *class_id, bool with_grades);

/*
 * update_student:
 * Updates the student stored in the database that has the same id as student
 * using the information stored in student.
 */
int update_student(Student student);

/*
 * delete_student:
 * Deletes the student stored in the database that has the same id as student.
 */
int delete_student(Student student);

/*
 * create_test:
 * Inserts a new test in the database using the informations
 * stored in test.
 */
int create_test(Test test);

/*
 * find_tests:
 * Selects all the tests from the database.
 * If class_id isn't null, limits the search to the tests
 * whose class_id matches class_id.
 * If with_grades is true, includes grades in test structure.
 */
Test *find_tests(Id *class_id, bool with_grades);

/*
 * update_test:
 * Updates the test stored in the database that has the same id as test
 * using the information stored in test.
 */
int update_test(Test test);

/*
 * delete_test:
 * Deletes the test stored in the database that has the same id as test.
 */
int delete_test(Test test);

/*
 * create_grade:
 * Inserts a new grade in the database using the informations
 * stored in grade.
 */
int create_grade(Grade grade);

/*
 * find_grades:
 * Selects all the tests from the database.
 * If student_id isn't null, limits the search to the grades
 * whose student_id matches student_id.
 * If test_id isn't null, limits the search to the grades
 * whose test_id matches test_id.
 */
Grade *find_grades(Id *student_id, Id *test_id);

/*
 * update_grade:
 * Updates the grade stored in the database that has the same id as grade
 * using the information stored in grade.
 */
int update_grade(Grade grade);

/*
 * delete_grade:
 * Deletes the grade stored in the database that has the same id as grade.
 */
int delete_grade(Grade grade);

#endif
