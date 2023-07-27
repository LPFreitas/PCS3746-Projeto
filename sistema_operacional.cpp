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
    int proxPIDdeUsuario, proxPIDdeUsuarioAux, proxPIDdeSO;
    string linhaExecutando;

    // Atributos para modo robin
    int quantum;
    int contadorRelogioQuantum;

    // Atributo para compactação
    int tempoCompactacao;
    int contadorRelogioCompactacao;

public:
    SistemaOperacional(string modo, int tamanhoMemoria = 20)
    {
        this->modo = modo;
        memoria = new Memoria(tamanhoMemoria);
        filaDeProntos = new FilaDeProntos();
        processoExecutando = NULL;
        proxPIDdeUsuario = 1;
        proxPIDdeUsuarioAux = 1;
        proxPIDdeSO = 1;
        linhaExecutando = "";
        quantum = 2;
        contadorRelogioQuantum = 0;
        tempoCompactacao = 4;
        contadorRelogioCompactacao = 0;
    }

    void incrementaProxPIDdeUsuario()
    {
        proxPIDdeUsuario += 1;
    }

    void incrementaProxPIDdeUsuarioAux()
    {
        proxPIDdeUsuarioAux += 1;
    }

    void decrementaProxPIDdeUsuarioAux()
    {
        proxPIDdeUsuarioAux -= 1;
    }

    void incrementaProxPIDdeSO()
    {
        proxPIDdeSO += 1;
    }

    void criaProcessoSOCreate(string tipo, vector<string> programa, int numPosicoesMemoria)
    {
        Processo *processoSOCreate = new Processo(proxPIDdeSO, tipo, programa, numPosicoesMemoria, proxPIDdeUsuarioAux);
        incrementaProxPIDdeUsuarioAux();
        incrementaProxPIDdeSO();
        (*filaDeProntos).insereNaFila(*processoSOCreate);
    }

    void criaProcessoSOKill(string tipo, int processoUsuarioPID)
    {
        Processo *processoSOKill = new Processo(proxPIDdeSO, tipo, processoUsuarioPID);
        incrementaProxPIDdeSO();
        (*filaDeProntos).insereNaFila(*processoSOKill);
    }

    void criaProcessoUsuario(string tipo, vector<string> programa)
    {
        Processo *processoUsuario = new Processo(proxPIDdeUsuario, tipo, programa);
        incrementaProxPIDdeUsuario();
        (*filaDeProntos).insereNaFila(*processoUsuario);
    }

    void mataProcessoUsuario(string tipo, int PID)
    {
        (*filaDeProntos).removeElementoFila(tipo, PID);
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
            executaProximoProcesso();
        // Algum processo já foi executado, estando a fila de prontos vazia ou não
        else
        {
            bool processoAnteriorTerminou = (*processoAnterior).getTipo() != "usuario" || linhaExecutadaAnteriormente == "HLT";
            bool robinTerminou = modo == "robin" && contadorRelogioQuantum == quantum;

            // Processo anterior em execução terminou, chamamos o próximo processo a ser executado da fila e já executa
            if (processoAnteriorTerminou || robinTerminou)
                executaProximoProcesso();
            // Processo usuário ainda não terminou de executar
            else
                executaProcessoUsuario();
        }

        // Processo usuário em execução chegou ao fim, desalocamos as memórias
        if (linhaExecutando == "HLT")
            desalocaProcessoUsuario();

        contadorRelogioQuantum += 1;
        contadorRelogioCompactacao += 1;

        if (contadorRelogioCompactacao == tempoCompactacao)
        {
            (*memoria).compactaMemoria();
            contadorRelogioCompactacao = 0;
        }
    }

    void executaProximoProcesso()
    {
        processoExecutando = dispatcher();
        linhaExecutando = "";
        contadorRelogioQuantum = 0;
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
        int processoUsuarioPID = (*processoExecutando).getProcessoUsuarioPID();

        cout << "Executando processo " << tipoProcessoExecutando << " " << processoUsuarioPID << endl
             << endl;

        if (tipoProcessoExecutando == "create")
        {
            // Aloca memória para o processo de usuário a ser criado
            bool alocou = (*memoria).alocaMemoria((*processoExecutando).getNumPosicoesMemoria(), proxPIDdeUsuario);
            if (!alocou)
            {
                cerr << "Erro ao alocar memória para o processo usuário " << proxPIDdeUsuario << ": memória insuficiente!" << endl
                     << endl;
                cout << "Não foi possível criar o processo usuário " << proxPIDdeUsuario << endl
                     << endl;
                // Decrementa variável auxiliar para manter coerência
                decrementaProxPIDdeUsuarioAux();
                return;
            }

            // Cria processo do tipo usuário
            vector<string> programa = (*processoExecutando).getPrograma();
            criaProcessoUsuario("usuario", programa);
        }
        else if (tipoProcessoExecutando == "kill")
        {
            // Obtém PID do processo usuário a ser encerrado
            int processoUsuarioPID = (*processoExecutando).getProcessoUsuarioPID();

            // Verifica se processo usuário está na fila de prontos
            if (!(*filaDeProntos).estaPresente("usuario", processoUsuarioPID))
            {
                cout << "O processo usuário " << processoUsuarioPID << " ainda não foi criado ou já executou!" << endl
                     << endl;
                return;
            }

            // Desaloca a memória do processo usuário
            bool desalocou = (*memoria).desalocaMemoria(processoUsuarioPID);
            if (!desalocou)
            {
                cerr << "Erro ao desalocar memória para o processo usuário " << processoUsuarioPID << endl
                     << endl;
                cout << "Não foi possível matar o processo usuário " << processoUsuarioPID << endl
                     << endl;
                return;
            }

            // Mata o processo usuário
            mataProcessoUsuario("usuario", processoUsuarioPID);
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
            cerr << "Erro ao desalocar memória para o processo usuário " << processoExecutandoPID << endl
                 << endl;
            cout << "Não foi possível encerrar o processo usuário " << processoExecutandoPID << ": memória não desalocou!" << endl
                 << endl;
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

        // Se não existe próximo, o processo atual pode continuar executando
        if (processoProximo == nullptr)
            return processoAnterior;

        (*filaDeProntos).insereNaFila(*processoAnterior);

        return processoProximo;
    }

    Processo *escalonador()
    {
        // Se fila estiver vazia, não há processos a executar
        if ((*filaDeProntos).tamanho() == 0)
            return NULL;

        // Remove e escalona o primeiro processo da fila de prontos
        return (*filaDeProntos).tiraDaFila();
    }
};