// A Hello World example

#include <stdio.h>
#include <time.h>
#include <sys/io.h>

int main (int argc, char **argv) {
  struct timespec t = {5, 0};
//  int ent;
//  char buf [20];
  printf ("Hello World!!!\n");
/*  scanf ("%d", &ent);
  scanf ("%s", buf);
  printf ("ent: %d\n", ent);
  printf ("buf: %s\n", buf);
  ent = inb (0x378); */
//  while (1);
  nanosleep (&t, 0);
  printf ("Bye bye!!!\n");
//  exit (0);
  return 0;
}
