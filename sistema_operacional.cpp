#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <utility>

#include "memoria.cpp"
#include "fila_de_prontos.cpp"

using namespace std;

class SistemaOperacional
{
    string modo;
    Memoria *memoria;
    FilaDeProntos *filaDeProntos;
    Processo *processoExecutando;
    int proxPIDdeUsuario, proxPIDdeSO;
    string linhaExecutando;
    int quantum;
    int contadorRelogio;

public:
    SistemaOperacional(string modo, int tamanhoMemoria = 20)
    {
        this->modo = modo;
        memoria = new Memoria(tamanhoMemoria);
        filaDeProntos = new FilaDeProntos();
        processoExecutando = NULL;
        proxPIDdeUsuario = 1;
        proxPIDdeSO = 1;
        linhaExecutando = "";
        quantum = 2;
        contadorRelogio = 0;
    }

    void incrementaProximoPIDdeUsuario()
    {
        proxPIDdeUsuario += 1;
    }

    void incrementaProximoPIDdeSO()
    {
        proxPIDdeSO += 1;
    }

    void criaProcessoSO(string tipo, vector<string> programa, int numPosicoesMemoria)
    {
        Processo *processoSO = new Processo( proxPIDdeSO, tipo, programa, numPosicoesMemoria);
        incrementaProximoPIDdeSO();
        (*filaDeProntos).insereNaFila(*processoSO);
    }

    void criaProcessoUsuario(string tipo, vector<string> programa)
    {
        Processo *processoUsuario = new Processo(proxPIDdeUsuario, tipo, programa);
        incrementaProximoPIDdeUsuario();
        (*filaDeProntos).insereNaFila(*processoUsuario);
    }

    void mataProcessoUsuario(int PID) // ?
    {
    }

    void executa()
    {
        Processo *processoAnterior = processoExecutando;
        string linhaExecutadaAnteriormente = linhaExecutando;
        
        // Nada a executar e nada foi executado
        if (processoAnterior == NULL && filaDeProntos == NULL)
            return;
        // Fila de prontos com processo, mas nenhum executado (Ex.: primeiro create do sistema vai executar) 
        else if (processoAnterior == NULL) {
            executaProximoProcesso();
        // Algum processo já foi executado, estando a fila de prontos vazia ou não
        } else {
            bool processoAnteriorTerminou = (*processoAnterior).getTipo()  != "usuario" || linhaExecutadaAnteriormente == "HLT";
            bool robinTerminou = modo == "robin" && contadorRelogio == quantum;
            // Processo anterior em execução terminou, chamamos o próximo processo a ser executado da fila e já executa
            if(processoAnteriorTerminou || robinTerminou)
                executaProximoProcesso();
            // Processo usuário ainda não terminou de executar
            else
                executaProcessoUsuario(); 
        }
        // Processo usuário em execução chegou ao fim, desalocamos as memórias
        if (linhaExecutando == "HLT") {
            desalocaProcessoUsuario();
        }

        contadorRelogio++;
    }

    void executaProximoProcesso() {
        processoExecutando = dispatcher();
        linhaExecutando = "";
        contadorRelogio = 0;
        if (processoExecutando == NULL)
            return;
        if( (*processoExecutando).getTipo()  == "usuario")
            executaProcessoUsuario();
        else
            executaProcessoSO();
    }

    void executaProcessoSO()
    {
        string tipoProcessoExecutando = (*processoExecutando).getTipo();
        if (tipoProcessoExecutando == "create")
        {
            // Aloca memoria para o processo de usuario a ser criado
            bool alocou = (*memoria).alocaMemoria((*processoExecutando).getNumPosicoesMemoria(), proxPIDdeUsuario);
            if (!alocou)
            {
                cerr << "Erro ao alocar memória para o processo usuário " << proxPIDdeUsuario << ": memória insuficiente!" << endl;
                cout << "Não foi possível criar o processo usuário " << proxPIDdeUsuario << endl;
                return;
            }

            // Cria processo do tipo usuario
            vector<string> programa = (*processoExecutando).getPrograma();
            criaProcessoUsuario("usuario", programa);

            // Atualiza o processo executando
            // processoExecutando = dispatcher();
        }
        else if (tipoProcessoExecutando == "kill")
        {
            // Robin
            // Desaloca a memoria
            // mata processo do tipo usuario - mataProcessoUsuario();
        }
    }

