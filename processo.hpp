#ifndef PROCESSO_HPP
#define PROCESSO_HPP

#include <string>
#include <vector>

using namespace std;

class Processo
{
protected:
    int PID;
    string tipo;
    vector<string> programa;

public:
    Processo(int PID, string tipo, vector<string> programa) : PID(PID), tipo(tipo), programa(programa)
    {
    }

    int getPID()
    {
        return PID;
    }

    string getTipo()
    {
        return tipo;
    }

    vector<string> getPrograma()
    {
        return programa;
    }
};

#endif
