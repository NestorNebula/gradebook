#ifndef DBSETUP_H
#define DBSETUP_H

/*
 * open_db:
 * Opens SQLite database using sqlite3_open.
 * Returns sqlite3_open call's return value.
 */
int open_db(void);

/*
 * close_db:
 * Closes SQLite database using sqlite3_close.
 * Returns sqlite3_close call's return value.
 */
int close_db(void);

#endif
