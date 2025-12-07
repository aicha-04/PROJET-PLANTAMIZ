#include "fonction.h"

//======== Fonctions couleurs et curseur ===========================================================================================

void Color(int couleurDuTexte,int couleurDeFond)
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
}

void gotoligcol(int lig, int col)
{
    COORD mycoord;
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), mycoord);
}

//======== Affichage tableau =======================================================================================================
void afficherTableau(char tab[LIG][COL], int curseurL, int curseurC) {
    char forme[]="SFPOM";
    int nb_aleatoire=0;

    if (tab[0][0] == '\0') {
        for(int lig=0; lig<LIG; lig++) {
            for(int col=0; col<COL; col++) {
                do {
                    nb_aleatoire = rand()%5;
                    tab[lig][col] = forme[nb_aleatoire];
                } while((lig>=2 && tab[lig][col]==tab[lig-2][col]) || (col>=2 && tab[lig][col]==tab[lig][col-2]));
            }
        }
    }

    for(int lig = 0; lig < LIG; lig++) {
        for(int col = 0; col < COL; col++) {
            if(lig == curseurL && col == curseurC)
                Color(0, 15);
            else if(tab[lig][col]=='S') Color(14,0);
            else if(tab[lig][col]=='F') Color(12,0);
            else if(tab[lig][col]=='P') Color(6,0);
            else if(tab[lig][col]=='O') Color(5,0);
            else if(tab[lig][col]=='M') Color(9,0);

            printf("%c", tab[lig][col]);
            Color(7,0);
        }
        printf("\n");
    }
}

//======== Affichage d’une case ===========================================================================================
void afficherCase(char tab[LIG][COL], int lig, int col, int surligner, int selection) {
    gotoligcol(lig, col);

    if(selection) Color(0, 14);
    else if(surligner) Color(0, 15);
    else if(tab[lig][col]=='S') Color(14,0);
    else if(tab[lig][col]=='F') Color(12,0);
    else if(tab[lig][col]=='P') Color(6,0);
    else if(tab[lig][col]=='O') Color(5,0);
    else if(tab[lig][col]=='M') Color(9,0);

    printf("%c", tab[lig][col]);
    Color(7,0);
}

//======== Vérification fin de partie ===========================================================================================
void verifierFinPartie(int *compteurMouvements, int maxCoups, int *finPartie) {
    (*compteurMouvements)++;
    Color(11,0);
    gotoligcol(LIG+1,0);
    printf("Mouvements : %d / %d\n", *compteurMouvements, maxCoups);
    Color(7,0);

    if(*compteurMouvements >= maxCoups) {
        *finPartie = 1;
        Color(12,0);
        system("cls");
        gotoligcol(LIG+2,0);
        printf("FIN DE PARTIE !\n");
        Color(7,0);
        gotoligcol(LIG+3,0);
        _getch();
        return;
    }
}

//======== Gravité ==============================================================================================================
void appliquerGravite(char tab[LIG][COL])
{
    for(int col = 0; col < COL; col++)
    {
        for(int lig = LIG-1; lig >= 0; lig--)
        {
            if(tab[lig][col] == ' ')
            {
                int k = lig - 1;
                while(k >= 0 && tab[k][col] == ' ')
                    k--;

                if(k >= 0)
                {
                    tab[lig][col] = tab[k][col];
                    tab[k][col] = ' ';
                }
                else
                {
                    char forme[] = "SFPOM";
                    tab[lig][col] = forme[rand() % 5];
                }
            }
        }
    }

    for(int i = 0; i < LIG; i++)
        for(int j = 0; j < COL; j++)
            afficherCase(tab, i, j, 0, 0);
}

void mettreAJourCompteurElimination(char tab[LIG][COL], int aEliminer[LIG][COL], int compteurElim[5], int quotas[5]) {
    char symbols[] = "SFPOM";

    for(int i = 0; i < LIG; i++) {
        for(int j = 0; j < COL; j++) {
            if(aEliminer[i][j]) {
                for(int k = 0; k < 5; k++) {
                    if(tab[i][j] == symbols[k]) {
                        compteurElim[k]++;
                        break;
                    }
                }
            }
        }
    }
    Color(10, 0);
    gotoligcol(0, COL + 2);
    printf("Eliminations / Quotas :\n");
    for(int k = 0; k < 5; k++) {
        gotoligcol(k + 1, COL + 2);
        printf("%c : %d / %d  ", symbols[k], compteurElim[k], quotas[k]);
    }
}

