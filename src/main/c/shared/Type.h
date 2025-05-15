#ifndef TYPE_HEADER
#define TYPE_HEADER

typedef enum {
	false = 0,
	true = 1
} boolean;

typedef enum DisplacementType {
	HORIZONTAL_D,
	VERTICAL_D,
	DIAGONAL_ASC_D,
	DIAGONAL_DESC_D
} DisplacementType;

typedef int Token;

#endif
