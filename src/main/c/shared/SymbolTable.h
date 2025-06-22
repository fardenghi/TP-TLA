#ifndef SYMBOL_TABLE_HEADER
#define SYMBOL_TABLE_HEADER
#include "Type.h"
#include <stdio.h>

typedef struct {
    char * name;
    boolean readOnly;
    int scope; // Nuevo campo para el scope
} Symbol;

#define MAX_SCOPES 128

typedef struct {
    Symbol * symbols;
    int size;
    int capacity;
    int *scopes;            // Stack dinámico de scopes
    int scopeDepth;         // Profundidad actual del stack
    int capacityScopes;     // Capacidad del stack de scopes
    int currentScope;       // Scope actual
    boolean failure;
} SymbolTable;

SymbolTable * createSymbolTable();
void destroySymbolTable(SymbolTable * symbolTable);
Symbol* insertReadOnlySymbol(SymbolTable * symbolTable, const char * name);
Symbol* insertSymbol(SymbolTable * symbolTable, const char * name);
Symbol * lookupSymbol(const SymbolTable * symbolTable, const char * name);
void pushScope(SymbolTable * symbolTable);
void popScope(SymbolTable * symbolTable);
int getCurrentScope(const SymbolTable * symbolTable);
void printScopeStack(const SymbolTable* symbolTable);
void printSymbolTable(const SymbolTable* symbolTable);


#endif
