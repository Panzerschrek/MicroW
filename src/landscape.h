#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include "mw.h"


#define WATER_LEVEL 65535*9/20
struct StaticObject;

class Landscape
{
	public:

	Landscape( unsigned int size_x= 512, unsigned int size_y=512 );
	~Landscape(){}
    float Height( float x, float y )const;
    float WaterLevel() const;
    void Normal( float x, float y, float* n )const;

    void SetTexture( unsigned int x, unsigned int y, unsigned char texture_id, unsigned int radius );
    void SetTexture( unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned char texture_id );

    void SetMaximumHeight( unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1 );
    void SetMinimumHeight( unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1 );

    void GenShadowMap( const float* sun_vector );
    const unsigned char* ShadowMap()const;

	const unsigned short* Heightmap() const;
    const unsigned char* Texturemap() const;
	unsigned int SizeX() const;
	unsigned int SizeY() const;
    float Amplitude() const;

    void PlaceObject( unsigned int x, unsigned int y, unsigned int radius, StaticObject* object );
    void PlaceObject( unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, StaticObject* object );

    bool CanPlaceObject( unsigned int x, unsigned int y, unsigned int radius );
    bool CanPlaceObject( unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1 );

    StaticObject* GetObject( int x, int y );

    void RegenHeightmap();
	private:

	void Gen();
    void Smooth();
    void GenWay();



    float amplitude;
	unsigned short* heightmap;
    unsigned char* texture_map;
    unsigned char* shadow_map;
    StaticObject** object_map;
    bool* free_space_map;
	unsigned int size_x, size_y;//size of landscape in vertices. size in polygons - size_x-1 x size_y-1
};


inline unsigned int Landscape::SizeX()const
{
	return size_x;
}

inline unsigned int Landscape::SizeY()const
{
	return size_y;
}

inline const unsigned short* Landscape::Heightmap()const
{
	return heightmap;
}
inline const unsigned char* Landscape::Texturemap() const
{
    return texture_map;
}

inline float Landscape::Amplitude() const
{
    return amplitude;
}

inline StaticObject* Landscape::GetObject( int x, int y )
{
    return object_map[ x + y * size_x ];
}


inline float Landscape::WaterLevel() const
{
    return amplitude * float(WATER_LEVEL)/float(65535) -0.5f;
}

inline const unsigned char* Landscape::ShadowMap()const
{
    return shadow_map;
}
#endif//LANDSCAPE_H
