#include "fonction.h"

//======== MAIN ================================================================================================================

int main() {
    srand(time(NULL));

    char tab[LIG][COL] = {0};
    int choix;
    int compteurMouvements = 0;
    int maxCoups = 0;
    int score = 0;
    int finPartie = 0;
    int curseurL = 0, curseurC = 0;
    int compteurElim[5] = {0};
    int quotas[5] = {0, 0, 0, 0, 0};
    int tempsLimite = 0;
    time_t debutPartie = time(NULL);

    // ================== SYSTEME DE NIVEAUX ===================
    Level niveaux[] = {
        { {20, 50, 20, 0, 0}, 30, 300 },   // Niveau 1
        { {30, 40, 25, 5, 0}, 25, 250 },   // Niveau 2
        { {40, 20, 30, 10, 0}, 20, 200 }   // Niveau 3
    };

    int nbNiveaux = sizeof(niveaux) / sizeof(niveaux[0]);
    int niveauActuel = 0; // On commence au niveau 1
    // ==========================================================


    printf("Que voulez vous faire :\n");
    printf("1 - Commencer une nouvelle Partie \n");
    printf("2 - Charger la partie precedente\n");
    scanf("%d", &choix);
    system("cls");

    if (choix == 1) {
        afficherTableau(tab, curseurL, curseurC);

        // Charger les paramètres du niveau
        for (int i = 0; i < 5; i++)
            quotas[i] = niveaux[niveauActuel].quotas[i];

        maxCoups = niveaux[niveauActuel].maxCoups;
        tempsLimite = niveaux[niveauActuel].tempsLimite;

        gotoligcol(LIG+1, 0);
        printf("Niveau %d sélectionné.\n", niveauActuel + 1);
    }

    else if (choix == 2) {
        if (!chargerPartie(tab, &compteurMouvements, &maxCoups, &curseurL, &curseurC, &score)) {
            printf("Impossible de charger la sauvegarde.\n");
            return 0;
        }

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

    // ================== BOUCLE DE JEU ===================
    boucleDeplacement(tab, &curseurL, &curseurC, &compteurMouvements,
                      maxCoups, &score, &finPartie, compteurElim, quotas,
                      debutPartie, tempsLimite);

    // Après la partie
    if (finPartie == 1) { // Victoire
        printf("\nBravo ! Vous avez terminé le niveau %d !\n", niveauActuel + 1);

        if (niveauActuel < nbNiveaux - 1) {
            niveauActuel++;
            printf("Passage au niveau %d !\n", niveauActuel + 1);
        }
        else {
            printf("Vous avez complété **TOUS** les niveaux !\n");
        }
    }
    else if (finPartie == 2) {
        printf("\nDommage, vous avez perdu.\n");
    }
    // =====================================================

    gotoligcol(LIG+3, 0);
    return 0;
}
