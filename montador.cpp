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

// Struct da tabela de instruções
typedef struct instruction {
  string name;
  int length;
  int opcode;
  int operands;
} instruction;

// Struct da tabela de diretivas
typedef struct directive {
  string name;
  int length;
  int operands;
} directive;

// Struct da tabela de símbolos
typedef struct symbol {
  string name;
  int value;
  bool defined;
  bool constante;
  bool externo;
  bool publico;
  vector<int> list;
} symbol;

// Struct da tabela de definições
typedef struct definitions {
  string name;
  int value;
} definitions;

// Struct da tabela de uso
typedef struct use {
  string name;
  int address;
} use;

// Tabela de Uso - indica os s´ımbolos externos utilizados no m´odulo.
// Tabela de Defini¸c˜oes - indica os s´ımbolos p´ublicos e seus atributos (´e
// uma fra¸c˜ao da TS).

vector<instruction> instruction_table; /*Tabela de instruções*/
vector<directive> directive_table;     /*Tabela de diretivas*/
vector<symbol> symbol_table;           /*Tabela de símbolos*/
vector<use> use_table;                 /*Tabela de uso*/
vector<definitions> definitions_table; /*Tabela de definiçẽos*/
vector<string> assembled_lines;        /*Linhas montadas*/
vector<string> errors;
map<int, int> pendencia;
// lê e escreve o pre
fstream preFile;
// apenas escreve o obj
ofstream objFile;

int positionCount = 0, sectionTextLine = -1, sectionDataLine = -1,
    stopLine = -1, lineCount = 0, stopPosition = -1;
int now_const = 0, now_extern = 0, now_public = 0, textLength = 0;

void global_reset() {
  positionCount = 0;
  sectionTextLine = -1;
  sectionDataLine = -1;
  stopLine = -1;
  stopPosition = -1;
  lineCount = 0;
  now_const = 0;
  now_extern = 0;
  textLength = 0;
  symbol_table.clear();
  errors.clear();
  assembled_lines.clear();
  pendencia.clear();
  use_table.clear();
  definitions_table.clear();
}
/*Inicializa a tabela de instruções*/
void instruction_table_init() {
  instruction instruction;

  instruction.name = "ADD";
  instruction.length = 2;
  instruction.opcode = 1;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "SUB";
  instruction.length = 2;
  instruction.opcode = 2;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "MULT";
  instruction.length = 2;
  instruction.opcode = 3;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "DIV";
  instruction.length = 2;
  instruction.opcode = 4;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "JMP";
  instruction.length = 2;
  instruction.opcode = 5;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "JMPN";
  instruction.length = 2;
  instruction.opcode = 6;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "JMPP";
  instruction.length = 2;
  instruction.opcode = 7;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "JMPZ";
  instruction.length = 2;
  instruction.opcode = 8;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "COPY";
  instruction.length = 3;
  instruction.opcode = 9;
  instruction.operands = 2;
  instruction_table.push_back(instruction);

  instruction.name = "LOAD";
  instruction.length = 2;
  instruction.opcode = 10;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "STORE";
  instruction.length = 2;
  instruction.opcode = 11;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "INPUT";
  instruction.length = 2;
  instruction.opcode = 12;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "OUTPUT";
  instruction.length = 2;
  instruction.opcode = 13;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "STOP";
  instruction.length = 1;
  instruction.opcode = 14;
  instruction.operands = 0;
  instruction_table.push_back(instruction);
}

/*Inicializa a tabela de diretivas*/
void directive_table_init() {
  directive directive;

  directive.name = "SECTION";
  directive.length = 0;
  directive.operands = 1;
  directive_table.push_back(directive);

  directive.name = "SPACE";
  directive.length = 1;
  directive.operands = 1;
  directive_table.push_back(directive);

  directive.name = "CONST";
  directive.length = 1;
  directive.operands = 1;
  directive_table.push_back(directive);

  directive.name = "EQU";
  directive.length = 0;
  directive.operands = 1;
  directive_table.push_back(directive);

  directive.name = "IF";
  directive.length = 0;
  directive.operands = 1;
  directive_table.push_back(directive);

  directive.name = "BEGIN";
  directive.length = 0;
  directive.operands = 0;
  directive_table.push_back(directive);

  directive.name = "END";
  directive.length = 0;
  directive.operands = 0;
  directive_table.push_back(directive);

  directive.name = "PUBLIC";
  directive.length = 0;
  directive.operands = 1;
  directive_table.push_back(directive);

  directive.name = "EXTERN";
  directive.length = 0;
  directive.operands = 0;
  directive_table.push_back(directive);
}

