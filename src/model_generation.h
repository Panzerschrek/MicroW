#ifndef MODEL_GENERATION_H
#define MODEL_GENERATION_H
#include "model.h"

void GenCylinder( Model* m, unsigned int segments, bool use_caps= false );
void GenTree(Model* m );
void GenAchtungTable( Model* m );
void GenShovel( Model* m );void GenShovelAnimations( Model* m );
void GenCone( Model* m, unsigned int segments );
void GenSphere( Model* m, unsigned int segments, unsigned int partition );
void GenCube( Model* m );
void GenCacodemon( Model* m );
void GenPistol( Model* m );void GenPistolAnimations( Model* m );
void GenMachineGun(Model* m);void GenMachineGunAnimations( Model* m );
void GenSmallHealthPack(Model* m );
void GenMediumHealthPack( Model* m );
void GenLargeHealthPack( Model* m );
void GenRobot( Model* m );void GenRobotAnimations( Model* m );
void GenSnowMan( Model* m );

extern void (*static_objects_generation_func[])( Model* m );
#endif//MODEL_GENERATION_H