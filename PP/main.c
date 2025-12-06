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

    // === Si tab est vide (nouvelle partie) on génère, sinon on laisse le tableau tel quel (chargé) ===
    if (tab[0][0] == '\0') {
        for(int lig=0; lig<LIG; lig++) {
            for(int col=0; col<COL; col++) {
                // Génération aléatoire avec contraintes
                do {
                    nb_aleatoire = rand()%5;
                    tab[lig][col] = forme[nb_aleatoire];
                } while((lig>=2 && tab[lig][col]==tab[lig-2][col]) || (col>=2 && tab[lig][col]==tab[lig][col-2]));
            }
        }
    }

    // Affichage (dans tous les cas)
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
        printf("Appuyez sur une touche pour continuer...");
        _getch();
    }
}

//======== Gravité ==============================================================================================================
void appliquerGravite(char tab[LIG][COL])
{
    for(int col = 0; col < COL; col++)    //On va de colonne en collonne
    {
        for(int lig = LIG-1; lig >= 0; lig--)   // sur chaque colonne on prend une ligne en commencant par la dernière
        {
            if(tab[lig][col] == ' ')  // si on a une case vide
            {
                int k = lig - 1;     //k=numéro de la ligne d'avant là ou on a la case vide
                // Cherche un item au-dessus
                while(k >= 0 && tab[k][col] == ' ')  // il décrement k jusqu'a ce qu'il trouve la première case non vide
                    k--;

                if(k >= 0)
                {
                    tab[lig][col] = tab[k][col]; // on fait tomber la case d'avant
                    tab[k][col] = ' ';           // nouvelle case vide
                }
                else   // Si on a rien au dessus
                {
                    // plus rien au-dessus → on régénère un item aléatoire
                    char forme[] = "SFPOM";
                    tab[lig][col] = forme[rand() % 5];
                }
            }
        }
    }

    // Réaffichage du tableau entier après gravité
    for(int i = 0; i < LIG; i++)
        for(int j = 0; j < COL; j++)
            afficherCase(tab, i, j, 0, 0);
}



//======== Analyse et élimination ================================================================================================

//======== Analyse et élimination ================================================================================================
void analyserEtEliminer(char tab[LIG][COL], int *score)
{
    int aEliminer[LIG][COL] = {0};  // Tableau pour marquer les cases à supprimer : 1 = à éliminer, 0 = garder
    int totalPoints = 0;             // Compteur de points pour cette élimination

    // ========== 1) GROUPES HORIZONTAUX ==========
    for(int i = 0; i < LIG; i++) // Parcours de chaque ligne
    {
        int j = 0;
        while(j < COL) // Parcours des colonnes de la ligne
        {
            int k = j + 1;
            // Cherche jusqu'où le même symbole se répète horizontalement
            while(k < COL && tab[i][j] == tab[i][k] && tab[i][j] != ' ')
                k++;

            int longueur = k - j; // Longueur du groupe identique

            if(longueur >= 3) // Si le groupe contient 3 cases ou plus
            {
                totalPoints += longueur; // 1 point par case éliminée
                for(int x = j; x < k; x++)
                    aEliminer[i][x] = 1; // Marque toutes les cases du groupe pour suppression
            }

            j = k; // Passe à la prochaine séquence
        }
    }

    // ========== 2) GROUPES VERTICAUX ==========
    for(int j = 0; j < COL; j++) // Parcours de chaque colonne
    {
        int i = 0;
        while(i < LIG) // Parcours des lignes de la colonne
        {
            int k = i + 1;
            // Cherche jusqu'où le même symbole se répète verticalement
            while(k < LIG && tab[i][j] == tab[k][j] && tab[i][j] != ' ')
                k++;

            int longueur = k - i; // Longueur du groupe identique

            if(longueur >= 3) // Si le groupe contient 3 cases ou plus
            {
                totalPoints += longueur; // 1 point par case éliminée
                for(int x = i; x < k; x++)
                    aEliminer[x][j] = 1; // Marque toutes les cases du groupe pour suppression
            }

            i = k; // Passe à la prochaine séquence
        }
    }

    // ========== 3) SUPPRESSION DES ITEMS ==========
    int found = 0; // Indique si au moins une case a été éliminée
    for(int i = 0; i < LIG; i++)
    {
        for(int j = 0; j < COL; j++)
        {
            if(aEliminer[i][j]) // Si la case est marquée
            {
                tab[i][j] = ' '; // On vide la case
                found = 1;       // On note qu'une suppression a eu lieu
            }
        }
    }

    // Réaffichage de toutes les cases après suppression
    for(int i = 0; i < LIG; i++)
        for(int j = 0; j < COL; j++)
            afficherCase(tab, i, j, 0, 0);

    // ========== 4) GRAVITÉ SI NÉCESSAIRE ==========
    if(found) // Si au moins une case a été supprimée
        appliquerGravite(tab); // On fait tomber les cases au-dessus des vides

    // ========== 5) MISE À JOUR ET AFFICHAGE DES POINTS ==========
    if(totalPoints > 0) // Si des points ont été gagnés
    {
        *score += totalPoints;       // Cumule les points dans la variable globale
        gotoligcol(LIG+2, 0);       // Position du curseur pour afficher les points
        Color(11,0);                 // Couleur du texte
        printf("Points : %d\n", *score);
        Color(7,0);                  // Remise de la couleur par défaut
    }
}


