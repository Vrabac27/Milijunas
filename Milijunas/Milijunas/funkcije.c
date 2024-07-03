#define _CRT_SECURE_NO_WARNINGS
#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//makro funkcija 
#define INVALID_INPUT() \
    printf("Neispravan unos. Molimo pokusajte ponovo.\n");


//Imenovanje identifikatora(varijabli, konstanti, polja, funkcija, pokazivaèa…)
//Primjena kljuène rijeèi static za globalne i lokalne varijable.
static char currentPlayer[50];


Izbor dohvatiIzbor(const char* unos) {
    if (strcmp(unos, "Pocetak igre") == 0) return POCETAK_IGRE;
    if (strcmp(unos, "Pregled rezultata") == 0) return PREGLED_REZULTATA;
    if (strcmp(unos, "Pretrazivanje igraca") == 0) return PRETRAZIVANJE_IGRACA;
    if (strcmp(unos, "Ukloni igraca") == 0) return UKLONI_IGRACA;
    if (strcmp(unos, "Tutorial") == 0) return TUTORIJAL;
    if (strcmp(unos, "Izlaz") == 0) return IZLAZ;
    return NEVAZECI_IZBOR;
}


Kategorija dohvatiKategoriju(const char* unos) {
    if (strcmp(unos, "Sport") == 0) return SPORT;
    if (strcmp(unos, "Filmovi") == 0) return FILMOVI;
    if (strcmp(unos, "Geografija") == 0) return GEOGRAFIJA;
    if (strcmp(unos, "Kultura") == 0) return KULTURA;
    return NEVAZECI_KATEGORIJA;
}

//Izbornik/podizbornici.
void menu() {
    
    //Koristiti dinamièko zauzimanje memorije za bilo koji tip podatka
    char* input = (char*)malloc(100 * sizeof(char)); //koristenje malloc
    if (input == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        return;
    }
    Izbor choice;

    while (1) {
        printf("\nWHO WANTS TO BE A MILLIONAIRE?\n\n");
        printf("Izbor:\n");
        printf("1. Pocetak igre\n");
        printf("2. Pregled rezultata\n");
        printf("3. Pretrazivanje igraca\n");
        printf("4. Ukloni igraca\n");
        printf("5. Tutorial\n");
        printf("6. Izlaz\n\n");
        printf("Unesite svoj izbor: ");
        scanf(" %[^\n]", input); // Èitanje unosa do nove linije, sprema u input

        choice = dohvatiIzbor(input);

        system("cls");

        switch (choice) {
        case POCETAK_IGRE:
            startGame();
            system("cls");
            break;
        case PREGLED_REZULTATA:
            displayResults();
            break;
        case PRETRAZIVANJE_IGRACA:
            searchPlayer();
            break;
        case UKLONI_IGRACA:
            removeScore();
            break;
        case TUTORIJAL:
            aboutGame();
            break;
        case IZLAZ:
            exitGame();
            return;
        default:
            INVALID_INPUT();
            break;
        }
    }
}

//Generalno upotreba funkcija
void aboutGame() {
    printf("\nDobrodosli u igru 'Milijunas'!\n"
        "Za pocetak igre, odaberite 'Start' i unesite svoj nadimak.\n"
        "Zatim odaberite jednu od èetiri ponudene kategorije (Sport, Filmovi, Geografija, Kultura).\n"
        "Svaka kategorija sadrzava 20 pitanja, svako s cetiri ponudena odgovora.\n"
        "Unesite broj odgovara koji smatrate tocnim.\n"
        "Tocan odgovor donosi 500$, a netocan oduzima 500$.\n"
        "Igra zavrsava nakon što odgovorite na sva pitanja, a vas konacni saldo odreduje uspjeh.\n"
        "Ako zelite, mozete pregledati prethodne rezultate ili pretrazivati igrace koji su prije igrali igru te ukloniti igraca.\n"
        "Sretno!\n");
}


void saveResult(const char* player, int score) {
    FILE* file = fopen("results.txt", "a");
    if (file == NULL) {
        printf("Greska pri otvaranju datoteke za pisanje.\n");
        return;
    }
    fprintf(file, "Rezultati za igraca: %s\nRezultat: %d$\n", player, score);
    fclose(file);
}

