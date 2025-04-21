/* ccd: the hex sensor

 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define COLS 256

// which to use?
#define LLEN ((2 * (int)sizeof(unsigned long)) + 4 + (9 * COLS - 1) + COLS + 2)
// #define LLEN 4096

void print_help(FILE *stream, char *program) {
  char help_link[] = "https://linux.die.net/man/1/xxd";
  fprintf(stream, "Usage: %s [options] [infile]\n", program);
  fprintf(stream, "Please visit %s for full documentation.\n", help_link);
}

/* print hex dump of buf */
void print_hd(char line[], char buf[], int n_elements, int seek, int cols, int groupsize);


int main(int argc, char *argv[]) {

  /* default parameter values */
  bool autoskip = false; // ignore for now
  bool bits = false;     // ignore for now
  int cols = 16;
  int groupsize = 2;
  bool include = false; // ignore for now
  int len = EOF;
  bool plain = false;
  int seek = 0;
  bool uppercase = false; // ignore for now

  /* parse cli args */
  /* atoi can be unsafe for invalid inputs, consider using strtol instead */
  int opt;
  while ((opt = getopt(argc, argv, "abc:g:hil:ps:u")) != -1) {
    switch (opt) {
    case 'h':
      print_help(stdout, argv[0]);
      return EXIT_SUCCESS;
    case 'a':
      autoskip = true;
      break;
    case 'b':
      bits = true;
      groupsize = 1;
      break;
    case 'c':
      cols = atoi(optarg);
      if (cols > COLS) {
        fprintf(stderr, "%s: invalid number of columns (max: %d)\n", argv[0], COLS);
        return EXIT_FAILURE;
      }
      break;
    case 'g':
      groupsize = atoi(optarg);
      groupsize = (groupsize >= 0) ? groupsize : 2;
      break;
    case 'i':
      include = true;
      break;
    case 'l':
      len = atoi(optarg);
      break;
    case 'p':
      plain = true;
      break;
    case 's':
      seek = atoi(optarg);
      break;
    case 'u':
      uppercase = true;
      break;
    default:
      print_help(stderr, argv[0]);
    }
  }

  /*
  // to test that arg parsing went well, print everything here
  printf("autoskip: %d\n", autoskip);
  printf("bits: %d\n", bits);
  printf("cols: %d\n", cols);
  printf("groupsize: %d\n", groupsize);
  printf("include: %d\n", include);
  printf("len: %d\n", len);
  printf("plain: %d\n", plain);
  printf("seek: %d\n", seek);
  printf("uppercase: %d\n", uppercase);
  */

  /* open file if available, else use stdin */
  FILE *input = NULL;
  if (optind < argc) {
    input = fopen(argv[optind], "r");
    if (input == NULL) {
      fprintf(stderr, "%s: No such file or directory\n", argv[optind]);
      return EXIT_FAILURE;
    }
  } else {
    input = stdin;
  }

  /* start by "using up" the first 'seek' elements */
  for (int i = 0; i < seek; ++i) {
    fgetc(input);
  }

  /* then loop over what is left */
  char line[LLEN];
  char buf[cols];
  int n_elements;
  while ((n_elements = fread(buf, sizeof(char), cols, input)) > 0) {

    if (!plain) {
      print_hd(line, buf, n_elements, seek, cols, groupsize);
    } else {
      for (int i = 0; i < n_elements; ++i) {
        sprintf(&line[2*i], "%02hhx", (unsigned char)buf[i]);
      }
    }

    // display line
    printf("%s\n", line);
  }


  if (input != stdin) {
    fclose(input);
  }
  return EXIT_SUCCESS;
}




void print_hd(char line[], char buf[], int n_elements, int seek, int cols, int groupsize) {

  int start_idx = 10;
  int n_spaces = (cols / groupsize) + ((cols % groupsize != 0) ? 1 : 0);
  int ascii_idx = start_idx + (2 * cols) + n_spaces + 1;
  int space_count, current_idx;

  /* print off-set to line */
  sprintf(line, "%08hhx", seek);
  seek += cols;
  line[start_idx - 2] = ':';
  line[start_idx - 1] = ' ';
  
  /* add hex strings to line */
  space_count = 0; /* how many spaces have we passed already? */
  for (int i = 0; i < cols + n_spaces; ++i) {
    current_idx = start_idx + 2 * i - space_count;
    if ((i + 1) % (groupsize + 1) == 0) {
      line[current_idx] = ' ';
      ++space_count;
    } else if (i - space_count < n_elements) {
      sprintf(&line[current_idx], "%02hhx",
              (unsigned char)buf[i - space_count]);
    } else {
      --current_idx;
      break;
    }
  }
  
  /* add spaces up to ascii index */
  for (int i = ascii_idx - 1; i > current_idx; --i) {
    line[i] = ' ';
  }
  
  /* replace tabs and newlines with period '.' */
  for (int i = 0; i < cols; ++i) {
    if (i < n_elements) {
      line[ascii_idx + i] = (buf[i] == '\n' || buf[i] == '\t') ? '.' : buf[i];
    } else {
      line[ascii_idx + i] = ' ';
    }
  }

}
