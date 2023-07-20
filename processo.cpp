#include <string>
#include <vector>
#include <map>

using namespace std;

class Processo
{
private:
    int PC, PID, numPosicoesMemoria;
    string tipo;
    vector<string> programa;
    // map<string, int> regs;

public:
    Processo(int PID, const string &tipo, const vector<string> &programa)
        : PID(PID), tipo(tipo), programa(programa)
    {
        PC = 0;
    }

    Processo(int PID, const string &tipo, const vector<string> &programa, int numPosicoesMemoria)
        : PID(PID), tipo(tipo), programa(programa), numPosicoesMemoria(numPosicoesMemoria)
    {
    }

    int getPC()
    {
        return PC;
    }

    void setPC(int PC)
    {
        this->PC = PC;
    }

    void incrementaPC()
    {
        PC += 1;
    }

    int getPID()
    {
        return PID;
    }

    int getNumPosicoesMemoria()
    {
        return numPosicoesMemoria;
    }

    string getTipo()
    {
        return tipo;
    }

    vector<string> getPrograma()
    {
        return programa;
    }

    bool compara(Processo p)
    {
        return p.getTipo() == tipo && p.getPID() == PID;
    }
};
