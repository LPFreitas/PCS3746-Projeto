#include "processo.hpp"
#include <string>
#include <vector>
#include <map>

using namespace std;

class ProcessoUsuario : public Processo
{
private:
    int PC;
    map<string, int> regs;
    // TaskControlBlock processoTCB;

public:
    ProcessoUsuario(int PID, string tipo, vector<string> programa) : Processo(PID, tipo, programa)
    {
        PC = 0;
        regs["AX"] = 0;
        regs["BX"] = 0;
        regs["CX"] = 0;
        regs["DX"] = 0;
        // this->processoTCB = TaskControlBlock(PID);
    }

    static ProcessoUsuario *processoParaProcessoUsuario(Processo processo)
    {
        return new ProcessoUsuario(processo.getPID(), processo.getTipo(), processo.getPrograma());
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
};