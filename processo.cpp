#include <string>
#include <vector>
#include <map>

using namespace std;

class Processo
{
private:
    int PC, PID;
    int numPosicoesMemoria;
    string tipo;
    vector<string> programa;
    // map<string, int> regs;

public:
    Processo(int PID, string tipo, vector<string> programa)
    {
        PC = 0;
        this->PID = PID;
        this->tipo = tipo;
        this->programa = programa;
    }

    Processo(int PID, int numPosicoesMemoria, string tipo, vector<string> programa)
    {
        PC = 0;
        this->PID = PID;
        this->numPosicoesMemoria = numPosicoesMemoria;
        this->tipo = tipo;
        this->programa = programa;
    }
};
