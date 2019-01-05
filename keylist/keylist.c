//  Partie implantation du module keylist

#include <stdlib.h>
#include <string.h>
#include "keylist.h"

typedef struct ckeylist ckeylist;

struct ckeylist {
  char **key;
  ckeylist *next;
};

struct keylist {
  ckeylist *head;
  ckeylist *tail;
  size_t length;
};

keylist *keylist_empty(void) {
  keylist *kl = malloc(sizeof *kl);
  if (kl == NULL) {
    return NULL;
  }
  kl -> head = NULL;
  kl -> tail = NULL;
  kl -> length = 0;
  return kl;
}

char **keylist_add(keylist *kl, char **ptr, size_t order) {
  char **r = keylist_search(kl, ptr, order);
  if (r != NULL) {
    return r;
  }
  ckeylist *p = malloc(sizeof *p);
  if (p == NULL) {
    return NULL;
  }
  p -> key = ptr;
  p -> next = NULL;
  if (kl -> head == NULL) {
    kl -> head = p;
    kl -> tail = p;
  } else {
    kl -> tail -> next = p;
    kl -> tail = p;
  }
  kl -> length += 1;
  return NULL;
}

char **keylist_value(keylist *kl, size_t n) {
  if (kl == NULL || n >= kl -> length) {
    return NULL;
  }
  ckeylist *p = kl -> head;
  while (n != 0) {
    p = p -> next;
    --n;
  }
  return p -> key;
}

size_t keylist_length(keylist *kl) {
  return kl -> length;
}

size_t keylist_keylength(char **k, size_t order) {
  size_t count = 0;
  size_t r = 0;
  for (size_t i = 0 ; i < order; ++i) {
    r += strlen(k[count]);
    ++count;
  }
  return r;
}

void keylist_dispose(keylist **klptr) {
  if (*klptr == NULL) {
    return;
  }
  ckeylist *p = (*klptr) -> head;
  while (p != NULL) {
    ckeylist *t = p;
    p = p -> next;
    free(t -> key);
    free(t);
  }
  free(*klptr);
  *klptr = NULL;
}

char **keylist_search(keylist *kl, char **ptr, size_t order) {
  ckeylist *c = kl -> head;
  char **k = NULL;
  size_t count = 0;
  while (c != NULL) {
    k = c -> key;
    for (size_t i = 0; i < order; ++i) {
      if (strcmp(k[i], ptr[i]) == 0) {
        ++count;
      }
    }
    if (count == order) {
      return k;
    } else {
      count = 0;
    }
    c = c -> next;
  }
  return NULL;
}
