#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    // Cria um socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        cerr << "Erro ao criar o socket" << endl;
        return 1;
    }

    // Configura o endereco do servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);                  // Porta para a conexao
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Endereco do servidor (neste exemplo, e o localhost)

    // Conecta ao servidor
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        cerr << "Erro ao conectar ao servidor" << endl;
        return 1;
    }

    cout << "Conexão estabelecida" << endl;

    // Envia dados para o servidor
    while (true)
    {
        cout << "Comando: ";
        cin.getline(buffer, sizeof(buffer));

        ssize_t bytesSent = send(clientSocket, buffer, strlen(buffer), 0);
        if (bytesSent == -1)
        {
            cerr << "Erro ao enviar os dados" << endl;
            break;
        }
        else if (bytesSent == 0)
        {
            cout << "Conexão encerrada pelo servidor" << endl;
            break;
        }
    }

    // Fecha o socket
    close(clientSocket);

    return 0;
}
