# TCP
## Research



COPLIEN FORM TO COMPLETE
ostream display

## Types of error

    Negative numbers
    Overflow

# Mountain Climbing

## Research
    Given a grid of height, return the shortest path from the point with height=0 to the point with height = 27.
    The path can only go to a new cell with an height at most one higher. No diagonals.

Basé sur ça, j'ai déniché des algorithmes de recherche: Je suis tombée sur le **BFS (Best First Search)**.
J'ai choisi le conteneur queue pour la bonne complexité, je l'ai déclaré en tuple pour à la fois conserver la valeur ainsi que la longueur du chemin.

Durant l'exercice, j'ai codé une simple fonction d'affichage de la grille qui met un X majuscule à la dite position pour débuguer.
En début de boucle, je prends le premier élément de ma queue (toVisit) et la retire du conteneur. Ensuite je regarde ses voisins (en haut, en bas, à gauche et à droite), les ajoute à ma queue et les marque comme visité.

Si je tombe sur la cellule end 'E', et que le chemin est le chemin le plus court, alors je le stock dans une variable.
Je répète jusqu'à que la queue soit vide.

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
