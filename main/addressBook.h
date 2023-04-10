#ifndef _ADDRESSBOOK_H_
#define _ADDRESSBOOK_H_
// #include "hash.h"

struct addressEntry {
  /* Node User Name */
  char name[255] = "";
  /* Node ID - 1 byte */
  int nodeID;
};
typedef struct addressEntry ADDENTRY;
# endif /* _ADDRESSBOOK_H_ */