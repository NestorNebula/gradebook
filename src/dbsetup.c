#include <sqlite3.h>
#include "dbsetup.h"

#define DB_FILE "../db/gradebook.db"

sqlite3* db;

int open_db(void) {
  return sqlite3_open(DB_FILE, &db);
}

int close_db(void) {
  return sqlite3_close(db);
}
