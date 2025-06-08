#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

/*
 * Input:
 * Every function in this module clears what follows the input
 * up to and including the new line character or EOF.
 */

/*
 * read_number:
 * Reads a number from stream.
 * If a number could be read, returns it and stores true in success,
 * else, returns 0 and stores false in success.
 */
int read_number(bool *success, FILE *stream);

/*
 * Reads a string from stream and stores it in output, 
 * followed by a null character.
 * Reads a maximum of max_len - 1 characters.
 * Returns the number of characters read.
 */
int read_string(char *output, int max_len, FILE *stream);

/*
 * read_answer:
 * Reads the first character of stream.
 * If the character is a Y (for "Yes"), returns true, else returns false.
 */
bool read_answer(FILE *stream);

#endif
