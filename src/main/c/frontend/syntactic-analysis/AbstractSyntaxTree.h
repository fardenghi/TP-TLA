#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */

typedef enum ExpressionType ExpressionType;
typedef enum ArithmeticExpressionType ArithmeticExpressionType;
typedef enum FactorType FactorType;
typedef enum ProgramType ProgramType;
typedef enum ConstantType ConstantType;
typedef enum RangeType RangeType;
typedef enum TransitionExpressionType TransitionExpressionType;
typedef enum NeighborhoodExpressionType NeighborhoodExpressionType;

typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct ArithmeticExpression ArithmeticExpression;
typedef struct Factor Factor;
typedef struct Program Program;
typedef struct Option Option;

typedef struct NeighborhoodExpression NeighborhoodExpression;
typedef struct TransitionExpression TransitionExpression;

typedef struct Cell Cell;
typedef struct CellList CellList;

typedef struct IntArray IntArray;
typedef struct StringArray StringArray;
typedef struct Range Range;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

enum ProgramType {
	DEFAULT,
	TRANSITION,
	NEIGHBORHOOD
};

enum ArithmeticExpressionType {
	ADDITION,
	DIVISION,
	MULTIPLICATION,
	SUBTRACTION,
	MODULE,
	LOGIC_AND,
	LOGIC_OR,
	EQUALS,
	NOT_EQUALS,
	LOWER_THAN,
	LOWER_THAN_OR_EQUAL,
	GREATER_THAN,
	GREATER_THAN_OR_EQUAL,
	LOGIC_NOT,
	ALL_ARE,
	ANY_ARE,
	AT_LEAST_ARE,
	FACTOR,
	CONSTANT
};

enum RangeType {
	ARRAY,
	INTERVAL
};

enum ConstantType {
	INTEGER,
	STRING
};

enum TransitionExpressionType {
	BLOCK,
	ASSIGNMENT,
	FOR_LOOP,
	IF,
	ELSE,
	RETURN_STRING,
	RETURN_INT
};

enum NeighborhoodExpressionType {
	BLOCK,
	ASSIGNMENT,
	FOR_LOOP,
	IF,
	ELSE,
	ADD_CELL_EXP,
	REMOVE_CELL_EXP
};

struct Constant {
	ConstantType type;
	union 
	{
		int value;
		char * string;
	};
};

struct IntArray {
	char isLast;
	union {
		int lastValue;
		struct {
			int value;
			IntArray * next;
		};
	};
};

struct StringArray {
	char isLast;
	union {
		char * lastValue;
		struct {
			char * value;
			StringArray * next;
		};
	};
};

struct Cell {
	char isSingleCoordenate;
	union {
		Constant * displacement;
		struct {
			Constant * x;
			Constant * y;
		};
	};
};

struct CellList {
	char isLast;
	union {
		Cell * last;
		struct {
			Cell * cell;
			CellList * next;
		};
	};
};

struct Range {
	RangeType type;
	union {
		IntArray * array;
		struct {
			Constant * start;
			Constant * end;
		};
	};
};

struct TransitionExpression {
	TransitionExpressionType type;
	union {
		struct {
			TransitionExpression * leftExpression;
			TransitionExpression * rightExpression;
		};
		struct {
			Range * range;
			TransitionExpression * forBody;
		};
		struct {
			ArithmeticExpression * ifCondition;
			TransitionExpression * ifBody;
		};
		struct {
			ArithmeticExpression * ifElseCondition;
			TransitionExpression * ifElseBody;
		};
		char * returnString;
		ArithmeticExpression * returnValue;
	};
};

struct NeigborhoodExpression {
	NeighborhoodExpressionType type;
	union {
		struct {
			NeighborhoodExpression * leftExpression;
			NeighborhoodExpression * rightExpression;
		};
		struct {
			Range * range;
			NeighborhoodExpression * forBody;
		};
		struct {
			ArithmeticExpression * ifCondition;
			NeighborhoodExpression * ifBody;
		};
		struct {
			ArithmeticExpression * ifElseCondition;
			NeighborhoodExpression * ifElseBody;
		};
		CellList * toAddList;
		CellList * toRemoveList;
	};
};

struct Program {
	ProgramType type;
	Option * justOptions;
	union {
		Option * options;
		union {
			TransitionExpression * transitionExpression;
			NeighborhoodExpression * neighborhoodExpression;
		};
	};
};

struct ArithmeticExpression {
	ArithmeticExpressionType type;
	union {
		Expression * expression;
		struct {
			Expression * leftExpression;
			Expression * rightExpression;
		};
		CellList * cellList;
		Constant * constant;
	};
};

struct Option {
	// Una union gigante de structs de cada tipo de opcion con su contenido correspondiente 
};

/**
 * Node recursive destructors.
 */
void releaseConstant(Constant * constant);
void releaseExpression(Expression * expression);
void releaseFactor(Factor * factor);
void releaseProgram(Program * program);

#endif
