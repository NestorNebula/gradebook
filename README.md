<a id="top"></a>

<div align="center">
    <h3>Gradebook</h3>
</div>

---

### Table of Contents

- [Description](#description)
  - [Built With](#built-with)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
  - [Basic usage](#basic-usage)
  - [Database](#database)
  - [Data types and formats](#data-types-and-formats)
  - [Options effects](#options-effects)
- [Contributing](#contributing)
- [Acknoledgements](#acknoledgements)
- [License](#license)

---

## Description

Gradebook is a program that allows the user to store grades of classes of students.

The application can perform multiple operations (called options) on data, such as creating, updating, deleting and printing it to the terminal.

For each class stored by the program, it is capable of printing a table consisting of rows for students and columns for tests, displaying all the grades for that class.

### Built With

[![C](https://skillicons.dev/icons?i=c&theme=light)](https://www.c-language.org/)
[![CMake](https://skillicons.dev/icons?i=cmake&theme=light)](https://cmake.org/)
[![SQLite](https://skillicons.dev/icons?i=sqlite&theme=light)](https://www.sqlite.org/)

## Getting Started

### Prerequisites

- CMake
- vcpkg
- sqlite3

### Installation

1. Fork the [Project repository](https://github.com/NestorNebula/gradebook)
2. Clone the forked repository to your local machine
   ```
   git clone git@github.com:<your username>/<repo name>.git
   ```
3. Update remote URL

   ```
   # SSH:
   git remote add upstream git@github.com:NestorNebula/gradebook.git

   # HTTPS:
   git remote add upstream https://github.com/NestorNebula/gradebook.git
   ```

4. From the root directory of the project, create the project's database
   ```
   sqlite3 db/gradebook.db < db/schema.sql
   ```
5. "Build" the configuration files
   ```
   cmake -B build/ -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg
   ```
6. Go to the "build" directory created by the previous command and build the program
   ```
   make
   ```
7. Run the program
   ```
   ./gradebook
   ```

#### Tests

Tests suites can be found in the "tests" directory.

New files in this directory with the extension .cpp will automatically be considered as tests suites.

To run tests, follow this steps:

1. From the root directory of the project, go to the "tests" directory
   ```
   cd tests
   ```
2. "Build" the tests configuration files
   ```
   cmake -B build/ -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg
   ```
3. Go to the "build" directory created by the previous command and build the test file
   ```
   make
   ```
4. Run the tests
   ```
   ./gradebook-tests
   ```

## Usage

### Basic usage

Once the program is running, it proposes multiple options to the user with an associated number to each. The user must enter the number corresponding to the option they want to use.

Each option processus should then be easy to understand for the user, as they are guided by the program to enter the data needed by the option.

Anyway, the user should be aware of some specificities of the program that are discussed in the following sections.

### Database

The program uses a sqlite database that can be created by the user using the db/schema.sql file. Reading this file can give the user every information to understand what data types are expected for each table column.

If the user doesn't want/can't read the file, the following informations about each table should be kept in mind:

- Class, representing a group of students. Each class has a name and a year. Two classes can't have the same name and year.
- Student, representing a class student. Each student has a first name, a last name and a class id, referring to their class. Two students in the same class can't have the same name (combination of their first and last name).
- Test, representing a class exam. Each test has a name, a maximum score (the highest possible score that a student can have for that test), a date, a class id, referring to its class and an optional description. Two tests for the same class can't have the same name.
- Grade, representing a grade obtained by a student for a test. Each grade has a student id, referring to the student that got the grade, a class id, referring to its class and an optional score (the score can be empty). A student can only have one grade for the same test.

### Data types and formats

Some fields in the database must have particular types and formats to work correctly.

Despite the fact that the program can mostly catch errors, users must be aware that entering wrong data types (for example letters instead of numbers) and/or data in the wrong format (different that the one prompted by the program) can cause the program to terminate and in the work case affect the data stored in the database, thus affecting other runs of the program.

If the database was affected by invalid data, it is recommended to delete the affected data or in worst cases to delete the database.

### Options effects

It is advised to take care of the effects an option could have before deciding to use it.

While print options will only print existing data and create options will only create new data that won't affect the existing one, the other ones can affect existing data.

Update and delete options will change data irremediably after their execution. It is important to consider that this options won't the user's validation before executing (once they have entered the data needed).

Once the data has been updated/deleted, there is no way to recover the old data.

## Contributing

If you find any issue while using the program or want to contribute, please <a href="https://github.com/NestorNebula/gradebook/issues">open an issue</a>.

## Acknoledgements

- [GoogleTest](https://github.com/google/googletest)
- [vcpkg](https://vcpkg.io/)
- [FFF (Fake Function Framework)](https://github.com/meekrosoft/fff)

## License

[![MIT License](https://img.shields.io/badge/License-MIT-darkcyan.svg?style=for-the-badge)](https://github.com/NestorNebula/gradebook/blob/main/LICENSE)

© Noa Houssier - 2025

<p align='right'>(<a href='#top'>go back to the top</a>)</p>