int checkAndRemoveNickname(char* currentPlayer) {
    FILE* file = fopen("results.txt", "r");
    if (file == NULL) {
        return 0;  // File doesn't exist, no need to remove anything
    }

    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Greska pri otvaranju privremene datoteke.\n");
        fclose(file);
        return -1;
    }

    char line[100];
    int nicknameExists = 0;
    int skipNextLines = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, currentPlayer) != NULL) {
            nicknameExists = 1;
            skipNextLines = 2;
        }
        else if (skipNextLines > 0) {
            skipNextLines--;
        }
        else {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (nicknameExists) {
        remove("results.txt");
        rename("temp.txt", "results.txt");
    }
    else {
        remove("temp.txt");
    }

    return nicknameExists;
}

void startGame() {

    char* input = (char*)malloc(100 * sizeof(char));
    if (input == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        return;
    }

    if (input == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        return;
    }

    int nicknameExists;

    do {
        printf("Unesite svoj nadimak: ");
        scanf("%s", currentPlayer);
        nicknameExists = checkAndRemoveNickname(currentPlayer);

        if (nicknameExists) {
            printf("Nadimak postoji. Uklanjanje starog zapisa.\n");
        }
    } while (nicknameExists);


    Kategorija category;
    
    do {
        printf("Odaberite kategoriju:\n\n");
        printf("Sport\nFilmovi\nGeografija\nKultura\n\n");
        printf("Unesite naziv kategorije: ");
        scanf(" %[^\n]", input); // Èitanje unosa do nove linije

        category = dohvatiKategoriju(input);
        system("cls");

        switch (category) {
        case SPORT:
            sportCategory(currentPlayer);
            break;
        case FILMOVI:
            moviesCategory(currentPlayer);
            break;
        case GEOGRAFIJA:
            geographyCategory(currentPlayer);
            break;
        case KULTURA:
            cultureCategory(currentPlayer);
            break;
        default:
            INVALID_INPUT();
            break;
        }
    } while (category == NEVAZECI_KATEGORIJA);

    free(input); //koristiti funkciju free();
    input = NULL; //anuliranje svih pokazivaèa koji su bili usmjereni na memorijski prostor koji se dinamièki zauzeo
}


void sportCategory() {
    //Datoteke, koristiti tekstualnu ili binarnu, provjera pokazivaèa i zatvaranje datoteke.
    FILE* file = fopen("sport.txt", "r"); //upotreba pokazivaèa tamo gdje su potrebni
    FILE* results = fopen("results.txt", "a");

    if (file == NULL || results == NULL) {
        printf("Greska pri otvaranju datoteka.\n");
        if (results != NULL) {
            fclose(results);
        }
        printf("Pritisnite Enter za povratak na glavni izbornik.");
        while (getchar() != '\n');
        getchar();
        system("cls");
        return;
    }
    
    //skip bom
    if (fgetc(file) != 0xEF || fgetc(file) != 0xBB || fgetc(file) != 0xBF) fseek(file, 0, SEEK_SET);

    Question questions[100];
    int num_questions = load_questions("sport.txt", questions, 100);
    if (num_questions == -1) {
        fclose(results);
        return;
    }

    shuffle_questions(questions, num_questions);

    int score = 0;
    fprintf(results, "Rezultati za igraca: %s\n", currentPlayer);  

    printf("Pitanja iz kategorije 'Sport':\n\n");

    for (int i = 0; i < num_questions; i++) {
        printf("Pitanje %d: %s\n", i + 1, questions[i].question);
        for (int j = 0; j < 4; j++) {
            printf("%d. %s\n", j + 1, questions[i].answers[j]);
        }

        int playerAnswer;

        do {
            printf("\nUnesite broj odgovora: ");
            if (scanf("%d", &playerAnswer) != 1 || playerAnswer < 1 || playerAnswer > 4) {
                printf("Neispravan unos. Molimo unesite broj od 1 do 4.\n");
                while (getchar() != '\n'); // Èisti ulaz
            }
        } while (playerAnswer < 1 || playerAnswer > 4);

        if (playerAnswer == questions[i].correctAnswer) {
            printf("\nTocno! +500$\n\n");
            score += 500;
        }
        else {
            printf("\nNetocno. -500$\n\n");
            score -= 500;
        }
        printf("Trenutno stanje: %d$\n\n", score);

        printf("Pritisnite Enter za sljedece pitanje...");
        while (getchar() != '\n');
        getchar();
        system("cls");
    }

    system("cls");

    printf("Vas konacni rezultat: %d$\n", score);
    fprintf(results, "Rezultat: %d$\n\n", score);
    printf("Pritisni Enter za nastavak.\n", score);


    //Zaštita parametara kod svih funkcija.
    fclose(results);
    results = NULL;

    while (getchar() != '\n');
    printf("Pritisnite Enter za povratak na glavni izbornik.");
    getchar();
    system("cls");
}


