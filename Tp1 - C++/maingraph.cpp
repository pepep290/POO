#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include "Pais.h"
#include "Atleta.h"
#include "Modalidade.h"
#include "Medalha.h"
#include "Treinador.h"

using namespace std;

//vetor global p facilitar acesso e escrita
vector<Pais*> g_paises;
vector<Atleta*> g_atletas;
vector<Modalidade*> g_modalidades;
vector<Medalha*> g_medalhas;
vector<Treinador*> g_treinadores;

//elemento da interface
HWND hDisplay;//caixa pra exibir o texto final

//pais
HWND hPaisNome, hPaisCod;

//modalidade
HWND hModNome, hModCod;

//campo atleta
HWND hPessoaNome, hPessoaGenero, hPessoaAno, hPessoaPaisLink; 

//campo treinador
HWND hTreinaNome, hTreinaPaisLink;
HWND hTreinaAtletaLink; // Campo auxiliar para vincular atletas

//campo premiação
HWND hPremioAtletaNome, hPremioTipo, hPremioAno;

// IDs dos Botões (Identificadores)
#define ID_BTN_ADD_PAIS     1
#define ID_BTN_ADD_MOD      2
#define ID_BTN_ADD_ATLETA   3
#define ID_BTN_ADD_TREINA   4
#define ID_BTN_PREMIAR      5
#define ID_BTN_RELATORIO    6
#define ID_BTN_VINCULA_ATL  7 

//funcoes aux
//funçao que pega o texto da janela
string GetText(HWND hEdit) {
    int len = GetWindowTextLengthA(hEdit); 
    if(len == 0) return "";
    vector<char> buf(len + 1);
    GetWindowTextA(hEdit, &buf[0], len + 1);
    return string(&buf[0]);
}

//encontrar objetos pelo nome
Pais* BuscarPaisPorNome(string nome) {
    for(auto p : g_paises) {
        //normaliza o nome para busca
        if(p->nome == nome) return p; 
    }
    return nullptr;
}

Atleta* BuscarAtletaPorNome(string nome) {
    for(auto a : g_atletas) {
        if(a->nome == nome) return a;
    }
    return nullptr;
}


//logica de atribuicao das classes


void AcaoCadastrarPais() {
    string nome = GetText(hPaisNome);
    string cod = GetText(hPaisCod);

    if (nome.empty() || cod.empty()) {
        MessageBoxA(NULL, "Preencha Nome e Codigo do Pais!", "Erro", MB_ICONERROR);
        return;
    }

    Pais* p = new Pais(cod, nome);
    g_paises.push_back(p);
    MessageBoxA(NULL, "Pais cadastrado!", "Sucesso", MB_OK);
}

void AcaoCadastrarModalidade() {
    string nome = GetText(hModNome);
    string cod = GetText(hModCod);
    
    // Simplificação: evento 0 (Olimpíadas) fixo
    Modalidade* m = new Modalidade(nome, cod, 0); 
    g_modalidades.push_back(m);
    MessageBoxA(NULL, "Modalidade cadastrada!", "Sucesso", MB_OK);
}

void AcaoCadastrarAtleta() {
    string nome = GetText(hPessoaNome);
    string genero = GetText(hPessoaGenero);
    string sAno = GetText(hPessoaAno);
    string nomePais = GetText(hPessoaPaisLink);

    if(nome.empty() || sAno.empty()) {
        MessageBoxA(NULL, "Preencha pelo menos Nome e Ano!", "Erro", MB_OK);
        return;
    }

    int ano = stoi(sAno);
    //cria o Atleta
    Atleta* novo = new Atleta(nome, genero, ano, 0);

    //tenta vincular ao País
    Pais* p = BuscarPaisPorNome(nomePais);
    if (p != nullptr) {
        novo->setPais(p);
        p->addAtleta(novo); 
    } else if (!nomePais.empty()) {
        MessageBoxA(NULL, "Pais nao encontrado! Atleta criado sem pais.", "Aviso", MB_OK);
    }

    g_atletas.push_back(novo);
    MessageBoxA(NULL, "Atleta Cadastrado!", "Sucesso", MB_OK);
}

