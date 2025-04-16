/*



 */

#include <stdio.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {

  int c, n_c = 0;
  while ((c = getchar()) != EOF) {
    ++n_c;
    printf("%02hhx", (unsigned char)c);
    if (n_c % 2 == 0) {
      printf(" ");
    }
  }

  return 0;
}
