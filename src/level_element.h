#ifnedf LEVEL_ELEMENT_H
#define LEVEL_ELEMENT_H

//attachment point normals
#define NORMAL_POS_X 0
#define NORMAL_NEG_X 1
#define NORMAL_POS_Y 2
#define NORMAL_NEG_Y 3
#define NORMAL_POS_Z 4
#define NORMAL_NEG_Z 5


#define ROTATION_0
#define ROTATION_90
#define ROTATION_180
#define ROTATION_270

class LevelElement
{
	public:
	LevelElement(){}
	~LevelElement(){}

	void SetPosition( const float* pos );
	void SetRotation( unsigned char rot );

	bool HasIntersection( LevelElement* e );

	private:

	float bb_min[3], bb_max[3];//bounding box
	float pos[3];
	float rotation;

	unsigned char attachment_point_normals[8];
	float attachment_points[8][3];
	LevelElement* attached_elements[8];
	unsigned int attachment_point_count;

};

#endif//LEVEL_ELEMENT_H
