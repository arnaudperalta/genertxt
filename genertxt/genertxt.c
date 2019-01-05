#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "genertxt.h"
#include "holdall.h"
#include "keylist.h"
#include "valuelist.h"

#define STRING_LENGTH 30
#define FUN_SUCCESS 0
#define FUN_FAILURE -1

int genertxt_read(FILE *stream, hashtable *ht, holdall *ha, keylist *kl,
    size_t order) {
  if (stream == NULL) {
    return FUN_FAILURE;
  }
  int c;                 // caractère lu
  size_t char_count = 0; // compteur de char du mot en cours de lecture
  size_t word_count = 0; // compteur de mot du texte en cours de lecture
  size_t length = STRING_LENGTH;

  char **b = NULL; // uplet
  char **k = NULL; // uplet actuel
  char **t = NULL; // uplet tampon

  valuelist *vl = NULL;
  char *buffer = calloc(STRING_LENGTH + 1, sizeof (int)); //buffer de mot
  char *s = NULL;  //pointeur sur le mot copié de la taille de buffer
  bool eof = false; //booléen indiquant si la fin du fichier a été lu
  if (buffer == NULL) {
    return FUN_FAILURE;
  }
  b = malloc((sizeof (char *)) * order); //initilisation du n-uplet vide
  if (b == NULL) {
    fclose(stream);
    return FUN_FAILURE;
  }
  char *bptr = malloc(sizeof (int)); //création mot vide
  bptr[0] = '\0';
  holdall_put(ha, bptr); // insertion du mot vide dans le fourre tout
  for (size_t i = 0; i < order; ++i) { //remplissage du premier n-uplet
    b[i] = bptr;
  }
  // ajout du uplet vide
  keylist_add(kl, b, order); //ajout du n-uplet vide
  vl = valuelist_empty();
  hashtable_add(ht, b, vl);
  while (1) {
    if ((c = fgetc(stream)) == EOF) {
      eof = true;
      fclose(stream);
    }
    if (c == ' ' || c == '\n' || eof) {
      length = STRING_LENGTH; // réinitialisation de la longueur du buffer
      if (char_count > 0) {
        s = malloc(sizeof (int) * (strlen(buffer) + 1));
        if (s == NULL) {
          fclose(stream);
          return FUN_FAILURE;
        }
        memcpy(s, buffer, sizeof (int) * (strlen(buffer) + 1));
        free(buffer);
        holdall_put(ha, s); // insertion du mot dans le holdall
        ++word_count;
        if (word_count == 1) {
          valuelist_add(vl, s); //premier successeur du n-uplet vide
          k = malloc((sizeof (char *)) * order); // allocation d'un n-uplet
        } else if (word_count > 1) {
          //on verifie ici si la clé est deja entré
          t = keylist_add(kl, k, order);
          if (t == NULL) { //la clé n'existe pas dans keylist
            vl = valuelist_empty();
            hashtable_add(ht, k, vl); // enleve la valuelist
          } else { //la clé existe déja dans keylist
            free(k);
            b = t;
            vl = (valuelist *) hashtable_value(ht, t);
          }
          valuelist_add(vl, s); //ajout du successeur
          k = malloc((sizeof (char *)) * order); //allocation d'un n-uplet
        }
        //remplissage du n-uplet avec le précédent
        for (size_t i = 0; i < order - 1; ++i) {
          k[i] = b[i + 1];
        }
        k[order - 1] = s;
        b = k;
        char_count = 0; //réinitialisation du compteur de caractère.
        buffer = calloc(STRING_LENGTH + 1, sizeof (int)); //reset du buffer
        if (buffer == NULL) {
          fclose(stream);
          return FUN_FAILURE;
        }
      }
      if (eof) {
        //remplissage du n-uplet avec le précédent
        free(buffer);
        if (k == NULL) {
          valuelist_add((valuelist *) hashtable_value(ht, b), bptr);
          return EXIT_SUCCESS;
        }
        t = keylist_add(kl, k, order);
        if (t == NULL){ //la clé n'existe pas dans keylist
          vl = valuelist_empty();
          hashtable_add(ht, k, vl);
        } else { //la clé existe déja dans keylist
          free(k);
          b = t;
          vl = (valuelist *) hashtable_value(ht, t);
        }
        valuelist_add(vl, bptr); //ajout du successeur
        return FUN_SUCCESS;
      }
    } else {
      ++char_count;
      if (char_count > length) { // le buffer est plein
        length *= 2;             // on multiplie donc sa longueur par 2
        buffer = realloc(buffer, sizeof (int) * (length + 1));
      }
      sprintf(buffer, "%s%c", buffer, c); //concaténation avec le caractère lu
    }
  }
}

void genertxt_table(hashtable *ht, keylist *kl, size_t order) {
  char **key = NULL;
  valuelist *p = NULL;
  printf("%zu-uplets\tliste des successeurs\n", order);
  for (size_t k = 0; k < keylist_length(kl); ++k) { // ligne par ligne
    key = keylist_value(kl, k);
    if (key == NULL) {
      return;
    }
    printf("(");
    //affichage du n-uplet
    for (size_t k = 0; k < order; ++k) {
      if (k != 0) {
        printf(",");
      }
      printf("[%s]", key[k]);
    }
    printf(")\t");
    p = (valuelist *) hashtable_value(ht, key);
    printf("(");
    //affichage des successeurs
    for (size_t k = 0; k < valuelist_length(p); ++k) {
      if (k != 0) {
        printf(",");
      }
      printf("[%s]", (char *) valuelist_value(p, k));
    }
    printf(")\n");
  }
}

int genertxt_write(hashtable *ht, keylist *kl, size_t order, size_t limit) {
  size_t word_count = 0;
  size_t r = 0;
  valuelist *vl = NULL;
  char **k = malloc(sizeof (char *) * order);
  if (k == NULL) {
    return FUN_FAILURE;
  }
  memcpy(k, keylist_value(kl, r), sizeof (char *) * order);
  vl = (valuelist *) hashtable_value(ht, k);
  char *s = NULL;
  printf("\n");
  while (word_count < limit) {
    vl = (valuelist *) hashtable_value(ht, k);
    r = (size_t) rand() % valuelist_length(vl); //choix du successeur aléatoire
    s = valuelist_value(vl, r);
    //fin de la boucle si on atteint le successeur vide
    if (strcmp(s, "") == 0) {
      break;
    }
    printf("%s ", s);
    ++word_count;
    for (size_t i = 0; i < order - 1; ++i) {
      k[i] = k[i + 1];
    }
    k[order - 1] = s;
  }
  printf("\n");
  free(k);
  return FUN_SUCCESS;
}

void all_valuelist_dispose(hashtable *ht, keylist *kl) {
  char **key = NULL;
  valuelist *p = NULL;
  for (size_t k = 0; k < keylist_length(kl); ++k) {
    key = keylist_value(kl, k);
    if (key != NULL) {
      p = (valuelist *) hashtable_value(ht, key);
      valuelist_dispose(&p);
    }
  }
}
