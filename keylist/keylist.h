
//  Partie interface du module keylist

//  Le module keylist permet d'enregistrer toute les clés de la table de hashage

#ifndef KEYLIST__H
#define KEYLIST__H

#include <stddef.h>

//  struct keylist, keylist : structure regroupant les clés pointé
//    par des (char**) et qui seront utilisé dans la hashtable du programme
//    genertxt. La création de cette structure est confiée à la fonction
//    keylist_empty
typedef struct keylist keylist;

//  keylist_empty : crée une structure de données correspondant initialement à
//    une liste de clé vide. Renvoie NULL en cas de dépassement de capacité.
//    Renvoie un pointeur vers l'objet qui gère la structure de données sinon
extern keylist *keylist_empty(void);

//  keylist_add : ajoute ptr à la liste de clé associé à kl.
//    Renvoie une valeur non nulle en cas de dépassement de capacité.
//    Renvoie zéro sinon
extern char **keylist_add(keylist *kl, char **ptr, size_t order);

//  keylist_value : renvoie la clé associé à la n-ème valeur associé
//    à la keylist kl
extern char **keylist_value(keylist *kl, size_t n);

//  keylist_length : renvoie le nombre de clé dans la keylist associé kl
extern size_t keylist_length(keylist *kl);

//  keylist_keylength : renvoie la longueur de clé associé à k.
extern size_t keylist_keylength(char **k, size_t order);

//  keylist_dispose : si *klptr ne vaut pas NULL, libère les ressources allouées
//    à la structure de donnée associée à *klptr puis affecte à *klptr la valeur
//    NULL
extern void keylist_dispose(keylist **klptr);

// keylist_search : recherche dans la keylist associé kl la première clé égale
//    au pointeur **ptr au sens de strcmp. Renvoie NULL si aucune clé n'a été
//    trouvé, renvoie l'adresse de la clé sinon
extern char **keylist_search(keylist *kl, char **ptr, size_t order);

#endif
