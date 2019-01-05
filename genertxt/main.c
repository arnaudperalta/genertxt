#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "hashtable.h"
#include "holdall.h"
#include "keylist.h"
#include "valuelist.h"
#include "genertxt.h"

//------------------------------------------------------------------------------

#define ARG_LONG        "--"
#define ARG_SHORT       "-"
#define ARG_EQUAL       "="
#define ARG_HELP        "help"
#define ARG_TABLE       "display-table"
#define ARG_LIMIT       "limit"
#define ARG_ORDER       "order"
#define ARG_SEED        "seed"
#define ARG_HELP_SHORT  "h"
#define ARG_TABLE_SHORT "t"
#define ARG_LIMIT_SHORT "l"
#define ARG_ORDER_SHORT "n"
#define ARG_SEED_SHORT  "s"
#define DEFAULT_LIMIT   100
#define DEFAULT_ORDER   1
#define DEFAULT_SEED    0


// Fonction de hashage pour le module hashtable.
static size_t str_hashfun(const char **ptr);

// Fonction de comparaison des n-uplets.
static int keycmp(const char **k1, const char **k2);

// Variable globale pour la fonction de hashage.
size_t uplet_order;

// Fonction utilisé pour la désallocation du contenu du holdall.
int rfree(void *ptr);

// Renvoie sur la sortie standard l'aide à l'utilisation de ce programme
void print_help(void);

// Renvoie true si la chaîne de caractère entrée est un nombre, false sinon.
bool is_number(const char *s);

//---Fonction main--------------------------------------------------------------

int main(int argc, char *argv[]) {

  // Lecture de l'aide
  for (int k = 1; k < argc; k++) {
    if (strcmp(argv[k], ARG_LONG ARG_HELP) == 0 ||
        strcmp(argv[k], ARG_SHORT ARG_HELP_SHORT) == 0) {
      print_help();
      return EXIT_SUCCESS;
    }
  }

  // Table de hashage, fichiers.
  FILE *stream = stdin;
  hashtable *ht = hashtable_empty((int (*)(const void *, const void *)) keycmp,
      (size_t (*)(const void *)) str_hashfun);
  holdall *ha = holdall_empty();
  keylist *kl = keylist_empty();

  if (ht == NULL || ha == NULL || kl == NULL) {
    goto error_capacity;
  }

  // Lecture arguments et initialisation des paramètres
  bool arg_table = false;
  size_t limit = DEFAULT_LIMIT;
  uplet_order = DEFAULT_ORDER;
  unsigned int seed = DEFAULT_SEED;
  for (int k = 1; k < argc; ++k) {
    if (strcmp(argv[k], ARG_LONG ARG_TABLE) == 0 ||
        strcmp(argv[k], ARG_SHORT ARG_TABLE_SHORT) == 0) {
      arg_table = true;
    } else if (strcmp(argv[k], ARG_SHORT ARG_LIMIT_SHORT) == 0) {
      ++k;
      if (!is_number(argv[k])) {
        goto error_arg;
      }
      limit = (size_t) atoi(argv[k]);
    } else if (strcmp(argv[k], ARG_SHORT ARG_ORDER_SHORT) == 0) {
      ++k;
      if (!is_number(argv[k])) {
        goto error_arg;
      }
      uplet_order = (size_t) atoi(argv[k]);
    } else if (strcmp(argv[k], ARG_SHORT ARG_SEED_SHORT) == 0) {
      ++k;
      if (!is_number(argv[k])) {
        goto error_arg;
      }
      seed = (unsigned int) atoi(argv[k]);
    }
    else {
      char *t = strtok(argv[k], ARG_EQUAL);
      if (strcmp(t, ARG_LONG ARG_LIMIT) == 0) {
        t = strtok(NULL, ARG_EQUAL);
        if (!is_number(t)) {
          goto error_arg;
        }
        limit = (size_t) atoi(t);
      } else if (strcmp(t, ARG_LONG ARG_ORDER) == 0) {
        t = strtok(NULL, ARG_EQUAL);
        if (!is_number(t)) {
          goto error_arg;
        }
        uplet_order = (size_t) atoi(t);
      } else if (strcmp(t, ARG_LONG ARG_SEED) == 0) {
        t = strtok(NULL, ARG_EQUAL);
        if (!is_number(t)) {
          goto error_arg;
        }
        seed = (unsigned int) atoi(t);
      } else {
        goto error_arg;
      }
    }
  }

  // Lecture du texte et enregistrements des données
  if (genertxt_read(stream, ht, ha, kl, uplet_order) != 0) {
    return EXIT_FAILURE;
  }
  // Affichage du texte généré ou de la table
  if (arg_table) {
    genertxt_table(ht, kl, uplet_order);
  } else {
    srand(seed);
    if (genertxt_write(ht, kl, uplet_order, limit) != 0) {
      goto error_capacity;
    }
  }

  int r = EXIT_SUCCESS;
  goto dispose;

// Erreurs et désallocation
error_arg:
  fprintf(stderr, "*** Error: wrong argument\n");
  r = EXIT_FAILURE;
  goto dispose;

error_capacity:
  fprintf(stderr, "*** Error: not enough memory\n");
  r = EXIT_FAILURE;
  goto dispose;

dispose:
  fclose(stream);
  all_valuelist_dispose(ht, kl);
  hashtable_dispose(&ht);
  keylist_dispose(&kl);
  if (ha != NULL) {
    holdall_apply(ha, rfree);
  }
  holdall_dispose(&ha);
  return r;
}

size_t str_hashfun(const char **ptr) {
  size_t h = 0;
  const char *s = NULL;
  for (size_t i = 0; i < uplet_order; ++i) {
    s = ptr[i];
    for (const unsigned char *p = (const unsigned char *) s; *p != '\0'; ++p) {
      h = 37 * h + *p;
    }
  }
  return h;
}

int rfree(void *ptr) {
  free(ptr);
  return 0;
}

void print_help(void) {
  printf("Aide\n");
  printf("genertxt : Génération d'un texte aléatoire à partir du texte ");
  printf("entré sur l'entrée standard.\n");
  printf("Options de lancement : \n");
  printf("--display-table ou -t : supprime l'affichage par défaut et affiche");
  printf("la structure de données\n du programme, on y voit donc les n-uplets");
  printf(" ainsi que leurs successeurs.\n");
  printf("--limit=NUMBER ou -l NUMBER (par défaut 100) : change la limite de ");
  printf("mot à produire en fonction de NUMBER.\n");
  printf("--order=NUMBER ou -n NUMBER(par défaut 1) : change l'ordre des ");
  printf("n-uplets de la structure de données.\n");
  printf("--seed=NUMBER ou -s NUMBER(par défaut 0) : fixe au lancement du ");
  printf("programme le germe du générateur\npseudo-aléatoire à NUMBER.\n");
}

int keycmp(const char **k1, const char **k2) {
  int c = 0;
  for (size_t k = 0; k < uplet_order; ++k) {
    c = strcmp(k1[k], k2[k]);
    if (c < 0) {
      return c;
    }
    if (c > 0) {
      return c;
    }
  }
  return c;
}

bool is_number(const char *s) {
  size_t k = 0;
  char c = s[k];
  while (c != '\0') {
    if (!isdigit(c)) {
      return false;
    }
    ++k;
    c = s[k];
  }
  return true;
}
