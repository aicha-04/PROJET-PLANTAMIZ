#include "fonction.h"

//======== Fonctions couleurs et curseur ===================================================================
void Color(int couleurDuTexte,int couleurDeFond) {
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
}

void gotoligcol(int lig, int col) {
    COORD mycoord;
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), mycoord);
}

//======== Affichage tableau ================================================================================
void afficherTableau(char tab[LIG][COL], int curseurL, int curseurC, char nomJoueur[]) {
    char forme[]="SFPOM";
    int nb_aleatoire=0;

    if(tab[0][0] == '\0') {
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

    gotoligcol(0, COL + 2);
    printf("Partie de %s", nomJoueur);
}

//======== Affichage d’une case ===========================================================================
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

//======== Gravité ===========================================================================================
void appliquerGravite(char tab[LIG][COL]) {
    for(int col = 0; col < COL; col++) {
        for(int lig = LIG-1; lig >= 0; lig--) {
            if(tab[lig][col] == ' ') {
                int k = lig - 1;
                while(k >= 0 && tab[k][col] == ' ')
                    k--;
                if(k >= 0) {
                    tab[lig][col] = tab[k][col];
                    tab[k][col] = ' ';
                } else {
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

//======== Compteurs et quotas ===============================================================================
void mettreAJourCompteurElimination(char tab[LIG][COL], int aEliminer[LIG][COL], int compteurElim[5], int quotas[5], char nomJoueur[]) {
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
    printf("Partie de %s\n", nomJoueur);
    gotoligcol(1, COL + 2);
    printf("Eliminations / Quotas :\n");
    for(int k = 0; k < 5; k++) {
        gotoligcol(k + 2, COL + 2);
        printf("%c : %d / %d  ", symbols[k], compteurElim[k], quotas[k]);
    }
}

//======== Victoire ===========================================================================================
int verifierVictoire(int compteurElim[5], int quotas[5]) {
    for(int k = 0; k < 5; k++) {
        if(quotas[k] > 0 && compteurElim[k] < quotas[k])
            return 0;
    }
    return 1;
}

//======== Timer ==============================================================================================
void verifierTimer(time_t debutPartie, int tempsLimite, int *finPartie) {
    if(*finPartie) return;

    time_t maintenant = time(NULL);
    int tempsEcoule = (int)(maintenant - debutPartie);
    int tempsRestant = tempsLimite - tempsEcoule;

    Color(11, 0);
    gotoligcol(7, COL + 2);
    printf("Temps restant : %02d secondes   ", tempsRestant);
    Color(7, 0);

    if(tempsRestant <= 0) {
        *finPartie = 2; // Défaite
        return;
    }
}

//======== Analyse et élimination ===============================================================================
//======== Analyse et élimination ================================================================================================
void analyserEtEliminer(char tab[LIG][COL], int *score, int compteurElim[5], int quotas[5], int *finPartie, char nomJoueur[])
{
    int aEliminer[LIG][COL] = {0};
    int totalPoints = 0;

    // ===== Analyse horizontale =====
    for(int i = 0; i < LIG; i++) {
        int j = 0;
        while(j < COL) {
            int k = j + 1;
            while(k < COL && tab[i][j] == tab[i][k] && tab[i][j] != ' ')
                k++;

            int longueur = k - j;
            if(longueur >= 3) {
                if(longueur == 3) totalPoints += 3;
                else if(longueur == 4) totalPoints += 4;
                else if(longueur >= 6) {
                    char c = tab[i][j];
                    for(int x = 0; x < LIG; x++)
                        for(int y = 0; y < COL; y++)
                            if(tab[x][y] == c) {
                                aEliminer[x][y] = 1;
                                totalPoints++;
                            }
                }
                if(longueur < 6) {
                    for(int x = j; x < k; x++)
                        aEliminer[i][x] = 1;
                }
            }
            j = k;
        }
    }

    // ===== Analyse verticale =====
    for(int j = 0; j < COL; j++) {
        int i = 0;
        while(i < LIG) {
            int k = i + 1;
            while(k < LIG && tab[i][j] == tab[k][j] && tab[i][j] != ' ')
                k++;

            int longueur = k - i;
            if(longueur >= 3) {
                if(longueur == 3) totalPoints += 3;
                else if(longueur == 4) totalPoints += 4;
                else if(longueur >= 6) {
                    char c = tab[i][j];
                    for(int x = 0; x < LIG; x++)
                        for(int y = 0; y < COL; y++)
                            if(tab[x][y] == c) {
                                aEliminer[x][y] = 1;
                                totalPoints++;
                            }
                }
                if(longueur < 6) {
                    for(int x = i; x < k; x++)
                        aEliminer[x][j] = 1;
                }
            }
            i = k;
        }
    }

    // ===== Détection forme H =====
    for(int i = 0; i < LIG; i++) {
        for(int j = 0; j < COL; j++) {
            char caseCentre = tab[i][j];
            if(caseCentre == ' ') continue;

            int gauche = 0, droite = 0;
            int colG = j-1, colD = j+1;

            while(colG >= 0 && tab[i][colG] == caseCentre) { gauche++; colG--; }
            while(colD < COL && tab[i][colD] == caseCentre) { droite++; colD++; }

            if(gauche == droite && gauche > 1) {
                int haut = 0, bas = 0;
                while(i-haut-1 >= 0 && tab[i-haut-1][j-gauche] == caseCentre && tab[i-haut-1][j+droite] == caseCentre) haut++;
                while(i+bas+1 < LIG && tab[i+bas+1][j-gauche] == caseCentre && tab[i+bas+1][j+droite] == caseCentre) bas++;

                if(haut >= 1 && bas <= 1 && haut == bas) {
                    int interieurVide = 1;
                    for(int l = i-haut; l <= i+bas; l++)
                        for(int c = j-gauche+1; c < j+droite; c++)
                            if(tab[l][c] == caseCentre) interieurVide = 0;

                    int extGaucheBien = 1, extDroiteBien = 1;
                    if(j-gauche-1 >= 0 && tab[i][j-gauche-1] == caseCentre) extGaucheBien = 0;
                    if(j+droite+1 < COL && tab[i][j+droite+1] == caseCentre) extDroiteBien = 0;

                    if(interieurVide && extGaucheBien && extDroiteBien) {
                        for(int l = i-haut; l <= i+bas; l++) { aEliminer[l][j-gauche] = 1; aEliminer[l][j+droite] = 1; }
                        for(int c = j-gauche; c <= j+droite; c++) aEliminer[i][c] = 1;

                        int nbItemsH = (haut + bas + 1)*2 + (droite + gauche + 1);
                        totalPoints += 2 * nbItemsH;
                    }
                }
            }
        }
    }

    // ===== Détection rectangle =====
    for(int i = 0; i < LIG; i++) {
        for(int j = 0; j < COL; j++) {
            char caseDebutRect = tab[i][j];
            if(caseDebutRect == ' ') continue;

            for(int largeur = 2; largeur < COL-j; largeur++) {
                for(int hauteur = 2; hauteur < LIG-i; hauteur++) {
                    int coinHG_i = i, coinHG_j = j;
                    int coinHD_i = i, coinHD_j = j+largeur;
                    int coinBG_i = i+hauteur, coinBG_j = j;
                    int coinBD_i = i+hauteur, coinBD_j = j+largeur;

                    int conditionsOK = 1;

                    for(int c = coinHG_j; c <= coinHD_j; c++) if(tab[i][c] != caseDebutRect) conditionsOK = 0;
                    for(int c = coinBG_j; c <= coinBD_j; c++) if(tab[coinBG_i][c] != caseDebutRect) conditionsOK = 0;
                    for(int l = coinHG_i; l <= coinBG_i; l++) if(tab[l][coinHG_j] != caseDebutRect) conditionsOK = 0;
                    for(int l = coinHD_i; l <= coinBD_i; l++) if(tab[l][coinHD_j] != caseDebutRect) conditionsOK = 0;

                    if(coinHG_i-1 >= 0) for(int c = coinHG_j; c <= coinHD_j; c++) if(tab[coinHG_i-1][c] == caseDebutRect) conditionsOK = 0;
                    if(coinBG_i+1 < LIG) for(int c = coinBG_j; c <= coinBD_j; c++) if(tab[coinBG_i+1][c] == caseDebutRect) conditionsOK = 0;
                    if(coinHG_j-1 >= 0) for(int l = coinHG_i; l <= coinBG_i; l++) if(tab[l][coinHG_j-1] == caseDebutRect) conditionsOK = 0;
                    if(coinHD_j+1 < COL) for(int l = coinHD_i; l <= coinBD_i; l++) if(tab[l][coinHD_j+1] == caseDebutRect) conditionsOK = 0;

                    if(conditionsOK) {
                        for(int c = coinHG_j; c <= coinHD_j; c++) { aEliminer[coinHG_i][c] = 1; aEliminer[coinBG_i][c] = 1; }
                        for(int l = coinHG_i; l <= coinBG_i; l++) { aEliminer[l][coinHG_j] = 1; aEliminer[l][coinHD_j] = 1; }

                        int nbPointsRect = 2*((largeur+1)*(hauteur+1));
                        totalPoints += nbPointsRect;
                    }
                }
            }
        }
    }

    // ===== Mise à jour et suppression =====
    mettreAJourCompteurElimination(tab, aEliminer, compteurElim, quotas, nomJoueur);

    int found = 0;
    for(int i = 0; i < LIG; i++)
        for(int j = 0; j < COL; j++)
            if(aEliminer[i][j]) { tab[i][j] = ' '; found = 1; }

    for(int i = 0; i < LIG; i++)
        for(int j = 0; j < COL; j++)
            afficherCase(tab, i, j, 0, 0);

    if(found) appliquerGravite(tab);

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

//======== Sauvegarde ===========================================================================================
int sauvegarderPartie(char tab[LIG][COL], int niveauActuel, int maxCoups, int compteurMouvements,
                      int curseurL, int curseurC, int score, int compteurElim[5],
                      time_t debutPartie, int tempsLimite, char nomJoueur[], int vies) {
    FILE *f = fopen("sauvegarde.txt", "w");
    if(!f) { printf("Impossible de sauvegarder.\n"); return 0; }

    fprintf(f, "%s\n", nomJoueur);
    fprintf(f, "%d %d %d %d %d %d\n", niveauActuel, compteurMouvements, maxCoups, curseurL, curseurC, score);

    for(int k=0; k<5; k++)
        fprintf(f, "%d ", compteurElim[k]);
    fprintf(f, "\n");

    int tempsRestant = tempsLimite - (int)(time(NULL) - debutPartie);
    if(tempsRestant < 0) tempsRestant = 0;
    fprintf(f, "%d\n", tempsRestant);

    fprintf(f, "%d\n", vies); // Sauvegarde les vies

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

//======== Chargement ===========================================================================================
int chargerPartie(char tab[LIG][COL], int *niveauActuel, int *compteurMouvements, int *maxCoups,
                  int *curseurL, int *curseurC, int *score, int compteurElim[5],
                  time_t *debutPartie, int *tempsLimite, int quotas[5], Level niveaux[], char nomJoueur[], int *vies) {
    FILE *f = fopen("sauvegarde.txt","r");
    if(!f) return 0;

    fscanf(f, "%49s", nomJoueur);
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

    fscanf(f, "%d", vies); // Charger les vies

    for(int i=0; i<LIG; i++)
        for(int j=0; j<COL; j++)
            fscanf(f, " %c", &tab[i][j]);

    fclose(f);
    return 1;
}

//======== Boucle déplacement =====================================================================================
void boucleDeplacement(char tab[LIG][COL], int *curseurL, int *curseurC,
                       int *compteurMouvements, int maxCoups, int *score,
                       int *finPartie, int compteurElim[5], int quotas[5],
                       time_t debutPartie, int tempsLimite, int niveauActuel, char nomJoueur[],
                       int *vies) {
    int enDeplacement = 0, posInitL = -1, posInitC = -1;
    char symboleStocke = 0;

    while(1) {
        if(*finPartie) break;

        char c = _getch();
        int ancienL = *curseurL, ancienC = *curseurC;

        verifierTimer(debutPartie, tempsLimite, finPartie);
        if(*finPartie) break;

        int bougeCurseur = 0;

        if(c=='z'||c=='Z'){ if(*curseurL>0) { (*curseurL)--; bougeCurseur = 1; } }
        else if(c=='s'||c=='S'){ if(*curseurL<LIG-1) { (*curseurL)++; bougeCurseur = 1; } }
        else if(c=='q'||c=='Q'){ if(*curseurC>0) { (*curseurC)--; bougeCurseur = 1; } }
        else if(c=='d'||c=='D'){ if(*curseurC<COL-1) { (*curseurC)++; bougeCurseur = 1; } }

        afficherCase(tab, ancienL, ancienC, 0, (enDeplacement && ancienL==posInitL && ancienC==posInitC));
        afficherCase(tab, *curseurL, *curseurC, 1, (enDeplacement && *curseurL==posInitL && *curseurC==posInitC));

        if(bougeCurseur && !enDeplacement) {
            int aEliminer[LIG][COL] = {0};
            mettreAJourCompteurElimination(tab, aEliminer, compteurElim, quotas, nomJoueur);
        }

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
                        *finPartie = 2; // Défaite
                        return;
                    }

                    afficherCase(tab, *curseurL, *curseurC, 1, 0);
                    afficherCase(tab, posInitL, posInitC, 0, 0);

                    analyserEtEliminer(tab, score, compteurElim, quotas, finPartie, nomJoueur);

                    afficherCase(tab, *curseurL, *curseurC, 1, 0);
                }
                enDeplacement = 0;
            }
        }

        if(c=='a'||c=='A')
            sauvegarderPartie(tab, niveauActuel, maxCoups, *compteurMouvements,
                              *curseurL, *curseurC, *score, compteurElim,
                              debutPartie, tempsLimite, nomJoueur, *vies);
        if (c == 'E' || c == 'e') {
            *finPartie = 3;
            return;
        }
    }
}
