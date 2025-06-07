#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbqueries.h"

#define ML_SQL 200
#define DEF_ARR_SIZE 1

extern sqlite3 *db;
char sql[ML_SQL+1];
char *errmsg = NULL;

/*
 * handle_errmsg:
 * If errmsg isn't null, prints errmsg to stderr preceded by description
 * and releases memory taken by the errmsg, else does nothing.
 */
static void handle_errmsg(const char *description);

/*
 * find_column:
 * Searches column named named in stmt.
 * Stmt must have been prepared using sqlite3_prepare_v2 before
 * using this function.
 * If found, returns the index of the column, else returns -1.
 */
static int find_column(const char *name, sqlite3_stmt *stmt);

int create_class(Class_p class_p) {
  sprintf(sql, "INSERT INTO classes (name, year) VALUES ('%s', '%s');",
          class_p->name, class_p->year);
  int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  handle_errmsg("Error when inserting class");
  return rc;
}

Class_p *find_classes(bool with_students, bool with_tests) {
  sprintf(sql, "SELECT * FROM classes;");
  sqlite3_stmt *stmt;
  sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

  Class_p *classes = malloc(sizeof(Class_p) * DEF_ARR_SIZE);
  if (classes == NULL) return classes;
  int length = 0, size = DEF_ARR_SIZE;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    if (length + 1 == size) {
      Class_p *resized = realloc(classes, 2 * size * sizeof(Class_p));
      if (resized == NULL) break;
      classes = resized;
      size *= 2;
    }
    Class_p class_row = malloc(sizeof(struct class_type));
    if (class_row == NULL) break;
    class_row->id = sqlite3_column_int(stmt, find_column("id", stmt));
    strcpy(class_row->name, 
      (const char *) sqlite3_column_text(stmt, find_column("name", stmt)));
    strcpy(class_row->year, 
      (const char *) sqlite3_column_text(stmt, find_column("year", stmt)));
    class_row->students = with_students 
      ? find_students(&class_row->id, false)
      : NULL;
    class_row->tests = with_tests
      ? find_tests(&class_row->id, false)
      : NULL;
    classes[length++] = class_row;
  }
  classes[length] = NULL;

  sqlite3_finalize(stmt);

  return classes;
}

int update_class(Class_p class_p) {
  sprintf(sql, "UPDATE classes SET name = '%s', "
          "year = '%s' WHERE id = %d;", 
          class_p->name, class_p->year, class_p->id);
  int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  handle_errmsg("Error when updating class");
  return rc;
}

int delete_class(Class_p class_p) {
  sprintf(sql, "DELETE FROM grades WHERE student_id IN "
          "(SELECT id FROM students WHERE class_id = %d) OR test_id IN "
          "(SELECT id FROM tests WHERE class_id = %d);",
          class_p->id, class_p->id);
  int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  if (errmsg != NULL) {
    handle_errmsg("Error when deleting class' grades");
    return rc;
  }

  sprintf(sql, "DELETE FROM tests WHERE class_id = %d;", class_p->id);
  rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  if (errmsg != NULL) {
    handle_errmsg("Error when deleting class' tests");
    return rc;
  }

  sprintf(sql, "DELETE FROM students WHERE class_id = %d;", class_p->id);
  rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  if (errmsg != NULL) {
    handle_errmsg("Error when deleting class' students");
    return rc;
  }

  sprintf(sql, "DELETE FROM classes WHERE id = %d;", class_p->id);
  rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  handle_errmsg("Error when deleting class");
  return rc;
}

int create_student(Student_p student_p) {
  sprintf(sql, "INSERT INTO students (first_name, last_name, class_id) "
          "VALUES ('%s', '%s', %d);", 
          student_p->first_name, student_p->last_name, student_p->class_id);
  int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  handle_errmsg("Error when creating student");
  return rc;
}

