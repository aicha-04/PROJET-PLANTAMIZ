#include "macros.h"

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
void Color(int couleurDuTexte,int couleurDeFond);
void gotoligcol(int lig,int col);
void afficherTableau(char tab[LIG][COL], int curseurL, int curseurC);
void verifierTimer(time_t debutPartie, int tempsLimite, int *finPartie);
void afficherCase(char tab[LIG][COL], int lig,int col,int surligner,int selection);
void appliquerGravite(char tab[LIG][COL]);
void analyserEtEliminer(char tab[LIG][COL], int *score, int compteurElim[5], int quotas[5], int *finPartie);
void verifierFinPartie(int *compteurMouvements, int maxCoups, int *finPartie);
int sauvegarderPartie(char tab[LIG][COL], int maxCoups, int compteurMouvements, int curseurL, int curseurC, int score);
int chargerPartie(char tab[LIG][COL], int *compteurMouvements, int *maxCoups, int *curseurL, int *curseurC, int *score);
void boucleDeplacement(char tab[LIG][COL], int *curseurL, int *curseurC,
                       int *compteurMouvements, int maxCoups, int *score,
                       int *finPartie, int compteurElim[5], int quotas[5],
                       time_t debutPartie, int tempsLimite);