void moviesCategory() {
    FILE* file = fopen("movies.txt", "r");  //pokazivaci
    FILE* results = fopen("results.txt", "a");

    if (file == NULL || results == NULL) {
        printf("Greska pri otvaranju datoteka.\n");
        if (results != NULL) {
            fclose(results);
        }
        printf("Pritisnite Enter za povratak na glavni izbornik.");
        while (getchar() != '\n');
        getchar();
        system("cls");
        return;
    }

    Question questions[256];
    int num_questions = load_questions("movies.txt", questions, 256);
    if (num_questions == -1) {
        fclose(results);
        return;
    }

    shuffle_questions(questions, num_questions);

    int score = 0;
    fprintf(results, "Rezultati za igraca: %s\n", currentPlayer); 

    printf("Pitanja iz kategorije 'Sport':\n\n");

    for (int i = 0; i < num_questions; i++) {
        printf("Pitanje %d: %s\n", i + 1, questions[i].question);
        for (int j = 0; j < 4; j++) {
            printf("%d. %s\n", j + 1, questions[i].answers[j]);
        }

        int playerAnswer;

        do {
            printf("\nUnesite broj odgovora: ");
            if (scanf("%d", &playerAnswer) != 1 || playerAnswer < 1 || playerAnswer > 4) {
                printf("Neispravan unos. Molimo unesite broj od 1 do 4.\n");
                while (getchar() != '\n'); // Èisti ulaz
            }
        } while (playerAnswer < 1 || playerAnswer > 4);

        if (playerAnswer == questions[i].correctAnswer) {
            printf("\nTocno! +500$\n\n");
            score += 500;
        }
        else {
            printf("\nNetocno. -500$\n\n");
            score -= 500;
        }
        printf("Trenutno stanje: %d$\n\n", score);

        printf("Pritisnite Enter za sljedece pitanje...");
        while (getchar() != '\n');
        getchar();
        system("cls");
    }

    system("cls");

    printf("Vas konacni rezultat: %d$\n", score);
    fprintf(results, "Rezultat: %d$\n\n", score);
    printf("Pritisni Enter za nastavak.\n", score);


    //Zaštita parametara kod svih funkcija.
    fclose(results);
    results = NULL;

    while (getchar() != '\n');
    printf("Pritisnite Enter za povratak na glavni izbornik.");
    getchar();
    system("cls");
}



