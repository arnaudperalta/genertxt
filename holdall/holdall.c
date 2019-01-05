//  Partie implantation du module holdall

#include <stdlib.h>
#include "holdall.h"

//  struct holdall, holdall : implantation par liste dynamique simplement
//    chainée avec insertion en tête

typedef struct choldall choldall;

struct choldall {
  void *value;
  choldall *next;
};

struct holdall {
  choldall *head;
  size_t count;
};

holdall *holdall_empty(void) {
  holdall *ha = malloc(sizeof *ha);
  if (ha == NULL) {
    return NULL;
  }
  ha->head = NULL;
  ha->count = 0;
  return ha;
}

int holdall_put(holdall *ha, void *ptr) {
  choldall *p = malloc(sizeof *p);
  if (p == NULL) {
    return -1;
  }
  p->value = ptr;
  p->next = ha->head;
  ha->head = p;
  ha->count += 1;
  return 0;
}

size_t holdall_count(const holdall *ha) {
  return ha->count;
}

int holdall_apply(holdall *ha, int (*fun)(void *)) {
  choldall *p = ha->head;
  while (p != NULL) {
    int r = fun(p->value);
    if (r != 0) {
      return r;
    }
    p = p->next;
  }
  return 0;
}

int holdall_apply_context(holdall *ha,
    int (*fun)(void *arg, void *result_funcontext),
    void *(*funcontext)(void *context, void *arg), void *context) {
  choldall *p = ha->head;
  while (p != NULL) {
    int r = fun(p->value, funcontext(context, p->value));
    if (r != 0) {
      return r;
    }
    p = p->next;
  }
  return 0;
}

void holdall_dispose(holdall **haptr) {
  if (*haptr == NULL) {
    return;
  }
  choldall *p = (*haptr)->head;
  while (p != NULL) {
    choldall *t = p;
    p = p->next;
    free(t);
  }
  free(*haptr);
  *haptr = NULL;
}
