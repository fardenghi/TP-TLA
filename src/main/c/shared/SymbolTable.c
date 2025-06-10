#include "SymbolTable.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10

SymbolTable* createSymbolTable() {
    SymbolTable* table = (SymbolTable*)malloc(sizeof(SymbolTable));
    if (table == NULL) {
        return NULL;
    }

    table->symbols = (Symbol*)malloc(INITIAL_CAPACITY * sizeof(Symbol));
    if (table->symbols == NULL) {
        free(table);
        return NULL;
    }

    table->size = 0;
    return table;
}

void destroySymbolTable(SymbolTable* symbolTable) {
    if (symbolTable == NULL) {
        return;
    }

    // Liberar cada símbolo
    for (int i = 0; i < symbolTable->size; i++) {
        free(symbolTable->symbols[i].name);
    }

    // Liberar el array de símbolos
    free(symbolTable->symbols);
    
    // Liberar la tabla
    free(symbolTable);
}

Symbol* insertSymbol(SymbolTable* symbolTable, const char* name, const char type, const int value) {
    if (symbolTable == NULL || name == NULL) {
        return NULL;
    }

    // Verificar si el símbolo ya existe
    Symbol* existingSymbol = lookupSymbol(symbolTable, name);
    if (existingSymbol != NULL) {
        return existingSymbol;
    }

    // Crear nuevo símbolo
    Symbol* newSymbol = &(symbolTable->symbols[symbolTable->size]);
    
    // Asignar nombre (hacer una copia)
    newSymbol->name = strdup(name);
    if (newSymbol->name == NULL) {
        return NULL;
    }

    // Asignar tipo y valor
    newSymbol->type = type;
    newSymbol->value = value;

    // Incrementar el tamaño
    symbolTable->size++;

    return newSymbol;
}

Symbol* lookupSymbol(SymbolTable* symbolTable, const char* name) {
    if (symbolTable == NULL || name == NULL) {
        return NULL;
    }

    // Buscar el símbolo por nombre
    for (int i = 0; i < symbolTable->size; i++) {
        if (strcmp(symbolTable->symbols[i].name, name) == 0) {
            return &(symbolTable->symbols[i]);
        }
    }

    return NULL;
}
