#include "plistExtras.h"

inline size_t minSize(size_t a, size_t b) {
  return (a > b) ? b : a;
}

inline bool hasPreamble(const char *preamble, size_t preambleBytes, char *data, size_t dataBytes) {
  size_t bytes = minSize(dataBytes, preambleBytes);
  return 0 == memcmp(data, preamble, bytes);
}

int loadPlist(char *data, size_t bytes, plist_t *root) {
  fileType ftype = plistType(data, bytes);
  switch (ftype) {
    case PLIST_TYPE_BINARY:
      plist_from_bin(data, bytes, root);
      break;
    case PLIST_TYPE_XML:
      plist_from_xml(data, bytes, root);
      break;
    default:
      return 1;
  }
  return root ? 0 : 1;
} 

fileType plistType(char *data, size_t bytes) {
  const char *binPreamble = "bplist\x30\x30";
  const char *xmlPreamble = "<?xml ";

  if (0 == bytes) return PLIST_TYPE_UNKNOWN;
  if (hasPreamble(binPreamble, strlen(binPreamble)-1, data, bytes)) {
    return PLIST_TYPE_BINARY;
  }
  if (hasPreamble(xmlPreamble, strlen(xmlPreamble)-1, data, bytes)) {
    return PLIST_TYPE_XML;
  }
  return PLIST_TYPE_UNKNOWN;
}
