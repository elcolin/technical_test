# TCP
## Recherche et processus
    Receive a packet ID and return the IDs of the received packet in order, starting from 0, without gaps.
    Does not return already returned IDs.

Il faut renvoyer les IDs des packets dans l'ordre, j'ai donc recherché un conteneur approprié.

Le conteneur **set** me semble parfait pour l'exercice: La valeur est la clé et il est ordonné.

Il me suffit donc de stocker chaque ID reçu, d'attendre de recevoir la valeur du packet attendu et de me balader linéairement dans mon set tout en poussant dans mon vecteur que je vais renvoyer.

La boucle s'arrête dès que l'ID diverge de mon index signifiant qu'il a un gap et donc un ID non reçu. Elle s'arrête aussi si mon set est vide.


## Types d'erreurs

    Negative numbers
    Overflow

## Améliorations possibles et alternatives
    try, catch, throw
    Compléter la forme Coplienne
    Faire un ostream d'affichage des packets
    Makefile?

# Mountain Climbing

## Recherche et processus
    Given a grid of height, return the shortest path from the point with height=0 to the point with height = 27.
    The path can only go to a new cell with an height at most one higher. No diagonals.

Basé sur ça, j'ai déniché des algorithmes de recherche: Je suis tombée sur le **BFS (Best First Search)**.
J'ai choisi le conteneur queue pour la bonne complexité, je l'ai déclaré en tuple pour à la fois conserver la valeur ainsi que la longueur du chemin.

Durant l'exercice, j'ai codé une simple fonction d'affichage de la grille qui met un X majuscule à la dite position pour débuguer.
En début de boucle, je prends le premier élément de ma queue (toVisit) et la retire du conteneur. Ensuite je regarde ses voisins (en haut, en bas, à gauche et à droite), les ajoute à ma queue et les marque comme visité.

Si je tombe sur la cellule end 'E', et que le chemin est le chemin le plus court, alors je le stock dans une variable.
Je répète jusqu'à ce que la queue soit vide.

## Améliorations possibles et alternatives
Je n'ai pas poussé la gestion d'erreurs, si j'avais pu j'aurais impliqué des try/catch.
Je pense aussi que mon code profiterait d'un allègement des conditions un peu trop longues qui nuisent à la lisibilité du code.
Ne sachant pas à quel point l'optimisation était recherchée, je me suis permise des opérations et des boucles.

Au bout d'un certain temps j'ai eu envie de créer une classe "Grid", parce que j'imaginais que ce serait plus élégant, mais j'ai considéré que l'important était surtout l'algorithme.

Pas une amélioration, mais j'ai aussi vu une possibilité de réaliser l'algorithme en récursif, mais étant toujours plus complexe que l'itératif, je me suis abstenue.

## Types d'erreurs possibles
    Pas de S ou de E
    Grille, width ou height égale à zéro
    Overflow (int, size_t)

# Grid Shortest Path

## Recherche et processus
    Given a grid of cost, return the total cost of the smallest-cost path from the top-left corner to the bottom-right
    corner, going only right or down (no diagonals).
Très similaire à l'algorithme précédent, à la différence que celui-ci implique des poids. J'ai donc utilisé **l'algorithme de Dijkstra** en conséquence.

La difficulté auquelle j'ai fait face durant cet exercice est avec le conteneur "priority_queue" et la façon dont on détermine la priorité puisqu'il contenait des tuples et non de simples valeurs. Je désirais tout de même utilisé ce conteneur plutôt qu'un autre car il se prête parfaitement à la situation: Les chemins les moins coûteux se retrouvent en début de queue.

Au delà de ça, même structure que l'algorithme précédent (avec moins de directions), sauf qu'ici on garde le chemin le moins coûteux dans un vecteur dist et on le met à jour si on en trouve un autre avec encore moins de poids. 

## Types d'erreurs possibles
    Grille, width ou height égale à zéro
    Overflow (int, size_t)

## Améliorations possibles et alternatives
Même que pour le BFS.

Puisqu'il y a des fonctions communes, j'aurais pu faire un hpp en commun pour les deux algorithmes.

Je me demande aussi s'il serait possible d'utiliser des templates.