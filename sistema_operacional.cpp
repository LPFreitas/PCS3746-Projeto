#include <string>
#include <map>
#include <utility>
#include <iostream>

#include "memoria.cpp"
#include "fila_de_prontos.cpp"

using namespace std;

class SistemaOperacional
{
    string modo;
    Memoria *memoria;
    FilaDeProntos *filaDeProntos;
    Processo *processoExecutando;
    int proxPIDdeUsuario, proxPIDdeSO;
    string linhaExecutando;
    int contadorExecuta;
    int robin_cicles;

public:
    SistemaOperacional(string modo, int robin_cicles = 4, int tamanhoMemoria = 20)
    {
        this->modo = modo;
        this->robin_cicles = robin_cicles;
        memoria = new Memoria(tamanhoMemoria);
        filaDeProntos = new FilaDeProntos();
        processoExecutando = NULL;
        proxPIDdeUsuario = 1;
        proxPIDdeSO = 1;
        linhaExecutando = "";
    }

    void incrementaProximoPIDdeUsuario()
    {
        proxPIDdeUsuario += 1;
    }

    void incrementaProximoPIDdeSO()
    {
        proxPIDdeSO += 1;
    }

    void criaProcessoSO(string tipo, vector<string> programa, int numPosicoesMemoria)
    {
        Processo *processoSO = new Processo( proxPIDdeSO, tipo, programa, numPosicoesMemoria);
        incrementaProximoPIDdeSO();
        (*filaDeProntos).insereNaFila(*processoSO);
    }

    void criaProcessoUsuario(string tipo, vector<string> programa)
    {
        Processo *processoUsuario = new Processo(proxPIDdeUsuario, tipo, programa);
        incrementaProximoPIDdeUsuario();
        (*filaDeProntos).insereNaFila(*processoUsuario);
    }

    void mataProcessoUsuario(int PID) // ?
    {
    }

    void executa()
    {
        Processo *processoAnterior = processoExecutando;
        string linhaExecutadaAnteriormente = linhaExecutando;

        // cout << "Executando " << (*processoExecutando).getTipo() << " " << (*processoExecutando).getPID() << endl;
        // if (modo == "robin"){
        //     if (contadorExecuta == robin_cicles || linhaExecutadaAnteriormente == "HLT"){
        //         contadorExecuta = 0;
        //         processoExecutando = dispatcher();
        //     }
        // }
        
        
        // Nada a executar e nada foi executado
        if (processoAnterior == NULL && filaDeProntos == NULL)
            return;
        // Fila de prontos com processo, mas nenhum executado (Ex.: primeiro create do sistema vai executar) 
        else if (processoAnterior == NULL) {
            executaProximoProcesso();
            return;
        // Algum processo já foi executado, estando a fila de prontos vazia ou não
        } else {
            if (modo == "robin"){
                if (contadorExecuta == robin_cicles || linhaExecutadaAnteriormente == "HLT"){
                    contadorExecuta = 0;
                    processoExecutando = dispatcher();
                }
            }
            
            // Processo SO em execução terminou, chamamos o próximo processo a ser executado da fila
            if( (*processoAnterior).getTipo()  != "usuario") {
                executaProximoProcesso();
             } 
            else {
                // Processo usuário em execução acabou na última iteração, chamamos o próximo e já começamos sua execução
                if ((contadorExecuta == robin_cicles && modo == "robin") || linhaExecutadaAnteriormente == "HLT") {
                    executaProximoProcesso();
                    contadorExecuta = 0;
                }
                // Processo usuário ainda não terminou de executar
                else{
                    executaProcessoUsuario();
                }
            }
                    
        }

        // Processo usuário em execução chegou ao fim, desalocamos as memórias
        if (linhaExecutando == "HLT") {
            desalocaProcessoUsuario();
        }
    }

    void executaProximoProcesso() {
        linhaExecutando = "";
        processoExecutando = dispatcher();
        if (processoExecutando == NULL)
            return;
        if( (*processoExecutando).getTipo()  == "usuario")
            executaProcessoUsuario();
        else
            executaProcessoSO();
    }

    void executaProcessoSO()
    {
        string tipoProcessoExecutando = (*processoExecutando).getTipo();
        if (tipoProcessoExecutando == "create")
        {
            // Aloca memoria para o processo de usuario a ser criado
            bool alocou = (*memoria).alocaMemoria((*processoExecutando).getNumPosicoesMemoria(), proxPIDdeUsuario);
            if (!alocou)
            {
                cerr << "Erro ao alocar memória para o processo usuário " << proxPIDdeUsuario << ": memória insuficiente!" << endl;
                cout << "Não foi possível criar o processo usuário " << proxPIDdeUsuario << endl;
                return;
            }

            // Cria processo do tipo usuario
            vector<string> programa = (*processoExecutando).getPrograma();
            criaProcessoUsuario("usuario", programa);

            // Atualiza o processo executando
            // processoExecutando = dispatcher();
        }
        else if (tipoProcessoExecutando == "kill")
        {
            // Robin
            // Desaloca a memoria
            // mata processo do tipo usuario - mataProcessoUsuario();
        }
    }

    void executaProcessoUsuario()
    {
        int processoExecutandoPID = (*processoExecutando).getPID();
        vector<string> programaExecutando = (*processoExecutando).getPrograma();
        int programaExecutandoPC = (*processoExecutando).getPC();
        linhaExecutando = programaExecutando[programaExecutandoPC];

        cout << programaExecutandoPC << " " << linhaExecutando << endl;
        if (linhaExecutando != "HLT") // Final do programa do processo usuario
            // Incrementa o PC do processo usario
            (*processoExecutando).incrementaPC();
            
    }

    void desalocaProcessoUsuario() {
        // Desaloca memoria do processo usuario
        int processoExecutandoPID = (*processoExecutando).getPID();
        bool desalocou = (*memoria).desalocaMemoria(processoExecutandoPID);
        if (!desalocou)
        {
            cerr << "Erro ao desalocar memória para o processo usuário " << processoExecutandoPID << endl;
            cout << "Não foi possível encerrar o processo usuário " << processoExecutandoPID << ": memória não desalocou!" << endl;
        }
    }

    Memoria getMemoria()
    {
        return (*memoria);
    }

    FilaDeProntos getFilaDeProntos()
    {
        return (*filaDeProntos);
    }

    Processo *dispatcher()
    {
        if (modo == "fifo")
            return escalonador();
        
        // Robin
        Processo *processoAnterior = processoExecutando;
        if (modo  == "robin"){
            (*filaDeProntos).insereNaFila(processoAnterior);
            Processo *processoAtual = escalonador();
            //linhaExecutando = programaExecutando[(*processoAtual).getPC()]
            linhaExecutando = "";
            return 
            //atualizar a linhaExecutando
        }
    }

    // FIFO
    Processo *escalonador()
    {
        // Se fila estiver vazia, nao ha processos a executar
        if ((*filaDeProntos).tamanho() == 0)
            return NULL;

        // Remove e escalona o primeiro processo da fila de prontos
        return (*filaDeProntos).tiraDaFila();
    }
};