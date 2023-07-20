#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    // Cria um socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        std::cerr << "Erro ao criar o socket\n";
        return 1;
    }

    // Configura o endereco do servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);                  // Porta para a conexao
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Endereco do servidor (neste exemplo, e o localhost)

    // Conecta ao servidor
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Erro ao conectar ao servidor\n";
        return 1;
    }

    std::cout << "Conexão estabelecida\n";

    // Envia dados para o servidor
    while (true)
    {
        std::cout << "Mensagem: ";
        std::cin.getline(buffer, sizeof(buffer));

        ssize_t bytesSent = send(clientSocket, buffer, strlen(buffer), 0);
        if (bytesSent == -1)
        {
            std::cerr << "Erro ao enviar os dados\n";
            break;
        }
        else if (bytesSent == 0)
        {
            std::cout << "Conexão encerrada pelo servidor\n";
            break;
        }
    }

    // Fecha o socket
    close(clientSocket);

    return 0;
}
