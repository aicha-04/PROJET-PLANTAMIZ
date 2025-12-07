#include "fonction.h"



//======== MAIN ================================================================================================================
#include "fonction.h"

int main() {
    srand(time(NULL));

    char tab[LIG][COL] = {0};
    int choix;
    int compteurMouvements = 0;
    int maxCoups = 10;
    int score = 0;
    int finPartie = 0;
    int curseurL = 0, curseurC = 0;
    int compteurElim[5] = {0};       // Total de chaque type éliminé : S, F, P, O, M
    int quotas[5] = {50, 30, 20, 0, 0}; // Quotas de victoire pour chaque symbole (modifiable)
    int tempsLimite = 60;   // Durée de la partie en secondes (modifiable)
    time_t debutPartie = time(NULL);


    printf("Choisissez le tableau :\n");
    printf("1 - Tableau 1\n");
    printf("2 - Charger la partie precedente\n");
    scanf("%d", &choix);
    system("cls");

    if(choix == 1) {
        afficherTableau(tab, curseurL, curseurC);
        printf("Tableau 1 sélectionné.\n");
    }
    else if(choix == 2) {
        if(!chargerPartie(tab, &compteurMouvements, &maxCoups, &curseurL, &curseurC, &score)) {
            printf("Impossible de charger la sauvegarde.\n");
            return 0;
        }

        // ➜ afficherTableau ne régénèrera pas le plateau si tab est déjà rempli
        afficherTableau(tab, curseurL, curseurC);

        Color(11,0);
        gotoligcol(LIG+1, 0);

        printf("Mouvements : %d / %d\n", compteurMouvements, maxCoups);
        gotoligcol(LIG+2, 0);
        printf("Points : %d\n", score);
        Color(7,0);
    }

    gotoligcol(LIG+3, 0);
    printf("ZQSD = se déplacer | ESPACE = échanger | A = sauvegarder | E = quitter");

    boucleDeplacement(tab, &curseurL, &curseurC, &compteurMouvements,
                  maxCoups, &score, &finPartie, compteurElim, quotas,
                  debutPartie, tempsLimite);

    gotoligcol(LIG+3, 0);
    return 0;
}
