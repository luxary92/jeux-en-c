#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NLINE 3
#define NCOL 3

char plateau[NLINE][NCOL];
int centerX, centerY;

// Réinitialiser le plateau
void resetPlateau() {
    for (int i = 0; i < NLINE; i++)
        for (int j = 0; j < NCOL; j++)
            plateau[i][j] = ' ';
}

// Affichage du plateau centré
void afficherPlateau() {
    clear();
    for (int i = 0; i < NLINE; i++) {
        for (int j = 0; j < NCOL; j++) {
            mvprintw(centerY + i * 2, centerX + j * 4, " %c ", plateau[i][j]);
            if (j < NCOL - 1) mvprintw(centerY + i * 2, centerX + j * 4 + 3, "|");
        }
        if (i < NLINE - 1) mvprintw(centerY + i * 2 + 1, centerX, "---+---+---");
    }
}

// Vérifier la victoire
char verifierVictoire() {
    for (int i = 0; i < NLINE; i++) {
        if (plateau[i][0] == plateau[i][1] && plateau[i][1] == plateau[i][2] && plateau[i][0] != ' ')
            return plateau[i][0];
        if (plateau[0][i] == plateau[1][i] && plateau[1][i] == plateau[2][i] && plateau[0][i] != ' ')
            return plateau[0][i];
    }
    if (plateau[0][0] == plateau[1][1] && plateau[1][1] == plateau[2][2] && plateau[0][0] != ' ')
        return plateau[0][0];
    if (plateau[0][2] == plateau[1][1] && plateau[1][1] == plateau[2][0] && plateau[0][2] != ' ')
        return plateau[0][2];
    return ' ';
}

// Vérifier si le plateau est plein (match nul)
int estMatchNul() {
    for (int i = 0; i < NLINE; i++)
        for (int j = 0; j < NCOL; j++)
            if (plateau[i][j] == ' ') return 0;
    return 1;
}

// IA joue un coup aléatoire
void iaJoue(char symbole) {
    int x, y;
    do {
        x = rand() % NLINE;
        y = rand() % NCOL;
    } while (plateau[y][x] != ' ');

    plateau[y][x] = symbole;
}

// Fonction pour gérer une partie
void jouerMorpion(int mode) {
    resetPlateau();
    int x = 0, y = 0;
    char joueur = 'X';

    while (1) {
        int coupJoue = 0;

        afficherPlateau();
        mvprintw(centerY + NLINE * 2 + 1, centerX, "Joueur: %c | Fleches + Entrée pour jouer", joueur);
        move(centerY + y * 2, centerX + x * 4 + 1);
        refresh();

        if ((mode == 1 && joueur == 'X') || mode == 3) {
            int touche;
            while (1) {
                touche = getch();
                switch (touche) {
                    case KEY_UP: if (y > 0) y--; break;
                    case KEY_DOWN: if (y < NLINE - 1) y++; break;
                    case KEY_LEFT: if (x > 0) x--; break;
                    case KEY_RIGHT: if (x < NCOL - 1) x++; break;
                    case '\n':
                        if (plateau[y][x] == ' ') {
                            plateau[y][x] = joueur;
                            coupJoue = 1;
                        }
                        break;
                }
                if (coupJoue) break;
                move(centerY + y * 2, centerX + x * 4 + 1);
                refresh();
            }
        } else if (mode == 2) {
            napms(500);
            iaJoue(joueur);
            coupJoue = 1;
        } else {
            napms(500);
            iaJoue(joueur);
            coupJoue = 1;
        }

        if (verifierVictoire() != ' ') {
            afficherPlateau();
            mvprintw(centerY + NLINE * 2 + 2, centerX, "Le joueur %c a gagné !", joueur);
            getch();
            return;
        }
        if (estMatchNul()) {
            afficherPlateau();
            mvprintw(centerY + NLINE * 2 + 2, centerX, "Match nul !");
            getch();
            return;
        }

        if (coupJoue) {
            joueur = (joueur == 'X') ? 'O' : 'X';
        }
    }
}

// Menu principal avec option Quitter
int afficherMenu() {
    int choix = 0;
    while (1) {
        clear();
        mvprintw(LINES / 2 - 3, (COLS - 30) / 2, "=== MENU DU MORPION ===");
        mvprintw(LINES / 2, (COLS - 30) / 2, "1. Joueur vs IA");
        mvprintw(LINES / 2 + 1, (COLS - 30) / 2, "2. IA vs IA");
        mvprintw(LINES / 2 + 2, (COLS - 30) / 2, "3. Joueur vs Joueur");
        mvprintw(LINES / 2 + 3, (COLS - 30) / 2, "4. Quitter");
        mvprintw(LINES / 2 + 5, (COLS - 30) / 2, "Choisissez une option: ");

        refresh();
        choix = getch() - '0';
        if (choix >= 1 && choix <= 4) return choix;
    }
}

int main() {
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    srand(time(NULL));

    centerX = (COLS - (NCOL * 4)) / 2;
    centerY = (LINES - (NLINE * 2)) / 2;

    while (1) {
        int mode = afficherMenu();
        if (mode == 4) break;
        jouerMorpion(mode);
    }

    endwin();
    return 0;
}

