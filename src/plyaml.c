#include "plist/plist.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int plist2yaml(plist_t root);

int plistArray2yaml(plist_t array) {
  uint32_t arraySize = plist_array_get_size(array);
  printf("!!array<%u>\n", arraySize);
  uint32_t i=0;
  plist_t val = NULL;
  for (i=0; i<arraySize; i++) {
    printf("%u:", i);
    val = plist_array_get_item(array, i);
    plist2yaml(val);
  }
  return 0;
}

int plistDict2yaml(plist_t dict) {
  uint32_t dictSize = plist_dict_get_size(dict);
  printf("!!dict<%u>\n", dictSize);
  plist_dict_iter iter = NULL;
  plist_dict_new_iter(dict, &iter);
  char *key = NULL;
  plist_t val = NULL;
  uint32_t i = 0;
  for (i=0; i<dictSize; i++) {
    printf("%u:", i);
    plist_dict_next_item(dict, iter, &key, &val);
    printf("'%s'", key);
    free(key);
    plist2yaml(val);
  }
  return 0;
}

int plistBoolean2yaml(plist_t b) {
  printf("!!bool");
  uint8_t val = 0;
  plist_get_bool_val(b, &val);
  if (val) {
    printf("<TRUE>\n");
  } else {
    printf("<FALSE>\n");
  }
  return 0;
}

int plistString2yaml(plist_t str) {
  printf("!!string");
  char *val = NULL;
  plist_get_string_val(str, &val);
  if (val) {
    printf("<%s>\n", val);
    free(val);
  } else {
    printf("<>\n");
  }
  return 0;
}

int plistUint2yaml(plist_t u) {
  uint64_t val = 0;
  plist_get_uint_val(u, &val);
  printf("!!uint<%llu>\n", val);
  return 0;
}

int plistUid2yaml(plist_t u) {
  uint64_t val = 0;
  plist_get_uid_val(u, &val);
  printf("!!uid<%llu>\n", val);
  return 0;
}

int plistReal2yaml(plist_t real) {
  double val = 0.0;
  plist_get_real_val(real, &val);
  printf("!!real<%f>\n", val);
  return 0;
}

int plistData2yaml(plist_t data) {
  char *val = NULL;
  uint64_t dataSize = 0;
  plist_get_data_val(data, &val, &dataSize);
  printf("!!data<%llu>\n", dataSize);
  free(val);
  return 0;
}

int plistDate2yaml(plist_t date) {
  const int32_t osxEpoch = 978307200; // 01/01/2001 POSIX time
  const int32_t million = 1000000;
  int32_t osec = 0;
  int32_t ousec = 0;
  int64_t sec = 0;
  int32_t usec = 0;
  int32_t usecOverflow = 0;
  char buff[70] = "";
  plist_get_date_val(date, &osec, &ousec);

  // convert from OSX to POSIX epoch
  sec = osec + osxEpoch;

  // normalize 0<=usec<million
  usecOverflow = ousec / million;
  usec = ousec % million;
  sec += usecOverflow;
  while (usec < 0) {
    sec--;
    usec += million;
  }

  // display as ISO8601
  struct tm *utc = gmtime((time_t *) &sec);
  if (strftime(buff, sizeof(buff), "%Y-%m-%dT%H:%M:%S", utc)) {
    printf("!!date<%s.%06iZ>\n", buff, usec);
    return 0;
  } else {
    printf("!!date<INVALID %llis, %ius>\n", sec, usec);
    return 1;
  }
}

int plistKey2yaml(plist_t key) {
  printf("!!key");
  char *val = NULL;
  plist_get_key_val(key, &val);
  if (val) {
    printf("<%s>\n", val);
    free(val);
  } else {
    printf("<>\n");
  }
  return 0;
}

int plist2yaml(plist_t root) {
  plist_type nodeType = plist_get_node_type(root);
  switch (nodeType) {
    case PLIST_BOOLEAN:
      return plistBoolean2yaml(root);
    case PLIST_UINT:
      return plistUint2yaml(root);
    case PLIST_REAL:
      return plistReal2yaml(root);
    case PLIST_STRING:
      return plistString2yaml(root);
    case PLIST_ARRAY:
      return plistArray2yaml(root);
    case PLIST_DICT:
      return plistDict2yaml(root);
    case PLIST_DATE:
      return plistDate2yaml(root);
    case PLIST_DATA:
      return plistData2yaml(root);
    case PLIST_KEY:
      return plistKey2yaml(root);
    case PLIST_UID:
      return plistUid2yaml(root);
    case PLIST_NONE:
      printf("!!none\n");
      return 0;
    default:
      printf("UNKNOWN\n");
      return 1;
  }
}


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
    printf("Could not read binary plist\n");
    return 3;
  }

  return plist2yaml(root);
}

