#include <stdio.h>
#include "hashtable.h"
#include "holdall.h"
#include "keylist.h"

// Fonction de lecture du programme remplissant la hashtable donné ainsi
//    que la liste de clé (keylist) et initialisant des listes de valeurs
//    (valuelist) pour chaque clé. Lit chaque successeur de chaque n-uplet et
//    le rentre dans sa valuelist correspondante. Renvoie une valeur non nulle
//    en cas de dépassement de mémoire sinon renvoie 0.
extern int genertxt_read(FILE *stream, hashtable *ht, holdall *ha, keylist *kl,
    size_t order);

// Supprime l'affichage standard et le remplace par une table illustrant la
//    l'état de la structure du programme montrant les n-uplets et leurs
//    successeurs.
extern void genertxt_table(hashtable *ht, keylist *kl, size_t order);

// Génère un texte aléatoire écrit sur la sortie standard, la génération est
//    effectuée en écrivant le premier n-uplet puis en suivant les successeurs
//    choisi aléatoirement selon un seeding défini auparavant. La génération se
//    finit quand on obtient le successeur vide signifiant la fin du texte.
//    La fonction renvoie une valeur non nulle en cas de dépassement de mémoire
//    sinon renvoie 0.
extern int genertxt_write(hashtable *ht, keylist *kl, size_t order,
    size_t limit);

// Libère toutes les valueslist présente dans la table de hashage associé
//    au programme.
extern void all_valuelist_dispose(hashtable *ht, keylist *kl);

// Variable globale utilisée par la fonction de hashage définissant l'ordre des
//    n-uplets.
extern size_t uplet_order;
