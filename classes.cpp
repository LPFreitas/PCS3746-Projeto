#include <string>
#include "Memoria.cpp"

using namespace std;

class SistemaOperacional
{
    Escalonador escalonador;
    Memoria memoria;
    Dispatcher dispatcher;
    FilaDeProntos fila;
    int proximoPID;

public:
    SistemaOperacional(string modo) // cria os objetos de outras classes (usa construtor das outras classes)
    {
        this->escalonador = Escalonador(modo);
        this->memoria = Memoria();
        this->dispatcher = Dispatcher();
        this->fila = FilaDeProntos();
        proximoPID = 1;
    }

    void atualizaProximoPID()
    {
        proximoPID += 1;
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
    string modo;

public:
    Escalonador(string modo = "fifo")
    {
        this->modo = modo;
    };
};

class Processo
{
public:
    Processo()
    {
    }
};

class TCB
{
public:
    TCB()
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

class FilaDeProntos
{
public:
    FilaDeProntos()
    {
    }
};

int main(int argc, char *argv[])
{
    SistemaOperacional sistemaOperacional(argv[1]);
}