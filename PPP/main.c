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
        { {3, 0, 0, 0, 0}, 30, 300 },   // Niveau 1
        { {0, 0, 3, 0, 0}, 25, 250 },   // Niveau 2
        { {0, 3, 0, 0, 0}, 20, 200 }    // Niveau 3
    };

    int nbNiveaux = sizeof(niveaux) / sizeof(niveaux[0]);
    int niveauActuel = 0; // On commence au niveau 1

    // ======== Choix de la partie ========
    printf("Que voulez vous faire :\n");
    printf("ATTENTION, Avant n'oubliez pas de Sauvegarder la partie \n");
    printf("1 - Commencer une nouvelle Partie \n");
    printf("2 - Charger la partie precedente\n");
    scanf("%d", &choix);
    system("cls");

    int partieChargee = 0;

    if (choix == 2) {
        if (!chargerPartie(tab, &niveauActuel, &compteurMouvements, &maxCoups,
                  &curseurL, &curseurC, &score, compteurElim, &debutPartie, &tempsLimite, quotas, niveaux)) {
            printf("Impossible de charger la sauvegarde.\n");
            return 0;
        } else {
            partieChargee = 1; // On sait que la partie a été chargée
        }
    }

    // ================== BOUCLE DES NIVEAUX ===================
    while (niveauActuel < nbNiveaux) {
        if (!partieChargee) {
            // Réinitialisation pour un nouveau niveau seulement si pas de partie chargée
            memset(tab, 0, sizeof(tab));
            curseurL = 0;
            curseurC = 0;
            compteurMouvements = 0;
            score = 0;
            finPartie = 0;

            for (int i = 0; i < 5; i++)
                compteurElim[i] = 0;
            for (int i = 0; i < 5; i++)
                quotas[i] = niveaux[niveauActuel].quotas[i];

            maxCoups = niveaux[niveauActuel].maxCoups;
            tempsLimite = niveaux[niveauActuel].tempsLimite;
            debutPartie = time(NULL);
        } else {
            // Partie chargée : ne rien réinitialiser
            partieChargee = 0; // Une fois utilisé, on passe aux niveaux suivants normalement
        }

        system("cls");  // Efface l'écran avant chaque niveau
        afficherTableau(tab, curseurL, curseurC);

        // Affichage sous le tableau
        gotoligcol(LIG+1, 0);
        printf("Niveau : %d\n", niveauActuel + 1);
        printf("ZQSD = se deplacer | ESPACE = echanger | A = sauvegarder | E = quitter");

        // Boucle de jeu
        boucleDeplacement(tab, &curseurL, &curseurC,
                          &compteurMouvements, maxCoups, &score,
                          &finPartie, compteurElim, quotas,
                          debutPartie, tempsLimite, niveauActuel);

        // Clear après la partie, avant d’afficher le message
        system("cls");
        gotoligcol(0,0);

        if (finPartie == 1) { // Victoire
            printf("Bravo ! Vous avez terminé le niveau %d !\n", niveauActuel + 1);
            niveauActuel++; // Passage automatique au niveau suivant
        }
        else if (finPartie == 2) { // Défaite
            printf("Dommage, vous avez perdu.\n");
            break; // On sort de la boucle si perdu
        }
        else if (finPartie == 3) { // Quitter
            system("cls");
            gotoligcol(0,0);
            printf("Vous avez quitte le jeu.\n");
            break;
        }
    }

    // Message final si tous les niveaux sont complétés
    if (niveauActuel >= nbNiveaux) {
        system("cls");
        gotoligcol(0,0);
        printf("Vous avez complete **TOUS** les niveaux !\n");
    }

    return 0;
}
