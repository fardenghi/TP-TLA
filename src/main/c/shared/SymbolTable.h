typedef struct {
    char * name;
    char type;
    int value;
} Symbol;

typedef struct {
    Symbol * symbols;
    int size;
    int capacity; // Added capacity for dynamic allocation
} SymbolTable;

SymbolTable * createSymbolTable();

void destroySymbolTable(SymbolTable * symbolTable);

Symbol* insertSymbol(SymbolTable * symbolTable, const char * name, const char type, const int value);

Symbol * lookupSymbol(SymbolTable * symbolTable, const char * name);