/*Verifica os erros relacionados a operandos e operações*/
void check_instruction_errors(vector<string> words, int position_count) {
  string aux = "";
  int num = -1, space_count = 0;
  unsigned int i, j;

  if ((words[1] == "JMP") || (words[1] == "JMPN") || (words[1] == "JMPP") ||
      (words[1] == "JMPZ")) {
    for (i = 0; i < symbol_table.size(); i++) {
      if (words[2] == symbol_table[i].name) {
        num = symbol_table[i].value;
      }
    }
    if (num == -1) {
      cout << "ERRO SEMANTICO: Símbolo de salto não encontrado! Linha: "
           << lineCount << endl;
    }
    if ((num < sectionTextLine) ||
        ((num > sectionDataLine) && (num < sectionTextLine))) {
      cout << "ERRO SEMANTICO: Label de salto fora da seção TEXT! Linha: "
           << lineCount << endl;
    }
  }

  if ((words[1] == "ADD") || (words[1] == "SUB") || (words[1] == "MULT") ||
      (words[1] == "DIV") || (words[1] == "LOAD") || (words[1] == "STORE") ||
      (words[1] == "INPUT") || (words[1] == "OUTPUT") || (words[1] == "COPY")) {
    if (words[2].find(" ") == string::npos) {
      aux = words[2];
    } else if (words[2].find(" + ") != string::npos) {
      i = 0;
      while (words[2][i] != ' ') {
        aux = aux + words[2][i];
        i++;
      }
      for (i = 0; i < words[2].length(); i++) {
        if (words[2][i] == ' ') {
          space_count++;
        }
      }
      if (space_count > 2) {
        cout << "ERRO LEXICO: Expressão não valida! Linha: " << lineCount
             << endl;
      }
    } else {
      cout << "ERRO LEXICO: Expressão não valida! Linha: " << lineCount << endl;
    }
  }

  if (words[1] == "COPY") {
    space_count = 0;
    aux = "";
    num = -1;
    if (words[3].find(" ") == string::npos) {
      aux = words[3];
    } else if (words[3].find(" + ") != string::npos) {
      i = 0;
      while (words[3][i] != ' ') {
        aux = aux + words[3][i];
        i++;
      }
      for (i = 0; i < words[3].length(); i++) {
        if (words[3][i] == ' ') {
          space_count++;
        }
      }
      if (space_count > 2) {
        cout << "ERRO LEXICO: Expressão não valida! Linha: " << lineCount
             << endl;
      }
    } else {
      cout << "ERRO LEXICO: Expressão não valida! Linha: " << lineCount << endl;
    }
  }

  if (words[1] == "DIV") {
    for (i = 0; i < symbol_table.size(); i++) {
      if (symbol_table[i].constante) {
        if ((words[2] == symbol_table[i].name) &&
            (symbol_table[i].value == 0)) {
          cout << "ERRO SEMANTICO: Divisão por 0! Linha: " << lineCount << endl;
        }
      }
    }
  }

  if ((words[1] == "STORE") || (words[1] == "INPUT")) {
    for (i = 0; i < symbol_table.size(); i++) {
      if (symbol_table[i].constante) {
        if (words[2] == symbol_table[i].name) {
          cout << "ERRO SEMANTICO: STORE em CONST! Linha: " << lineCount
               << endl;
        }
      }
    }
  }

  if (words.size() >= 3) {
    for (j = 0; j < words[2].length(); j++) {
      if ((words[2][j] < '0' || words[2][j] > '9') &&
          (words[2][j] < 'a' || words[2][j] > 'z') &&
          (words[2][j] < 'A' || words[2][j] > 'Z') && words[2][j] != ' ' &&
          words[2][j] != '_' && words[2][j] != '+' && words[2][j] != '-') {
        cout << "ERRO LEXICO: Operando possui um caractere não válido Linha: "
             << lineCount << endl;
      }
    }
  }

  if (words.size() == 4) {
    for (j = 0; j < words[3].length(); j++) {
      if ((words[3][j] < '0' || words[3][j] > '9') &&
          (words[3][j] < 'a' || words[3][j] > 'z') &&
          (words[3][j] < 'A' || words[3][j] > 'Z') && words[3][j] != ' ' &&
          words[3][j] != '_' && words[3][j] != '+' && words[3][j] != '-') {
        cout << "ERRO LEXICO: Operando possui um caractere não válido Linha: "
             << lineCount << endl;
      }
    }
  }

  if (words.size() >= 3) {
    if ((words[2][0] >= '0' && words[2][0] <= '9') && (words[1] != "SPACE") &&
        (words[1] != "CONST")) {
      cout << "ERRO LEXICO: Operando começa com um número Linha: " << lineCount
           << endl;
    }
  }

  if (words.size() == 4) {
    if ((words[3][0] >= '0' && words[3][0] <= '9') && (words[1] != "SPACE") &&
        (words[1] != "CONST")) {
      cout << "ERRO LEXICO: Operando começa com um número Linha: " << lineCount
           << endl;
    }
  }
}

