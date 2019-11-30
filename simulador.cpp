#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector> /*Amor da minha vida*/
using namespace std;

int acc = 0; /*Acumulador xD */
int stopLocation = 0; /*Endereço que separa o text do data xD */

ifstream objFile; /* apenas lê do asm */

map<string, int> op_map;


const vector<string> explode(const string& s, const char& c){
  op_map.insert(std::pair<string, int>("1", 1));
  op_map.insert(std::pair<string, int>("2", 1));
  op_map.insert(std::pair<string, int>("3", 1));
  op_map.insert(std::pair<string, int>("4", 1));
  op_map.insert(std::pair<string, int>("5", 1));
  op_map.insert(std::pair<string, int>("6", 1));
  op_map.insert(std::pair<string, int>("7", 1));
  op_map.insert(std::pair<string, int>("8", 1));
  op_map.insert(std::pair<string, int>("9", 2));
  op_map.insert(std::pair<string, int>("10", 1));
  op_map.insert(std::pair<string, int>("11", 1));
  op_map.insert(std::pair<string, int>("12", 1));
  op_map.insert(std::pair<string, int>("13", 1));
  op_map.insert(std::pair<string, int>("14", 0));
  
	string buff{""};
	vector<string> v;
	int prog_counter = 0, op_counter = 0, in_data = 0;

	for(auto n:s){
		if(n != c) buff+=n; else{
		if(n == c && buff != "") { 
      v.push_back(buff);
      if(in_data == 0){
        if(op_counter == 0){
          if(buff == "14"){ 
            stopLocation = prog_counter;
            in_data++;
          }
        }
        if(op_counter > 0){
          op_counter--; 
        }else{
          op_counter = op_map.at(buff);
        }
      }
      prog_counter++;
      buff = ""; 
      }
    }
	}

	if(buff != ""){
    v.push_back(buff);
    prog_counter++;
  } 
	
	return v;
}

int input(){
  int in = 0;
  cout << "Favor, inserir um valor númerico: ";
  cin >> in;
  return in;
}

void output(int num){
  cout << "O valor de saída é: " << num << endl;
  return;
}

int is_memory(int position){
  if (position <= stopLocation){
    return -1;
  }
  return position;
}

void memory_access(int position, int data){
  int value = is_memory(position);
  if (value >= 0){
    cout << "POSIÇÃO DE MEMÓRIA " << value << " ACESSADA, VALOR: " << data << endl;
  }
}

