#include <iostream>

#include "processo.cpp"

using namespace std;

// Definição da estrutura do nó da lista
struct ElementoFila
{
    Processo processo;
    ElementoFila *proximoProcesso;

    ElementoFila(Processo p) : processo(p), proximoProcesso(nullptr) {}
};

class FilaDeProntos
{
private:
    ElementoFila *primeiroProcesso; // Aponta para o primeiro elemento da fila (início da lista)
    ElementoFila *ultimoProcesso;   // Aponta para o último elemento da fila (final da lista)

public:
    FilaDeProntos() : primeiroProcesso(nullptr), ultimoProcesso(nullptr) {}

    // Verifica se a fila está vazia
    bool filaVazia()
    {
        return primeiroProcesso == nullptr;
    }

    bool estaPresente(string tipo, int PID)
    {
        ElementoFila *atual = primeiroProcesso;
        while (atual)
        {
            if ((atual->processo).compara(tipo, PID))
                return true;
            atual = atual->proximoProcesso;
        }
        return false;
    }

    // Insere um elemento no final da fila
    void insereNaFila(Processo processo)
    {
        ElementoFila *novoElementoFila = new ElementoFila(processo);
        if (filaVazia())
            primeiroProcesso = ultimoProcesso = novoElementoFila;
        else
        {
            ultimoProcesso->proximoProcesso = novoElementoFila;
            ultimoProcesso = novoElementoFila;
        }
    }

    // Remove o elemento na frente da fila (início da lista) e o retorna
    Processo *tiraDaFila()
    {
        if (filaVazia())
            throw runtime_error("Fila de prontos vazia. Impossível retirar um processo.");

        Processo *processo = new Processo(primeiroProcesso->processo);
        ElementoFila *temp = primeiroProcesso;
        primeiroProcesso = primeiroProcesso->proximoProcesso;
        delete temp;
        return processo;
    }

    // Função para imprimir os elementos da fila
    void imprimeFila()
    {
        // Imprime o cabeçalho da fila de prontos
        cout << "+------------------------+" << endl;
        cout << "|    Fila de Prontos     |" << endl;
        cout << "+------------------------+" << endl;

        if (filaVazia())
        {
            cout << "|         Vazia          |" << endl;
            cout << "+------------------------+" << endl;
            cout << endl;
            return;
        }

        ElementoFila *atual = primeiroProcesso;

        while (atual != nullptr)
        {
            // Obtém tipo do processo
            string tipoProcesso = (atual->processo).getTipo();

            if (tipoProcesso == "usuario")
            {
                int processoPID = (atual->processo).getPID();
                cout << "| PID " << setw(18) << processoPID << " |" << endl;
            }
            else
            {
                int processoUsuarioPID = (atual->processo).getProcessoUsuarioPID();
                if (tipoProcesso == "create")
                    cout << "| create " << setw(15) << processoUsuarioPID << " |" << endl;
                else if (tipoProcesso == "kill")
                    cout << "| kill " << setw(17) << processoUsuarioPID << " |" << endl;
            }

            atual = atual->proximoProcesso;
        }

        // Imprime a moldura inferior da fila de prontos
        cout << "+------------------------+" << endl;
        cout << endl;
    }

    void removeElementoFila(string tipo, int PID)
    {
        ElementoFila *atual = primeiroProcesso;
        ElementoFila *anterior = nullptr;

        while (atual != nullptr)
        {
            if ((atual->processo).compara(tipo, PID))
            {
                if (anterior == nullptr) // Caso o elemento a ser removido seja o primeiro da fila
                    primeiroProcesso = atual->proximoProcesso;
                else
                    anterior->proximoProcesso = atual->proximoProcesso;
                delete atual;
                break;
            }
            anterior = atual;
            atual = atual->proximoProcesso;
        }
    }

    int tamanho()
    {
        int count = 0;
        ElementoFila *atual = primeiroProcesso;

        while (atual != nullptr)
        {
            count++;
            atual = atual->proximoProcesso;
        }

        return count;
    }
};