#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h> // Pour _getch()

#define LIG 25 // Nombre de lignes
#define COL 45 // Nombre de colonnes

typedef struct {
    int S; // Soleils
    int F; // Fraises
    int P; // Pommes
    int O; // Oignons
    int M; // Mandarines
} Contrat;