int tokenErrors(string label) {
  if (isdigit(label[0])) {
    cout
        << "ERRO LEXICO: Identificadores não podem começar com números! Linha: "
        << endl;
    exit(-1);
  }
  if ((int)label.size() > 50) {
    cout << "ERRO LEXICO: Identificadore não podem ter mais de 50 caracteres! "
            "Linha: "
         << lineCount << endl;
    exit(-1);
  }
  // for (char c : label){
  //   if(!isalnum(c) && !(c=='_')){
  //     cout << "ERRO LEXICO: Caracteres não permitidos no identificador!" <<
  //     endl; exit(-1);
  //   }
  // }
  return 0;
}

void addDefinitionTable() {
  int achou = 0;
  for (symbol s : symbol_table) {
    if (s.publico) {
      for (definitions d : definitions_table) {
        if (d.name == s.name) {
          achou++;
        }
      }
      if (!achou) {
        definitions_table.push_back({s.name, s.value});
      }
    }
  }
}

void updateDefinitionTable() {
  for (symbol s : symbol_table) {
    for (definitions d : definitions_table) {
      if (d.name == s.name) {
        d.value = s.value;
      }
    }
  }
}

// SUCESSO
void symbolTableCheck() {
  // percorre toda a tabela de símbolos e se tiver algum undefined, encerra com
  // erro
  for (symbol s : symbol_table) {
    cout << "[symbolTableCheck] "
         << "Símbolo " << s.name << "  " << s.value
         << (s.defined ? " T " : " F ") << (s.externo ? "externo" : "")
         << (s.publico ? "publico" : "") << endl;

    if (s.defined == false) {
      cout << "[symbolTableCheck] "
           << "ERRO SEMANTICO: Símbolo " << s.name
           << " não definido Linha: " << lineCount << endl;

      updateDefinitionTable();
    }
  }

  cout << endl;
  for (use u : use_table) {
    cout << "[UseTableCheck] "
         << "name " << u.name << "  " << u.address << endl;
  }
  cout << endl;
  for (definitions d : definitions_table) {
    cout << "[DefinitionsTableCheck] "
         << "name " << d.name << "  " << d.value << endl;
  }
}

// TODO
// Funcao que checa se:
// 1. string tem até 50 de tamanho
// 2. não começa com número
// +
// Adicionar suporte a _ na regex

vector<string> tokeniza(string line) {
  vector<string> tokens;  //
  // ler a linha
  smatch sm;
  regex label_regex(
      "(?:((?:[[:alnum:]]|\\_)+):\\s)?([A-Z]+)\\s?(-?(?:[[:alnum:]]|\\+|\\-|\\_"
      ")+)?(?:,(-?(?:[[:alnum:]]|\\+|\\-|\\_)+))?");

  regex_match(line, sm, label_regex);

  // pegar label
  tokenErrors(sm[1]);
  tokens.push_back(sm[1]);
  // pegar instruction
  tokens.push_back(sm[2]);
  // pegar operator1
  tokens.push_back(sm[3]);
  // pegar operator2
  tokens.push_back(sm[4]);

  return tokens;
}

// APENAS PARA LABEL
int checkLabelInTable(string label) {
  for (symbol s : symbol_table) {
    if (s.name == label) {
      if (s.defined == true) {
        return 1;
      }
    }
  }
  return 0;
}

int lineHasSymbol(string op1, string op2, int lineCount) {
  int resultado = 0;
  for (auto c : op1) {
    if (isalpha(c)) {
      if (isupper(c)) {
        resultado = 1;
      }
      break;
    }
  }
  for (auto c : op2) {
    if (isalpha(c)) {
      if (isupper(c)) {
        resultado += 2;
      }
      break;
    }
  }
  switch (resultado) {
    case 0:

      break;
    case 1:

      break;
    case 2:

      break;
    case 3:

      break;
    default:
      break;
  }
  return resultado;
}

// PASSO_6

int addSymbolToTable(string name, int position) {
  bool konst = false;
  // bool publico = false;
  if (now_const) {
    konst = true;
    now_const = 0;
  }
  // if (now_public) {
  //   cout << name << " Tá como PUBLIC" << endl;
  //   publico = true;
  //   now_public = 0;
  // }
  vector<int> lista;
  lista.push_back(position);
  symbol s = {name, -1, false, konst, false, false, lista};
  symbol_table.push_back(s);
  addDefinitionTable();
  return 0;
}

