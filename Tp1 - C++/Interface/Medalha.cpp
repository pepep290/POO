#include "Medalha.h"
#include "Modalidade.h"

using namespace std;

// Construtor
Medalha::Medalha(string t, int a, Modalidade* m)
    : tipo(t), ano(a), modalidade(m) {
}

// Destrutor
Medalha::~Medalha() {}

// Mostra dados de medalha
void Medalha::exibirDados() {
    cout << "  [Medalha]: " << tipo << " (" << ano << ")";
    if (modalidade != nullptr) {
        cout << " - Modalidade: " << modalidade->nome << endl;
    } 
    else {
        cout << " - Modalidade: Nao informada" << endl;
    }
}