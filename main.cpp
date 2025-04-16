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
        for (int i = rozmiar; i > 0; i--) dane[i] = dane[i - 1];
        dane[0] = wartosc;
        rozmiar++;
    }

    void DodajNaKoniec(int wartosc) {
        if (rozmiar == pojemnosc) Powieksz();
        dane[rozmiar++] = wartosc;
    }

    void DodajNaIndeks(int indeks, int wartosc) {
        if (indeks < 0 || indeks > rozmiar) return;
        if (rozmiar == pojemnosc) Powieksz();
        for (int i = rozmiar; i > indeks; i--) dane[i] = dane[i - 1];
        dane[indeks] = wartosc;
        rozmiar++;
    }

    void UsunZPoczatku() {
        if (rozmiar == 0) return;
        for (int i = 0; i < rozmiar - 1; i++) dane[i] = dane[i + 1];
        rozmiar--;
    }

    void UsunZKonca() {
        if (rozmiar > 0) rozmiar--;
    }

    void UsunZIndeksu(int indeks) {
        if (indeks < 0 || indeks >= rozmiar) return;
        for (int i = indeks; i < rozmiar - 1; i++) dane[i] = dane[i + 1];
        rozmiar--;
    }

    int Znajdz(int wartosc) {
        for (int i = 0; i < rozmiar; i++)
            if (dane[i] == wartosc) return i;
        return -1;
    }
};

class ListaSekwencyjna {
private:
    struct Wezel {
        int wartosc;
        Wezel* nastepny;
        Wezel(int w) : wartosc(w), nastepny(nullptr) {}
    };

    Wezel* glowa;

public:
    ListaSekwencyjna() : glowa(nullptr) {}

    ~ListaSekwencyjna() {
        while (glowa) {
            Wezel* temp = glowa;
            glowa = glowa->nastepny;
            delete temp;
        }
    }

    void DodajNaPoczatek(int wartosc) {
        Wezel* nowy = new Wezel(wartosc);
        nowy->nastepny = glowa;
        glowa = nowy;
    }

    void DodajNaKoniec(int wartosc) {
        Wezel* nowy = new Wezel(wartosc);
        if (!glowa) {
            glowa = nowy;
            return;
        }
        Wezel* temp = glowa;
        while (temp->nastepny) temp = temp->nastepny;
        temp->nastepny = nowy;
    }

    void DodajNaIndeks(int indeks, int wartosc) {
        if (indeks < 0) return;
        if (indeks == 0) {
            DodajNaPoczatek(wartosc);
            return;
        }
        Wezel* temp = glowa;
        for (int i = 0; i < indeks - 1 && temp; i++) temp = temp->nastepny;
        if (!temp) return;
        Wezel* nowy = new Wezel(wartosc);
        nowy->nastepny = temp->nastepny;
        temp->nastepny = nowy;
    }

    void UsunZPoczatku() {
        if (!glowa) return;
        Wezel* temp = glowa;
        glowa = glowa->nastepny;
        delete temp;
    }

    void UsunZKonca() {
        if (!glowa) return;
        if (!glowa->nastepny) {
            delete glowa;
            glowa = nullptr;
            return;
        }
        Wezel* temp = glowa;
        while (temp->nastepny->nastepny) temp = temp->nastepny;
        delete temp->nastepny;
        temp->nastepny = nullptr;
    }

    void UsunZIndeksu(int indeks) {
        if (indeks < 0 || !glowa) return;
        if (indeks == 0) {
            UsunZPoczatku();
            return;
        }
        Wezel* temp = glowa;
        for (int i = 0; i < indeks - 1 && temp->nastepny; i++) temp = temp->nastepny;
        if (temp->nastepny) {
            Wezel* doUsuniecia = temp->nastepny;
            temp->nastepny = doUsuniecia->nastepny;
            delete doUsuniecia;
        }
    }

    int Znajdz(int wartosc) {
        Wezel* temp = glowa;
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
    const int N = 100;

    TablicaDynamiczna tablica;
    ListaSekwencyjna lista;

    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) tablica.DodajNaKoniec(i);
    auto end = chrono::high_resolution_clock::now();
    cout << "Tablica: dodawanie na koniec: " << (end - start).count() << " ns\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) lista.DodajNaKoniec(i);
    end = chrono::high_resolution_clock::now();
    cout << "Lista: dodawanie na koniec: " << (end - start).count() << " ns\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) tablica.DodajNaPoczatek(i);
    end = chrono::high_resolution_clock::now();
    cout << "Tablica: dodawanie na poczatek: " << (end - start).count() << " ns\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) lista.DodajNaPoczatek(i);
    end = chrono::high_resolution_clock::now();
    cout << "Lista: dodawanie na poczatek: " << (end - start).count() << " ns\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) tablica.Znajdz(i);
    end = chrono::high_resolution_clock::now();
    cout << "Tablica: szukanie: " << (end - start).count() << " ns\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) lista.Znajdz(i);
    end = chrono::high_resolution_clock::now();
    cout << "Lista: szukanie: " << (end - start).count() << " ns\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) tablica.UsunZPoczatku();
    end = chrono::high_resolution_clock::now();
    cout << "Tablica: usuwanie z poczatku: " << (end - start).count() << " ns\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) lista.UsunZPoczatku();
    end = chrono::high_resolution_clock::now();
    cout << "Lista: usuwanie z poczatku: " << (end - start).count() << " ns\n";

    // Dodaj dane ponownie do testów usuwania z końca
    for (int i = 0; i < N; i++) tablica.DodajNaKoniec(i);
    for (int i = 0; i < N; i++) lista.DodajNaKoniec(i);

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) tablica.UsunZKonca();
    end = chrono::high_resolution_clock::now();
    cout << "Tablica: usuwanie z konca: " << (end - start).count() << " ns\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) lista.UsunZKonca();
    end = chrono::high_resolution_clock::now();
    cout << "Lista: usuwanie z konca: " << (end - start).count() << " ns\n";

    // Dodawanie/usuwanie na indeksie
    for (int i = 0; i < N; i++) tablica.DodajNaKoniec(i);
    for (int i = 0; i < N; i++) lista.DodajNaKoniec(i);

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; i++) tablica.DodajNaIndeks(500, i);
    end = chrono::high_resolution_clock::now();
    cout << "Tablica: dodawanie na indeks: " << (end - start).count() << " ns\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; i++) lista.DodajNaIndeks(500, i);
    end = chrono::high_resolution_clock::now();
    cout << "Lista: dodawanie na indeks: " << (end - start).count() << " ns\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; i++) tablica.UsunZIndeksu(500);
    end = chrono::high_resolution_clock::now();
    cout << "Tablica: usuwanie z indeksu: " << (end - start).count() << " ns\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; i++) lista.UsunZIndeksu(500);
    end = chrono::high_resolution_clock::now();
    cout << "Lista: usuwanie z indeksu: " << (end - start).count() << " ns\n";

    return 0;
}