int verifierVictoire(int compteurElim[5], int quotas[5]) {
    for(int k = 0; k < 5; k++) {
        if(quotas[k] > 0 && compteurElim[k] < quotas[k])
            return 0;
    }
    return 1;
}

void verifierTimer(time_t debutPartie, int tempsLimite, int *finPartie)
{
    if(*finPartie) return;

    time_t maintenant = time(NULL);
    int tempsEcoule = (int)(maintenant - debutPartie);
    int tempsRestant = tempsLimite - tempsEcoule;

    Color(11, 0);
    gotoligcol(7, COL + 2);
    printf("Temps restant : %02d secondes   ", tempsRestant);
    Color(7, 0);

    if(tempsRestant <= 0) {
        *finPartie = 1;

        Color(12, 0);
        system("cls");
        printf("\n\n==== TEMPS ÉCOULÉ ====\n");
        printf("La partie est terminée !\n");
        Color(7, 0);
        _getch();
        return;
    }
}

//======== Analyse et élimination ================================================================================================
void analyserEtEliminer(char tab[LIG][COL], int *score, int compteurElim[5], int quotas[5], int *finPartie)
{
    int aEliminer[LIG][COL] = {0};
    int totalPoints = 0;

    for(int i = 0; i < LIG; i++) {
        int j = 0;
        while(j < COL) {
            int k = j + 1;
            while(k < COL && tab[i][j] == tab[i][k] && tab[i][j] != ' ')
                k++;

            int longueur = k - j;
            if(longueur >= 3) {
                totalPoints += longueur;
                for(int x = j; x < k; x++)
                    aEliminer[i][x] = 1;
            }
            j = k;
        }
    }

    for(int j = 0; j < COL; j++) {
        int i = 0;
        while(i < LIG) {
            int k = i + 1;
            while(k < LIG && tab[i][j] == tab[k][j] && tab[i][j] != ' ')
                k++;

            int longueur = k - i;
            if(longueur >= 3) {
                totalPoints += longueur;
                for(int x = i; x < k; x++)
                    aEliminer[x][j] = 1;
            }
            i = k;
        }
    }

    mettreAJourCompteurElimination(tab, aEliminer, compteurElim, quotas);

    int found = 0;
    for(int i = 0; i < LIG; i++) {
        for(int j = 0; j < COL; j++) {
            if(aEliminer[i][j]) {
                tab[i][j] = ' ';
                found = 1;
            }
        }
    }

    for(int i = 0; i < LIG; i++)
        for(int j = 0; j < COL; j++)
            afficherCase(tab, i, j, 0, 0);

    if(found)
        appliquerGravite(tab);

    if(totalPoints > 0) {
        *score += totalPoints;
        gotoligcol(9, COL + 2);
        Color(11,0);
        printf("Points : %d\n", *score);
        Color(7,0);
    }

    if(verifierVictoire(compteurElim, quotas)) {
        *finPartie = 1;
        Color(10,0);
        gotoligcol(LIG + 4, 0);
        printf("VOUS AVEZ GAGNE !\n");
        Color(7,0);
    }
}

//======== Sauvegarde ================================================================================================
int sauvegarderPartie(char tab[LIG][COL], int niveauActuel, int maxCoups, int compteurMouvements,
                      int curseurL, int curseurC, int score, int compteurElim[5],
                      time_t debutPartie, int tempsLimite) {
    FILE *f = fopen("sauvegarde.txt", "w");
    if(!f) { printf("Impossible de sauvegarder.\n"); return 0; }

    fprintf(f, "%d %d %d %d %d %d\n", niveauActuel, compteurMouvements, maxCoups, curseurL, curseurC, score);

    for(int k=0; k<5; k++)
        fprintf(f, "%d ", compteurElim[k]);
    fprintf(f, "\n");

    int tempsRestant = tempsLimite - (int)(time(NULL) - debutPartie);
    if(tempsRestant < 0) tempsRestant = 0;
    fprintf(f, "%d\n", tempsRestant);

    for(int i=0; i<LIG; i++){
        for(int j=0; j<COL; j++)
            fprintf(f,"%c ", tab[i][j]);
        fprintf(f,"\n");
    }

    fclose(f);

    gotoligcol(LIG+2, 0);
    printf("Sauvegarde réussie !\n");
    return 1;
}

