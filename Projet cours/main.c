#include "fonction.h"

int main()
{
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires

    char tab[LIG][COL]={0}; // Tableau principal initialisé à zéro

    // Affiche le tableau complet avec poistion intial
    afficherTableau(tab, 0, 0);

    gotoligcol(LIG+1, 0); // Place éléments à la fin du tableau
    printf("\nAppuyez sur ZQSD pour vous deplacer, espace pour selectionner et e pour quitter");

    //Fonction pour se déplacer
    boucleDeplacement(tab);

    gotoligcol(LIG+1, 0); // Place éléments à la fin du tableau
    return 0;
}
