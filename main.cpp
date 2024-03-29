#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <cstring>
#include <fstream>
#include <regex>

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

    while (true)
    {
        // Lê comando do socket
        if (leComando())
        {
            // Fecha os sockets se ocorreu erro na leitura do comando
            close(clientSocket);
            close(serverSocket);
            return 1;
        }

        // Decodifica comando
        string comando = buffer;

        regex PadraoComandoCreate("create -m (\\d+) -p ([^\\s]+)");
        regex PadraoComandoKill("kill (\\d+)");
        regex PadraoProximaInstrucao("n");
        smatch matches;

        // create
        if (regex_search(comando, matches, PadraoComandoCreate) && matches.size() == 3)
        {
            // Obtém do comando numPosicoesMemoria e programa
            int numPosicoesMemoria = stoi(matches[1]);
            string arquivoPrograma = matches[2];
            vector<string> programa = leArquivo(arquivoPrograma);

            // Cria processo de SO do tipo create
            sistemaOperacional.criaProcessoSOCreate("create", programa, numPosicoesMemoria);
        }
        // kill
        else if (regex_search(comando, matches, PadraoComandoKill) && matches.size() == 2)
        {
            int PID = stoi(matches[1]);

            // Cria processo de SO do tipo kill
            sistemaOperacional.criaProcessoSOKill("kill", PID);
        }
        else if (!(regex_search(comando, matches, PadraoProximaInstrucao) && matches.size() == 1))
        {
            cout << "Comando inválido" << endl;
            continue;
        }

        // Imprime cabeçalho da iteração
        cout << endl
             << "<------------------------>" << endl;
        cout << "|        Iteração        |" << endl;
        cout << "<------------------------>" << endl
             << endl;

        // Executa a próxima iteração
        sistemaOperacional.executa();

        // Atualiza interface
        sistemaOperacional.getMemoria().printMapaDeBits();
        sistemaOperacional.getFilaDeProntos().imprimeFila();
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
        cerr << "Erro ao criar o socket" << endl;
        return 1;
    }

    // Configura o endereço do servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Porta para a conexao
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Vincula o socket ao endereço do servidor
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        cerr << "Erro ao vincular o socket ao endereço" << endl;
        return 1;
    }

    // Aguarda por conexoes
    if (listen(serverSocket, 1) == -1)
    {
        cerr << "Erro ao aguardar por conexões" << endl;
        return 1;
    }

    cout << "Aguardando por conexões..." << endl;

    // Aceita uma conexao de cliente
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket == -1)
    {
        cerr << "Erro ao aceitar a conexão" << endl;
        return 1;
    }

    cout << "Conexão estabelecida" << endl;

    return 0;
}

int leComando()
{
    memset(buffer, 0, sizeof(buffer)); // Limpa o buffer
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == -1)
    {
        cerr << "Erro ao receber os dados" << endl;
        return 1;
    }
    else if (bytesRead == 0)
    {
        cout << "Cliente desconectado" << endl;
        return 1;
    }
    else
    {
        cout << "Comando: " << buffer << endl;
    }

    return 0;
}

vector<string> leArquivo(const string &nomeArquivo)
{
    ifstream arquivo(nomeArquivo);
    vector<string> linhasPrograma;

    // Verifica se o arquivo foi aberto corretamente
    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o arquivo" << endl;
        return linhasPrograma;
    }

    string linha;
    while (getline(arquivo, linha))
        linhasPrograma.push_back(linha);

    arquivo.close();

    return linhasPrograma;
}