//======== Sauvegarde ================================================================================================
int sauvegarderPartie(char tab[LIG][COL], int maxCoups, int compteurMouvements, int curseurL, int curseurC, int score) {
    FILE *f = fopen("sauvegarde.txt","w");
    if(!f){printf("Impossible de sauvegarder.\n"); return 0;}

    // Sauvegarde du compteur et maxCoups
    fprintf(f,"%d %d %d %d %d\n", compteurMouvements, maxCoups, curseurL, curseurC, score);

    for(int i=0;i<LIG;i++){
        for(int j=0;j<COL;j++) fprintf(f,"%c ",tab[i][j]);
        fprintf(f,"\n");
    }

    fclose(f);

    gotoligcol(LIG+2, 0);
    printf("Sauvegarde réussie !\n");
    return 1;
}

//======== Chargement ================================================================================================
int chargerPartie(char tab[LIG][COL], int *compteurMouvements, int *maxCoups,int *curseurL, int *curseurC, int *score)
{
    FILE *f = fopen("sauvegarde.txt","r");
    if(!f) return 0;

    // Lecture des 5 valeurs sauvegardées
    fscanf(f,"%d %d %d %d %d",
           compteurMouvements,
           maxCoups,
           curseurL,
           curseurC,
           score);

    // Lecture du tableau
    for(int i = 0; i < LIG; i++)
        for(int j = 0; j < COL; j++)
            fscanf(f, " %c", &tab[i][j]); // espace avant %c pour ignorer les retours à la ligne

    fclose(f);
    return 1;
}

//======== Boucle déplacement ================================================================================================
void boucleDeplacement(char tab[LIG][COL], int *curseurL, int *curseurC,
                       int *compteurMouvements, int maxCoups, int *score, int *finPartie) {
    int enDeplacement = 0, posInitL = -1, posInitC = -1;
    char symboleStocke = 0;

    while(1) {
        if(*finPartie) break;
        char c = _getch();
        int ancienL = *curseurL, ancienC = *curseurC;

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

                    verifierFinPartie(compteurMouvements, maxCoups, finPartie);

                    afficherCase(tab, *curseurL, *curseurC, 1, 0);
                    afficherCase(tab, posInitL, posInitC, 0, 0);

                analyserEtEliminer(tab, score);

                    afficherCase(tab, *curseurL, *curseurC, 1, 0);
                }
                enDeplacement = 0;
            }
        }

        if(c=='a'||c=='A') sauvegarderPartie(tab, maxCoups, *compteurMouvements, *curseurL, *curseurC, *score);
        else if(c=='e'||c=='E') break;
    }
}

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

    boucleDeplacement(tab, &curseurL, &curseurC, &compteurMouvements, maxCoups, &score, &finPartie);

    gotoligcol(LIG+3, 0);
    return 0;
}