//cadastra o treinador
void AcaoCadastrarTreinador() {
    string nome = GetText(hTreinaNome);
    string nomePais = GetText(hTreinaPaisLink);

    if (nome.empty() || nomePais.empty()) {
        MessageBoxA(NULL, "Nome do Treinador e Pais sao obrigatorios!", "Erro", MB_ICONERROR);
        return;
    }

    //cria o Treinador
    Treinador* novoTreinador = new Treinador(nome);

    //tenta vincular ao País
    Pais* p = BuscarPaisPorNome(nomePais);
    if (p != nullptr) {
        novoTreinador->setPais(p); 
    } else if (!nomePais.empty()) {
        MessageBoxA(NULL, "Pais nao encontrado! Treinador criado sem pais.", "Aviso", MB_OK);
    }

    g_treinadores.push_back(novoTreinador);
    MessageBoxA(NULL, "Treinador Cadastrado!", "Sucesso", MB_OK);
}

//VINCULAR ATLETA A TREINADOR
void AcaoVincularAtletaTreinador() {
    //assumime que o treinador a ser vinculado é o que está no campo hTreinaNome
    string nomeTreinador = GetText(hTreinaNome); 
    string nomeAtleta = GetText(hTreinaAtletaLink);

    if (nomeTreinador.empty() || nomeAtleta.empty()) {
        MessageBoxA(NULL, "Preencha o Nome do Treinador e o Nome do Atleta!", "Erro", MB_ICONERROR);
        return;
    }

    Treinador* t = nullptr;
    for (auto treinador : g_treinadores) {
        if (treinador->nome == nomeTreinador) {
            t = treinador;
            break;
        }
    }

    if (t == nullptr) {
        MessageBoxA(NULL, "ERRO: Treinador nao encontrado!", "Erro", MB_ICONERROR);
        return;
    }

    Atleta* a = BuscarAtletaPorNome(nomeAtleta);
    if (a == nullptr) {
        MessageBoxA(NULL, "ERRO: Atleta nao encontrado!", "Erro", MB_ICONERROR);
        return;
    }

    // vincula o atleta usando a função da sua classe Treinador
    t->vincularAtleta(a);
    MessageBoxA(NULL, "Atleta vinculado ao Treinador!", "Sucesso", MB_OK);
}

void AcaoPremiarAtleta() {
    string nomeAtleta = GetText(hPremioAtletaNome);
    string tipoMedalha = GetText(hPremioTipo); 
    string sAno = GetText(hPremioAno);

    Atleta* a = BuscarAtletaPorNome(nomeAtleta);
    if (a == nullptr) {
        MessageBoxA(NULL, "Atleta nao encontrado!", "Erro", MB_OK);
        return;
    }

    // Simplificação: A medalha é criada, não buscada (como no seu criaMedalha)
    Medalha* m = new Medalha(tipoMedalha, stoi(sAno), nullptr); 
    a->setMedalhas(m);
    g_medalhas.push_back(m); 

    MessageBoxA(NULL, "Medalha Adicionada!", "Sucesso", MB_OK);
}

void AcaoGerarRelatorio() {
    stringstream ss;
    ss << "=== QUADRO GERAL DE MEDALHAS ===\r\n\r\n";

    // Reimplementação da exibição do Quadro de Medalhas
    for(auto p : g_paises) {
        int ouro = 0, prata = 0, bronze = 0;
        
        for(auto a : p->getAtletas()) {
            for(auto m : a->getMedalhas()) {
                if(m->tipo == "ouro") ouro++;
                else if(m->tipo == "prata") prata++;
                else if(m->tipo == "bronze") bronze++;
            }
        }
        ss << "PAIS: " << p->nome << " | Ouro: " << ouro << " | Prata: " << prata << " | Bronze: " << bronze << "\r\n";
    }

    ss << "\r\n=== LISTA DE TREINADORES ===\r\n";
    for(auto t : g_treinadores) {
        ss << t->nome;
        if(t->getPais()) ss << " (" << t->getPais()->nome << ")";
        ss << "\r\n";
        
        // Exibe atletas vinculados (simulando Treinador::exibirDados)
        if (!t->getAtletas().empty()) {
            ss << "   -> Vinculados: ";
            for (size_t i = 0; i < t->getAtletas().size(); ++i) {
                ss << t->getAtletas()[i]->nome << (i < t->getAtletas().size() - 1 ? ", " : "");
            }
            ss << "\r\n";
        }
    }
    
    // Lista Atletas (exemplo simples)
    ss << "\r\n=== LISTA DE ATLETAS ===\r\n";
    for(auto a : g_atletas) {
        ss << a->nome << " - Pais: " << (a->getPais() ? a->getPais()->nome : "N/A") << "\r\n";
    }

    SetWindowTextA(hDisplay, ss.str().c_str());
}


