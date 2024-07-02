#define _CRT_SECURE_NO_WARNINGS

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>


typedef enum {
    POCETAK_IGRE,
    PREGLED_REZULTATA,
    PRETRAZIVANJE_IGRACA,
    UKLONI_IGRACA,
    TUTORIJAL,
    IZLAZ,
    NEVAZECI_IZBOR
} Izbor;


typedef struct {
    char question[256];
    char answers[4][128];
    int correctAnswer;
    int playerAnswer;
} Question;

typedef enum {
    SPORT,
    FILMOVI,
    GEOGRAFIJA,
    KULTURA,
    NEVAZECI_KATEGORIJA
} Kategorija;


typedef struct {
    char name[100];
    int score;
} PlayerResult;


extern char currentPlayer[50];

Izbor dohvatiIzbor(const char* unos);
Kategorija dohvatiKategoriju(const char* unos);

void startGame();

void aboutGame();

void exitGame();

void sportCategory(const char* nickname);
void moviesCategory(const char* nickname);
void geographyCategory(const char* nickname);
void cultureCategory(const char* nickname);

void displayResults();

void searchPlayer();

void shuffle_questions();

void removeScore();

int compareResults();

int compareNames();
#endif
