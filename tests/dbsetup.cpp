#include <gtest/gtest.h>

extern "C" {
#include <sqlite3.h>
#include "dbsetup.h"
}

TEST(open_db, returns_sqlite_ok) {
  EXPECT_EQ(open_db(), SQLITE_OK);
}

TEST(close_db, returns_sqlite_ok) {
  open_db();
  EXPECT_EQ(close_db(), SQLITE_OK);
}