void geographyCategory() {
    FILE* file = fopen("geography.txt", "r");  //pokazivaci
    FILE* results = fopen("results.txt", "a");


    if (file == NULL || results == NULL) {
        printf("Greska pri otvaranju datoteka.\n");
        if (results != NULL) {
            fclose(results);
        }
        printf("Pritisnite Enter za povratak na glavni izbornik.");
        while (getchar() != '\n');
        getchar();
        system("cls");
        return;
    }

    Question questions[100];
    int num_questions = load_questions("geography.txt", questions, 100);
    if (num_questions == -1) {
        fclose(results);
        return;
    }

    shuffle_questions(questions, num_questions);

    int score = 0;
    fprintf(results, "Rezultati za igraca: %s\n", currentPlayer);

    printf("Pitanja iz kategorije 'Geografija':\n\n");

    for (int i = 0; i < num_questions; i++) {
        printf("Pitanje %d: %s\n", i + 1, questions[i].question);
        for (int j = 0; j < 4; j++) {
            printf("%d. %s\n", j + 1, questions[i].answers[j]);
        }

        int playerAnswer;

        do {
            printf("\nUnesite broj odgovora: ");
            if (scanf("%d", &playerAnswer) != 1 || playerAnswer < 1 || playerAnswer > 4) {
                printf("Neispravan unos. Molimo unesite broj od 1 do 4.\n");
                while (getchar() != '\n'); // Èisti ulaz
            }
        } while (playerAnswer < 1 || playerAnswer > 4);

        if (playerAnswer == questions[i].correctAnswer) {
            printf("\nTocno! +500$\n\n");
            score += 500;
        }
        else {
            printf("\nNetocno. -500$\n\n");
            score -= 500;
        }
        printf("Trenutno stanje: %d$\n\n", score);

        printf("Pritisnite Enter za sljedece pitanje...");
        while (getchar() != '\n');
        getchar();
        system("cls");
    }

    system("cls");

    printf("Vas konacni rezultat: %d$\n", score);
    fprintf(results, "Rezultat: %d$\n\n", score);
    printf("Pritisni Enter za nastavak.\n", score);


    //Zaštita parametara kod svih funkcija.
    fclose(results);
    results = NULL;

    while (getchar() != '\n');
    printf("Pritisnite Enter za povratak na glavni izbornik.");
    getchar();
    system("cls");
}


void cultureCategory() {
    FILE* file = fopen("culture.txt", "r");  //pokazivaci
    FILE* results = fopen("culture.txt", "a");

    if (file == NULL || results == NULL) {
        printf("Greska pri otvaranju datoteka.\n");
        if (results != NULL) {
            fclose(results);
        }
        printf("Pritisnite Enter za povratak na glavni izbornik.");
        while (getchar() != '\n');
        getchar();
        system("cls");
        return;
    }


    Question questions[100];
    int num_questions = load_questions("culture.txt", questions, 100);
    if (num_questions == -1) {
        fclose(results);
        return;
    }

    shuffle_questions(questions, num_questions);

    int score = 0;
    fprintf(results, "Rezultati za igraca: %s\n", currentPlayer);

    printf("Pitanja iz kategorije 'Geografija':\n\n");

    for (int i = 0; i < num_questions; i++) {
        printf("Pitanje %d: %s\n", i + 1, questions[i].question);
        for (int j = 0; j < 4; j++) {
            printf("%d. %s\n", j + 1, questions[i].answers[j]);
        }

        int playerAnswer;

        do {
            printf("\nUnesite broj odgovora: ");
            if (scanf("%d", &playerAnswer) != 1 || playerAnswer < 1 || playerAnswer > 4) {
                printf("Neispravan unos. Molimo unesite broj od 1 do 4.\n");
                while (getchar() != '\n'); // Èisti ulaz
            }
        } while (playerAnswer < 1 || playerAnswer > 4);

        if (playerAnswer == questions[i].correctAnswer) {
            printf("\nTocno! +500$\n\n");
            score += 500;
        }
        else {
            printf("\nNetocno. -500$\n\n");
            score -= 500;
        }
        printf("Trenutno stanje: %d$\n\n", score);

        printf("Pritisnite Enter za sljedece pitanje...");
        while (getchar() != '\n');
        getchar();
        system("cls");
    }

    system("cls");

    printf("Vas konacni rezultat: %d$\n", score);
    fprintf(results, "Rezultat: %d$\n\n", score);
    printf("Pritisni Enter za nastavak.\n", score);


    //Zaštita parametara kod svih funkcija.
    fclose(results);
    results = NULL;

    while (getchar() != '\n');
    printf("Pritisnite Enter za povratak na glavni izbornik.");
    getchar();
    system("cls");
}


int compareNames(const void* key, const void* element) { 
    const char* name = (const char*)key;
    const PlayerResult* result = (const PlayerResult*)element;
    return strcmp(name, result->name);
}


