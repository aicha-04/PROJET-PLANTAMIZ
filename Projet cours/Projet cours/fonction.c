#include "fonction.h"

//-------------------------------------------------------------
// Change la couleur du texte et du fond dans la console
//-------------------------------------------------------------
void Color(int couleurDuTexte,int couleurDeFond)
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,couleurDuTexte + 16*couleurDeFond);
}

//-------------------------------------------------------------
// Déplace le curseur de la console à une position donnée
//-------------------------------------------------------------
void gotoligcol(int lig, int col)
{
    COORD mycoord;
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), mycoord);
}

//-------------------------------------------------------------
// Génère et affiche le tableau complet avec quotas de symboles
//-------------------------------------------------------------
void afficherTableau(char tab[LIG][COL], int curseurL, int curseurC, Contrat contrat)
{
    char forme[] = "SFPOM";
    int quotas[5] = {contrat.S, contrat.F, contrat.P, contrat.O, contrat.M};

    // Génération du tableau
    for(int lig=0; lig<LIG; lig++)
    {
        for(int col=0; col<COL; col++)
        {
            char choisi = 'S';
            for(int t=0; t<100; t++)
            {
                int r = rand()%5;
                if(quotas[r]<=0) continue;
                if(lig>=2 && tab[lig-2][col]==forme[r]) continue;
                if(col>=2 && tab[lig][col-2]==forme[r]) continue;
                choisi = forme[r];
                quotas[r]--;
                break;
            }
            tab[lig][col] = choisi;
        }
    }

    // Affichage du tableau
    for(int i=0; i<LIG; i++)
    {
        for(int j=0; j<COL; j++)
        {
            if(i==curseurL && j==curseurC) Color(0,15);
            else if(tab[i][j]=='S') Color(14,0);
            else if(tab[i][j]=='F') Color(12,0);
            else if(tab[i][j]=='P') Color(6,0);
            else if(tab[i][j]=='O') Color(5,0);
            else if(tab[i][j]=='M') Color(9,0);

            printf("%c", tab[i][j]);
            Color(7,0);
        }
        printf("\n");
    }
}

//-------------------------------------------------------------
// Affiche UNE SEULE case (optimisé pour déplacement)
//-------------------------------------------------------------
void afficherCase(char tab[LIG][COL], int lig, int col, int surligner, int selection)
{
    gotoligcol(lig, col);

    if(selection) Color(0,14);
    else if(surligner) Color(0,15);
    else if(tab[lig][col]=='S') Color(14,0);
    else if(tab[lig][col]=='F') Color(12,0);
    else if(tab[lig][col]=='P') Color(6,0);
    else if(tab[lig][col]=='O') Color(5,0);
    else if(tab[lig][col]=='M') Color(9,0);

    printf("%c", tab[lig][col]);
    Color(7,0);
}

//-------------------------------------------------------------
// Sauvegarde complète du jeu
//-------------------------------------------------------------
int sauvegarderPartie(char tab[LIG][COL], int maxCoups, int compteurMouvements)
{
    FILE *f = fopen("sauvegarde.txt","w");
    if(!f) { printf("Impossible de sauvegarder.\n"); return 0; }

    // Sauvegarde du compteur et maxCoups
    fprintf(f,"%d %d\n", compteurMouvements, maxCoups);

    // Sauvegarde du tableau
    for(int i=0;i<LIG;i++)
    {
        for(int j=0;j<COL;j++) fprintf(f,"%c ",tab[i][j]);
        fprintf(f,"\n");
    }

    fclose(f);
    printf("Sauvegarde réussie !\n");
    return 1;
}

//-------------------------------------------------------------
// Charge une partie sauvegardée
//-------------------------------------------------------------
int chargerPartie(char tab[LIG][COL], int *compteurMouvements, int *maxCoups)
{
    FILE *f = fopen("sauvegarde.txt","r");
    if(!f) return 0;

    // Lecture compteur et maxCoups
    fscanf(f,"%d %d", compteurMouvements, maxCoups);

    // Lecture du tableau
    for(int i=0;i<LIG;i++)
        for(int j=0;j<COL;j++) fscanf(f," %c",&tab[i][j]);

    fclose(f);
    return 1;
}


//-------------------------------------------------------------
// Compteur de mouvements et retour si limite atteinte
//-------------------------------------------------------------
int compteurMouvement(int maxCoups, int *compteurMouvements)
{
    (*compteurMouvements)++;
    gotoligcol(LIG+1,0);
    Color(11,0);
    printf("Mouvements : %d / %d\n", *compteurMouvements, maxCoups);
    Color(7,0);
    return (*compteurMouvements >= maxCoups);
}

void afficherTableauSeul(char tab[LIG][COL], int curseurL, int curseurC)
{
    for(int i=0; i<LIG; i++)
    {
        for(int j=0; j<COL; j++)
        {
            if(i==curseurL && j==curseurC) Color(0,15);
            else if(tab[i][j]=='S') Color(14,0);
            else if(tab[i][j]=='F') Color(12,0);
            else if(tab[i][j]=='P') Color(6,0);
            else if(tab[i][j]=='O') Color(5,0);
            else if(tab[i][j]=='M') Color(9,0);

            printf("%c", tab[i][j]);
            Color(7,0);
        }
        printf("\n");
    }
}

void afficherCompteur(int compteurMouvements, int maxCoups)
{
    gotoligcol(LIG+2, 0);
    Color(11,0);
    printf("Mouvements : %d / %d\n", compteurMouvements, maxCoups);
    Color(7,0);
}


