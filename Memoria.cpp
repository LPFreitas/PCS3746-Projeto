
#include <cstring>
#include <cstdio>
class Memoria
{

private:
    int tamanho_memoria;
    int *mapa_de_bits;

public:
    Memoria(int tamanho_memoria = 20)
    {
        this->tamanho_memoria = tamanho_memoria;
        this->mapa_de_bits = new int[tamanho_memoria];
        for (int i=0; i<tamanho_memoria;i++)
            this->mapa_de_bits[i] = 0;

    }

    bool alocProcesso(int tamanho_desejado, int PID)
    {
        bool alocouProcesso = NULL;

        // passa a memoria inteira
        for (int i = 0; i < (this->tamanho_memoria); i++)
        {
            
            if (this->mapa_de_bits[i] == 0)
            {
                
                int j = i;
                // verifica se o processo cabe naquele espaço
                while (mapa_de_bits[j] == 0 && (j - i + 1) < tamanho_desejado && j + 1 < this->tamanho_memoria)
                    j++;

                // caso caiba, coloca PID no mapa de bits
                if (j - i + 1 == tamanho_desejado)
                {
                    for (int k = i; k - i < tamanho_desejado; k++)
                        mapa_de_bits[k] = PID;

                    return 1;
                }
            }
        }

        return alocouProcesso;
    }

    bool desalocMemoria(int PID)
    {
        bool desaloc = NULL;
        for(int i=0; i < (this->tamanho_memoria); i++)
        {
            if (this->mapa_de_bits[i] == PID){
                this->mapa_de_bits[i] = 0;
                desaloc = 1;
            }
        }
        return desaloc;
    }

    void compactaMemoria()
    {
        int j = 0;
        for (int i = 0; i < this->tamanho_memoria; i++) {
            if (this->mapa_de_bits[i] != 0) {
                this->mapa_de_bits[j] = this->mapa_de_bits[i];
                j++;
            }
        }

        for (; j < this->tamanho_memoria; j++) {
            this->mapa_de_bits[j] = 0;
        }
    }

    void printMapaDeBits()
    {
        printf("Mapa de Bits: ");
        for (int i = 0; i < 20; i++) {
            printf("%d | ", this->mapa_de_bits[i]);
        }
    }
};