    void executaProcessoUsuario()
    {
        int processoExecutandoPID = (*processoExecutando).getPID();
        vector<string> programaExecutando = (*processoExecutando).getPrograma();
        int programaExecutandoPC = (*processoExecutando).getPC();
        linhaExecutando = programaExecutando[programaExecutandoPC];

        // imprime o status do programa 
        imprimeProgramaEmExecucao(programaExecutando, programaExecutandoPC);
        // encontra quais registradores o processo usa e usa para imprimir a TCB
        set<string> registradoresEncontrados = encontraRegistradores(programaExecutando);
        imprimeTCB(registradoresEncontrados, processoExecutandoPID, programaExecutandoPC);

        if (linhaExecutando != "HLT") // Final do programa do processo usuario
            (*processoExecutando).incrementaPC();    
    }

    void imprimeProgramaEmExecucao(const vector<string> &programa, int linhaAtual)
    {
         // Imprime o cabeçalho
    cout << "+--------------------+" << endl;
    cout << "|       Status       |" << endl;
    cout << "+--------------------+" << endl;

     // Imprime o programa com a seta na linha atual
    for (int i = 0; i < programa.size(); ++i)
    {
        if (i == linhaAtual)
            cout << "| " << left << setw(13) << programa[i] << " <--- |" << endl;
        else
            cout << "| " << left << setw(18) << programa[i] << " |" << endl;
    }

    // Imprime a moldura inferior do programa
    cout << "+--------------------+" << endl;
    }


    set<string> encontraRegistradores(const vector<string>& programa) {
        set<string> registradores;
        vector<string> registradoresPossiveis = { "AX", "BX", "CX", "DX" };

        for (const string& linha : programa) {
            for (const string& registrador : registradoresPossiveis) {
                if (linha.find(registrador) != string::npos) {
                    registradores.insert(registrador);
                }
            }
        }

        return registradores;
    }

    void imprimeTCB(const set<string>& registradoresEncontrados, int PID, int PC) {
        cout << endl;
        cout << "+----------------------------------+" << endl;
        cout << "|          TCB do Processo         |" << endl;
        cout << "+----------------------------------+" << endl;
        cout << "| PID: " << setw(27) << PID << " |" << endl;
        
        // cout << "PID: " << PID << endl;
        for (const string& registrador : registradoresEncontrados) {
             cout << "| REG " << setw(28) << registrador << " |" << endl;
        }
         cout << "| PC: " << setw(28) << PC << " |" << endl;
        cout << "+----------------------------------+" << endl;
        cout << endl;
    }

    void desalocaProcessoUsuario() {
        // Desaloca memoria do processo usuario
        int processoExecutandoPID = (*processoExecutando).getPID();
        bool desalocou = (*memoria).desalocaMemoria(processoExecutandoPID);
        if (!desalocou)
        {
            cerr << "Erro ao desalocar memória para o processo usuário " << processoExecutandoPID << endl;
            cout << "Não foi possível encerrar o processo usuário " << processoExecutandoPID << ": memória não desalocou!" << endl;
        }
    }

    Memoria getMemoria()
    {
        return (*memoria);
    }

    FilaDeProntos getFilaDeProntos()
    {
        return (*filaDeProntos);
    }

    Processo *dispatcher()
    {
        if (processoExecutando == NULL || modo == "fifo" || (*processoExecutando).getTipo() != "usuario" || linhaExecutando == "HLT")
            return escalonador();

        Processo *processoAnterior = processoExecutando;
        Processo *processoProximo = escalonador();

        // se nao existe proximo, o processo atual pode continuar executando
        if (processoProximo == nullptr) {
            return processoAnterior;
        }

        (*filaDeProntos).insereNaFila(*processoAnterior);
        
        return processoProximo;
    }

    // FIFO
    Processo *escalonador()
    {
        // Se fila estiver vazia, nao ha processos a executar
        if ((*filaDeProntos).tamanho() == 0)
            return NULL;

        // Remove e escalona o primeiro processo da fila de prontos
        return (*filaDeProntos).tiraDaFila();
    }
};