// PASSO_7
int addEntryToSymbolOcurrenceList(string simbolo, int position) {
  bool konst = false;

  if (now_const) {
    konst = true;
    now_const = 0;
  }

  int i;
  for (i = 0; i < (int)symbol_table.size(); i++) {
    if (symbol_table[i].name == simbolo) {
      symbol_table[i].list.push_back(position);
      symbol_table[i].constante = konst;
      return 0;
    }
  }
  return -1;
}

// PARA SIMBOLOS COM +
void getValueWithOffsetFromST(string simbolo) {}

void pushToUseTable(string simbolo, int pos) {
  for (symbol s : symbol_table) {
    if (s.name == simbolo) {
      if (s.externo == true) {
        use uso;
        uso.address = pos;
        uso.name = simbolo;
        use_table.push_back(uso);
      }
    }
  }
}

int handlePublicDirective(string name, int position) {
  cout << "Tratando diretiva " << name << " como PUBLIC" << endl;
  for (int i = 0; i < (int)symbol_table.size(); i++) {
    if (symbol_table[i].name == name) {
      symbol_table[i].publico = true;

      cout << name << " está como "
           << (symbol_table[i].publico ? "publico" : "") << endl;
    }
  }

  return 0;
}

int searchSTForSymbol(string simbolo, int pos) {
  int is_vector = 0;
  for (auto x : simbolo) {
    if (x == '+') {
      is_vector++;
    }
  }
  if (is_vector > 1) {
    cout << "ERRO LEXICO: TOKEN INVALIDO! Linha: " << lineCount << endl;
    exit(-1);
  } else if (is_vector > 0) {
    int offset = 0;
    string name = "", buff = "";
    for (auto s : simbolo) {
      if (s != '+') {
        // Enquanto não chegar no +, enche o buffer
        buff += s;
      } else if (s == '+' && buff != "") {
        // Quando chegar no +, salva o conteudo do buffer
        name = buff;
        buff = "";
      }
    }
    if (buff != "") {
      offset = stoi(buff);
    }
    pendencia.insert(std::pair<int, int>(pos, offset));
    simbolo = name;
    // getchar();
  }

  for (symbol s : symbol_table) {
    if (s.name == simbolo) {
      if (!s.defined) {  // Não ta definido
        // VAI PARA PASSO_7
        addEntryToSymbolOcurrenceList(simbolo, pos);
      }
      // VAI PARA PASSO_3
      return 0;
    }
  }
  // VAI PRO PASSO_6
  addSymbolToTable(simbolo, pos);
  return 0;
}

string consulST(string name, int pos) {
  for (auto symb : symbol_table) {
    if (symb.name == name) {
      int flag = 0, value = 0;
      for (auto it = pendencia.begin(); it != pendencia.end(); it++) {
        if (it->first == pos) {
          flag = 1;
          value = it->second;
        }
      }
      if (flag == 1) {
        return to_string(symb.value + value);
      } else {
        return to_string(symb.value);
      }
    }
  }
  return name;
}

int addOrDefineLabelinTable(string name, string inst, int pos) {
  bool konst = false;
  bool esterno = false;
  bool publico = false;

  if (inst == "CONST") {
    konst = true;
  }

  if (inst == "EXTERN") {
    esterno = true;
  }

  if (inst == "PUBLIC") {
    publico = true;
  }

  int tam = symbol_table.size();
  for (int k = 0; k < tam; k++) {
    if (symbol_table[k].name == name) {
      symbol_table[k].value = pos;
      symbol_table[k].constante = konst;
      symbol_table[k].externo = esterno;
      symbol_table[k].publico = publico;
      symbol_table[k].defined = true;
      return 0;
    }
  }
  vector<int> list;
  list.push_back(-1);
  symbol s = {name, pos, true, konst, esterno, publico, list};
  // PASSO 4

  symbol_table.push_back(s);
  return 0;
}

