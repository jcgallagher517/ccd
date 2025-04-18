/* ccd: the hex sensor



 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define COLS 256
#define LLEN ((2*(int)sizeof(unsigned long)) + 4 + (9*COLS-1) + COLS + 2)

int main(int argc, char* argv[]) {

  /* default parameter values */
  char help_link[] = "https://linux.die.net/man/1/xxd";
  bool autoskip = false;
  bool bits = false;
  int cols = 16;
  int groupsize = 2;
  bool include = false;
  int len = 0; /* 0 means read til EOF */
  bool plain = false;
  int seek = 0; /* initial offset in bytes */
  bool uppercase = false;

  /* parse cli args */
  /* atoi can be unsafe for invalid inputs, consider using strtol instead */
  int opt;
  while ((opt = getopt(argc, argv, "abc:g:hil:ps:u")) != -1) {
    switch (opt) {
    case 'h':
      printf("Usage: %s [options] [infile]\n", argv[0]);
      printf("Please visit %s for full documentation.\n", help_link);
      return EXIT_SUCCESS;
    case 'a':
      autoskip = true;
      break;
    case 'b':
      bits = true;
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
      fprintf(stderr, "Usage: %s [options] [infile]\n", argv[0]);
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


  /* start actual processing here */
  char buffer[LLEN];
  printf("%d\n", LLEN);
  // while (fgets(buffer, sizeof(buffer), input)) {
  // }




  if (input != stdin) {
    fclose(input);
  }
  return EXIT_SUCCESS;
}
