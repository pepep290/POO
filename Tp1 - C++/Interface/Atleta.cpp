#include <iostream>
#include <string>
#include <vector>
#include "Atleta.h"
#include "Pais.h"

using namespace std;

// (Herança) - Construtor: repassa os dados básicos para o construtor de Pessoa.
Atleta::Atleta(string nome, string genero, int ano, int p)
    : Pessoa(nullptr, nome, genero, ano), participacoes(p) {};

// Destrutor
Atleta::~Atleta() {}

// Getters
int Atleta::getParticipacoes() {
    return this->participacoes;
}

vector<Medalha *> Atleta::getMedalhas() {
    return this->medalhas;
}

// Setters
void Atleta::setParticipacoes(int participacoes) {
    this->participacoes = participacoes;
}

void Atleta::setMedalhas(Medalha *medalha) {
    this->medalhas.push_back(medalha);
}

// (Polimorfismo) - exibirDados: exibe os dados herdados de Pessoa e os específicos do Atleta
void Atleta::exibirDados() {
    Pessoa::exibirDados();

    cout << "Participações Olímpicas: " << this->participacoes << endl;
    if (!this->medalhas.empty())
    {
        cout << "Medalhas conquistadas: " << this->medalhas.size() << endl;
        for (Medalha *m : this->medalhas) {
            cout << " - " << m->tipo << " (" << m->ano << ")" << endl;
        }
    }
    else {
        cout << "Nenhuma medalha cadastrada." << endl;
    }
}