void singlePass(string fileName) {
  ifstream inputfile;
  vector<vector<string>> prelinhas;
  string line;
  vector<string> tokens;
  int labelDefined = 0;
  while (!preFile.eof()) {
    int instrType = 0;
    getline(preFile, line);
    lineCount++;
    // cout << "Linha " << lineCount << " : " << line << endl;
    tokens = tokeniza(line);
    /* tokens[0] = label
     * tokens[1] = instrução/diretiva
     * tokens[2] = operando 1
     * tokens[3] = operando 2
     */

    if (tokens.at(1) == "SECTION") {
      if (tokens.at(2) == "TEXT") {
        sectionTextLine = lineCount;
      } else if (tokens.at(2) == "DATA") {
        sectionDataLine = lineCount;
        if (sectionDataLine > sectionTextLine) {
          textLength = positionCount;
        }
      } else {
        cout << "ERRO SEMANTICO: seção inválida! Linha: " << lineCount << endl;
        exit(-1);
      }
      continue;
    }

    if (tokens.at(1) == "STOP") {
      stopLine = lineCount;
      if (sectionDataLine < sectionTextLine) {
        textLength = positionCount;
      }
      stopPosition = positionCount;
    }

    if (tokens.at(0) != "") {
      // cout << "TEM LABEL" << endl;
      labelDefined = checkLabelInTable(tokens.at(0));
      if (labelDefined) {
        getchar();
        exit(-1);
      }

      addOrDefineLabelinTable(tokens.at(0), tokens.at(1), positionCount);
      // Se tem label e uma operação com uma "variavel"

      if (tokens.at(2) != "") {
        pushToUseTable(tokens.at(2), positionCount + 1);
      }
    } else {
      //  addOrDefineLabelinTable(tokens.at(2), tokens.at(1), positionCount);
      if (tokens.at(2) != "") {
        pushToUseTable(tokens.at(2), positionCount + 1);
      }
      // cout << "NÃO TEM LABEL" << endl;
    }
    // PASSO 2
    // Confere se o numero de operandos está correto
    for (directive d : directive_table) {
      if (d.name == tokens.at(1)) {
        int val = 0;
        val = tokens.at(2) == "" ? val : val + 1;
        val = tokens.at(3) == "" ? val : val + 1;
        if ((d.name == "SPACE") && (val == 0)) {
          /**
           * SPACE pode receber 0 ou 1 operandos, na tabela registramos 1
           * Para que a verificação seja correta, quando o valor for 0, mudamos
           * para 1
           */
          val = 1;
        }
        // cout << tokens.at(2) << "    " << tokens.at(3) << endl;

        if (d.operands == val) {
          vector<string> lili;
          if (tokens.at(1) == "SPACE") {
            searchSTForSymbol(tokens.at(0), positionCount + 1);

            if (tokens.at(2) != "") {
              // vetor
              string aaa = tokens.at(2);
              int max = stoi(aaa.c_str());
              for (int n = 0; n < max; n++) {
                lili.push_back("0");
                positionCount++;
              }
            } else {
              lili.push_back("0");
              positionCount++;
              // 1espaco
            }
            prelinhas.push_back(lili);
          } else if (tokens.at(1) == "CONST") {
            now_const = 1;
            searchSTForSymbol(tokens.at(0), positionCount + 1);
            // Já está como constante na ST
            // Se o valor for 0, ve se tem alguma div pendente
            if (tokens.at(2) == "0") {
              for (auto x : symbol_table) {
                if (x.name == tokens.at(0)) {
                  for (auto prelinha : prelinhas) {
                    for (int p = 0; p < (int)prelinha.size(); p++) {
                      if (prelinha[p] == "4") {
                        if (prelinha[p + 1] == tokens.at(0)) {
                          cout << "ERRO SEMANTICO: Não existe divisão por 0! "
                                  "Linha: "
                               << lineCount << endl;
                        }
                      }
                    }
                  }
                }
              }
            }
            positionCount += 1;
            if (tokens.at(2) == "") {
              cout
                  << "ERRO SINTATICO: QUANTIDADE DE OPERANDOS INVÁLIDA! Linha: "
                  << lineCount << endl;
              exit(-1);
            }

            regex caraio("(-)?(0X)?([[:digit:]]+)$");
            smatch tinder;
            regex_match(tokens.at(2), tinder, caraio);

            std::istringstream converter(tinder[3]);
            int value;
            converter >> std::hex >> value;

            if (tinder[1] == "-") {
              value = value * -1;
            }

            lili.push_back(to_string(value));
            prelinhas.push_back(lili);
          } else if (tokens.at(1) == "BEGIN") {
            searchSTForSymbol(tokens.at(0), positionCount + 1);
          } else if (tokens.at(1) == "END") {
            // Faz nada
          } else if (tokens.at(1) == "PUBLIC") {
            searchSTForSymbol(tokens.at(2), positionCount + 1);
            handlePublicDirective(tokens.at(2), positionCount + 1);
            // addDefinitionTable();
          } else if (tokens.at(1) == "EXTERN") {
            now_extern = 1;
            searchSTForSymbol(tokens.at(0), positionCount + 1);
          } else {
            cout << "ERRO SEMANTICO: DIRETIVA INVÁLIDA! Linha: " << lineCount
                 << endl;
            exit(-1);
          }
        } else {
          cout << "ERRO : Nº DE OPERANDOS ERRADO! Linha: " << lineCount << endl;
          exit(-1);
          getchar();
        }
        instrType = 1;
        break;
      }
    }
    if (!instrType) {
      for (instruction i : instruction_table) {
        if (i.name == tokens.at(1)) {
          int val = 0;
          val = tokens.at(2) == "" ? val : val + 1;
          val = tokens.at(3) == "" ? val : val + 1;

          if (i.operands == val) {
            int symbolFlag =
                lineHasSymbol(tokens.at(2), tokens.at(3), lineCount);
            // PASSO_3
            // Montagem da linha

            for (auto linha : prelinhas) {
              for (auto l : linha) {
              }
            }
            vector<string> lolo;
            int opio;
            string valeu;
            string comais = tokens.at(2);
            string semais = "", buff = "";
            switch (symbolFlag) {
              case 0:
                // CASO STOP
                for (auto inst : instruction_table) {
                  if (inst.name == tokens.at(1)) {
                    opio = inst.opcode;
                  }
                }
                lolo.push_back(to_string(opio));
                positionCount++;
                break;
              case 1:
                searchSTForSymbol(tokens.at(2), positionCount + 1);
                for (auto inst : instruction_table) {
                  if (inst.name == tokens.at(1)) {
                    opio = inst.opcode;
                  }
                }

                lolo.push_back(to_string(opio));

                for (auto s : comais) {
                  if (s != '+') {
                    // Enquanto não chegar no +, enche o buffer
                    buff += s;
                  } else if (s == '+' && buff != "") {
                    // Quando chegar no +, salva o conteudo do buffer
                    semais = buff;
                    break;
                  }
                  if (buff != "") {
                    semais = buff;
                  }
                }

                for (auto sy : symbol_table) {
                  if (sy.name == semais) {
                    valeu = sy.name;
                  }
                }
                lolo.push_back(valeu);
                positionCount += 2;
                break;
              case 2:
                searchSTForSymbol(tokens.at(3), positionCount + 2);
                for (auto inst : instruction_table) {
                  if (inst.name == tokens.at(1)) {
                    opio = inst.opcode;
                  }
                }
                lolo.push_back(to_string(opio));
                for (auto sy : symbol_table) {
                  if (sy.name == tokens.at(3)) {
                    valeu = sy.name;
                  }
                }
                lolo.push_back(valeu);
                positionCount += 3;
                break;
              case 3:
                searchSTForSymbol(tokens.at(2), positionCount + 1);
                searchSTForSymbol(tokens.at(3), positionCount + 2);

                for (auto inst : instruction_table) {
                  if (inst.name == tokens.at(1)) {
                    opio = inst.opcode;
                  }
                }
                lolo.push_back(to_string(opio));
                for (auto sy : symbol_table) {
                  if (sy.name == tokens.at(2)) {
                    valeu = sy.name;
                  }
                }
                lolo.push_back(valeu);

                for (auto sy : symbol_table) {
                  if (sy.name == tokens.at(3)) {
                    valeu = sy.name;
                  }
                }

                lolo.push_back(valeu);
                positionCount += 3;
                break;
              default:
                break;
            }

            check_instruction_errors(tokens, positionCount);

            prelinhas.push_back(lolo);
          } else {
            cout << "ERRO : Nº DE OPERANDOS ERRADO! Linha: " << lineCount
                 << endl;
            getchar();
            exit(-1);
          }
          break;
        }
      }
    }
  }
  if ((sectionDataLine < sectionTextLine) && stopPosition == -1) {
    textLength = positionCount;
  }
  preFile.close();
  updateDefinitionTable();
  symbolTableCheck();

  inputfile.open((fileName).c_str());
  int offset = fileName.length() - 4;
  string printfileName = fileName.replace(offset, 4, "");
  fileName.replace(offset, 4, ".obj");
  ofstream outfile(fileName);
  outfile << "H: " << printfileName << endl;
  outfile << "H: " << textLength - 1 << endl;
  outfile << "H: ";
  for (int w = 0; w < textLength; w++) {
    int flag = 0;
    for (use u : use_table) {
      if (u.address == w) {
        flag = 1;
      }
    }
    if (flag == 1) {
      outfile << "1";
    } else {
      outfile << "0";
    }
  }
  outfile << endl;
  outfile << "H: " << use_table.size();
  for (use u : use_table) {
    outfile << " " << u.name << " " << u.address;
  }
  outfile << endl;
  outfile << "H: " << definitions_table.size();
  for (definitions d : definitions_table) {
    outfile << " " << d.name << " " << d.value;
  }
  outfile << endl;
  outfile << "T: ";
  int j = 0;
  for (auto pre : prelinhas) {
    for (int i = 0; i < (int)pre.size(); i++) {
      pre[i] = consulST(pre[i], j);
      j++;
      outfile << pre[i] << " ";
    }
  }

  cout << endl;

  outfile.close();
}

