using namespace std;
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>

constexpr char INCREMENT = '+';
constexpr char DECREMENT = '-';
constexpr char RIGHT = '>';
constexpr char LEFT = '<';
constexpr char LOOP_START = '[';
constexpr char LOOP_END = ']';
constexpr char OUTPUT = '.';
constexpr char INPUT = ',';

constexpr int MEMORY_SIZE = 1024;

int main(int argc, char* argv[]) {
  unsigned char memory[MEMORY_SIZE];
  unsigned int ptr = 0;
  unsigned int code_ptr = 0;
  unsigned int code_len = 0;

  stack<int> loops;

  memset(memory, 0, sizeof(memory));

  if (argc < 2) {
    cerr << "Error: A Brainfuck code is not passed as a command-line argument."
         << endl;
    return -1;
  }

  ifstream file(argv[1]);
  if (!file) {
    cerr << "Error: The file, " << argv[1] << ", cannot be opened." << endl;
    return -1;
  }

  stringstream buffer;

  buffer << file.rdbuf();
  string code(buffer.str());
  code_len = code.size();

  //   cout << code << endl;

  while (code_ptr < code_len) {
    // allows 1 byte
    switch (code[code_ptr]) {
      case INCREMENT:
        memory[ptr]++;
        break;

      case DECREMENT:
        memory[ptr]--;
        break;

      case RIGHT:
        ptr = (ptr >= MEMORY_SIZE - 1) ? 0 : ptr + 1;
        break;

      case LEFT:
        ptr = (ptr <= 0) ? MEMORY_SIZE - 1 : ptr - 1;
        break;

      case LOOP_START:
        loops.push(code_ptr);

        if (memory[ptr] == 0) {
          int depth = 1;
          while (depth > 0) {
            code_ptr++;
            if (code_ptr >= code_len) {
              cerr << "Error: Cannot find \"]\"." << endl;
              return -1;
            }
            if (code[code_ptr] == LOOP_START) depth++;
            if (code[code_ptr] == LOOP_END) depth--;
          }

          loops.pop();
        }

        break;

      case LOOP_END:
        if (loops.empty()) {
          cerr << "Error: Loop start order, " << LOOP_START << ", is not found."
               << endl;
          return -1;
        }
        code_ptr = loops.top() - 1;
        loops.pop();
        break;

      case OUTPUT:
        putchar(memory[ptr]);
        break;

      case INPUT:
        memory[ptr] = getchar();
        break;

      default:
        break;
    }

    code_ptr++;
  }

  cout << "\n" << endl;

  return 0;
}