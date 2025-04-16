#include <iostream>
#include <chrono>
using namespace std;

class TablicaDynamiczna {
private:
    int* dane;
    int rozmiar;
    int pojemnosc;

public:
    TablicaDynamiczna() {
        rozmiar = 0;
        pojemnosc = 10;
        dane = new int[pojemnosc];
    }

    ~TablicaDynamiczna() {
        delete[] dane;
    }

    void Powieksz() {
        pojemnosc *= 2;
        int* noweDane = new int[pojemnosc];
        for (int i = 0; i < rozmiar; i++) noweDane[i] = dane[i];
        delete[] dane;
        dane = noweDane;
    }

    void DodajNaPoczatek(int wartosc) {
        if (rozmiar == pojemnosc) Powieksz();
        for (int i = rozmiar; i > 0; i--) {
            dane[i] = dane[i - 1];
        }
        dane[0] = wartosc;
        rozmiar++;
    }

    void DodajNaKoniec(int wartosc) {
        if (rozmiar == pojemnosc) Powieksz();
        dane[rozmiar++] = wartosc;
    }

    void UsunZPoczatku() {
        if (rozmiar > 0) {
            for (int i = 0; i < rozmiar - 1; i++) {
                dane[i] = dane[i + 1];
            }
            rozmiar--;
        }
    }

    void UsunZKonca() {
        if (rozmiar > 0) rozmiar--;
    }

    int Znajdz(int wartosc) {
        for (int i = 0; i < rozmiar; i++) {
            if (dane[i] == wartosc) return i;
        }
        return -1;
    }
};

class ListaSekwencyjna {
private:
    struct Wezel {
        int wartosc;
        Wezel* nastepny;
        Wezel(int wartosc) : wartosc(wartosc), nastepny(nullptr) {}
    };

    Wezel* PoczatekListy;

public:
    ListaSekwencyjna() : PoczatekListy(nullptr) {}

    ~ListaSekwencyjna() {
        while (PoczatekListy) {
            Wezel* temp = PoczatekListy;
            PoczatekListy = PoczatekListy->nastepny;
            delete temp;
        }
    }

    void DodajNaPoczatek(int wartosc) {
        Wezel* nowy = new Wezel(wartosc);
        nowy->nastepny = PoczatekListy;
        PoczatekListy = nowy;
    }

    void DodajNaKoniec(int wartosc) {
        Wezel* nowy = new Wezel(wartosc);
        if (!PoczatekListy) {
            PoczatekListy = nowy;
            return;
        }
        Wezel* temp = PoczatekListy;
        while (temp->nastepny) temp = temp->nastepny;
        temp->nastepny = nowy;
    }

    void UsunZPoczatku() {
        if (!PoczatekListy) return;
        Wezel* temp = PoczatekListy;
        PoczatekListy = PoczatekListy->nastepny;
        delete temp;
    }

    void UsunZKonca() {
        if (!PoczatekListy) return;
        if (!PoczatekListy->nastepny) {
            delete PoczatekListy;
            PoczatekListy = nullptr;
            return;
        }
        Wezel* temp = PoczatekListy;
        while (temp->nastepny && temp->nastepny->nastepny) {
            temp = temp->nastepny;
        }
        delete temp->nastepny;
        temp->nastepny = nullptr;
    }

    int Znajdz(int wartosc) {
        Wezel* temp = PoczatekListy;
        int index = 0;
        while (temp) {
            if (temp->wartosc == wartosc) return index;
            temp = temp->nastepny;
            index++;
        }
        return -1;
    }
};

int main() {
    const int N = 10;
    
    TablicaDynamiczna tablica;
    ListaSekwencyjna lista;

    // Pomiar czasu dodawania na początek - tablica
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) tablica.DodajNaPoczatek(i);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long long, std::nano> diff = end - start;
    std::cout << "Czas dodawania na poczatek do tablicy: " << diff.count() << " nanosekund" << std::endl;

    // Pomiar czasu dodawania na początek - lista
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) lista.DodajNaPoczatek(i);
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Czas dodawania na poczatek do listy: " << diff.count() << " nanosekund" << std::endl;

    // Pomiar czasu dodawania na koniec - tablica
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) tablica.DodajNaKoniec(i);
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Czas dodawania na koniec do tablicy: " << diff.count() << " nanosekund" << std::endl;

    // Pomiar czasu dodawania na koniec - lista
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) lista.DodajNaKoniec(i);
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Czas dodawania na koniec do listy: " << diff.count() << " nanosekund" << std::endl;

    // Pomiar czasu usuwania z początku - tablica
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) tablica.UsunZPoczatku();
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Czas usuwania z poczatku z tablicy: " << diff.count() << " nanosekund" << std::endl;

    // Pomiar czasu usuwania z początku - lista
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) lista.UsunZPoczatku();
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Czas usuwania z poczatku z listy: " << diff.count() << " nanosekund" << std::endl;

    // Pomiar czasu usuwania z końca - tablica
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) tablica.UsunZKonca();
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Czas usuwania z konca z tablicy: " << diff.count() << " nanosekund" << std::endl;

    // Pomiar czasu usuwania z końca - lista
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) lista.UsunZKonca();
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Czas usuwania z konca z listy: " << diff.count() << " nanosekund" << std::endl;

    // Pomiar czasu wyszukiwania w tablicy
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) tablica.Znajdz(i);
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Czas wyszukiwania w tablicy: " << diff.count() << " nanosekund" << std::endl;

    // Pomiar czasu wyszukiwania w liście
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) lista.Znajdz(i);
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Czas wyszukiwania w liscie: " << diff.count() << " nanosekund" << std::endl;

    return 0;
}
