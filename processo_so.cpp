#include "classes.cpp"
#include "processo.cpp"

class ProcessoSO : public Processo
{
private:
    int numPosicoesMemoria;

public:
    ProcessoSO(int PID, const string &tipo, int numPosicoesMemoria, const vector<string> &programa)
        : Processo(PID, tipo, programa), numPosicoesMemoria(numPosicoesMemoria)
    {
    }

    // static ProcessoSO processoParaProcessoSO(Processo &processo, int numPosicoesMemoria)
    // {
    //     return ProcessoSO(processo.getPID(), processo.getTipo(), numPosicoesMemoria, processo.getPrograma());
    // }

    int getNumPosicoesMemoria()
    {
        return numPosicoesMemoria;
    }
};