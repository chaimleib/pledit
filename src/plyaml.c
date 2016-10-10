#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <plist/plist.h>
#include <pl2yaml.h>
#include <plextras.h>


void printPlistType(char *data, size_t bytes) {
  fileType fType = plistType(data, bytes);
  switch (fType) {
    case PLIST_TYPE_BINARY:
      printf("Plist type: binary\n");
      break;
    case PLIST_TYPE_XML:
      printf("Plist type: XML\n");
      break;
    default:
      printf("Plist type: unknown\n");
  }
}

/**
 * Read a file specified by fPath into buffer, and set bytes to the file size.
 * @param fPath location of the file
 * @param buffer pointer to the resulting file data. Caller is responsible for freeing it
 * @param bytes size of the file
 * @return whether there was an error
 */
int loadFile(char *fPath, char **buffer, size_t *bytes) {
  int err = 0;
  FILE *file = NULL;
  struct stat *filestats = (struct stat *) malloc(sizeof(struct stat));
  *bytes = 0;

  do {
    file = fopen(fPath, "rb");
    if (!file) {
      err = 1;
      break;
    }
    stat(fPath, filestats);
    *bytes = filestats->st_size;
    *buffer = (char *) malloc(sizeof(char) * (*bytes + 1));
    fread(*buffer, sizeof(char), *bytes, file);
    fclose(file);
  } while (false);

  free(filestats);
  return err;
}

int main(int argc, char *argv[]) {
  plist_t root = NULL;
  char *fpathIn = NULL;
  size_t sizeIn = 0;
  char* bufIn = NULL;

  if (argc != 2) {
    printf("No input file provided\n");
    return 1;
  }

  fpathIn = argv[1];
  if (loadFile(fpathIn, &bufIn, &sizeIn)) {
    printf("Could not open file\n");
    return 2;
  }

  printPlistType(bufIn, sizeIn);

  if (loadPlist(bufIn, sizeIn, &root)) {
    free(bufIn);
    printf("Could not read plist\n");
    return 3;
  }
  free(bufIn);

  return plist2yaml(root);
}

