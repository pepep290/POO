#include <iostream>
#include <vector>
#include "procedimentos.h"

using namespace std;

int main()
{
    int opcao;
    vector<Pais *> paises;
    vector<Modalidade *> modalidades;
    vector<Medalha *> medalhas;
    vector<Atleta *> atletas;
    vector<Treinador *> treinadores;

    do
    {
        cout << endl;
        cout << "====SISTEMA DE GESTAO DE EVENTOS OLIMPICOS====" << endl;
        cout << "1 - Cadastrar país" << endl;
        cout << "2 - Cadastrar atleta" << endl;
        cout << "3 - Cadastrar treinador" << endl;
        cout << "4 - Criar modalidade" << endl;
        cout << "5 - Criar medalha" << endl;
        cout << "6 - Premiar atleta com medalha" << endl;
        cout << "7 - Exibir quadro de medalhas" << endl;
        cout << "8 - Listar atletas de uma modalidade" << endl;
        cout << "9 - Gerar relatórios e estatísticas" << endl;
        cout << "10 - Carregar dados" << endl;
        cout << "11 - Salvar dados e sair" << endl;
        cout << "====================================" << endl;
        cout << "Escolha uma opção:" << endl;
        cin >> opcao;
        while (cin.fail())
        {
            cin.clear();            // limpa o erro
            cin.ignore(1000, '\n'); // descarta o restante da entrada
            cout << "ERRO: A opção escolhida é inválida" << endl;
            cin >> opcao;
        }
        cout << "====================================" << endl;
        switch (opcao)
        {
        case 1:
            CadastrarPais(paises);
            break;
        case 2:
            cadastrarAtleta(atletas, paises, modalidades);
            break;
        case 3:
            cadastrarTreinador(treinadores, paises, modalidades, atletas);
            break;
        case 4:
            criaModalidade(modalidades);
            break;
        case 5:
            criaMedalha(medalhas, modalidades);
            break;
        case 6:
            premiarAtletaComMedalha(atletas, medalhas);
            break;
        case 7:
            exibirQuadroDeMedalhas(paises);
            break;
        case 8:
            listarAtletas(atletas, modalidades);
            break;
        case 9:
            gerarRelatoriosEEstatisticas(atletas, paises, medalhas);
            break;
        case 10:
            carregarDados(atletas, medalhas, modalidades, paises, treinadores);
            break;
        case 11:
            salvarDados(atletas, medalhas, modalidades, paises, treinadores);
            break;
        default:
            cout << "ERRO: A opção escolhida é inválida" << endl;
            break;
        }
    } while (opcao != 11);
}