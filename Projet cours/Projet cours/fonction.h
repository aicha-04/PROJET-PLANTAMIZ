#include "marcos.h"

// Change la couleur du texte et du fond
void Color(int couleurDuTexte,int couleurDeFond);

// Déplace le curseur dans la console
void gotoligcol(int lig, int col);

// Génère et affiche le tableau complet
void afficherTableau(char tab[LIG][COL], int curseurL, int curseurC, Contrat contrat);

// Affiche une seule case (optimisé pour les déplacements)
void afficherCase(char tab[LIG][COL], int lig, int col, int surligner, int selection);

// Boucle principale pour les déplacements, échanges et sauvegarde
void boucleDeplacement(char tab[LIG][COL], int maxCoups, int *compteurMouvements);

// Compteur de mouvements et vérification de la limite
int compteurMouvement(int maxCoups, int *compteurMouvements);

// Sauvegarde complète : tableau + compteur + maxCoups
int sauvegarderPartie(char tab[LIG][COL], int maxCoups, int compteurMouvements);

// Chargement complet : tableau + compteur + maxCoups
int chargerPartie(char tab[LIG][COL], int *compteurMouvements, int *maxCoups);

// Affiche le tableau sans le régénérer
void afficherTableauSeul(char tab[LIG][COL], int curseurL, int curseurC);

void afficherCompteur(int compteurMouvements, int maxCoups);

void analyserEtEliminer(char tab[LIG][COL]);
void appliquerGravite(char tab[LIG][COL]);

