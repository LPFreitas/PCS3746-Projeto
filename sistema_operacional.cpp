#include <iostream>
#include <vector>
#include <map>
#include <string>
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
        Processo *processoSO = new Processo(proxPIDdeSO, tipo, programa, numPosicoesMemoria);
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
        // PENDENTE
    }

    void executa()
    {
        Processo *processoAnterior = processoExecutando;
        string linhaExecutadaAnteriormente = linhaExecutando;

        // Nada a executar e nada foi executado
        if (processoAnterior == NULL && filaDeProntos == NULL)
            return;
        // Fila de prontos com processo, mas nenhum executado (Ex.: primeiro create do sistema vai executar)
        else if (processoAnterior == NULL)
        {
            executaProximoProcesso();
            return;
        }
        // Algum processo já foi executado, estando a fila de prontos vazia ou não
        else
        {
            // Processo SO em execução terminou, chamamos o próximo processo a ser executado da fila
            if ((*processoAnterior).getTipo() != "usuario")
                executaProximoProcesso();
            // Processo usuário em execução acabou na última iteração, chamamos o próximo e já começamos sua execução
            else if (linhaExecutadaAnteriormente == "HLT")
                executaProximoProcesso();
            // Processo usuário ainda não terminou de executar
            else
                executaProcessoUsuario();
        }

        // Processo usuário em execução chegou ao fim, desalocamos as memórias
        if (linhaExecutando == "HLT")
            desalocaProcessoUsuario();
    }

    void executaProximoProcesso()
    {
        linhaExecutando = "";
        processoExecutando = dispatcher();
        if (processoExecutando == NULL)
            return;
        if ((*processoExecutando).getTipo() == "usuario")
            executaProcessoUsuario();
        else
            executaProcessoSO();
    }

    void executaProcessoSO()
    {
        string tipoProcessoExecutando = (*processoExecutando).getTipo();
        if (tipoProcessoExecutando == "create")
        {
            // Aloca memória para o processo de usuário a ser criado
            bool alocou = (*memoria).alocaMemoria((*processoExecutando).getNumPosicoesMemoria(), proxPIDdeUsuario);
            if (!alocou)
            {
                cerr << "Erro ao alocar memória para o processo usuário " << proxPIDdeUsuario << ": memória insuficiente!" << endl;
                cout << "Não foi possível criar o processo usuário " << proxPIDdeUsuario << endl;
                return;
            }

            // Cria processo do tipo usuário
            vector<string> programa = (*processoExecutando).getPrograma();
            criaProcessoUsuario("usuario", programa);
        }
        else if (tipoProcessoExecutando == "kill")
        {
            // PENDENTE

            // Desaloca a memória
            // Mata processo do tipo usuário - mataProcessoUsuario();
        }
    }

    void executaProcessoUsuario()
    {
        vector<string> programaExecutando = (*processoExecutando).getPrograma();
        int programaExecutandoPC = (*processoExecutando).getPC();
        linhaExecutando = programaExecutando[programaExecutandoPC];

        // Imprime Status e TCB do processo executando
        (*processoExecutando).imprimeStatusTCB();

        if (linhaExecutando != "HLT") // Final do programa do processo usuário
            (*processoExecutando).incrementaPC();
    }

    void desalocaProcessoUsuario()
    {
        // Desaloca memória do processo usuário
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
        // FIFO
        return escalonador();

        // Robin
        // salva tcb ?
    }

    // FIFO
    Processo *escalonador()
    {
        // Se fila estiver vazia, não há processos a executar
        if ((*filaDeProntos).tamanho() == 0)
            return NULL;

        // Remove e escalona o primeiro processo da fila de prontos
        return (*filaDeProntos).tiraDaFila();
    }
};