//abre janela e lopp principal


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        //PAÍS E MODALIDADE
        CreateWindowExA(0, "STATIC", "CADASTRO PAIS/MODALIDADE", WS_VISIBLE | WS_CHILD, 10, 10, 250, 20, hwnd, NULL, NULL, NULL);
        // País
        CreateWindowExA(0, "STATIC", "Pais Nome:", WS_VISIBLE | WS_CHILD, 10, 30, 70, 20, hwnd, NULL, NULL, NULL);
        hPaisNome = CreateWindowExA(0, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 80, 30, 100, 20, hwnd, NULL, NULL, NULL);
        CreateWindowExA(0, "STATIC", "Cod:", WS_VISIBLE | WS_CHILD, 190, 30, 30, 20, hwnd, NULL, NULL, NULL);
        hPaisCod = CreateWindowExA(0, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 220, 30, 50, 20, hwnd, NULL, NULL, NULL);
        CreateWindowExA(0, "BUTTON", "Salvar Pais", WS_VISIBLE | WS_CHILD, 280, 30, 100, 20, hwnd, (HMENU)ID_BTN_ADD_PAIS, NULL, NULL);
        // Modalidade
        CreateWindowExA(0, "STATIC", "Mod Nome:", WS_VISIBLE | WS_CHILD, 10, 60, 70, 20, hwnd, NULL, NULL, NULL);
        hModNome = CreateWindowExA(0, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 80, 60, 100, 20, hwnd, NULL, NULL, NULL);
        CreateWindowExA(0, "STATIC", "Cod:", WS_VISIBLE | WS_CHILD, 190, 60, 30, 20, hwnd, NULL, NULL, NULL);
        hModCod = CreateWindowExA(0, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 220, 60, 50, 20, hwnd, NULL, NULL, NULL);
        CreateWindowExA(0, "BUTTON", "Salvar Mod.", WS_VISIBLE | WS_CHILD, 280, 60, 100, 20, hwnd, (HMENU)ID_BTN_ADD_MOD, NULL, NULL);

        //ATLETA
        CreateWindowExA(0, "STATIC", "CADASTRO ATLETA", WS_VISIBLE | WS_CHILD, 10, 95, 200, 20, hwnd, NULL, NULL, NULL);
        // Linha 1: Dados básicos
        CreateWindowExA(0, "STATIC", "Nome:", WS_VISIBLE | WS_CHILD, 10, 115, 40, 20, hwnd, NULL, NULL, NULL);
        hPessoaNome = CreateWindowExA(0, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 50, 115, 150, 20, hwnd, NULL, NULL, NULL);
        CreateWindowExA(0, "STATIC", "Gen:", WS_VISIBLE | WS_CHILD, 210, 115, 30, 20, hwnd, NULL, NULL, NULL);
        hPessoaGenero = CreateWindowExA(0, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 240, 115, 30, 20, hwnd, NULL, NULL, NULL);
        CreateWindowExA(0, "STATIC", "Ano:", WS_VISIBLE | WS_CHILD, 280, 115, 30, 20, hwnd, NULL, NULL, NULL);
        hPessoaAno = CreateWindowExA(0, "EDIT", "2000", WS_VISIBLE | WS_CHILD | WS_BORDER, 310, 115, 40, 20, hwnd, NULL, NULL, NULL);
        // Linha 2: Vínculo e Botão
        CreateWindowExA(0, "STATIC", "Vincular ao Pais (Nome):", WS_VISIBLE | WS_CHILD, 10, 145, 150, 20, hwnd, NULL, NULL, NULL);
        hPessoaPaisLink = CreateWindowExA(0, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 145, 150, 20, hwnd, NULL, NULL, NULL);
        CreateWindowExA(0, "BUTTON", "Salvar Atleta", WS_VISIBLE | WS_CHILD, 320, 145, 140, 20, hwnd, (HMENU)ID_BTN_ADD_ATLETA, NULL, NULL);
        
        //TREINADOR
        CreateWindowExA(0, "STATIC", "CADASTRO TREINADOR", WS_VISIBLE | WS_CHILD, 10, 180, 200, 20, hwnd, NULL, NULL, NULL);
        // Linha 1: Dados básicos
        CreateWindowExA(0, "STATIC", "Nome Treinador:", WS_VISIBLE | WS_CHILD, 10, 200, 100, 20, hwnd, NULL, NULL, NULL);
        hTreinaNome = CreateWindowExA(0, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 110, 200, 150, 20, hwnd, NULL, NULL, NULL);
        CreateWindowExA(0, "STATIC", "Vincular ao Pais (Nome):", WS_VISIBLE | WS_CHILD, 10, 230, 150, 20, hwnd, NULL, NULL, NULL);
        hTreinaPaisLink = CreateWindowExA(0, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 230, 150, 20, hwnd, NULL, NULL, NULL);
        CreateWindowExA(0, "BUTTON", "Salvar Treinador", WS_VISIBLE | WS_CHILD, 320, 230, 140, 20, hwnd, (HMENU)ID_BTN_ADD_TREINA, NULL, NULL);
        // Linha 2: Vínculo Atleta
        CreateWindowExA(0, "STATIC", "Vincular Atleta (Nome):", WS_VISIBLE | WS_CHILD, 10, 260, 150, 20, hwnd, NULL, NULL, NULL);
        hTreinaAtletaLink = CreateWindowExA(0, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 160, 260, 150, 20, hwnd, NULL, NULL, NULL);
        CreateWindowExA(0, "BUTTON", "Vincular Atleta", WS_VISIBLE | WS_CHILD, 320, 260, 140, 20, hwnd, (HMENU)ID_BTN_VINCULA_ATL, NULL, NULL);


        // PREMIAÇÃO
        CreateWindowExA(0, "STATIC", "PREMIAR ATLETA", WS_VISIBLE | WS_CHILD, 10, 290, 200, 20, hwnd, NULL, NULL, NULL);
        CreateWindowExA(0, "STATIC", "Nome Atleta:", WS_VISIBLE | WS_CHILD, 10, 310, 80, 20, hwnd, NULL, NULL, NULL);
        hPremioAtletaNome = CreateWindowExA(0, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 90, 310, 150, 20, hwnd, NULL, NULL, NULL);
        CreateWindowExA(0, "STATIC", "Medalha (ouro/prata/bronze):", WS_VISIBLE | WS_CHILD, 250, 310, 180, 20, hwnd, NULL, NULL, NULL);
        hPremioTipo = CreateWindowExA(0, "EDIT", "ouro", WS_VISIBLE | WS_CHILD | WS_BORDER, 430, 310, 60, 20, hwnd, NULL, NULL, NULL);
        CreateWindowExA(0, "STATIC", "Ano:", WS_VISIBLE | WS_CHILD, 500, 310, 30, 20, hwnd, NULL, NULL, NULL);
        hPremioAno = CreateWindowExA(0, "EDIT", "2024", WS_VISIBLE | WS_CHILD | WS_BORDER, 530, 310, 40, 20, hwnd, NULL, NULL, NULL);
        CreateWindowExA(0, "BUTTON", "Dar Medalha", WS_VISIBLE | WS_CHILD, 10, 340, 150, 20, hwnd, (HMENU)ID_BTN_PREMIAR, NULL, NULL);


        // RELATÓRIOS 
        CreateWindowExA(0, "BUTTON", "ATUALIZAR QUADRO DE MEDALHAS E RELATORIOS", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
                        10, 380, 560, 30, hwnd, (HMENU)ID_BTN_RELATORIO, NULL, NULL);
        hDisplay = CreateWindowExA(0, "EDIT", "", 
                        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY, 
                        10, 420, 560, 250, hwnd, NULL, NULL, NULL);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
            case ID_BTN_ADD_PAIS: AcaoCadastrarPais(); break;
            case ID_BTN_ADD_MOD: AcaoCadastrarModalidade(); break;
            case ID_BTN_ADD_ATLETA: AcaoCadastrarAtleta(); break;
            case ID_BTN_ADD_TREINA: AcaoCadastrarTreinador(); break; // NOVO
            case ID_BTN_VINCULA_ATL: AcaoVincularAtletaTreinador(); break; // NOVO
            case ID_BTN_PREMIAR: AcaoPremiarAtleta(); break;
            case ID_BTN_RELATORIO: AcaoGerarRelatorio(); break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASSA wc = {0}; 
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = "ClasseJanelaOlimpica";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClassA(&wc)) return -1; 

    CreateWindowExA(0, "ClasseJanelaOlimpica", "Sistema Olímpico", 
                  WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
                  50, 50, 600, 720, NULL, NULL, NULL, NULL);

    MSG msg = {0};
    while (GetMessageA(&msg, NULL, 0, 0)) { // Use GetMessageA
        TranslateMessage(&msg);
        DispatchMessageA(&msg); // Use DispatchMessageA
    }
    return 0;
}