# Ligador
Este repositório contém o Projeto 2 da Disciplina Software Básico de 2019/2. O trabalho consiste em implementar em C/C++ um ligador da linguagem  assembly hipotética vista em sala de aula. E além disso uma calculadora em NASM.

## Objetivo
Fixar o funcionamento de um processo de tradução. Especificamente as de carregamento e ligação. 

## Para Executar

##### Para rodar o montador
Em seu Terminal rode o comando `g++ -std=c++11 -Wall -o montador montador.cpp && ./montador fat_mod_a.asm fat_mod_b.asm`

Nosso montador vai gerar arquivos com a seguinte configuração:
```
H: nome do aquivo
H: tamanho do arquivo
H: informações de realocação
H: tabela de uso
H: tabela de definições 
T: código objeto
```

##### Para rodar o ligador
Em seu Terminal rode o comando `g++ -std=c++11 -Wall -o ligador ligador.cpp && ./ligador fat_mod_a.obj fat_mod_b.obj`

##### Para rodar o Simulador
Em seu Terminal rode o comando `g++ -std=c++11 -Wall -o simulador simulador.cpp && ./simulador linkedProgram.obj `

##### Para rodar a calculadora
Em seu Terminal rode o comando `nasm -f elf -o calculadora.o calculadora.asm && ld -m elf_i386 -o calculadora calculadora.o  && ./calculadora`

## Tarefas que precisamos fazer

#### Montador:

- [x] Receber e abrir um ou dois arquivos
- [x] Checagem das Diretivas BEGIN e END
- [x] Inserir dados na tabela de definições
- [x] Inserir dados na tabela de uso
- [x] Gerar arquivo obj com Nome do programa, tamanho do código e informação de realocação. 

#### Ligador:
- [X] Receber e abrir os dois arquivos
- [X] Tabela global a partir da de uso e definição
- [X] Calculo do endereço offset
- [X] Encaixar o modulo no endereço certo
- [X] Gerar arquivoo obj com o progama ligado

#### Calculadora:
- [X] Mostrar mensagens de boas vindas
- [X] Pegar o nome de de usuários
- [X] Implementar menu
- [X] Implementar Soma
- [X] Implementar Subtração
- [X] Implementar Multiplicação
- [X] Implementar Divisão
- [X] Implementar Mod
- [X] Implementar Sair rsrs
- [x] Pedir 2 paramêtros
- [x] Resolver a conta
- [x] Função para leitura de numero inteiro
- [x] Função para escrita de numero inteiro
- [x] Função para leitura de strings
- [x] Função para escrita de strings

## Ambiente utilizado para testes

- Sistema Operacional: Pop!_OS 19.10 x86_64 (Baseado em Ubuntu 19.10)
- Kernel: 5.3.0-22-generic 
- g++ (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
- NASM version 2.14.02
- GNU ld (GNU Binutils for Ubuntu) 2.33