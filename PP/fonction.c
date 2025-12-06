#include "fonction.h"
int compteurMouvements = 0; // Variable compteur 0 de base

void Color(int couleurDuTexte,int couleurDeFond)
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE); // Récupère le handle de la console
    SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte); // Applique la couleur du texte et du fond
}

void gotoligcol( int lig, int col )
{
 COORD mycoord;
 mycoord.X = col; // Position horizontale
 mycoord.Y = lig; // Position verticale
 SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord ); // Déplace le curseur
}

void afficherTableau(char tab[LIG][COL], int curseurL, int curseurC) {

    char forme[]="SFPOM";
    int lig=0;
    int col=0;
    int nb_aleatoire=0;

    // === Création tableau initial avec valeurs interdites ===
    for(lig=0; lig<LIG; lig++)
    {
        for(col=0; col<COL; col++)
        {
            if(lig == 0 && col == 0) { nb_aleatoire=rand()%5; tab[lig][col]=forme[nb_aleatoire]; }
            if(lig == 0 && col == 1) { nb_aleatoire=rand()%5; tab[lig][col]=forme[nb_aleatoire]; }
            if(lig == 1 && col == 0) { nb_aleatoire=rand()%5; tab[lig][col]=forme[nb_aleatoire]; }

            if(lig == 0 && col >= 2){
                do{ nb_aleatoire=rand()%5; tab[lig][col]=forme[nb_aleatoire]; }
                while(tab[lig][col] == tab[0][col-2]);
            }

            if(lig >= 2 && col ==0){
                do{ nb_aleatoire=rand()%5; tab[lig][col]=forme[nb_aleatoire]; }
                while(tab[lig][col] == tab[lig-2][0]);
            }

            if(lig == 1 && col == 1) {
                nb_aleatoire=rand()%5; tab[lig][col]=forme[nb_aleatoire];
            }

            if(lig == 1 && col == 2) {
                do{ nb_aleatoire=rand()%5; tab[lig][col]=forme[nb_aleatoire]; }
                while(tab[lig][col] == tab[1][0]);
            }

            if(lig == 2 && col == 1) {
                do{ nb_aleatoire=rand()%5; tab[lig][col]=forme[nb_aleatoire]; }
                while(tab[lig][col] == tab[0][1]);
            }

            if(lig ==1 && col >= 2){
                do{ nb_aleatoire=rand()%5; tab[lig][col]=forme[nb_aleatoire]; }
                while(tab[lig][col]== tab[1][col-2]);
            }

            if(lig >= 2 && col ==1){
                do{ nb_aleatoire=rand()%5; tab[lig][col]=forme[nb_aleatoire]; }
                while(tab[lig][col] == tab[lig-2][1]);
            }

            if(lig>=2 && col>=2){
                do{ nb_aleatoire=rand()%5; tab[lig][col]=forme[nb_aleatoire]; }
                while(tab[lig][col] == tab[lig][col-2] ||
                      tab[lig][col] == tab[lig-2][col]);
            }
        }
    }



    // === Affichage du tableau ===
    for(lig = 0; lig < LIG; lig++)
    {
        for(col = 0; col < COL; col++)
        {
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

void afficherCase(char tab[LIG][COL], int lig, int col, int surligner, int selection) {
    gotoligcol(lig, col); // Se positionne sur la case à afficher

    // Si la case est sélectionnée
    if(selection) Color(0, 14); // Fond jaune pour case sélectionnée
    else if(surligner) Color(0, 15); // Fond blanc pour le curseur
    else if(tab[lig][col]=='S') Color(14,0); // Soleil : jaune
    else if(tab[lig][col]=='F') Color(12,0); // Fraise : rouge
    else if(tab[lig][col]=='P') Color(6,0);  // Pomme : vert kaki
    else if(tab[lig][col]=='O') Color(5,0);  // Oignon : violet
    else if(tab[lig][col]=='M') Color(9,0);  // Mandarine : bleu

    printf("%c", tab[lig][col]); // Affiche le symbole
    Color(7,0); // Réinitialise la couleur
}

void boucleDeplacement(char tab[LIG][COL])
{
    int curseurL = 0, curseurC = 0; // Position actuelle du curseur
    int enDeplacement = 0;           // Indique si une case a été sélectionnée pour échange
    int posInitL = -1, posInitC = -1;// Position initiale de la case sélectionnée
    char symboleStocke = 0;          // Symbole sélectionné pour déplacement

    while (1)
    {
        char c = _getch(); // Lecture de la touche pressée

        int ancienL = curseurL, ancienC = curseurC; // Sauvegarde de l'ancienne position

        // Déplacement du curseur avec touches ZQSD
        if (c == 'z' || c == 'Z') { if (curseurL > 0) curseurL--; } // Monte
        else if (c == 's' || c == 'S') { if (curseurL < LIG-1) curseurL++; } // Descend
        else if (c == 'q' || c == 'Q') { if (curseurC > 0) curseurC--; } // Gauche
        else if (c == 'd' || c == 'D') { if (curseurC < COL-1) curseurC++; } // Droite

        // Réaffiche uniquement l'ancienne et la nouvelle case
        afficherCase(tab, ancienL, ancienC, 0, (enDeplacement && ancienL==posInitL && ancienC==posInitC)); // Ancienne case
        afficherCase(tab, curseurL, curseurC, 1, (enDeplacement && curseurL==posInitL && curseurC==posInitC)); // Nouvelle case en surbrillance

        // Sélection / échange avec la touche espace
        if (c == ' ') {
            if (!enDeplacement) {
                // Sélection de la case actuelle
                posInitL = curseurL;
                posInitC = curseurC;
                symboleStocke = tab[posInitL][posInitC];
                enDeplacement = 1; // Active l'état de sélection
                afficherCase(tab, curseurL, curseurC, 0, 1); // Met la case en surbrillance spéciale sélection
            }
            else {
                // Vérifier que la case ciblée est adjacente
                int distL = abs(curseurL - posInitL);
                int distC = abs(curseurC - posInitC);

                if ((distL + distC) == 1) { // Adjacent si distance Manhattan = 1
                    // ÉCHANGE des symboles
                    char tmp = tab[curseurL][curseurC];
                    tab[curseurL][curseurC] = symboleStocke;
                    tab[posInitL][posInitC] = tmp;
                    compteurMouvement();

                    // Redessine les deux cases après échange
                    afficherCase(tab, curseurL, curseurC, 1, 0);
                    afficherCase(tab, posInitL, posInitC, 0, 0);
                }

                enDeplacement = 0; // Réinitialise l'état de sélection
            }
        }

        // Quitter : touche 'e'
        else if (c == 'e' || c == 'E') break;
    }
}

void compteurMouvement()
{
    compteurMouvements++;      // Incrémente
    Color(11,0);               // Couleur bleu clair (modifiable)
    gotoligcol(LIG+1, 0); // Place éléments à la fin du tableau
    printf("Movement : %d\n", compteurMouvements); // Affiche
    Color(7,0);                // Reset couleur
}
