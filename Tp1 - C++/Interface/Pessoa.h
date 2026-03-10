#ifndef PESSOA_H
#define PESSOA_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Pais;
class Modalidade;

class Pessoa {
private:
    Pais* pais;
    vector<Modalidade*> modalidades;
public:
    string nome;
    string genero;
    int anoNascimento;

    // Construtor
    Pessoa( Pais* pais, string nome, string genero, int anoNascimento);

    // Destrutor
    virtual ~Pessoa();

    // Getters
    Pais* getPais();
    string getNome();
    vector<Modalidade*> getModalidade();

    // Setters
    void setPais(Pais* p);
    void setModalidade(Modalidade* m);

    virtual void exibirDados();
};
#endif // PESSOA_H