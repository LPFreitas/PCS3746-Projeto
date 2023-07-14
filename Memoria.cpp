
#include <cstring>

class Memoria {

private:
    int tamanho_memoria;
    int *mapa_de_bits;

public:

    Memoria(int tamanho_memoria = 20) {
        this->tamanho_memoria = tamanho_memoria;
        this->mapa_de_bits = new int[tamanho_memoria];
        memset(this->mapa_de_bits, 0, tamanho_memoria);
    }

    bool alocaProcesso (int tamanho_desejado, int PID){
        bool alocouProcesso = NULL;
        
        //passa a memoria inteira
        for(int i = 0; i < (this->tamanho_memoria); i++){
            if(this->mapa_de_bits[i] == 0){
                int j=i; 
                //verifica se o processo cabe naquele espaço
                while(mapa_de_bits[j]== 0 && (i-j+1)<tamanho_desejado && j+1<this->tamanho_memoria){
                    j++;
                }

                //caso caiba, coloca PID no mapa de bits
                if (i-j+1 == tamanho_memoria){
                    for(int k=j;k-j<tamanho_desejado;k++){
                        mapa_de_bits[k] = PID;
                    }
                    return 1;
                }
            }
        } 

        return alocouProcesso;
    }
};