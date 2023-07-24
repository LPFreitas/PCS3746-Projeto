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

    // Insere um elemento no final da fila
    void insereNaFila(Processo processo)
    {
        ElementoFila *novoElementoFila = new ElementoFila(processo);
        if (filaVazia())
        {
            primeiroProcesso = ultimoProcesso = novoElementoFila;
        }
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
        {
            throw runtime_error("Fila de prontos vazia. Impossível retirar um processo.");
        }

        Processo *processo = new Processo(primeiroProcesso->processo);
        ElementoFila *temp = primeiroProcesso;
        primeiroProcesso = primeiroProcesso->proximoProcesso;
        delete temp;
        return processo;
    }

    // Função para imprimir os elementos da fila
    void imprimeFila()
    {
        
        if (filaVazia()){
            cout << "Fila Vazia" << endl;
            return;    
        }
        ElementoFila *atual = primeiroProcesso;
       
        while (atual != nullptr)
        {
             
            string tipoProcesso = (atual->processo).getTipo();
            int processoPID = (atual->processo).getPID();
            if (tipoProcesso == "usuario")
                cout << "PID " << processoPID << " | ";
            else
                cout << tipoProcesso << " " << processoPID << " | ";
            atual = atual->proximoProcesso;
        }
        cout << endl;
    }

    void removeElementoFila(Processo processoVitima)
    {
        ElementoFila *atual = primeiroProcesso;
        ElementoFila *anterior = nullptr;
        while (atual != nullptr)
        {
            if ((atual->processo).compara(processoVitima))
            {
                if (anterior == nullptr)
                {
                    // Caso o elemento a ser removido seja o primeiro da fila
                    primeiroProcesso = atual->proximoProcesso;
                }
                else
                {
                    anterior->proximoProcesso = atual->proximoProcesso;
                }
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