void controller(int argc, string fileName) {
  int val, mem;
  string line;
  objFile.open(fileName.c_str(), ios::in);
  getline( objFile, line);
  vector<string> obj{explode(line, ' ')};
  for(int i=0; i < (int)obj.size(); i++){   
    switch (stoi(obj[i])){
      case 1:
        cout << "EXECUTANDO INSTRUÇÃO: ADD " << endl;
        mem = stoi(obj[i+1]);
        acc = acc + stoi(obj[mem]);
        memory_access(mem, stoi(obj[mem]));
        cout << "Acumulador após ADD: " << acc <<endl <<endl;
        i++;
      break;

      case 2:
        cout << "EXECUTANDO INSTRUÇÃO: SUB " << endl;
        mem = stoi(obj[i+1]);
        acc = acc - stoi(obj[mem]);
        memory_access(mem, stoi(obj[mem]));
        cout << "Acumulador após SUB: " << acc <<endl <<endl;
        i++;
      break;

      case 3:
        cout << "EXECUTANDO INSTRUÇÃO: MULT " << endl;
        mem = stoi(obj[i+1]);
        acc = acc * stoi(obj[mem]);
        memory_access(mem, stoi(obj[mem]));
        cout << "Acumulador após MULT: " << acc <<endl <<endl;
        i++;
      break;

      case 4:
        cout << "EXECUTANDO INSTRUÇÃO: DIV " << endl;
        mem = stoi(obj[i+1]);
        acc = acc / stoi(obj[mem]);
        memory_access(mem, stoi(obj[mem]));
        cout << "Acumulador após DIV: " << acc <<endl <<endl;
        i++;
      break;

      case 5:
        cout << "EXECUTANDO INSTRUÇÃO: JMP " << endl;
        i = stoi(obj[i+1]);
        cout << "VALOR DE PC: " << i <<endl;
        cout << "Acumulador após JMP: " << acc <<endl <<endl;
        i--;       
      break;

      case 6:
        cout << "EXECUTANDO INSTRUÇÃO: JMPN " << endl;
        if(acc < 0){
          i = stoi(obj[i+1]);
          cout << "VALOR DE PC: " << i <<endl;
          cout << "Acumulador após JMPN: " << acc <<endl <<endl;           
          i--;
        }else{
          cout << "VALOR DE PC: " << i <<endl;
          cout << "Acumulador após JMPN: " << acc <<endl <<endl;   
          i++;   
        } 
      break;

      case 7:
        cout << "EXECUTANDO INSTRUÇÃO: JMPP " << endl;
        if(acc > 0){
          i = stoi(obj[i+1]);
          cout << "VALOR DE PC: " << i <<endl;
          cout << "Acumulador após JMPP: " << acc <<endl <<endl;               
          i--;
        }else{
          cout << "VALOR DE PC: " << i <<endl;
          cout << "Acumulador após JMPP: " << acc <<endl <<endl;    
          i++;   
        }
      break;

      case 8:
        cout << "EXECUTANDO INSTRUÇÃO: JMPZ " << endl;
        if(acc == 0){
          i = stoi(obj[i+1]);
          cout << "VALOR DE PC: " << i <<endl;
          cout << "Acumulador após JMPz: " << acc <<endl <<endl;                
          i--;
        }else{
          cout << "VALOR DE PC: " << i <<endl;
          cout << "Acumulador após JMPZ: " << acc <<endl <<endl;    
          i++;   
        }
      break;

      case 9:
        cout << "EXECUTANDO INSTRUÇÃO: COPY " << endl;
        obj[stoi(obj[i+2])] = obj[stoi(obj[i+1])];
        memory_access(stoi(obj[i+1]), stoi(obj[stoi(obj[i+1])]));
        memory_access(stoi(obj[i+2]), stoi(obj[stoi(obj[i+2])]));
        cout << "Acumulador após COPY: " << acc <<endl <<endl;    
        i+= 2;
      break;

      case 10:
        cout << "EXECUTANDO INSTRUÇÃO: LOAD " << endl;
        mem = stoi(obj[i+1]);
        acc = stoi(obj[mem]);
        memory_access(mem, stoi(obj[mem]));
        cout << "Acumulador após LOAD: " << acc <<endl <<endl;
        i++;
      break;

      case 11:
        cout << "EXECUTANDO INSTRUÇÃO: STORE " << endl;
        mem = stoi(obj[i+1]);
        obj[mem] = to_string(acc);
        memory_access(mem, stoi(obj[mem]));
        cout << "Acumulador após STORE: " << acc <<endl <<endl;    
        i++;
      break;
      
      case 12:
        cout << "EXECUTANDO INSTRUÇÃO: INPUT " << endl;
         val = input();
         mem = stoi(obj[i+1]);
        obj[mem] = to_string(val);
        memory_access(mem, stoi(obj[mem]));
        cout << "Acumulador após INPUT: " << acc <<endl <<endl;    
        i++;
      break;

      case 13:
        cout << "EXECUTANDO INSTRUÇÃO: OUTPUT " << endl;
        mem = stoi(obj[i+1]);
        output(stoi(obj[mem]));
        memory_access(mem, stoi(obj[mem]));
        cout << "Acumulador após OUTPUT: " << acc <<endl <<endl;    
        i++;
      break;

      case 14:
        cout << "EXECUTANDO INSTRUÇÃO: STOP " << endl;
        exit(0);
      break;

      default:
      break;
    }
  }  
  objFile.close();
}

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