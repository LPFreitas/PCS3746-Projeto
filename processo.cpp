#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

class Processo
{
private:
    int PC, PID, numPosicoesMemoria, processoUsuarioPID;
    string tipo;
    vector<string> programa;

public:
    // Usuário
    Processo(int PID, const string &tipo, const vector<string> &programa)
        : PC(0), PID(PID), tipo(tipo), programa(programa)
    {
    }

    // SO - create
    Processo(int PID, const string &tipo, const vector<string> &programa, int numPosicoesMemoria)
        : PID(PID), tipo(tipo), programa(programa), numPosicoesMemoria(numPosicoesMemoria)
    {
    }

    // SO - kill
    Processo(int PID, const string &tipo, int processoUsuarioPID)
        : PID(PID), tipo(tipo), processoUsuarioPID(processoUsuarioPID)
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
        // if (tipo == "usuario")
        //     return -1;

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

    int getProcessoUsuarioPID()
    {
        return processoUsuarioPID;
    }

    bool compara(string tipo, int PID)
    {
        return this->tipo == tipo && this->PID == PID;
    }

    void imprimeStatusTCB()
    {
        // Imprime o cabeçalho do status do programa
        cout << "+------------------------+" << endl;
        cout << "|         Status         |" << endl;
        cout << "+------------------------+" << endl;

        // Imprime o programa com a seta na linha atual
        for (int i = 0; i < programa.size(); ++i)
        {
            if (i == PC)
                cout << "| " << left << setw(17) << programa[i] << " <--- |" << endl;
            else
                cout << "| " << left << setw(22) << programa[i] << " |" << endl;
        }

        // Imprime a moldura inferior do status do programa
        cout << "+------------------------+" << endl;

        // Imprime o cabeçalho da TCB do processo
        cout << endl;
        cout << "+------------------------+" << endl;
        cout << "|     TCB do Processo    |" << endl;
        cout << "+------------------------+" << endl;

        // Imprime PID do processo
        cout << "| PID: " << setw(17) << PID << " |" << endl;

        // Obtém registradores utilizados no programa do processo
        set<string> registradoresEncontrados = encontraRegistradores();
        // Imprime registradores do processo
        for (const string &registrador : registradoresEncontrados)
            cout << "| REG " << setw(18) << registrador << " |" << endl;

        // Imprime PC do processo
        cout << "| PC: " << setw(18) << PC << " |" << endl;

        // Imprime a moldura inferior da TCB do processo
        cout << "+------------------------+" << endl;
        cout << endl;
    }

    set<string> encontraRegistradores()
    {
        set<string> registradores;
        vector<string> registradoresPossiveis = {"AX", "BX", "CX", "DX"};

        for (const string &linha : programa)
        {
            for (const string &registrador : registradoresPossiveis)
            {
                if (linha.find(registrador) != string::npos)
                    registradores.insert(registrador);
            }
        }

        return registradores;
    }
};
