#include <iostream>
#include <iomanip>

void printMapaDeBits(const int mapa_bits[], int tamanho) {
    // Imprime a moldura superior do cabeçalho
    std::cout << "+";
    for (int i = 0; i < 24; ++i) {
        std::cout << "-";
    }
    std::cout << "+" << std::endl;

    // Imprime o cabeçalho
    std::cout << "|       Mapa de Bits     |" << std::endl;

    // Imprime a moldura intermediária do cabeçalho
    std::cout << "+";
    for (int i = 0; i < 24; ++i) {
        std::cout << "-";
    }
    std::cout << "+" << std::endl;

    // Imprime a moldura superior da tabela
    std::cout << "+";
    for (int i = 0; i < 5; ++i) {
        std::cout << "----+";
    }
    std::cout << std::endl;

    // Imprime o mapa de bits como uma tabela de 4 linhas e 5 colunas
    int counter = 0;
    for (int i = 0; i < tamanho / 5; ++i) {
        // Imprime a moldura lateral esquerda da tabela
        std::cout << "| ";
        for (int j = 0; j < 5; ++j) {
            std::cout << std::setw(2) << mapa_bits[counter] << " | ";
            ++counter;
        }
        std::cout << std::endl;

        // Imprime a moldura intermediária da tabela
        std::cout << "+";
        for (int j = 0; j < 5; ++j) {
            std::cout << "----+";
        }
        std::cout << std::endl;
    }
}

int main() {
    int mapa_bits[20] = {0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1};

    printMapaDeBits(mapa_bits, 20);

    return 0;
}
