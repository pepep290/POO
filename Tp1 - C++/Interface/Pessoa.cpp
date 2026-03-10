#include "Pessoa.h"
#include "Pais.h"
#include "Modalidade.h"
#include <vector>

using namespace std;

// Construtor
Pessoa::Pessoa( Pais* p,string n, string g, int a ) : pais(p), nome(n), genero(g), anoNascimento(a) {}

// Destrutor
Pessoa::~Pessoa() {}

// Getters
Pais* Pessoa::getPais() {
    return this->pais;
}

vector<Modalidade*> Pessoa::getModalidade() {
    return this->modalidades;
}

string Pessoa::getNome() {
    return this->nome;
}

// Setters
void Pessoa::setPais(Pais* p) {
    this->pais = p;
}

void Pessoa::setModalidade(Modalidade* m) {
    if (m != nullptr) {
        this->modalidades.push_back(m);
    }
}

void Pessoa::exibirDados() {
    cout << "Nome: " << this->nome << endl;
    cout << "Gênero: " << this->genero << endl;
    cout << "Ano de Nascimento: " << this->anoNascimento << endl;
    if (this->getPais() != nullptr) {
        cout << "País: " << this->getPais() << endl;
    }
    else {
        cout << "País: Não associado" << endl;
    }
    vector<Modalidade *> mods = this->getModalidade();
    if (!mods.empty()) {
        cout << "Modalidades: ";
        for (size_t i = 0; i < mods.size(); i++) {
            cout << mods[i]->nome << (i < mods.size() - 1 ? ", " : ".");
        }
        cout << endl;
    }
}