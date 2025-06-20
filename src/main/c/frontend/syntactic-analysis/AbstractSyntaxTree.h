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
typedef enum OptionType OptionType;

typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct ArithmeticExpression ArithmeticExpression;
typedef struct Factor Factor;
typedef struct Program Program;
typedef struct Option Option;
typedef struct Configuration Configuration;
typedef struct Evolution Evolution;

typedef struct NeighborhoodSequence NeighborhoodSequence;
typedef struct TransitionSequence TransitionSequence;
typedef struct NeighborhoodExpression NeighborhoodExpression;
typedef struct TransitionExpression TransitionExpression;

typedef struct Cell Cell;
typedef struct CellList CellList;

typedef struct IntArray IntArray;
typedef struct StringArray StringArray;
typedef struct ConstantArray ConstantArray;
typedef struct Range Range;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

enum ProgramType {
	DEFAULT,
	TRANSITION,
	NEIGHBORHOOD_PROGRAM
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
	CONSTANT,
	CELL_ARITHETIC_EXPRESSION
};

enum RangeType {
	ARRAY,
	INTERVAL
};

enum ConstantType {
	INTEGER_C,
	STRING_C,
	CELL_C
};

enum TransitionExpressionType {
	TRANSITION_ASSIGNMENT,
	TRANSITION_FOR_LOOP,
	TRANSITION_IF,
	TRANSITION_IF_ELSE,
	RETURN_VALUE
};

enum NeighborhoodExpressionType {
	NEIGHBORHOOD_ASSIGNMENT,
	NEIGHBORHOOD_FOR_LOOP,
	NEIGHBORHOOD_IF,
	NEIGHBORHOOD_IF_ELSE,
	ADD_CELL_EXP,
	REMOVE_CELL_EXP
};

enum OptionType {
    HEIGHT_OPTION,       
    WIDTH_OPTION,                 
    FRONTIER_OPTION,               
    COLORS_OPTION,                 
    STATES_OPTION,                 
    NEIGHBORHOOD_OPTION,           
    EVOLUTION_OPTION               
};

struct Program {
	union {
		Configuration * justConfiguration;
		struct {
			Configuration * configuration;
			union {
				TransitionSequence * transitionSequence;
				NeighborhoodSequence * neighborhoodSequence;
			};
		};
	};
	ProgramType type;
};

struct ArithmeticExpression {
	ArithmeticExpressionType type;
	union {
		ArithmeticExpression * expression;
		struct {
			ArithmeticExpression * leftExpression;
			ArithmeticExpression * rightExpression;
		};
		struct {
			CellList * cellList;
			int count;
		};
		Cell * cell;
		Constant * constant;
	};
};

struct Option {
	OptionType type;
	union {
		int value;
		FrontierEnum frontierType;
		IntArray * colors;
		StringArray * states;
		NeighborhoodEnum neighborhoodEnum;
		Evolution * evolution;
	};
};

struct Evolution {
	boolean isDefault;
	union {
		EvolutionEnum evolutionTypes;
		struct {
			IntArray * array;
			int value;
		};
	};
};

struct Configuration {
	boolean isLast;
	union {
		Option * lastOption;
		struct {
			Option * option;
			Configuration * next;
		};
		
	};	
};

struct Constant {
	ConstantType type;
	union 
	{
		int value;
		char * string;
		Cell * cell;
	};
};

struct IntArray {
	boolean isLast;
	union {
		int lastValue;
		struct {
			int value;
			IntArray * next;
		};
	};
};

struct StringArray {
	boolean isLast;
	union {
		char * lastValue;
		struct {
			char * value;
			StringArray * next;
		};
	};
};

struct ConstantArray {
	boolean isLast;
	union {
		Constant * lastValue;
		struct {
			Constant * value;
			ConstantArray * next;
		};
	};
};

struct Cell {
	boolean isSingleCoordenate;
	union {
		struct {
			Constant * displacement;
			DisplacementType displacementType;
		};
		struct {
			Constant * x;
			Constant * y;
		};
	};
};

struct CellList {
	boolean isLast;
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
		ConstantArray * array;
		struct {
			Constant * start;
			Constant * end;
		};
	};
};

struct TransitionSequence {
	boolean binary;
	union {
		TransitionExpression * expression;
		struct {
			TransitionSequence * sequence;
			TransitionExpression *rightExpression;
		};
	};
};

struct NeighborhoodSequence {
	boolean binary;
	union {
		NeighborhoodExpression * expression;
		struct {
			NeighborhoodSequence * sequence;
			NeighborhoodExpression * rightExpression;
		};
	};
};

struct TransitionExpression {
	TransitionExpressionType type;
	union {
		struct {
			ArithmeticExpression * assignment;
			char * variable;
		};
		struct {
			char * forVariable;
			Range * range;
			TransitionSequence * forBody;
		};
		struct {
			ArithmeticExpression * ifCondition;
			TransitionSequence * ifBody;
		};
		struct {
			ArithmeticExpression * ifElseCondition;
			TransitionSequence * ifElseIfBody;
			TransitionSequence * ifElseElseBody;
		};
		ArithmeticExpression * returnValue;
	};
};

struct NeighborhoodExpression {
	NeighborhoodExpressionType type;
	union {
		struct {
			ArithmeticExpression * assignment;
			char * variable;
		};
		struct {
			char * forVariable;
			Range * range;
			NeighborhoodSequence * forBody;
		};
		struct {
			ArithmeticExpression * ifCondition;
			NeighborhoodSequence * ifBody;
		};
		struct {
			ArithmeticExpression * ifElseCondition;
			NeighborhoodSequence * ifElseIfBody;
			NeighborhoodSequence * ifElseElseBody;
		};
		CellList * toAddList;
		CellList * toRemoveList;
	};
};

/**
 * Node recursive destructors.
 */
void releaseConstant(Constant * constant);
void releaseArithmeticExpression(ArithmeticExpression * expression);
void releaseProgram(Program * program);
void releaseTransitionSequence(TransitionSequence * transitionSequence);
void releaseNeighborhoodSequence(NeighborhoodSequence * neighborhoodSequence);
void releaseTransitionExpression(TransitionExpression * transitionExpression);
void releaseNeighborhoodExpression(NeighborhoodExpression * neighborhoodExpression);
void releaseOption(Option * option);
void releaseEvolution(Evolution * evolution);
void releaseConfiguration(Configuration * configuration);
void releaseIntArray(IntArray * array);
void releaseStringArray(StringArray * array);
void releaseConstantArray(ConstantArray * array);
void releaseCell(Cell * cell);
void releaseCellList(CellList * list);
void releaseRange(Range * range);

#endif
