typedef struct {
    char * name;
    char type;
    int value;
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
} SymbolTable;

SymbolTable * createSymbolTable();
void destroySymbolTable(SymbolTable * symbolTable);
Symbol* insertSymbol(SymbolTable * symbolTable, const char * name, const char type, const int value);
Symbol * lookupSymbol(SymbolTable * symbolTable, const char * name);
void pushScope(SymbolTable * symbolTable);
void popScope(SymbolTable * symbolTable);
int getCurrentScope(SymbolTable * symbolTable);