void searchPlayer() {
    //Koristiti statièki zauzeta polja gdje su potrebna, nikako ne koristiti VLA polja.
    char searchNickname[100];
    int found = 0;

    do {
        printf("Unesite nadimak igraca za pretragu: ");
        scanf("%s", searchNickname);

        FILE* resultsFile = fopen("results.txt", "r");
        if (resultsFile == NULL) {
            printf("Nema prethodnih rezultata.\n");
            return;
        }

        PlayerResult results[100];
        int count = 0;

        while (fscanf(resultsFile, "Rezultati za igraca: %s\nRezultat: %d$\n", results[count].name, &results[count].score) == 2) {
            count++;
            if (count >= 100) break;
        }

        fclose(resultsFile);

        //Pretraživanje – preporuka koristiti ugraðenu bsearch() funkciju.
        // Sortiranje rezultata po imenu
        qsort(results, count, sizeof(PlayerResult), compareNames);

        // Traženje igraèa koristeæi bsearch
        PlayerResult key;
        strcpy(key.name, searchNickname);

        PlayerResult* foundResult = (PlayerResult*)bsearch(key.name, results, count, sizeof(PlayerResult), compareNames);

        if (foundResult != NULL) {
            printf("Igrac %s:\n", foundResult->name);
            printf("Rezultat: %d$\n", foundResult->score);
            found = 1;  // Postavljamo found na 1 jer smo pronašli rezultat
        }
        else {
            printf("Nema rezultata za igraca '%s'.\n", searchNickname);
            found = 0;  // Postavljamo found na 0 jer nismo pronašli rezultat
        }

    } while (!found);  // Ponavljamo petlju dok god nismo pronašli rezultat
}


int load_questions(const char* filename, Question questions[], int max_questions) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");//perror
        return -1;
    }

    //Koristiti funkcije fseek(), ftell(), rewind(), ovisno o potrebi – neku od njih ako ne sve.
    fseek(file, 0, SEEK_END); //fseek; pomakni se na kraj datoteke
    long fileSize = ftell(file); //ftell; dohvati trenutni pokazivaè datoteke (velièina datoteke)
    rewind(file); //rewind; Vðvrati pokazivaè na poèetak datoteke

    int count = 0;
    while (count < max_questions && fscanf(file, "%[^\n]\n", questions[count].question) == 1) {
        for (int i = 0; i < 4; i++) {
            fscanf(file, "%[^\n]\n", questions[count].answers[i]);
        }
        fscanf(file, "%d\n", &questions[count].correctAnswer);
        count++;
    }

    fclose(file);
    return count;
}

void shuffle_questions(Question questions[], int count) {
    srand(time(NULL));
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Question temp = questions[i];
        questions[i] = questions[j];
        questions[j] = temp;
    }
}


int compareResults(const void* a, const void* b) {
    const PlayerResult* resultA = (const PlayerResult*)a;
    const PlayerResult* resultB = (const PlayerResult*)b;
    return resultB->score - resultA->score; // Sort descending by score
}


void displayResults() {
    FILE* file = fopen("results.txt", "r"); 
    if (file == NULL) {
        printf("Greska pri otvaranju datoteke s rezultatima.\n"); 
        return;
    }

    PlayerResult results[100]; // Deklaracija niza za pohranu rezultata
    int count = 0; // Brojaè za praæenje broja uèitanih rezultata

    // Uèitavanje rezultata iz datoteke
    while (fscanf(file, "Rezultati za igraca: %s\nRezultat: %d$\n", results[count].name, &results[count].score) == 2) {
        count++; // Inkrementiranje brojaèa uèitanih rezultata
        if (count >= 100) break; // Provjera da ne preðemo maksimalan broj rezultata (100)
    }

    fclose(file);

    //Sortiranje – preporuka koristiti ugraðenu qsort() funkciju.
    // Sortiranje rezultata od najboljeg do najgoreg
    qsort(results, count, sizeof(PlayerResult), compareResults);

    printf("\nSortirani rezultati:\n\n");
    for (int i = 0; i < count; i++) {
        printf("%d. Igrac: %s -> Rezultat: %d$\n", i + 1, results[i].name, results[i].score);
    }

    while (getchar() != '\n'); // Èišæenje ulaza
    printf("\nPritisnite Enter za povratak na glavni izbornik.");
    getchar(); // Èekanje unosa Enter
    system("cls"); 
}