vector<string> lineParsing(string line, ifstream *inputfile, int *line_count) {
  vector<string> words, wordsAux;
  string aux;
  unsigned int i = 0, j = 0, finish = 0, rotCount = 0;
  int size;
  while (finish == 0) {
    while (i <= line.length()) { /*Le a linha toda*/
      while (((line[i] == ' ') || (line[i] == '\t')) &&
             (i < line.length())) { /*Remove espaços vazios*/
        i++;
      }

      if (line[i] == ';') { /*Ignora comentarios*/
        break;
      }

      if ((line[i] == '\0') && (aux == "")) { /*Ignora comentarios*/
        break;
      }

      aux = aux + line[i];

      if ((line[i + 1] == ' ') || (line[i + 1] == '\t') ||
          (line[i + 1] == '\n') || (i == line.length())) {
        if ((wordsAux.size() == 0) && (aux.find(":") == string::npos)) {
          wordsAux.push_back("none:");
        }
        if (aux[aux.length() - 1] == '\0') {
          aux.erase(aux.length() - 1, aux.length() - 1);
        }
        wordsAux.push_back(aux);
        aux = "";
        j = 0;
      }

      j++;
      i++;
    }
    if (wordsAux.size() == 0) {
      break;
    }
    if ((wordsAux.size() != 1) &&
        (wordsAux[0].find(":") != string::npos)) { /*Caso só tenha a label ele
                                                      junta com a outra linha*/
      finish = 1;
    } else {
      getline(*inputfile, line);
      transform(line.begin(), line.end(), line.begin(),
                ::toupper); /*Deixa toda a string maiuscula*/
      i = 0;
      j = 0;
      *line_count = *line_count + 1;
    }
  }

  if (wordsAux.size() >= 2) {
    words.push_back(wordsAux[0]);
    words.push_back(wordsAux[1]);
  }
  for (i = 2; i < wordsAux.size(); i++) {
    aux = wordsAux[i];
    size = wordsAux[i].length() - 1;
    while ((wordsAux[i][size] != ',') && (i != wordsAux.size() - 1)) {
      aux = aux + " " + wordsAux[i + 1];
      i++;
      size = wordsAux[i].length() - 1;
    }
    words.push_back(aux);
  }

  for (i = 0; i < words.size(); i++) { /*retira virgulas e dois pontos*/
    size = words[i].length() - 1;
    if (words[i][size] == ':') {
      rotCount++;
    }
    if ((words[i][size] == ':') || (words[i][size] == ',')) {
      words[i].erase(size, size);
    }
  }

  if (rotCount >= 2) {
    cout << "ERRO SINTATICO: Dois rotulos para mesma instrução, linha: "
         << line_count << endl;

    exit(-1);
  }
  return words;
}

