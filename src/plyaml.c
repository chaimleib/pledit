#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <plist/plist.h>
#include <pl2yaml.h>


int main(int argc, char *argv[]) {
  FILE *plistIn = NULL;
  plist_t root = 0;
  char *fpathIn;
  int sizeIn = 0;
  struct stat *filestats = (struct stat *) malloc(sizeof(struct stat));
  char * bufIn = 0;

  if (argc != 2) {
    printf("No input file provided\n");
    return 1;
  }

  fpathIn = argv[1];
  plistIn = fopen(fpathIn, "rb");

  if (!plistIn) {
    printf("Could not open file\n");
    return 2;
  }

  stat(fpathIn, filestats);
  sizeIn = filestats->st_size;
  bufIn = (char *) malloc(sizeof(char) * (sizeIn + 1));
  fread(bufIn, sizeof(char), sizeIn, plistIn);
  fclose(plistIn);

  plist_from_bin(bufIn, sizeIn, &root);
  if (!root) {
    plist_from_xml(bufIn, sizeIn, &root);
  }
  if (!root) {
    printf("Could not read plist\n");
    return 3;
  }

  return plist2yaml(root);
}

