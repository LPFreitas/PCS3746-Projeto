#include <string>
#include <map>
#include <utility>
#include <iostream>
// #include "processo_so.cpp"
// #include "processo_usuario.cpp"
#include "processo.cpp"
#include "fila_de_prontos.cpp"

using namespace std;

class SistemaOperacional
{
    // MapaDeBits mapa;
    FilaDeProntos filaDeProntos;
    Processo *processoExecutando;
    int proxPIDdeUsuario;
    int proxPIDdeSO;

public:
    SistemaOperacional(string modo)
    {
        // if (modo == "robin")
        //     escalonador = EscalonadorRobin();
        // else
        //     escalonador = EscalonadorFIFO();

        // escalonador = (modo == "robin") ? EscalonadorRobin() : EscalonadorFIFO();
        // mapa = MapaDeBits();
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

    void criaProcessoSO(string tipo, int numPosicoesMemoria, vector<string> programa)
    {
        Processo *processoSO = new Processo(proxPIDdeSO, numPosicoesMemoria, tipo, programa);
        incrementaProximoPIDdeSO();
        filaDeProntos.insereNaFila(*processoSO);
    }

    void criaProcessoUsuario(string tipo, vector<string> programa)
    {
        Processo *processoUsuario = new Processo(proxPIDdeUsuario, tipo, programa);
        incrementaProximoPIDdeUsuario();
        filaDeProntos.insereNaFila(*processoUsuario);
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
            // Aloca memoria
            vector<string> programa = (*processoExecutando).getPrograma();
            criaProcessoUsuario("usuario", programa);
            processoExecutando = dispatcher();
        }
        else if (tipoProcessoExecutando == "kill")
        {
            // robin
            // Desaloca a memoria
            // mata processo do tipo usuario - mataProcessoUsuario();
        }
    }

    void executaProcessoUsuario()
    {
        ProcessoUsuario processoUsuarioExecutando = ProcessoUsuario::processoParaProcessoUsuario(*processoExecutando);
        vector<string> programaExecutando = processoUsuarioExecutando.getPrograma();
        int programaExecutandoPC = processoUsuarioExecutando.getPC();
        string linhaExecutando = programaExecutando[programaExecutandoPC];

        // if (processoExecutando == NULL)
        //     return;

        cout << programaExecutandoPC << " " << linhaExecutando << endl;
        if (linhaExecutando == "HLT")
        {
            // desaloca memoria do processo
            processoExecutando = dispatcher();
        }
        else
        {
            processoUsuarioExecutando.incrementaPC();
        }

        // revisar/melhorar - usar um metodo?
        cout << "PC antes " << processoUsuarioExecutando.getPC() << endl;
        processoExecutando = &processoUsuarioExecutando;
        cout << "PC depois " << ProcessoUsuario::processoParaProcessoUsuario(*processoExecutando).getPC() << endl;

        return;
    }

    // void insereFilaDeProntos(Processo processo)
    // {
    //     filaDeProntos.push(processo);
    // }

    // Processo *removeFilaDeProntos()
    // {
    //     Processo *primeiroProcesso = &(filaDeProntos.front());
    //     filaDeProntos.pop();
    //     return primeiroProcesso;
    // }

    FilaDeProntos getFilaDeProntos()
    {
        return filaDeProntos;
    }

    // int getTamanhoFilaDeProntos()
    // {
    //     return filaDeProntos.size();
    // }

    Processo *dispatcher()
    {
        // robin
        // salva tcb

        // fifo
        return escalonador();
    }

    // FIFO
    Processo *escalonador()
    {
        if (filaDeProntos.size() == 0)
            return NULL;

        return filaDeProntos.tiraDaFila();
    }
};