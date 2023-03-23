// #include "hash.h"

struct addressEntry {
  /* Node User Name */
  char name[255] = "";
  /* Node ID - 1 byte */
  int nodeID;
};
typedef struct addressEntry ADDENTRY;