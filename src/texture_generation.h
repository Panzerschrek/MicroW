#ifndef TEXTURE_GENERATION_H
#define TEXTURE_GENERATION_H
#include "texture.h"

void GenBrick( Texture* t );
void GenGrass( Texture* t );
void GenTile( Texture* t );
void GenDirt( Texture* t );
void GenSand( Texture* t );
void GenSpruceBranch(Texture* t);
void GenItemGlow( Texture* t );

extern void (*texture_gen_func[])(Texture* t);

#define TEXTURE_GRASS 0
#define TEXTURE_TILE 1
#define TEXTURE_BRICK 2
#define TEXTURE_SAND 3
#define TEXTURE_DIRT 4
#define TEXTURE_SPRUCE_BRANCH 5
#define TEXTURE_TREE_BARK 6
#define TEXTURE_ACHTUNG_MINES 7
#define TEXTURE_METAL 8
#define TEXTURE_CACODEMON_SKIN 9
#define TEXTURE_PAVING_STONE 10
#define TEXTURE_HEALTHPACK 11
#define TEXTURE_WOOD 12
#define TEXTURE_COUNT 13

#endif//TEXTURE_GENERATION_H