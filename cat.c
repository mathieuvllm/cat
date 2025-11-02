/*
 * -- Minimal UNIX "cat" command --
 * (original command written by Torbjorn Granlund and Richard M. Stallman)
 */

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Taken from
// https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
int is_regular_file(const char *path) {
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISREG(path_stat.st_mode);
}

// Same thing here for directory
int isDirectory(const char *path) {
  struct stat statbuf;
  if (stat(path, &statbuf) != 0)
    return 0;
  return S_ISDIR(statbuf.st_mode);
}

int main(int argc, char **argv) {
  int current_arg;
  FILE *file;
  char *fname;
  char ch;
  int line_number = 1;
  int next_line = 0;
  int option_index;

  // Options defaulted to 0
  int show_ends = 0;
  int number_lines = 0;

  // Setting options
  for (current_arg = 1; current_arg < argc; current_arg++) {
    fname = argv[current_arg];

    if (fname[0] != '-')
      continue;

    for (option_index = 1; fname[option_index] != '\0'; option_index++) {
      switch (fname[option_index]) {
      case 'E':
        show_ends = 1;
        break;
      case 'n':
        number_lines = 1;
        break;
      default:
        fprintf(stderr, "Error: invalid flag: -%c.\n", fname[option_index]);
        break;
      }
    }
  }

  // Iterating over files
  for (current_arg = 1; current_arg < argc; current_arg++) {
    fname = argv[current_arg];

    if (fname[0] == '-')
      continue;

    if (isDirectory(fname)) {
      fprintf(stderr, "Error: %s is a directory.\n", fname);
      continue;
    }

    if (!(is_regular_file(fname))) {
      fprintf(stderr, "Error: not a regular file: %s.\n", fname);
      continue;
    }

    if (!(file = fopen(fname, "r")))
      fprintf(stderr, "Error: cannot open file: %s.\n", fname);
    else {
      while ((ch = fgetc(file)) != EOF) {
        if (number_lines == 1 && (next_line == 1 || line_number == 1)) {
          printf("     %d  ", line_number++);
          next_line = 0;
        }

        if (ch == '\n') {
          if (show_ends == 1)
            putchar('$');
          next_line = 1;
        }

        putchar(ch);
      }
    }
    fclose(file);
  }

  return 0;
}
