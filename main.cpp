#include <iostream>
#include <string>

// Funktion die Daten verarbeitet und ausgibt
void ausgabe(const std::string& nachricht, int wert) {
    std::cout << nachricht << ": " << wert << std::endl;
}

// Funktion die berechnet und andere Funktion aufruft
void berechne(int a, int b) {
    int summe = a + b;
    int produkt = a * b;

    ausgabe("Summe", summe);
    ausgabe("Produkt", produkt);
}

// Hauptfunktion die Daten empfängt und weiterleitet
void verarbeite(int zahl) {
    std::cout << "Eingabe: " << zahl << std::endl;

    berechne(zahl, zahl * 2);
}

int main() {
    std::cout << "=== Einfaches C++ Beispiel ===" << std::endl;

    // Daten an die Verarbeitungskette übergeben
    verarbeite(5);

    std::cout << std::endl;
    verarbeite(10);

    return 0;
}
