#include "fonction.h"

int main()
{
    srand(time(NULL)); // Initialisation du générateur aléatoire

    char tab[LIG][COL] = {0};   // Tableau principal
    int choix;                  // Choix de l'utilisateur pour le tableau
    int maxCoups = 0;           // Nombre maximum de mouvements autorisés
    int compteurMouvements = 0; // Compteur local des mouvements

    // Affichage du menu
    printf("Choisissez le tableau :\n");
    printf("1 - Tableau 1\n");
    printf("2 - Tableau 2\n");
    printf("3 - Charger la partie precedente\n");
    scanf("%d", &choix);

    system("cls"); // Efface l'écran

    Contrat contrat; // Structure pour gérer les quotas de symboles

    // --------------------- TABLEAU 1 ---------------------
    if(choix == 1)
    {
        int reste = LIG*COL - (20 + 50 + 20); // Calcul des Soleils et Pommes
        contrat.S = reste/2;
        contrat.P = reste - contrat.S;
        contrat.F = 20;
        contrat.O = 50;
        contrat.M = 20;
        maxCoups = 40;

        afficherTableau(tab, 0, 0, contrat);
        printf("Tableau 1 sélectionné.\n");
    }

    // --------------------- TABLEAU 2 ---------------------
    else if(choix == 2)
    {
        contrat.S = 55;
        contrat.P = 35;
        contrat.F = 30;
        contrat.O = 45;
        contrat.M = 0;
        maxCoups = 30;

        afficherTableau(tab, 0, 0, contrat);
        printf("Tableau 2 sélectionné.\n");
    }

    // --------------------- CHARGEMENT ---------------------
else if (choix == 3)
{
    if(!chargerPartie(tab, &compteurMouvements, &maxCoups))
    {
        printf("Impossible de charger la sauvegarde.\n");
        return 0;
    }

    afficherTableauSeul(tab, 0, 0);
    afficherCompteur(compteurMouvements, maxCoups); // Affiche bien le compteur
    gotoligcol(LIG+1, 0);
    printf("Partie chargée.\n");
}


    // --------------------- TEXTE D’AIDE ---------------------
    gotoligcol(LIG+1, 0);
    printf("\nZQSD = se déplacer | ESPACE = échanger | A = sauvegarder | E = quitter");

    // Lancement de la boucle de déplacement et d’échange
    boucleDeplacement(tab, maxCoups, &compteurMouvements);

    gotoligcol(LIG+1, 0);
    return 0;
}
