//  Partie interface du module valuelist

//  Le module keylist permet d'enregistrer toute les clés de la table de hashage

#ifndef VALUELIST__H
#define VALUELIST__H

#include <stddef.h>

//  struct valuelist, valuelist : structure regroupant les valeurs correspondant
//    aux successeurs des n-uplets du programme genertxt et qui seront utilisé
//    dans la hashtable en tant que valeur. La création de cette structure est
//    confiée à la fonction valuelist_empty
typedef struct valuelist valuelist;

//  valuelist_empty : crée une structure de données correspondant initialement à
//    une liste de valeur vide. Renvoie NULL en cas de dépassement de capacité.
//    Renvoie un pointeur vers l'objet qui gère la structure de données sinon
extern valuelist *valuelist_empty(void);

//  valuelist_add : ajoute ptr à la liste de valeur associé à vl.
//    Renvoie une valeur non nulle en cas de dépassement de capacité.
//    Renvoie zéro sinon
extern int valuelist_add(valuelist *vl, void *ptr);

//  valuelist_value : renvoie la valeur associé à la n-ème valeur associé
//    à la valuelist vl
extern void *valuelist_value(valuelist *vl, size_t n);

//  valuelist_length : renvoie le nombre de valeur dans la valuelist associé vl
extern size_t valuelist_length(valuelist *vl);

//  valuelist_dispose : si *vlptr ne vaut pas NULL,
//    libère les ressources allouées à la structure de donnée associée à *vlptr
//    puis affecte à *vlptr la valeur NULL
extern void valuelist_dispose(valuelist **vlptr);

#endif
