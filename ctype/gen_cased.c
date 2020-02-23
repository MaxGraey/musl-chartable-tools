#include <stdio.h>
#include <stdlib.h>

int main() {
  char *set = calloc(0x110000, 1);
  char table1[0x300];
  char buf[128], dummy;
  int a, b;
  FILE *f;

  f = fopen("data/DerivedCoreProperties.txt", "rb");
  while (fgets(buf, sizeof buf, f)) {
    if (sscanf(buf, "%x..%x ; Cased%c", &a, &b, &dummy) == 3)
      for (; a <= b; a++)
        set[a] = 1;
    else if (sscanf(buf, "%x ; Cased%c", &a, &dummy) == 2)
      set[a] = 1;
  }
  fclose(f);

#define STEP 0x100
  int blocks_needed = 0;
  int i = 0;
  for (a = 0; a < 0x20000; a += STEP, i++) {
    for (b = 0; b < STEP; b++) {
      if (set[a + b] != set[a])
        break;
    }
    printf("%d,", b != STEP ? 18 + blocks_needed++ : 16 + !!set[a]);
    if (i && (((i + 1) % 16) == 0))
      printf("\n");
  }
  for (a = 0; a < 32; a++, i++) {
    printf("0,");
    if (((i + 1) % 16) == 0)
      printf("\n");
  }
  for (a = 0; a < 32; a++, i++) {
    printf("255,");
    if (((i + 1) % 16) == 0)
      printf("\n");
  }
  for (a = 0; a < 0x20000; a += STEP) {
    for (b = 0; b < STEP; b++)
      if (set[a + b] != set[a])
        break;
    unsigned x = 0;
    if (b != STEP) {
      for (b = 0; b < STEP; b++, i++) {
        x = x / 2 + 128 * !!set[a + b];
        if (!(b + 1 & 7)) {
          printf("%d,", x & 255);
          if (((i + 1) % (16 * 12)) == 0)
            printf("\n");
        }
      }
    }
  }
}
