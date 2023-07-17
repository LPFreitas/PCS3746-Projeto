#include <string>
#include <map>
#include <queue>
#include <utility>
#include <iostream>
// #include "processo.cpp"
#include "processo_so.cpp"
#include "processo_usuario.cpp"

using namespace std;

class SistemaOperacional
{
    // MapaDeBits mapa;
    queue<Processo> filaDeProntos;
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
        proxPIDdeUsuario++;
    }

    void incrementaProximoPIDdeSO()
    {
        proxPIDdeUsuario++;
    }

    void criaProcessoSO(string tipo, int numPosicoesMemoria, vector<string> programa)
    {
        ProcessoSO processoSO = ProcessoSO(proxPIDdeSO, tipo, numPosicoesMemoria, programa);
        incrementaProximoPIDdeSO();
        insereFilaDeProntos(processoSO);
    }

    void criaProcessoUsuario(string tipo, vector<string> programa)
    {
        ProcessoUsuario processoUsuario = ProcessoUsuario(proxPIDdeUsuario, tipo, programa);
        incrementaProximoPIDdeUsuario();
        insereFilaDeProntos(processoUsuario);
    }

    void mataProcessoUsuario(int PID) // ?
    {
    }

    void executa()
    {
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

        cout << processoUsuarioExecutando.getPC() << " " << linhaExecutando << endl;
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
        *(processoExecutando) = processoUsuarioExecutando;

        return;
    }

    void insereFilaDeProntos(Processo processo)
    {
        filaDeProntos.push(processo);
    }

    Processo *removeFilaDeProntos()
    {
        Processo *primeiroProcesso = &(filaDeProntos.front());
        filaDeProntos.pop();
        return primeiroProcesso;
    }

    queue<Processo> getFilaDeProntos()
    {
        return filaDeProntos;
    }

    int getTamanhoFilaDeProntos()
    {
        return filaDeProntos.size();
    }

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
        if (getTamanhoFilaDeProntos() == 0)
            return NULL;

        return removeFilaDeProntos();
    }
};