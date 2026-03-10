#include "Modalidade.h"
#include <iostream>

// Construtor
Modalidade::Modalidade(string n, string c, int e) : nome(n), codigo(c), evento(e) {}

// Exibe os Dados de Modalidade
void Modalidade::exibirDados() {
    cout << "\nNome: " << nome;
    cout << "\nCodigo: " << codigo;
    cout << "\nEvento: " << evento;
}