void preProcessor(string fileName, bool twoFiles) {
  ifstream inputfile;
  string line;
  map<string, int> EQUTable;
  int flag = 0, textSectionFound = 0, lineCount = 0, data_location = -1,
      beginFound = 0, endFound = 0;
  vector<string> sm;
  vector<string> data_camp;
  inputfile.open((fileName).c_str());
  int offset = fileName.length() - 3;
  fileName.replace(offset, 3, "pre");
  ofstream outfile(fileName);
  while (getline(inputfile, line)) {
    lineCount++;
    for (int i = 0; i < (int)line.length(); i++) {
      line[i] = toupper(line[i]);
    }
    sm = lineParsing(line, &inputfile, &lineCount);

    if (sm.size() > 0) {
    } else {
      continue;
    }
    for (string x : sm) {
    }

    if (sm[1] == "SECTION") {
      if (sm[2] == "TEXT") {
        textSectionFound++;
      } else if (sm[2] != "DATA" && sm[2] != "TEXT") {
        cout << "ERRO SEMANTICO: SEÇÃO INVALIDA! " << endl;
        exit(-1);
      }
    }

    if (sm[1] == "SECTION") {
      if (sm[2] == "DATA") {
        if (!textSectionFound) {
          // salva as parada na memoria, pq a DATA veio antes da TEXT
          data_location = lineCount;
          continue;
        }
      }
    }
    if (data_location > 0 && textSectionFound == 0) {
      continue;
    }

    if (sm[1] == "EQU") {
      EQUTable.insert(std::pair<string, int>(sm[0], stoi(sm[2])));
    }

    if (sm[1] == "IF") {
      int hasEQU = 0;
      for (auto it = EQUTable.begin(); it != EQUTable.end(); it++) {
        string str = it->first;
        if (str == sm[2] && it->second == 0) {
          flag = 1;
        }

        if (str == sm[2]) {
          hasEQU = 1;
        }
      }
      if (hasEQU == 0) {
        cout << "ERRO SEMANTICO:  IF UTILIZANDO LABEL QUE NÃO FOI DECLARADA "
                "NO "
                "EQU! lINHA? "
             << lineCount << endl;
      } else {
        hasEQU = 0;
      }
    }

    if (sm[1] == "BEGIN" && twoFiles) {
      beginFound = 1;
    }

    if (sm[1] == "END" && twoFiles) {
      endFound = 1;
    }

    if ((sm[1] == "BEGIN" && !twoFiles) || (sm[1] == "END" && !twoFiles)) {
      cout << "ERROR: as diretivas BEGIN e END só podem ser usadas se mais de "
              "1 arquivo forem utilizados"
           << endl;
      exit(0);
    }

    if (flag == 0 && sm[1] != "EQU" && sm[1] != "IF") {
      // cout << "ENTROU NO IF" << endl;
      if (sm[0] != "none") {
        outfile << sm[0] << ": ";
      }
      int n_tokens = (int)sm.size();

      outfile << sm[1];
      if (n_tokens > 2) {
        outfile << " " << sm[2];
      }
      if (n_tokens > 3) {
        outfile << "," << sm[3];
      }
      outfile << endl;

    } else {
      if (sm[1] != "IF") {
        flag = 0;
      }
    }
  }

  if (textSectionFound == 0) {
    cout << "ERRO : SEÇÃO TEXT FALTANDO! " << endl;
  } else if (textSectionFound > 1) {
    cout << "ERRO : MAIS DE UMA SEÇÃO TEXT NO ARQUIVO!" << endl;
  }

  // SE DATA foi declarado antes do TEXT, manda bala
  if (data_location > 0) {
    vector<string> sm;
    int lineCount = 0;
    string line;
    // limpa a flag de EOF
    inputfile.clear();
    // Volta pro inicio do arquivo
    inputfile.seekg(0, std::ios::beg);
    // Vai até o "SECTION DATA"
    for (int i = 0; i < data_location; i++) {
      getline(inputfile, line);
      lineCount = i;
    }
    outfile << "SECTION DATA" << endl;
    // Fica lendo até o "SECTION TEXT"
    while (1) {
      getline(inputfile, line);
      lineCount++;
      // CAPSLOCKA a linha
      for (int ix = 0; ix < (int)line.length(); ix++) {
        line[ix] = toupper(line[ix]);
      }

      sm = lineParsing(line, &inputfile, &lineCount);

      int tokenCount = sm.size();
      if (tokenCount > 0) {
        if (tokenCount > 2) {
          if (sm[1] == "SECTION" && sm[2] == "TEXT") {
            break;
          }
        }
        if (sm[0] != "none") {
          outfile << sm[0] << ": ";
        }
        outfile << sm[1];
        if (tokenCount > 2) {
          outfile << " " << sm[2];
        }
        if (tokenCount > 3) {
          outfile << "," << sm[3];
        }
        outfile << endl;

      } else {
        continue;
      }
    }
  }
  if (beginFound != endFound) {
    cout << "ERROR: BEGIN e END utilizados de maneira inválida" << endl;
    exit(0);
  }

  preFile.close();

  outfile.close();
}