Student_p *find_students(Id *class_id, bool with_grades) {
  if (class_id != NULL) {
    sprintf(sql, "SELECT * FROM students WHERE class_id = %d;", *class_id);
  } else {
    sprintf(sql, "SELECT * FROM students;");
  }
  sqlite3_stmt *stmt;
  sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

  Student_p *students = malloc(sizeof(Student_p) * DEF_ARR_SIZE);
  if (students == NULL) return students;
  int length = 0, size = DEF_ARR_SIZE;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    if (length + 1 == size) {
      Student_p *resized = realloc(students, 2 * size * sizeof(Student_p));
      if (resized == NULL) break;
      students = resized;
      size *= 2;
    }
    Student_p student_row = malloc(sizeof(struct student_type));
    if (student_row == NULL) break;
    student_row->id = sqlite3_column_int(stmt, find_column("id", stmt));
    strcpy(student_row->first_name, 
      (const char *) sqlite3_column_text(stmt, 
                                         find_column("first_name", stmt)));
    strcpy(student_row->last_name,
      (const char *) sqlite3_column_text(stmt, 
                                         find_column("last_name", stmt)));
    student_row->class_id = sqlite3_column_int(stmt,
                                               find_column("class_id", stmt));
    student_row->grades = with_grades 
      ? find_grades(&student_row->id, NULL)
      : NULL;
    students[length++] = student_row;
  }
  students[length] = NULL;

  sqlite3_finalize(stmt);

  return students;
}

int update_student(Student_p student_p) {
  sprintf(sql, "UPDATE students SET first_name = '%s', "
          "last_name = '%s' WHERE id = %d;", 
          student_p->first_name, student_p->last_name, student_p->id);
  int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  handle_errmsg("Error when updating student");
  return rc;
}

int delete_student(Student_p student_p) {
  sprintf(sql, "DELETE FROM grades WHERE student_id = %d;", student_p->id);
  int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  if (errmsg != NULL) {
    handle_errmsg("Error when deleting student's grades");
    return rc;
  }

  sprintf(sql, "DELETE FROM students WHERE id = %d;", student_p->id);
  rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  handle_errmsg("Error when deleting student");
  return rc;
}

int create_test(Test_p test_p) {
  sprintf(sql, "INSERT INTO tests "
          "(name, description, max_score, date, class_id) "
          "VALUES ('%s', '%s', %d, '%s', %d);",
          test_p->name, test_p->description, test_p->max_score, test_p->date,
          test_p->class_id);
  int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  handle_errmsg("Error when creating test");
  return rc;
}

Test_p *find_tests(Id *class_id, bool with_grades) {
  if (class_id != NULL) {
    sprintf(sql, "SELECT * FROM tests WHERE class_id = %d;", *class_id);
  } else {
    sprintf(sql, "SELECT * FROM tests;");
  }
  sqlite3_stmt *stmt;
  sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

  Test_p *tests_arr = malloc(sizeof(Test_p) * DEF_ARR_SIZE);
  if (tests_arr == NULL) return tests_arr;
  int length = 0, size = DEF_ARR_SIZE;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    if (length + 1 == size) {
      Test_p *resized = realloc(tests_arr, 2 * size * sizeof(Test_p));
      if (resized == NULL) break;
      tests_arr = resized;
      size *= 2;
    }
    Test_p test_row = malloc(sizeof(struct test_type));
    if (test_row == NULL) break;
    test_row->id = sqlite3_column_int(stmt, find_column("id", stmt));
    strcpy(test_row->name, 
      (const char *) sqlite3_column_text(stmt, find_column("name", stmt)));
    strcpy(test_row->description, 
      (const char *) sqlite3_column_text(stmt, 
                                         find_column("description", stmt)));
    test_row->max_score =  sqlite3_column_int(stmt, 
                                              find_column("max_score", stmt));
    strcpy(test_row->date, 
      (const char *) sqlite3_column_text(stmt, find_column("date", stmt)));
    test_row->class_id = sqlite3_column_int(stmt, 
                                            find_column("class_id", stmt));
    test_row->grades = with_grades
      ? find_grades(NULL, &test_row->id)
      : NULL;
    tests_arr[length++] = test_row;
  }
  tests_arr[length] = NULL;

  sqlite3_finalize(stmt);

  return tests_arr;
}

