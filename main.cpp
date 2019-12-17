#include <iostream>
using namespace std;

int main(int argc, char** argv) {
    const char* fileName = argv[1];
    FILE* file = fopen(fileName, "r");
    if (!file) {
        exit(1);
    }

    return 0;
}

char** lexer(FILE* file) {

}

