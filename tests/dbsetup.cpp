#include <gtest/gtest.h>
#include <fff.h>
DEFINE_FFF_GLOBALS;

extern "C" {
#include <sqlite3.h>
#include "dbsetup.h"
}

FAKE_VALUE_FUNC(int, sqlite3_open, const char *, sqlite3 **);
FAKE_VALUE_FUNC(int, sqlite3_close, sqlite3 *);

TEST(open_db, returns_sqlite3open_returns_value) {
  RESET_FAKE(sqlite3_open);
  sqlite3_open_fake.return_val = SQLITE_OK;
  EXPECT_EQ(open_db(), SQLITE_OK);
  EXPECT_EQ(sqlite3_open_fake.call_count, 1);
  close_db();
}

TEST(close_db, returns_sqlite3open_returns_value) {
  RESET_FAKE(sqlite3_close);
  open_db();
  sqlite3_close_fake.return_val = SQLITE_OK;
  EXPECT_EQ(close_db(), SQLITE_OK);
  EXPECT_EQ(sqlite3_close_fake.call_count, 1);
}
