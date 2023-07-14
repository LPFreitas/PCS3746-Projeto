#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer[1024];

    // Cria um socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Erro ao criar o socket\n";
        return 1;
    }

    // Configura o endereço do servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Porta para a conexão
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Vincula o socket ao endereço do servidor
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Erro ao vincular o socket ao endereço\n";
        return 1;
    }

    // Aguarda por conexões
    if (listen(serverSocket, 1) == -1) {
        std::cerr << "Erro ao aguardar por conexões\n";
        return 1;
    }

    std::cout << "Aguardando por conexões...\n";

    // Aceita uma conexão de cliente
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket == -1) {
        std::cerr << "Erro ao aceitar a conexão\n";
        return 1;
    }

    std::cout << "Conexão estabelecida\n";

    // Lê os dados enviados pelo cliente e os exibe
    while (true) {
        memset(buffer, 0, sizeof(buffer)); // Limpa o buffer
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead == -1) {
            std::cerr << "Erro ao receber os dados\n";
            break;
        } else if (bytesRead == 0) {
            std::cout << "Cliente desconectado\n";
            break;
        } else {
            std::cout << "Cliente: " << buffer << std::endl;
        }
    }

    // Fecha os sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
