#include "fonction.h"
#include <locale.h>

int main() {
    SetConsoleOutputCP(65001);
    system("cls"); // Nettoie l'écran



    srand(time(NULL));

    char tab[LIG][COL] = {0};
    char nomJoueur[50] = "";
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
    int vies = 5; // Nombre de vies global

    Level niveaux[] = {
        { {0, 20, 0, 50, 20}, 40, 300 },
        { {20, 20, 30, 50, 20}, 45, 300  },
        { {50, 50, 50, 50, 50}, 35, 300 }
    };
    int nbNiveaux = sizeof(niveaux) / sizeof(niveaux[0]);
    int niveauActuel = 0;
    printf("✨✨✨ BIENVENUE DANS LE SUPER JEU ✨✨✨");
    do {
        printf("Que voulez vous faire :\n");
        printf("ATTENTION, Avant n'oubliez pas de Sauvegarder la partie \n");
        printf("1 - Commencer une nouvelle Partie \n");
        printf("2 - Charger la partie precedente\n");
        scanf("%d", &choix);
    } while (choix != 1 && choix != 2);

    system("cls");

    int partieChargee = 0;
    if (choix == 1) {
        printf("Veuillez entrer votre prenom : ");
        scanf("%49s", nomJoueur);
    }

    if (choix == 2) {
        if (!chargerPartie(tab, &niveauActuel, &compteurMouvements, &maxCoups,
                  &curseurL, &curseurC, &score, compteurElim, &debutPartie, &tempsLimite, quotas, niveaux, nomJoueur, &vies)) {
            printf("Impossible de charger la sauvegarde.\n");
            return 0;
        } else {
            partieChargee = 1;
        }
    }

    while (niveauActuel < nbNiveaux) {
        if (!partieChargee) {
            memset(tab, 0, sizeof(tab));
            curseurL = 0; curseurC = 0;
            compteurMouvements = 0; score = 0; finPartie = 0;
            for(int i = 0; i < 5; i++) compteurElim[i] = 0;
            for(int i = 0; i < 5; i++) quotas[i] = niveaux[niveauActuel].quotas[i];
            maxCoups = niveaux[niveauActuel].maxCoups;
            tempsLimite = niveaux[niveauActuel].tempsLimite;
            debutPartie = time(NULL);
        } else partieChargee = 0;

        system("cls");
        afficherTableau(tab, curseurL, curseurC, nomJoueur);
        gotoligcol(LIG+1,0);
        printf("Niveau : %d | Vies : %d\n", niveauActuel+1, vies);
        printf("ZQSD = se deplacer | ESPACE = echanger | A = sauvegarder | E = quitter");

        boucleDeplacement(tab, &curseurL, &curseurC,
                          &compteurMouvements, maxCoups, &score,
                          &finPartie, compteurElim, quotas,
                          debutPartie, tempsLimite, niveauActuel, nomJoueur,
                          &vies);

        system("cls");
        gotoligcol(0,0);

        if (finPartie == 1) {
            printf("Bravo ! Vous avez terminé le niveau %d !\n", niveauActuel + 1);
            niveauActuel++;
        } else if (finPartie == 2) {
            vies--;
            if(vies <= 0) {
                printf("Vous avez perdu toutes vos vies. Game Over !\n");
                break;
            } else {
                printf("Vous avez perdu une vie ! Vies restantes : %d\n", vies);
                printf("Voulez-vous : \n1 - Recommencer le niveau\n2 - Quitter le jeu\n");
                int choixVie;
                scanf("%d", &choixVie);
                if(choixVie == 1) {
                    memset(tab, 0, sizeof(tab));
                    curseurL = curseurC = compteurMouvements = score = 0;
                    finPartie = 0;
                    for(int i=0;i<5;i++) compteurElim[i]=0;
                    for(int i=0;i<5;i++) quotas[i]=niveaux[niveauActuel].quotas[i];
                    maxCoups = niveaux[niveauActuel].maxCoups;
                    tempsLimite = niveaux[niveauActuel].tempsLimite;
                    debutPartie = time(NULL);
                    system("cls");
                    afficherTableau(tab, curseurL, curseurC, nomJoueur);
                    gotoligcol(LIG+1,0);
                    printf("Niveau : %d | Vies : %d\n", niveauActuel+1, vies);
                    printf("ZQSD = se deplacer | ESPACE = echanger | A = sauvegarder | E = quitter");
                    boucleDeplacement(tab, &curseurL, &curseurC,
                                      &compteurMouvements, maxCoups, &score,
                                      &finPartie, compteurElim, quotas,
                                      debutPartie, tempsLimite, niveauActuel, nomJoueur,
                                      &vies);
                    continue;
                } else break;
            }
        } else if (finPartie == 3) {
            system("cls");
            gotoligcol(0,0);
            printf("Vous avez quitte le jeu.\n");
            break;
        }
    }

    if (niveauActuel >= nbNiveaux) {
        system("cls");
        gotoligcol(0,0);
        printf("Vous avez complete **TOUS** les niveaux !\n");
    }

    return 0;
}
