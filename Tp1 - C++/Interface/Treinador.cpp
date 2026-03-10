#include "Treinador.h"
#include "Atleta.h"

// (Herança) - Construtor: repassa os dados básicos para o construtor de Pessoa.
Treinador::Treinador(string nome) 
    : Pessoa(nullptr, nome, "", 0) {
}

// Destrutor
Treinador::~Treinador() {}

// Cria o vinculo entre o treinador e o atleta
void Treinador::vincularAtleta(Atleta *atleta) {
    this->atletas.push_back(atleta);
}

// Getter
vector<Atleta *> Treinador::getAtletas() {
    return this->atletas;
}

// (Polimorfismo) - exibirDados: exibe os dados herdados de Pessoa e os específicos do Treinador
void Treinador::exibirDados() {
    Pessoa::exibirDados();
    cout << "--- Atletas Vinculados ---" << endl;
    if (atletas.empty()) {
        cout << "Nenhum atleta vinculado." << endl;
    } else {
        for (Atleta* a : atletas) {
            cout << "- " << a->getNome() << endl; 
        }
    }
}