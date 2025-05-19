#ifndef TYPE_HEADER
#define TYPE_HEADER

typedef enum
{
	false = 0,
	true = 1
} boolean;

typedef enum DisplacementType
{
	HORIZONTAL_D,
	VERTICAL_D,
	DIAGONAL_ASC_D,
	DIAGONAL_DESC_D
} DisplacementType;

typedef int Token;

typedef enum FrontierEnum
{
	PERIODIC = 0,
	OPEN = 1,
	MIRROR = 2
} FrontierEnum;

typedef enum
{
	CUSTOM = 0,
	MOORE = 1,
	VON_NEUMANN = 2,
	K_NEIGHBORHOOD = 3
} NeighborhoodEnum;

typedef enum
{
	CONWAY = 0,
	SEEDS = 1,
} EvolutionEnum;

#endif