void removeScore() {
    char playerToRemove[50];
    printf("Unesite nadimak igraca ciji rezultat zelite obrisati: ");
    scanf("%s", playerToRemove);

    FILE* resultsFile = fopen("results.txt", "r");
    if (resultsFile == NULL) {
        printf("Nema prethodnih rezultata.\n");
        return;
    }

    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Greska pri otvaranju privremene datoteke.\n");
        fclose(resultsFile);
        return;
    }

    char line[256];
    int skip = 0; // Indikator koji oznaèava treba li preskoèiti redove u datoteci
    int playerFound = 0; // Indikator je li pronaðen traženi igraè

    while (fgets(line, sizeof(line), resultsFile)) { // Èitanje red po red iz datoteke results.txt
        if (strstr(line, playerToRemove) != NULL) { // Provjera sadrži li trenutni red nadimak igraèa koji se želi izbrisati
            skip = 1; // Ako je pronaðen nadimak, postavlja se skip na 1 kako bi se preskoèili sljedeæi redovi
            playerFound = 1; // Oznaèavamo da je igraè pronaðen
        }
        else if (skip && strstr(line, "Rezultati za igraca: ") != NULL) { // Provjera je li pronaðen kraj bloka rezultata za traženog igraèa
            skip = 0; // Ako je pronaðen kraj bloka, postavlja se skip na 0 kako bi se prestalo s preskakanjem redaka
        }

        if (!skip) { // Ako nije postavljen skip, tada se sadržaj trenutnog retka upisuje u privremenu datoteku temp.txt
            fputs(line, tempFile);
        }
    }

    //Zaštita parametara kod svih funkcija.
    fclose(resultsFile);
    fclose(tempFile);
    
    /*if (fopen("temp.txt", "r") != NULL) {
        printf("temp.txt je stvoren.\n");
        fclose(tempFile);
    }*/

    //Koristiti funkcije remove(), rename()
    remove("results.txt"); // Brisanje originalne datoteke
    rename("temp.txt", "results.txt"); // Preimenovanje temp.txt u results.txt

    if (playerFound) {
        printf("Rezultati za igraca %s su obrisani.\n", playerToRemove);

        // Nakon brisanja, ponovno sortiraj preostale rezultate
        resultsFile = fopen("results.txt", "r");
        if (resultsFile == NULL) {
            printf("Greska pri otvaranju datoteke s rezultatima.\n");
            return;
        }

        PlayerResult results[100];
        int count = 0;

        // Uèitavanje preostalih rezultata iz datoteke
        while (fscanf(resultsFile, "Rezultati za igraca: %s\nRezultat: %d$\n", results[count].name, &results[count].score) == 2) {
            count++;
            if (count >= 100) break;
        }

        fclose(resultsFile);

        // Ponovno sortiranje rezultata nakon brisanja
        quickSort(results, 0, count - 1, compareResults);

        // Ispis sortiranih rezultata
        printf("\nSortirani rezultati nakon brisanja:\n\n");
        for (int i = 0; i < count; i++) {
            printf("%d. Igrac: %s -> Rezultat: %d$\n", i + 1, results[i].name, results[i].score);
        }
    }
    else {
        printf("Nadimak nije pronaden.\n");
    }
}


// Quick Sort funkcija
void quickSort(PlayerResult arr[], int low, int high, int (*compare)(const void*, const void*)) {
    if (low < high) {
        // Dobavi indeks particije
        int pi = partition(arr, low, high, compare);

        // Rekurzivno sortiraj elemente prije i poslije particije
        quickSort(arr, low, pi - 1, compare);
        quickSort(arr, pi + 1, high, compare);
    }
}

void swap(PlayerResult* a, PlayerResult* b) {
    PlayerResult temp = *a;
    *a = *b;
    *b = temp;
}

// Funkcija za particioniranje niza
int partition(PlayerResult arr[], int low, int high, int (*compare)(const void*, const void*)) {
    PlayerResult pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (compare(&arr[j], &pivot) < 0) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void exitGame() {
    printf("Izlaz iz igre.\n");
    exit(0);
}