#include "marcos.h"

// === TABLE DES COULEURS CONSOLE ===
// 0 : Noir
// 1 : Bleu foncé
// 2 : Vert foncé
// 3 : Turquoise
// 4 : Rouge foncé
// 5 : Violet
// 6 : Vert caca d'oie
// 7 : Gris clair
// 8 : Gris foncé
// 9 : Bleu fluo
// 10 : Vert fluo
// 11 : Turquoise
// 12 : Rouge fluo
// 13 : Violet 2
// 14 : Jaune
// 15 : Blanc

// Fonction pour changer la couleur du texte et du fond dans la console
void Color(int couleurDuTexte,int couleurDeFond);

// Fonction pour déplacer le curseur à une position spécifique dans la console
void gotoligcol( int lig, int col );

// Fonction pour afficher le tableau complet (uniquement pour l'initialisation avec les principal valeur pour es affichage de couleur)
void afficherTableau(char tab[LIG][COL], int curseurL, int curseurC);

// Fonction pour afficher une seule case (optimisé pour déplacement)
void afficherCase(char tab[LIG][COL], int lig, int col, int surligner, int selection);

//deplacement des cases afin d'eviter de reset tout le tableau
void boucleDeplacement(char tab[LIG][COL]);

//compteur de swithc de mouvement pour les points
void compteurMouvement();

