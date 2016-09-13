#ifndef __PLIST_EXTRAS_H
#define __PLIST_EXTRAS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>

#include <plist/plist.h>


typedef enum {
  PLIST_TYPE_UNKNOWN,
  PLIST_TYPE_BINARY,
  PLIST_TYPE_XML,
  PLIST_TYPE_YAML
} fileType;

int loadPlist(char *data, size_t bytes, plist_t *root);

fileType plistType(char *data, size_t bytes);

#endif
