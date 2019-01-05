//  Partie implantation du module valuelist

#include <stdlib.h>
#include "valuelist.h"

typedef struct cvaluelist cvaluelist;

struct cvaluelist {
  void *value;
  cvaluelist *next;
};

struct valuelist {
  cvaluelist *head;
  cvaluelist *tail;
  size_t length;
};

valuelist *valuelist_empty(void) {
  valuelist *vl = malloc(sizeof *vl);
  if (vl == NULL) {
    return NULL;
  }
  vl -> head = NULL;
  vl -> tail = NULL;
  vl -> length = 0;
  return vl;
}

int valuelist_add(valuelist* vl, void *ptr) {
  cvaluelist *p = malloc(sizeof *p);
  if (p == NULL) {
    return -1;
  }
  p -> next = NULL;
  p -> value = ptr;
  if (vl -> head == NULL){
    vl -> head = p;
    vl -> tail = p;
  } else {
    vl -> tail -> next = p;
    vl -> tail = p;
  }
  vl -> length += 1;
  return 0;
}

void *valuelist_value(valuelist *vl, size_t n) {
  if (vl == NULL || n >= vl -> length) {
    return NULL;
  }
  cvaluelist *p = vl -> head;
  while (n != 0) {
    p = p -> next;
    --n;
  }
  return p -> value;
}

size_t valuelist_length(valuelist *vl) {
  return vl -> length;
}

void valuelist_dispose(valuelist **vlptr) {
  if (*vlptr == NULL) {
    return;
  }
  cvaluelist *p = (*vlptr)->head;
  while (p != NULL) {
    cvaluelist *t = p;
    p = p->next;
    free(t);
  }
  free(*vlptr);
  *vlptr = NULL;
}
