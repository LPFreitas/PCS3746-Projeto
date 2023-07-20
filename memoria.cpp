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

        // passa a memoria inteira
        for (int i = 0; i < (tamanhoMemoria); i++)
        {

            if (mapaDeBits[i] == 0)
            {
                int j = i;
                // verifica se o processo cabe naquele espaço
                while (mapaDeBits[j] == 0 && (j - i + 1) < tamanho_desejado && j + 1 < tamanhoMemoria)
                    j++;

                // caso caiba, coloca PID no mapa de bits
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
        // Imprime a moldura superior do cabeçalho
        cout << "+";
        for (int i = 0; i < 24; ++i)
        {
            cout << "-";
        }
        cout << "+" << endl;

        // Imprime o cabeçalho
        cout << "|       Mapa de Bits     |" << endl;

        // Imprime a moldura intermediária do cabeçalho
        cout << "+";
        for (int i = 0; i < 24; ++i)
        {
            cout << "-";
        }
        cout << "+" << endl;

        // Imprime a moldura superior da tabela
        cout << "+";
        for (int i = 0; i < 5; ++i)
        {
            cout << "----+";
        }
        cout << endl;

        // Imprime o mapa de bits como uma tabela de 4 linhas e 5 colunas
        int contador = 0;
        for (int i = 0; i < tamanhoMemoria / 5; ++i)
        {
            // Imprime a moldura lateral esquerda da tabela
            cout << "| ";
            for (int j = 0; j < 5; ++j)
            {
                cout << setw(2) << mapaDeBits[contador] << " | ";
                ++contador;
            }
            cout << endl;

            // Imprime a moldura intermediária da tabela
            cout << "+";
            for (int j = 0; j < 5; ++j)
            {
                cout << "----+";
            }
            cout << endl;
        }
    }
};