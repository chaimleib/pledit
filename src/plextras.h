#ifndef _PLEXTRAS_H
#define _PLEXTRAS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>

#include <plist/plist.h>


/**
 * Return values from plistType().
 */
typedef enum {
  PLIST_TYPE_UNKNOWN,
  PLIST_TYPE_BINARY,
  PLIST_TYPE_XML,
  PLIST_TYPE_YAML
} fileType;

/**
 * Convert an array of bytes into tree of plist nodes.
 * @param data the input buffer
 * @param bytes the size of the input buffer
 * @param root receives the root plist node
 * @returns error code
 */
int loadPlist(char *data, size_t bytes, plist_t *root);

/**
 * Identify the format of the plist data.
 * @param data the input buffer
 * @param bytes the size of the input buffer
 * @returns a fileType
 */
fileType plistType(char *data, size_t bytes);

#endif
