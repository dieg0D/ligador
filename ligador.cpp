// Alunos
// Diego Antônio Barbosa Cardoso - 16/0005116
// Icaro Nery Rezende - 15/0037023

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector> /*Amor da minha vida*/
using namespace std;

int main(int argc, char *argv[]) {
  // Verifica se os argumentos foram passados
  if (argc > 1) {
    controller(argc, argv[1]);
  } else {
    cout << "[main] "
         << "O programa precisa de 1 argumento. Por favor "
            "insira um arquivo =D"
         << endl;
    return -1;
  }
  return 0;
}