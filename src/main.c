/* ccd: the hex sensor

 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define COLS 256

// which to use?
#define LLEN ((2*(int)sizeof(unsigned long)) + 4 + (9*COLS-1) + COLS + 2)
// #define LLEN 4096

void print_help(FILE* stream, char* program) {
  char help_link[] = "https://linux.die.net/man/1/xxd";
  fprintf(stream, "Usage: %s [options] [infile]\n", program);
  fprintf(stream, "Please visit %s for full documentation.\n", help_link);
}


void reset_line(char line[], int offset) {

  /* 00000000: 1111 2222 3333 4444 5555 6666 7777 8888  1234567890123456

     starts with hex offset, always eight characters
     then colon and spaces
     ignore with -p flag

     then ((cols / groupsize) + (cols % groupsize)) groups
     each with (2*grouspize) characters
     each separated by a space

     this is governed by both cols and groupsize parameters
     if -p flag:
         ignore groupsize
         do not ignore cols

     then two spaces
     then the (cols) characters from which the hex info comes from
     ignore with -p flag

   */
  
  
}

int main(int argc, char* argv[]) {

  /* default parameter values */
  bool autoskip = false;   // ignore for now
  bool bits = false;       // ignore for now
  int cols = 16;
  int groupsize = 2;
  bool include = false;    // ignore for now
  int len = EOF;
  bool plain = false;
  int seek = 0;
  bool uppercase = false;  // ignore for now

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


  char line[LLEN];
  char buf[cols];
  int start_idx = 10; /* first 10 spots reserved for printing off-set */
  int ascii_idx = start_idx + (2*groupsize + 1)*(cols / groupsize) + 2; /* where to start printing ascii rep */
  while (fread(buf, sizeof(char), cols, input)) {

    // print off-set to line
    sprintf(line, "%08hhx", seek);
    seek += cols;
    line[start_idx - 2] = ':';
    line[start_idx - 1] = ' ';

    // add hex strings to line
    for (int i = 0; i < (ascii_idx - start_idx); i += 2) {

      if ((i + 1) % (2*groupsize + 1) == 0) {
        line[start_idx + i] = ' ';
        --i;
        continue;
      }

      sprintf(&line[i + start_idx], "%02hhx", (unsigned char)buf[i]);
    }

    // add two spaces before ascii representation
    strncpy(&line[ascii_idx - 2], "  ", 2);


    // replace newlines with period '.'
    for (int i = 0; i < cols; ++i) {
      line[ascii_idx + i] = (buf[i] == '\n' || buf[i] == '\t') ? '.' : buf[i];
    }
    
    // display line
    printf("%s\n", line);

  }



  if (input != stdin) {
    fclose(input);
  }
  return EXIT_SUCCESS;
}