//======== Chargement ================================================================================================
int chargerPartie(char tab[LIG][COL], int *niveauActuel, int *compteurMouvements, int *maxCoups,
                  int *curseurL, int *curseurC, int *score, int compteurElim[5],
                  time_t *debutPartie, int *tempsLimite, int quotas[5], Level niveaux[])
{
    FILE *f = fopen("sauvegarde.txt","r");
    if(!f) return 0;

    fscanf(f,"%d %d %d %d %d %d", niveauActuel, compteurMouvements, maxCoups, curseurL, curseurC, score);

    for(int k=0; k<5; k++)
        fscanf(f, "%d", &compteurElim[k]);

    int tempsRestant;
    fscanf(f, "%d", &tempsRestant);
    if(tempsRestant < 0) tempsRestant = 0;

    *tempsLimite = niveaux[*niveauActuel].tempsLimite;
    for(int k=0; k<5; k++)
        quotas[k] = niveaux[*niveauActuel].quotas[k];

    *debutPartie = time(NULL) - (*tempsLimite - tempsRestant);

    for(int i=0; i<LIG; i++)
        for(int j=0; j<COL; j++)
            fscanf(f, " %c", &tab[i][j]);

    fclose(f);
    return 1;
}

//======== Boucle déplacement ================================================================================================
void boucleDeplacement(char tab[LIG][COL], int *curseurL, int *curseurC,
                       int *compteurMouvements, int maxCoups, int *score,
                       int *finPartie, int compteurElim[5], int quotas[5],
                       time_t debutPartie, int tempsLimite, int niveauActuel)
{
    int enDeplacement = 0, posInitL = -1, posInitC = -1;
    char symboleStocke = 0;

    while(1) {
        if(*finPartie) break;

        char c = _getch();
        int ancienL = *curseurL, ancienC = *curseurC;

        verifierTimer(debutPartie, tempsLimite, finPartie);
        if(*finPartie) break;

        if(c=='z'||c=='Z'){ if(*curseurL>0) (*curseurL)--; }
        else if(c=='s'||c=='S'){ if(*curseurL<LIG-1) (*curseurL)++; }
        else if(c=='q'||c=='Q'){ if(*curseurC>0) (*curseurC)--; }
        else if(c=='d'||c=='D'){ if(*curseurC<COL-1) (*curseurC)++; }

        afficherCase(tab, ancienL, ancienC, 0, (enDeplacement && ancienL==posInitL && ancienC==posInitC));
        afficherCase(tab, *curseurL, *curseurC, 1, (enDeplacement && *curseurL==posInitL && *curseurC==posInitC));

        if(c==' ') {
            if(!enDeplacement) {
                posInitL = *curseurL;
                posInitC = *curseurC;
                symboleStocke = tab[posInitL][posInitC];
                enDeplacement = 1;
                afficherCase(tab, *curseurL, *curseurC, 0, 1);
            } else {
                int distL = abs(*curseurL - posInitL);
                int distC = abs(*curseurC - posInitC);
                if((distL + distC) == 1) {
                    char tmp = tab[*curseurL][*curseurC];
                    tab[*curseurL][*curseurC] = symboleStocke;
                    tab[posInitL][posInitC] = tmp;

                    (*compteurMouvements)++;
                    gotoligcol(8, COL + 2);
                    Color(11,0);
                    printf("Mouvements : %d / %d  ", *compteurMouvements, maxCoups);
                    Color(7,0);

                    if(*compteurMouvements >= maxCoups) {
                        *finPartie = 1;
                        Color(12,0);
                        gotoligcol(LIG+2,0);
                        printf("FIN DE PARTIE !\n");
                        Color(7,0);
                        gotoligcol(LIG+3,0);
                        _getch();
                        return;
                        break;
                    }

                    afficherCase(tab, *curseurL, *curseurC, 1, 0);
                    afficherCase(tab, posInitL, posInitC, 0, 0);

                    analyserEtEliminer(tab, score, compteurElim, quotas, finPartie);

                    afficherCase(tab, *curseurL, *curseurC, 1, 0);
                }
                enDeplacement = 0;
            }
        }

        if(c=='a'||c=='A')
            sauvegarderPartie(tab, niveauActuel, maxCoups, *compteurMouvements, *curseurL, *curseurC, *score, compteurElim, debutPartie, tempsLimite);
        if (c == 'E' || c == 'e') {
            *finPartie = 3; // Signal pour quitter
        return;
        }

    }
}
