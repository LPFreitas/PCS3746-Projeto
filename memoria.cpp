#include <cstring>
#include <cstdio>
#include <iostream>
#include <iomanip>

using namespace std;

class Memoria
{

private:
    int tamanhoMemoria;
    int *mapaDeBits;

public:
    Memoria(int tamanhoMemoria)
    {
        this->tamanhoMemoria = tamanhoMemoria;
        mapaDeBits = new int[tamanhoMemoria];
        for (int i = 0; i < tamanhoMemoria; i++)
            mapaDeBits[i] = 0;
    }

    bool alocaMemoria(int tamanho_desejado, int PID)
    {
        bool alocouProcesso = false;

        // Passa a memória inteira
        for (int i = 0; i < (tamanhoMemoria); i++)
        {

            if (mapaDeBits[i] == 0)
            {
                int j = i;
                // Verifica se o processo cabe naquele espaço
                while (mapaDeBits[j] == 0 && (j - i + 1) < tamanho_desejado && j + 1 < tamanhoMemoria)
                    j++;

                // Caso caiba, coloca PID no mapa de bits
                if (j - i + 1 == tamanho_desejado)
                {
                    for (int k = i; k - i < tamanho_desejado; k++)
                        mapaDeBits[k] = PID;

                    alocouProcesso = true;
                    break;
                }
            }
        }

        return alocouProcesso;
    }

    bool desalocaMemoria(int PID)
    {
        bool desalocouProcesso = false;

        for (int i = 0; i < (tamanhoMemoria); i++)
        {
            if (mapaDeBits[i] == PID)
            {
                int j = i;
                while (mapaDeBits[j] == PID)
                {
                    mapaDeBits[j] = 0;
                    j++;
                }
                desalocouProcesso = true;
            }
        }

        return desalocouProcesso;
    }

    void compactaMemoria()
    {
        int j = 0;
        for (int i = 0; i < tamanhoMemoria; i++)
        {
            if (mapaDeBits[i] != 0)
            {
                mapaDeBits[j] = mapaDeBits[i];
                j++;
            }
        }

        for (; j < tamanhoMemoria; j++)
        {
            mapaDeBits[j] = 0;
        }
    }

    void printMapaDeBits()
    {
        // Imprime o cabeçalho do mapa de bits
        cout << "+------------------------+" << endl;
        cout << "|      Mapa de Bits      |" << endl;
        cout << "+------------------------+" << endl;

        // Imprime a moldura superior da tabela
        cout << "+----+----+----+----+----+" << endl;

        // Imprime o mapa de bits como uma tabela de 4 linhas e 5 colunas
        int contador = 0;
        int bit = 1;
        for (int i = 0; i < tamanhoMemoria / 5; ++i)
        {
            // Imprime a moldura lateral esquerda da tabela
            cout << "| ";
            for (int j = 0; j < 5; ++j)
            {
                bit = (mapaDeBits[contador] > 0) ? 0 : 1;
                cout << setw(2) << bit << " | ";
                ++contador;
            }
            cout << endl;

            // Imprime a moldura intermediária da tabela
            cout << "+----+----+----+----+----+" << endl;
        }
        cout << endl;
    }
};