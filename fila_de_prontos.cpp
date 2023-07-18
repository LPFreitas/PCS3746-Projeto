#include <iostream>
#include "processo.hpp"

using namespace std;

// Definição da estrutura do nó da lista
struct ElementoFila {
    Processo processo;
    ElementoFila* proximoProcesso;

    ElementoFila(Processo p) : processo(p), proximoProcesso(nullptr) {}
};

class FilaDeProntos {
private:
    ElementoFila* primeiroProcesso; // Aponta para o primeiro elemento da fila (início da lista)
    ElementoFila* ultimoProcesso;  // Aponta para o último elemento da fila (final da lista)

public:
    FilaDeProntos() : primeiroProcesso(nullptr), ultimoProcesso(nullptr) {}

    // Verifica se a fila está vazia
    bool filaVazia() {
        return primeiroProcesso == nullptr;
    }

    // Insere um elemento no final da fila
    void insereNaFila(Processo processo) {
        ElementoFila* novoElementoFila = new ElementoFila(processo);
        if (filaVazia()) {
            primeiroProcesso = ultimoProcesso = novoElementoFila;
        } else {
            ultimoProcesso->proximoProcesso = novoElementoFila;
            ultimoProcesso = novoElementoFila;
        }
    }

    // Remove o elemento na frente da fila (início da lista) e o retorna
    Processo tiraDaFila() {
        if (filaVazia()) {
            throw std::runtime_error("Fila de prontos vazia. Impossível retirar um processo.");
        }

        Processo processo = primeiroProcesso->processo;
        ElementoFila* temp = primeiroProcesso;
        primeiroProcesso = primeiroProcesso->proximoProcesso;
        delete temp;
        return processo;
    }

    // Função para imprimir os elementos da fila
    void imprimeFila() {
        ElementoFila* atual = primeiroProcesso;
        while (atual != nullptr) {
            cout << (atual->processo).getTipo() << " ";
            atual = atual->proximoProcesso;
        }
        cout << endl;
    }

    void removeElementoFila(Processo processoVitima) {
        ElementoFila* atual = primeiroProcesso;
        ElementoFila* anterior = nullptr;
        while (atual != nullptr) {
            if ((atual->processo).isEqual(processoVitima)) {
                if (anterior == nullptr) {
                    // Caso o elemento a ser removido seja o primeiro da fila
                    primeiroProcesso = atual->proximoProcesso;
                } else {
                    anterior->proximoProcesso = atual->proximoProcesso;
                }
                delete atual;
                break;
            }
            anterior = atual;
            atual = atual->proximoProcesso;
        }
    }
};