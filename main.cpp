// #include "classes.cpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <cstring>
#include <fstream>

#include "sistema_operacional.cpp"

using namespace std;

int serverSocket, clientSocket;
struct sockaddr_in serverAddr, clientAddr;
socklen_t clientAddrLen = sizeof(clientAddr);
char buffer[1024];

int preparaConexaoSocket();

int leComando();

vector<string> leArquivo(const string &nomeArquivo);

int main(int argc, char *argv[])
{
    string modo = argv[1];

    if (preparaConexaoSocket())
        return 1;

    SistemaOperacional sistemaOperacional(modo);

    // Le os dados enviados pelo cliente e os exibe
    while (true)
    {
        if (leComando())
        {
            close(clientSocket);
            close(serverSocket);
            return 1;
        }
        // Decodifica comando
        // create -m [numero de posicoes memoria] -p [programa]
        // kill [PID]

        string comando = buffer;
        vector<string> parametrosComando;
        istringstream iss(comando);

        // Lendo cada parametro dao comando usando o operador >>
        string parametro;
        while (iss >> parametro)
            parametrosComando.push_back(parametro);

        // create -m 4 -p teste.txt
        if (parametrosComando[0] == "create" && parametrosComando[1] == "-m" && parametrosComando[3] == "-p")
        {
            int numPosicoesMemoria = stoi(parametrosComando[2]);
            string arquivoPrograma = parametrosComando[4];

            vector<string> programa = leArquivo(arquivoPrograma);

            // Colocar Create na fila ou para execucao
            sistemaOperacional.criaProcessoSO("create", numPosicoesMemoria, programa);
        }
        else if (parametrosComando[0] == "kill")
        {
            int PID = stoi(parametrosComando[1]);

            // cout << "Comando "
            //      << parametrosComando[0] << " "
            //      << parametrosComando[1] << endl;
        }
        else if (!(parametrosComando.size() == 1 && parametrosComando[0] == "n"))
        {
            cout << "Comando inválido" << endl;
        }

        sistemaOperacional.getFilaDeProntos().imprimeFila();
        sistemaOperacional.executa();

        // cout << "Proxima instrução" << endl;
        // atualiza interface
        // getchar (n - proximo instrucao)
    }

    // Fecha os sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}

int preparaConexaoSocket()
{
    // Cria um socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        cerr << "Erro ao criar o socket\n";
        return 1;
    }

    // Configura o endereço do servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Porta para a conexão
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Vincula o socket ao endereço do servidor
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        cerr << "Erro ao vincular o socket ao endereço\n";
        return 1;
    }

    // Aguarda por conexões
    if (listen(serverSocket, 1) == -1)
    {
        cerr << "Erro ao aguardar por conexões\n";
        return 1;
    }

    cout << "Aguardando por conexões...\n";

    // Aceita uma conexão de cliente
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket == -1)
    {
        cerr << "Erro ao aceitar a conexão\n";
        return 1;
    }

    cout << "Conexão estabelecida\n";

    return 0;
}

int leComando()
{
    memset(buffer, 0, sizeof(buffer)); // Limpa o buffer
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == -1)
    {
        cerr << "Erro ao receber os dados\n";
        return 1;
    }
    else if (bytesRead == 0)
    {
        cout << "Cliente desconectado\n";
        return 1;
    }
    else
    {
        cout << "Cliente: " << buffer << endl;
    }

    return 0;
}

vector<string> leArquivo(const string &nomeArquivo)
{
    ifstream arquivo(nomeArquivo);
    vector<string> linhasPrograma;

    // Verificar se o arquivo foi aberto corretamente
    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o arquivo." << endl;
        return linhasPrograma;
    }

    string linha;
    while (getline(arquivo, linha))
        linhasPrograma.push_back(linha);

    arquivo.close();

    return linhasPrograma;
}