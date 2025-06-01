#include <sqlite3.h>
#include <stdio.h>
#include "dbsetup.h"

#ifdef TEST_ENV
#define DB_FILE "gradebook-tests.db"
#else
#define DB_FILE "gradebook.db"
#endif

sqlite3* db;

int open_db(void) {
  return sqlite3_open(DB_FILE, &db);
}

int close_db(void) {
  return sqlite3_close(db);
}
