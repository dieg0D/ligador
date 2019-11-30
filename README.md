# Ligador

Este repositório contém o Projeto 2 da Disciplina Software Básico de 2019/2.O trabalho consiste em implementar em C/C++ um método de tradução de um ligador da linguagem  assembly hipotética vista em sala de aula. E além disso uma calculadora em Nasm.

## Para Executar

Em seu Terminal rode o comando `g++ -std=c++11 -Wall -o montador montador.cpp && ./montador fat_mod_a.asm fat_mod_b.asm`

## Objetivo

Fixar o funcionamento de um processo de tradução. Especificamente as de carregamento e ligação.

## Tarefas que precisamos fazer

#### Montador:


- [x] Receber e abrir um ou dois arquivos
- [x] Checagem das Diretivas BEGIN e END
- [ ] Inserir dados na tabela de definições
- [ ] Inserir dados na tabela de uso
- [ ] Gerar arquio obj com Nome do programa, tamanho do código e informação de realocação. 

#### Ligador:
- [ ] Receber e abrir os dois arquivos
- [ ] Tabela global a partir da de uso e definição
- [ ] Calculo do endereço offset
- [ ] Encaixar o modulo no endereço certo
- [ ] Gerar arquivoo obj com o progama ligado