//-------------------------------------------------------------
// Boucle de déplacement / échange / sauvegarde
//-------------------------------------------------------------
void boucleDeplacement(char tab[LIG][COL], int maxCoups, int *compteurMouvements)
{
    int curseurL=0, curseurC=0;
    int enDeplacement=0, posInitL=-1, posInitC=-1;
    char symboleStocke=0;

    while(1)
    {
        char c=_getch();
        int oldL=curseurL, oldC=curseurC;

        if(c=='z'||c=='Z') if(curseurL>0) curseurL--;
        if(c=='s'||c=='S') if(curseurL<LIG-1) curseurL++;
        if(c=='q'||c=='Q') if(curseurC>0) curseurC--;
        if(c=='d'||c=='D') if(curseurC<COL-1) curseurC++;

        afficherCase(tab, oldL, oldC, 0, (enDeplacement && oldL==posInitL && oldC==posInitC));
        afficherCase(tab, curseurL, curseurC, 1, (enDeplacement && curseurL==posInitL && curseurC==posInitC));

        if(c==' ')
        {
            if(!enDeplacement)
            {
                enDeplacement=1;
                posInitL=curseurL;
                posInitC=curseurC;
                symboleStocke=tab[curseurL][curseurC];
                afficherCase(tab, curseurL, curseurC,0,1);
            }
            else
            {
                int d=abs(curseurL-posInitL)+abs(curseurC-posInitC);
                if(d==1)
                {
                    char tmp=tab[curseurL][curseurC];
                    tab[curseurL][curseurC]=symboleStocke;
                    tab[posInitL][posInitC]=tmp;

                    if(compteurMouvement(maxCoups, compteurMouvements))
                    {
                        gotoligcol(LIG+2,0);
                        printf("Nombre de coups maximum atteint !\n");
                        break;
                    }

                    afficherCase(tab, curseurL, curseurC,1,0);
                    afficherCase(tab,posInitL,posInitC,0,0);
                }
                enDeplacement=0;
            }
        }

        if(c=='a'||c=='A') sauvegarderPartie(tab,maxCoups,*compteurMouvements);
        if(c=='e'||c=='E') break;
        // dans boucle deplavement rajouter sur la derniere ligne parceque c'est apres un mouvement qu'il faudra analyser les position des items et éliminer
        analyserEtEliminer(tab);

    }




}



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



void analyserEtEliminer(char tab[LIG][COL])
{
    int aEliminer[LIG][COL] = {0};  // Quand une case sera a supprimer on mettra 1
    int totalPoints = 0; // Donne le nombre de points liés aux éliminations

    // ========== 1) GROUPES HORIZONTAUX ==========
    for(int i = 0; i < LIG; i++) // on parcourt chaque ligne
    {
        int j = 0;
        while(j < COL) // on parcourt les colonnes
        {
            int k = j + 1; // K est une colonne apres celle ou on se trouve
            while(k < COL && tab[i][j] == tab[i][k] && tab[i][j] != ' ') // tannt qu'on est dans le grille, que les deux cases sont identiques et pas vides
                k++; // On continue d'avancer

            int longueur = k - j;

            if(longueur >= 3)
            {
                if(longueur == 3) totalPoints += 3;
                else if(longueur == 4) totalPoints += 4;
                else if(longueur >= 6)
                {
                    char c = tab[i][j];
                    for(int x = 0; x < LIG; x++)
                        for(int y = 0; y < COL; y++)
                            if(tab[x][y] == c) // sur chaque case avec le meme charactère
                            {
                                aEliminer[x][y] = 1; // on élimine
                                totalPoints++; // pour chaque case ou c'est le cas on prend un point en plus
                            }
                }

                if(longueur < 6)
                {
                    for(int x = j; x < k; x++)
                        aEliminer[i][x] = 1; // On élimine partout ou longeur est inférieur à 6
                }
            }

            j = k;
        }
    }

    // ========== 2) GROUPES VERTICAUX ==========
    for(int j = 0; j < COL; j++)
    {
        int i = 0;
        while(i < LIG)
        {
            int k = i + 1;
            while(k < LIG && tab[i][j] == tab[k][j] && tab[i][j] != ' ')
                k++;

            int longueur = k - i;

            if(longueur >= 3)
            {
                if(longueur == 3) totalPoints += 3;
                else if(longueur == 4) totalPoints += 4;
                else if(longueur >= 6)
                {
                    char c = tab[i][j];
                    for(int x = 0; x < LIG; x++)
                        for(int y = 0; y < COL; y++)
                            if(tab[x][y] == c)
                            {
                                aEliminer[x][y] = 1;
                                totalPoints++;
                            }
                }

                if(longueur < 6)
                {
                    for(int x = i; x < k; x++)
                        aEliminer[x][j] = 1;
                }
            }

            i = k;
        }
    }

    // ========== 3) SUPPRESSION DES ITEMS ==========

    int found = 0;

    for(int i = 0; i < LIG; i++)
    {
        for(int j = 0; j < COL; j++)
        {
            if(aEliminer[i][j]) //si on doit eliminer une case 1
            {
                tab[i][j] = ' '; // on remplace la case par rien
                found = 1;
            }
        }
    }

    // Mise à jour affichage pour voir les cases supprimées
    for(int i = 0; i < LIG; i++)
        for(int j = 0; j < COL; j++)
            afficherCase(tab, i, j, 0, 0);

    // ========== 4) GRAVITÉ SI NÉCESSAIRE ==========
    if(found) //Si found=1 on apelle lafonction gravité
    {
        appliquerGravite(tab);
    }

    // ========== 5) AFFICHAGE DES POINTS ==========

    if(totalPoints > 0) // affiche le nombre de points de la partie
    {
        gotoligcol(LIG+2, 0);
        Color(11,0);

        printf("Points : %d\n", totalPoints);
        Color(7,0);
    }
}

