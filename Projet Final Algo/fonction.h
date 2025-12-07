#include "macros.h"

// Déclaration du type Level
typedef struct {
    int quotas[5];
    int maxCoups;
    int tempsLimite;
} Level;

// Fonctions utilitaires
void Color(int couleurDuTexte,int couleurDeFond);
void gotoligcol(int lig, int col);

// Affichage
void afficherTableau(char tab[LIG][COL], int curseurL, int curseurC, char nomJoueur[]);
void afficherCase(char tab[LIG][COL], int lig, int col, int surligner, int selection);

// Jeu
void verifierFinPartie(int *compteurMouvements, int maxCoups, int *finPartie);
void appliquerGravite(char tab[LIG][COL]);
void mettreAJourCompteurElimination(char tab[LIG][COL], int aEliminer[LIG][COL], int compteurElim[5], int quotas[5], char nomJoueur[]);
int verifierVictoire(int compteurElim[5], int quotas[5]);
void verifierTimer(time_t debutPartie, int tempsLimite, int *finPartie);
void analyserEtEliminer(char tab[LIG][COL], int *score, int compteurElim[5], int quotas[5], int *finPartie, char nomJoueur[]);

// Sauvegarde
int sauvegarderPartie(char tab[LIG][COL], int niveauActuel, int maxCoups, int compteurMouvements, int curseurL, int curseurC, int score, int compteurElim[5], time_t debutPartie, int tempsLimite, char nomJoueur[], int vies);
int chargerPartie(char tab[LIG][COL], int *niveauActuel, int *compteurMouvements, int *maxCoups, int *curseurL, int *curseurC, int *score, int compteurElim[5], time_t *debutPartie, int *tempsLimite, int quotas[5], Level niveaux[], char nomJoueur[], int *vies);

// Déplacement
void boucleDeplacement(char tab[LIG][COL], int *curseurL, int *curseurC, int *compteurMouvements, int maxCoups, int *score, int *finPartie, int compteurElim[5], int quotas[5], time_t debutPartie, int tempsLimite, int niveauActuel, char nomJoueur[], int *vies);
