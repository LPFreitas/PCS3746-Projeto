# PCS3732---Projeto

## Passos:

1. Para limpar os arquivos de saída e executáveis, execute:

```
make clean
```

2. Para compilar todos os arquivos necessários, exceto o arquivo `envia.cpp`, execute:

```
make
```

3. Para compilar o arquivo `envia.cpp`, execute uma única vez no início (a não ser que o arquivo `envia.cpp` seja alterado):

```
make envia
```

4. Para rodar o simulador de um sistema operacional em um certo modo do escalador, execute em um terminal:

```
./main [modo]
```

onde modo é "fifo" para executar no modo de escalonamento **FIFO (não-preemptivo)**, ou "robin" para executar no modo **ROUND-ROBIN (preemptivo)**.

5. Para enviar os comandos para o simulador de SO, execute em outro terminal:

```
./envia
```

## Comandos válidos

- Criar um processo usuário:

```
create -m [nº de posições do processo na memória] -p [nome do arquivo que contém o programa]
```

- Encerrar um processo usuário:

```
kill [PID do processo]
```

- Próxima instrução:

```
n
```

**_Observação_:** os comandos para criar e encerrar um processo usuário também executam a próxima instrução
