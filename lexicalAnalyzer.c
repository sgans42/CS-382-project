/* 
CS 382 Team Project: Lexical Analyzer and Parser
By: Stephen Gansman and Jeremiah Harris
*/
#include <stdio.h>
#include <ctype.h>

// variables
int charClass;
int lexLen;
int token;
int nextToken;
char lexeme[100];
char nextChar;

FILE *inFile;

// functions
void addChar();
void getChar();
void getNonBlank();
int lex();
void expr(FILE *outFile);
void term(FILE *outFile);
void factor(FILE *outFile);

// character classes
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

// codes for tokens
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAR 25
#define RIGHT_PAR 26
#define EOF (-1)

int main() {
  // open input file and process its contents
  if ((inFile = fopen("front.in", "r")) == NULL) {
    printf("ERROR - front.in cannot be opened \n");
  } else {
    FILE *outFile;
    if ((outFile = fopen("syntree_output.txt", "w")) == NULL) {
      printf("ERROR - syntree_output.txt cannot be opened for writing\n");
        return 1;
    }

    getChar();
      do {
        lex();
          expr(outFile);
    } while (nextToken != EOF);

    fclose(outFile);
  }
  return 0;
}

// This function looks for the operators and parentheses
int lookup(char ch) {
  switch (ch) {
    case '(':
      addChar();
      nextToken = LEFT_PAR;
    break;

    case ')':
      addChar();
      nextToken = RIGHT_PAR;
    break;

    case '+':
      addChar();
      nextToken = ADD_OP;
    break;

    case '-':
      addChar();
      nextToken = SUB_OP;
    break;

    case '*':
      addChar();
      nextToken = MULT_OP;
    break;

    case '/':
      addChar();
      nextToken = DIV_OP;
    break;

    case '=':
      addChar();
      nextToken = ASSIGN_OP;
    break;

    default:
      addChar();
      nextToken = EOF;
      break;
  }
  return nextToken;
}

// This function adds nextChar to the lexeme
void addChar() {
  if (lexLen <= 98) {
    lexeme[lexLen++] = nextChar;
    lexeme[lexLen] = 0;
  } else {
    printf("ERROR - lexeme is too long \n");
  }
}

// This function gets the character of the input and determins its character class
void getChar() {
  nextChar = getc(inFile);
  if (nextChar == EOF) {
    charClass = EOF;
  } else {
    if (isalpha(nextChar)) {
      charClass = LETTER;
    } else if (isdigit(nextChar)) {
      charClass = DIGIT;
    } else {
      charClass = UNKNOWN;
    }
  }
}

// This function calls getChar until a non-blank space is found
void getNonBlank() {
  while (isspace(nextChar)) {
    getChar();
  }
}

// This function is the lexical analyzer for arithmetic
int lex() {
  lexLen = 0;
  getNonBlank();

  switch (charClass) {
    case LETTER:
      addChar();
      getChar();
      while (charClass == LETTER || charClass == DIGIT) {
        addChar();
        getChar();
      }
      nextToken = IDENT;
    break;
    
    case DIGIT:
      addChar();
      getChar();
      while (charClass == DIGIT) {
        addChar();
        getChar();
      }
      nextToken = INT_LIT;
    break;

    case UNKNOWN:
      lookup(nextChar);
      getChar();
    break;

    case EOF:
      nextToken = EOF;
      lexeme[0] = 'E';
      lexeme[1] = 'O';
      lexeme[2] = 'F';
      lexeme[3] = 0;
    break;
  }

  printf("Next token is: %d, next lexeme is %s \n", nextToken, lexeme);

  return nextToken;
}

void expr(FILE *outFile) {
  fprintf(outFile, "[expr\n");
  term(outFile);

  while (nextToken == ADD_OP || nextToken == SUB_OP) {
    char op = (nextToken == ADD_OP) ? '+' : '-';
    lex();
    fprintf(outFile, " [%c]\n", op);
    term(outFile);
  }

  fprintf(outFile, "]\n");
}

void term(FILE *outFile) {
  fprintf(outFile, " [term\n");
  factor(outFile);
  while (nextToken == DIV_OP || nextToken == MULT_OP) {
    char op = (nextToken == DIV_OP) ? '/' : '*';
    lex();
    fprintf(outFile, "  [%c]\n", op);
    factor(outFile);
  }
  fprintf(outFile, " ]\n");
}

void factor(FILE *outFile) {
  fprintf(outFile, "  [factor\n   ");

  if (nextToken == IDENT || nextToken == INT_LIT) {
    fprintf(outFile, "  [id [%s]]\n", lexeme);
    lex();
  } else {
    if (nextToken == LEFT_PAR) {
      lex();
      expr(outFile);
       if (nextToken == RIGHT_PAR) {
        lex();
        }
    }
  }

  fprintf(outFile, "   ]\n");
}