int update_test(Test_p test_p) {
  sprintf(sql, "UPDATE tests SET name = '%s', description = '%s', "
               "max_score = %d, date = '%s' WHERE id = %d;",
          test_p->name, test_p->description, test_p->max_score,
          test_p->date, test_p->id);
  int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  handle_errmsg("Error when updating test");
  return rc;
}

int delete_test(Test_p test_p) {
  sprintf(sql, "DELETE FROM grades WHERE test_id = %d;", test_p->id);
  int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  if (errmsg != NULL) {
    handle_errmsg("Error when deleting test's grades");
    return rc;
  }

  sprintf(sql, "DELETE FROM tests WHERE id = %d;", test_p->id);
  rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  handle_errmsg("Error when deleting test");
  return rc;
}

int create_grade(Grade_p grade_p) {
  if (grade_p->score != NULL) {
    sprintf(sql, "INSERT INTO grades (score, student_id, test_id) "
            "VALUES (%f, %d, %d);",
            *grade_p->score, grade_p->student_id, grade_p->test_id);
  } else {
    sprintf(sql, "INSERT INTO grades (student_id, test_id) VALUES (%d, %d);",
            grade_p->student_id, grade_p->test_id);
  }
  int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  handle_errmsg("Error when creating grade");
  return rc;
}

Grade_p *find_grades(Id *student_id, Id *test_id) {
  sprintf(sql, "SELECT * FROM grades WHERE student_id %s %d AND "
               "test_id %s %d;",
          student_id != NULL ? "=" : "!=",
          student_id != NULL ? *student_id : 0,
          test_id != NULL ? "=" : "!=",
          test_id != NULL ? *test_id : 0);
  sqlite3_stmt *stmt;
  sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

  Grade_p *grades = malloc(sizeof(Grade_p) * DEF_ARR_SIZE);
  if (grades == NULL) return grades;
  int length = 0, size = DEF_ARR_SIZE;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    if (length + 1 == size) {
      Grade_p *resized = realloc(grades, 2 * size * sizeof(Grade_p));
      if (resized == NULL) break;
      grades = resized;
      size *= 2;
    }
    Grade_p grade_row = malloc(sizeof(struct grade_type));
    if (grade_row== NULL) break;
    if (sqlite3_column_type(stmt, find_column("score", stmt)) == SQLITE_NULL) {
      grade_row->score = NULL;
    } else {
      Score *score = malloc(sizeof(Score));
      if (score) {
        *score = sqlite3_column_double(stmt, find_column("score", stmt));
      }
      grade_row->score = score;
    }
    grade_row->student_id = 
      sqlite3_column_int(stmt, find_column("student_id", stmt));
    grade_row->test_id =
      sqlite3_column_int(stmt, find_column("test_id", stmt));
    grades[length++] = grade_row;
  }
  grades[length] = NULL;

  sqlite3_finalize(stmt);

  return grades;
}

int update_grade(Grade_p grade_p) {
  if (grade_p->score != NULL) {
    sprintf(sql, "UPDATE grades SET score = %f WHERE "
            "student_id = %d AND test_id = %d;",
            *grade_p->score, grade_p->student_id, grade_p->test_id);
  } else {
    sprintf(sql, "UPDATE grades SET score = NULL WHERE "
            "student_id = %d AND test_id = %d;",
            grade_p->student_id, grade_p->test_id);
  }
  int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  handle_errmsg("Error when updating grade");
  return rc;
}

int delete_grade(Grade_p grade_p) {
  sprintf(sql, "DELETE FROM grades WHERE student_id = %d AND test_id = %d;",
          grade_p->student_id, grade_p->test_id);
  int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
  handle_errmsg("Error when deleting grade");
  return rc;
}

static void handle_errmsg(const char *description) {
  if (errmsg != NULL) {
    fprintf(stderr, "%s: %s\n", description, errmsg);
    sqlite3_free(errmsg);
  }
}

static int find_column(const char *name, sqlite3_stmt *stmt) {
  int column_count = sqlite3_column_count(stmt);
  for (int i = 0; i < column_count; i++) {
    if (!strcmp(sqlite3_column_name(stmt, i), name)) return i;
  }
  return -1;
}
