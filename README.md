# genertxt
Génère un texte a partir d'un texte donné sur l'entrée standard grâce à une structure principale liant des n-uplets avec leur successeurs dans le texte. Des options de lancement sont disponibles pour faire varier les résultats et leur affichages.

## Options de lancement
`--help` affiche l'aide sur la sortie standard

`-t ou --display-table` supprime le résultat et affiche la structure principale du programme

`-l NUMBER ou --limit=NUMBER` fixer la limite du nombre de mots à produire à NUMBER

`-n NUMBER ou --order=NUMBER` fixer l’ordre de la génération à NUMBER

`-s NUMBER ou --seed=NUMBER` fixer au lancement du programme le germe du générateur pseudoaléatoire
à NUMBER.

## Exemple
Sans paramètre de lancement

![alt text](https://i.imgur.com/oIRwike.png)

L'option display-table

![alt text](https://i.imgur.com/7vzBQMG.png)

avec l'option order

![alt text](https://i.imgur.com/qHXtgfS.png)

L'option seed

![alt text](https://i.imgur.com/WJNVhJZ.png)

L'option limit

![alt text](https://i.imgur.com/5AgP4M3.png)
![alt text](https://i.imgur.com/GL64st0.png)

Valgrind

![alt text](https://i.imgur.com/2rxa20o.png)
