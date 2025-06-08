#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "input.h"

/*
 * clear_line:
 * Reads stream until meaning a new line character or EOF.
 */
static void clear_line(FILE *stream);

int read_number(bool *success, FILE *stream) {
  int num = 0;
  *success = fscanf(stream, "%d", &num);
  clear_line(stream);
  return num;
}

int read_string(char *output, int max_len, FILE *stream) {
  int ch, i = 0;
  while (i < max_len - 1 && (ch = getc(stream)) != '\n' && ch != EOF) {
    output[i++] = ch;
  }
  output[i] = '\0';
  if (i == max_len - 1) clear_line(stream);
  return i;
}

bool read_answer(FILE *stream) {
  int ch = getc(stream);
  if (ch != '\n' && ch != EOF) clear_line(stream);
  return tolower(ch) == 'y';
}

static void clear_line(FILE *stream) {
  int ch = getc(stream);
  while (ch != '\n' && ch != EOF) ch = getc(stream);
}
