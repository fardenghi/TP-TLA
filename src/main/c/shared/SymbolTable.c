#include "SymbolTable.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10
#define INITIAL_SCOPE 0
#define INITIAL_SCOPES_CAPACITY 16

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
    table->capacity = INITIAL_CAPACITY;

    table->scopes = (int*)malloc(INITIAL_SCOPES_CAPACITY * sizeof(int));
    if (table->scopes == NULL) {
        free(table->symbols);
        free(table);
        return NULL;
    }
    table->capacityScopes = INITIAL_SCOPES_CAPACITY;
    table->scopeDepth = 1;
    table->scopes[0] = INITIAL_SCOPE;
    table->currentScope = INITIAL_SCOPE;
    return table;
}

void destroySymbolTable(SymbolTable* symbolTable) {
    if (symbolTable == NULL) {
        return;
    }
    for (int i = 0; i < symbolTable->size; i++) {
        free(symbolTable->symbols[i].name);
    }
    free(symbolTable->symbols);
    free(symbolTable->scopes);
    free(symbolTable);
}

void pushScope(SymbolTable* symbolTable) {
    int newScope = symbolTable->currentScope + 1;
    if (symbolTable->scopeDepth == symbolTable->capacityScopes) {
        int newCapacity = symbolTable->capacityScopes * 2;
        int* newScopes = (int*)realloc(symbolTable->scopes, newCapacity * sizeof(int));
        if (newScopes == NULL) {
            return;
        }
        symbolTable->scopes = newScopes;
        symbolTable->capacityScopes = newCapacity;
    }
    symbolTable->scopes[symbolTable->scopeDepth++] = newScope;
    symbolTable->currentScope = newScope;
}

void popScope(SymbolTable* symbolTable) {
    if (symbolTable->scopeDepth > 1) {
        symbolTable->scopeDepth--;
        symbolTable->currentScope = symbolTable->scopes[symbolTable->scopeDepth - 1];
        int i = 0;
        while (i < symbolTable->size) {
            if (symbolTable->symbols[i].scope > symbolTable->currentScope) {
                free(symbolTable->symbols[i].name);
                for (int j = i; j < symbolTable->size - 1; j++) {
                    symbolTable->symbols[j] = symbolTable->symbols[j + 1];
                }
                symbolTable->size--;
            } else {
                i++;
            }
        }
    }
}

int getCurrentScope(const SymbolTable* symbolTable) {
    return symbolTable->currentScope;
}


static Symbol * privateInsertSymbol(SymbolTable *symbolTable, const char *name, const boolean readOnly) {
    if (symbolTable == NULL || name == NULL) {
        return NULL;
    }

    // Permitir símbolos con el mismo nombre en distintos scopes
    for (int i = 0; i < symbolTable->size; i++) {
        if (strcmp(symbolTable->symbols[i].name, name) == 0 && symbolTable->symbols[i].scope == symbolTable->currentScope) {
            return NULL; // Ya existe en este scope
        }
    }
    if (symbolTable->size == symbolTable->capacity) {
        const int newCapacity = symbolTable->capacity * 2;
        Symbol* newSymbols = (Symbol*)realloc(symbolTable->symbols, newCapacity * sizeof(Symbol));
        if (newSymbols == NULL) {
            return NULL;
        }
        symbolTable->symbols = newSymbols;
        symbolTable->capacity = newCapacity;
    }
    Symbol* newSymbol = &(symbolTable->symbols[symbolTable->size]);
    newSymbol->name = strdup(name);
    if (newSymbol->name == NULL) {
        return NULL;
    }
    newSymbol->readOnly = readOnly;
    newSymbol->scope = symbolTable->currentScope;
    symbolTable->size++;
    return newSymbol;
}

Symbol * insertReadOnlySymbol(SymbolTable *symbolTable, const char *name) {
    return privateInsertSymbol(symbolTable, name, true);
}



Symbol* insertSymbol(SymbolTable* symbolTable, const char* name) {
    return privateInsertSymbol(symbolTable, name, false);
}

Symbol* lookupSymbol(const SymbolTable* symbolTable, const char* name) {
    if (symbolTable == NULL || name == NULL) {
        return NULL;
    }
    // Buscar desde el scope más interno al más externo
    for (int s = symbolTable->scopeDepth - 1; s >= 0; s--) {
        const int scope = symbolTable->scopes[s];
        for (int i = symbolTable->size - 1; i >= 0; i--) {
            if (strcmp(symbolTable->symbols[i].name, name) == 0 && symbolTable->symbols[i].scope == scope) {
                return &(symbolTable->symbols[i]);
            }
        }
    }
    return NULL;
}
