#include <string>
#include <map>
#include <queue>
#include <utility>

using namespace std;

class SistemaOperacional
{
    Escalonador escalonador;
    MapaDeBits mapa;
    Dispatcher dispatcher;
    queue<pair<string, int>> filaDeProntos;
    int proxPIDdeUsuario;
    int proxPIDdeSO;

public:
    SistemaOperacional(string modo) // cria os objetos de outras classes (usa construtor das outras classes)
    {
        if (modo == "robin")
            this->escalonador = EscalonadorRobin();
        else
            this->escalonador = EscalonadorFIFO();

        // this->escalonador = (modo == "robin") ? EscalonadorRobin() : EscalonadorFIFO();
        this->mapa = MapaDeBits();
        this->dispatcher = Dispatcher();
        proxPIDdeUsuario = 1;
    }

    void atualizaProximoPID()
    {
        proxPIDdeUsuario += 1;
    } // incrementa proximoPID
};

class Dispatcher
{

public:
    Dispatcher()
    {
    }
};

class Escalonador
{
};

class EscalonadorFIFO : public Escalonador
{
public:
    EscalonadorFIFO(){};

    pair<string, int> atualizaFila()
    {
    }
};

class EscalonadorRobin : public Escalonador
{
public:
    EscalonadorRobin(){};

    void atualizaFila(int PID)
    {
    }
};

class TaskControlBlock
{
private:
    int PID, PC;
    map<string, int> regs;

public:
    TaskControlBlock(int PID)
    {
        this->PID = PID;
        this->PC = 0;
        this->regs["AX"] = 0;
        this->regs["BX"] = 0;
        this->regs["CX"] = 0;
        this->regs["DX"] = 0;
    }

    void setTCB()
    {
    }

    void getTCB()
    {
    }
};

class Processo
{
private:
    int PID, PC;
    string tipo = "usuario"; // "so"
    map<string, int> regs;
    TaskControlBlock processoTCB;
    string *programa;

public:
    Processo(int PID, string *programa)
    {
        this->PID = PID;
        this->PC = 0;
        this->regs["AX"] = 0;
        this->regs["BX"] = 0;
        this->regs["CX"] = 0;
        this->regs["DX"] = 0;
        this->processoTCB = TaskControlBlock(PID);
        this->programa = programa;
    }

    void proximaInstrucao()
    {
    }
    void salvaTCB()
    {
    }
};

class MapaDeBits
{
public:
    MapaDeBits()
    {
    }

    bool alocaMemoria()
    {
    }
    bool desalocaMemoria()
    {
    }
    bool compacta()
    {
    }
};

class Interface
{
public:
    Interface()
    {
    }
};

// class FilaDeProntos
// {
// public:
//     FilaDeProntos()
//     {
//     }
// };