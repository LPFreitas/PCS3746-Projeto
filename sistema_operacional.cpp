#include <string>
#include <map>
#include <utility>
#include <iostream>

#include "memoria.cpp"
#include "fila_de_prontos.cpp"

using namespace std;

class SistemaOperacional
{
    Memoria *memoria;
    FilaDeProntos *filaDeProntos;
    Processo *processoExecutando;
    int proxPIDdeUsuario, proxPIDdeSO;

public:
    SistemaOperacional(string modo, int tamanhoMemoria = 20)
    {
        memoria = new Memoria(tamanhoMemoria);
        filaDeProntos = new FilaDeProntos();
        processoExecutando = NULL;
        proxPIDdeUsuario = 1;
        proxPIDdeSO = 1;
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
        Processo *processoSO = new Processo(proxPIDdeUsuario, tipo, programa, numPosicoesMemoria);
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
        // cout << "Executando " << (*processoExecutando).getTipo() << " " << (*processoExecutando).getPID() << endl;
        if (processoExecutando == NULL)
            processoExecutando = dispatcher();
        else if ((*processoExecutando).getTipo() == "usuario")
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
            (*memoria).alocaMemoria((*processoExecutando).getNumPosicoesMemoria(), proxPIDdeUsuario);

            // Cria processo do tipo usuario
            vector<string> programa = (*processoExecutando).getPrograma();
            criaProcessoUsuario("usuario", programa);

            // Atualiza o processo executando
            processoExecutando = dispatcher();
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
        vector<string> programaExecutando = (*processoExecutando).getPrograma();
        int programaExecutandoPC = (*processoExecutando).getPC();
        string linhaExecutando = programaExecutando[programaExecutandoPC];

        cout << programaExecutandoPC << " " << linhaExecutando << endl;
        if (linhaExecutando == "HLT") // Final do programa do processo usuario
        {
            // Desaloca memoria do processo usuario
            (*memoria).desalocaMemoria((*processoExecutando).getPID());

            // Atualiza o processo executando
            processoExecutando = dispatcher();
        }
        else // Processo usuario nao chegou ao fim do programa
        {
            // Incrementa o PC do processo usario
            (*processoExecutando).incrementaPC();
        }

        return;
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
        // Se fila estiver vazia, nao ha processos a executar
        if ((*filaDeProntos).tamanho() == 0)
            return NULL;

        // Remove e escalona o primeiro processo da fila de prontos
        return (*filaDeProntos).tiraDaFila();
    }
};