// Função que abre o arquivo texto que vem da linha de comando
void ioController(int argc, string fileName) {
  int offset = fileName.length() - 3;
  fileName.replace(offset, 3, "pre");
  preFile.open(fileName.c_str(), ios::out | ios::in);
}

int main(int argc, char *argv[]) {
  // Verifica se os argumentos foram passados
  if (argc == 2) {
    cout << "[main] "
         << "Arquivo recebido, tentado abrir..." << endl;

    preProcessor(argv[1], false);
    ioController(argc, argv[1]);
    cout << "[main] "
         << "Inicializando a tabela de instruções..." << endl;
    instruction_table_init();
    cout << "[main] "
         << "Inicializando a tabela de diretivas..." << endl;
    directive_table_init();
    singlePass(argv[1]);
  } else if (argc == 3) {
    cout << "3 args, vlw" << endl;
    preProcessor(argv[1], true);
    ioController(argc, argv[1]);
    cout << "[main] "
         << "Inicializando a tabela de instruções..." << endl;
    instruction_table_init();
    directive_table_init();
    cout << "[main] "
         << "Primeiro arquivo..." << endl;
    singlePass(argv[1]);

    global_reset();
    preProcessor(argv[2], true);
    ioController(argc, argv[2]);
    cout << "[main] "
         << "Segundo arquivo..." << endl;
    singlePass(argv[2]);
  } else {
    cout << "[main] "
         << "ERRO SEMANTICO: O programa precisa de 1 argumento. Por favor "
            "insira um arquivo =D"
         << endl;
    return -1;
  }
  return 0;
}
