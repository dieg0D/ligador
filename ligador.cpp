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

typedef struct definitions {
  string name;
  int value;
} definitions;

typedef struct use {
  string name;
  int address;
} use;

typedef struct obj {
  string name;
  int length;
  vector<use> uses;
  vector<int> reallocate;
  vector<int> code;
} obj;

vector<definitions> global_definitions_table; /*Tabela de global definiçẽos*/
vector<int> linkedIn;                         /* codigo final ligado */

obj obj1, obj2;

void reallocate() {
  for (int i = 0; i < (int)obj2.reallocate.size(); i++) {
    if (obj2.reallocate[i] == 1) {
      obj2.code[i] = obj2.code[i] + obj1.length;
    }
  }
}

void linker() {
  for (auto i : obj1.code) {
    linkedIn.push_back(i);
  }
  for (auto i : obj2.code) {
    linkedIn.push_back(i);
  }
  ofstream outfile("linkedProgram.obj");
  for (auto i : linkedIn) {
    outfile << i << " ";
  }

  outfile.close();
}

void uzumaki() {
  for (auto i : obj1.uses) {
    for (auto j : global_definitions_table) {
      if (i.name == j.name) {
        obj1.code[i.address] = j.value;
      }
    }
  }

  for (auto i : obj2.uses) {
    for (auto j : global_definitions_table) {
      if (i.name == j.name) {
        obj2.code[i.address] = j.value;
      }
    }
  }
}

void singleFile(string fileName) {
  ifstream inputfile;
  string line;
  inputfile.open((fileName).c_str());

  ofstream outfile("linkedProgram.obj");
  while (getline(inputfile, line)) {
    if (line[0] == 'T') {
      line.replace(0, 3, "");
      outfile << line;
    }
  }
  outfile.close();
  inputfile.close();
}

void gdt_add_entry(string name, int value) {
  global_definitions_table.push_back({name, value});
}

void twoFiles(string firstFile, obj* object, bool realloc) {
  ifstream inputfile;
  string line, linha, lili;
  inputfile.open((firstFile).c_str());
  ofstream outfile("linkedProgram.obj");
  // Faz magia

  // Pegando nome do arquivo
  getline(inputfile, line);
  object->name = line.replace(0, 3, "");
  // Pegando o tamanho do text
  getline(inputfile, line);
  object->length = stoi(line.replace(0, 3, ""));
  // Pegando as informações de realocação
  getline(inputfile, line);
  line.replace(0, 3, "");
  for (auto i : line) {
    object->reallocate.push_back(atoi(&i));
  }

  // Pegando a tabela de uso
  getline(inputfile, line);
  line.replace(0, 3, "");
  vector<string> aux;
  string delimiter = " ";
  size_t pos = 0;
  string token;

  while ((pos = line.find(delimiter)) != string::npos) {
    token = line.substr(0, pos);
    aux.push_back(token);
    line.erase(0, pos + delimiter.length());
  }

  for (int j = 0; j < (int)aux.size(); j += 2) {
    object->uses.push_back({aux[j], stoi(aux[j + 1])});
  }

  // Pegando a tabela de Definições
  getline(inputfile, line);
  line.replace(0, 3, "");
  aux.clear();
  delimiter = " ";
  pos = 0;
  token = "";

  while ((pos = line.find(delimiter)) != string::npos) {
    token = line.substr(0, pos);
    aux.push_back(token);
    line.erase(0, pos + delimiter.length());
  }

  for (int j = 0; j < (int)aux.size(); j += 2) {
    if (realloc) {
      gdt_add_entry(aux[j], (stoi(aux[j + 1]) + obj1.length));
    } else {
      gdt_add_entry(aux[j], stoi(aux[j + 1]));
    }
  }

  // Pegando o codigo objeto
  getline(inputfile, line);
  line.replace(0, 3, "");
  vector<int> helper;
  delimiter = " ";
  pos = 0;
  token = "";
  while ((pos = line.find(delimiter)) != string::npos) {
    token = line.substr(0, pos);
    helper.push_back(stoi(token));
    line.erase(0, pos + delimiter.length());
  }

  for (int j = 0; j < (int)helper.size(); j++) {
    object->code.push_back(helper[j]);
  }
  cout << endl;
  inputfile.close();
}

int main(int argc, char* argv[]) {
  // Verifica se os argumentos foram passados
  if (argc == 2) {
    cout << "[main] "
         << "Arquivo recebido, tentado abrir..." << endl;
    singleFile(argv[1]);
  } else if (argc == 3) {
    cout << "[main] "
         << "Primeiro arquivo..." << endl;
    twoFiles(argv[1], &obj1, false);
    cout << "[main] "
         << "Segundo arquivo..." << endl;
    twoFiles(argv[2], &obj2, true);
    uzumaki();
    reallocate();
    linker();
  } else {
    cout << "[main] "
         << "ERROr: O programa precisa de 1 argumento. Por favor "
            "insira um arquivo =D"
         << endl;
    return -1;
  }
  return 0;
}
