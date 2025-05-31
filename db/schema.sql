CREATE TABLE IF NOT EXISTS classes (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name VARCHAR(20) NOT NULL,
  year VARCHAR(5) NOT NULL,
  UNIQUE(name, year)
);

CREATE TABLE IF NOT EXISTS students (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  first_name VARCHAR(50) NOT NULL,
  last_name VARCHAR(50) NOT NULL,
  class_id INTEGER NOT NULL,
  FOREIGN KEY(class_id) REFERENCES classes(id),
  UNIQUE(first_name, last_name, class_id)
);

CREATE TABLE IF NOT EXISTS tests (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name VARCHAR(30) NOT NULL,
  description VARCHAR(100),
  max_score INTEGER NOT NULL,
  date VARCHAR(10) NOT NULL,
  class_id INTEGER NOT NULL,
  FOREIGN KEY(class_id) REFERENCES classes(id),
  UNIQUE(name, class_id)
);

CREATE TABLE IF NOT EXISTS grades (
  score INTEGER,
  student_id INTEGER NOT NULL,
  test_id NOT NULL,
  FOREIGN KEY(student_id) REFERENCES students(id),
  FOREIGN KEY(test_id) REFERENCES tests(id),
  UNIQUE(student_id, test_id)
);
