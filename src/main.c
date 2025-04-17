/* ccd: the hex sensor



 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


int main(int argc, char* argv[]) {

  FILE *input = NULL;
  int opt;

  // default parameter values
  bool autoskip = false;
  bool bits = false;
  int cols = 16;
  int groupsize = 2;
  bool help = false;
  bool include = false;
  int len = 0; // what to put for default value here?
  bool plain = false;
  int seek;
  bool uppercase = false;

  while ((opt = getopt(argc, argv, "abc:g:hil:ps:u")) != -1) {
    switch (opt) {
    case 'a':
      autoskip = true;
      break;
    case 'b':
      bits = true;
      break;
    case 'c':
      cols = atoi(optarg);
      if (cols > 256) {
        fprintf(stderr, "%s: invalid number of columns (max: 256)\n", argv[0]);
        return EXIT_FAILURE;
      }
    case 'g':
      groupsize = atoi(optarg);
      break;
    case 'h':
      help = true;
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

  // to test that arg parsing went well, print everything here
  printf("autoskip: %d\n", autoskip);
  printf("bits: %d\n", bits);
  printf("cols: %d\n", cols);
  printf("groupsize: %d\n", groupsize);
  printf("help: %d\n", help);
  printf("include: %d\n", include);
  printf("len: %d\n", len);
  printf("plain: %d\n", plain);
  printf("seek: %d\n", seek);
  printf("uppercase: %d\n", uppercase);

  /* this revealed a peculiar bug:
     if I specify columns, it also changes groupsize

     e.g.
     ccd -c 10
     =>
     cols = 10
     groupsize = 10

     ccd -c 10 -g 20
     cols = 10
     groupsize = 20
     
     fix this later
   */


  // open file if available, else use stdin
  if (optind < argc) {
    input = fopen(argv[optind], "r");
    if (input == NULL) {
      fprintf(stderr, "%s: No such file or directory\n", argv[optind]);
      return EXIT_FAILURE;
    }
  } else {
    input = stdin;
  }


  // start actual processing here
  char buffer[1024];
  // while (fgets(buffer, sizeof(buffer), input)) {
  // }

  if (input != stdin) {
    fclose(input);
  }
  return EXIT_SUCCESS;
}
