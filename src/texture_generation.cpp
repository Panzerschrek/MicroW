#include "texture.h"
#include "math.h"
#include "text.h"

void GenBrick( Texture* t )
{
    static unsigned char clear_color[]= { 128, 128, 128, 255 };
    static unsigned char color[]= { 240, 96, 64, 255 };
    const char color_delta= 16;
    char d;
    unsigned char final_color[4];
    final_color[3]= 255;

    t->Clear( clear_color);
     for( int i= 0; i< 4; i++ )
        for( int j= 0; j< 8; j++ )
        {
            d= color_delta * rand() / RAND_MAX;
            for( int k= 0; k<3; k++ )
                final_color[k]= color[k] + d;
            t->FillRect( i*1024/4+8 + j*1024/8, j*1024/8+8, (i+1)*1024/4-8+ j*1024/8, (j+1)*1024/8-8, final_color );
        }

     Texture t2(t->SizeXLog2(), t->SizeYLog2() );
     t2.Noise();
     t2.Mul(64);
     t2.Add(190);

     t->Mul(&t2);
}

void GenGrass( Texture* t )
{
    const char color_delta= 32;
    static unsigned char grass_color[]= { 18, 156, 32, 255 };
    static unsigned char dirt_color[]= { 112, 89, 82, 255 };
    unsigned char final_color[4];
    final_color[3]= 255;

    float triangle[9]= { -4.0f, -4.0f, 0.0f,
                        4.0f, -4.0f, 0.0f,
                        0.0f, 60.0f, 0.0f };
    float rot_triangle[9];

    float  m_r[16], m_t[16];//, m_s[16];

    short s_triangle[6];
    float a;
    

   // for( int j= 0; j< 4; j++ )
     //   final_color[j]= grass_color[j]>>1;
    t->Clear( dirt_color );

      Texture t2(t->SizeXLog2(), t->SizeYLog2() );
     t2.Noise();
     t2.Mul(128);
     t2.Add(127);

     t->Mul(&t2);
  //  Mat4Identity(m_s);
   // m_s[0]= m_s[5]= 1.2f;

    for( int i= 0; i< 2048; i++ )
    {
        Mat4Identity( m_t );
       

        a= MW_2PI * float( rand() ) / float( RAND_MAX ); 

        for( int j= 0; j< 2; j++ )
            m_t[j+12]= 2048.0f * float( rand() ) / float( RAND_MAX ) - 512.0f;

        Mat4RotateXY( m_r, a );

        Mat4Mul( m_r, m_t );

        for( int j= 0; j< 3; j++ )
        {
            Vec3Mat4Mul( triangle + 3*j, m_r, rot_triangle + j*3 );
            s_triangle[j*2]= short( rot_triangle[j*3] );
            s_triangle[j*2+1]= short( rot_triangle[j*3+1] );
        }

           for( unsigned int j= 0; j< 3; j++ )
               final_color[j]= grass_color[j] + color_delta * rand() / RAND_MAX;

           t->DrawTriangle( s_triangle, final_color );
    }
}

void GenTile( Texture* t )
{
    static unsigned char color[]= { 128, 110, 100, 255 };
    const unsigned char color_delta= 32;
    unsigned char d;
    unsigned char final_color[4];
    final_color[3]= 255;

   

    for( int k= 0; k< 3; k++ )
        final_color[k]= color[k]>>1;
    t->Clear(final_color);

    for( int i= 0; i< 8; i++ )
        for( int j= 0; j< 8; j++ )
        {
             d= color_delta * rand() / RAND_MAX;
             for(  int k= 0; k< 3; k++ )
               final_color[k]= color[k] + d;

            t->FillRect( i * t->SizeX()/8 + 3, j * t->SizeY()/8 + 3,
                (i+1) * t->SizeX()/8 - 3, (j+1) * t->SizeY()/8 - 3, final_color );

        }


        Texture t2(t->SizeXLog2(), t->SizeYLog2() );
        t2.Noise();
        t2.Mul(128);
        t2.Add(128);

        t->Mul(&t2);

}

void GenDirt( Texture* t )
{
    const char color_delta= -16;
    static unsigned char grass_color[]= { 100, 80, 76, 255 };
    static unsigned char dirt_color[]= { 112, 89, 82, 255 };
    unsigned char final_color[4];
    final_color[3]= 255;
    char d;
    static const float triangle[9]= { 0.0f, 0.0f, 0.0f,
                        16.0f, 0.0f, 0.0f,
                        8.0f, 13.856f, 0.0f };
    float rot_triangle[9];

    float  m_r[16], m_t[16];//, m_s[16];

    short s_triangle[6];
    float a;
    short x, y, r;

    t->Clear( dirt_color );

     Texture t2(t->SizeXLog2(), t->SizeYLog2() );
     t2.Noise();
     t2.Mul(128);
     t2.Add(127);

     t->Mul(&t2);
    for( int i= 0; i< 2048; i++ )
    {
        Mat4Identity( m_t );
        //Mat4Identity(m_r);

        a= MW_2PI * float( rand() ) / float( RAND_MAX ); 

        for( int j= 0; j< 2; j++ )
            m_t[j+12]= 2048.0f * float( rand() ) / float( RAND_MAX ) - 512.0f;
        /*m_r[0]= cos( a );
        m_r[1]= sin( a );
        m_r[4]= - m_r[1];
        m_r[5]= m_r[0];*/
        Mat4RotateXY( m_r, a );

        Mat4Mul( m_r, m_t );

        for( int j= 0; j< 3; j++ )
        {
            Vec3Mat4Mul( triangle + 3*j, m_r, rot_triangle + j*3 );
            s_triangle[j*2]= short( rot_triangle[j*3] );
            s_triangle[j*2+1]= short( rot_triangle[j*3+1] );
        }

            d=  color_delta * rand() / RAND_MAX;
           for( unsigned int j= 0; j< 3; j++ )
               final_color[j]= grass_color[j] + d;

           t->DrawTriangle( s_triangle, final_color );


           x=  2048 * rand() / RAND_MAX - 512;
           y=  2048 * rand() / RAND_MAX - 512;
           r= 6 + 8 * rand() / RAND_MAX;
           t->DrawCircle( x, y, r, final_color );

    }
     t->Mul(&t2);
}


void GenSand( Texture* t )
{
    unsigned char* data= ( unsigned char*) t->Data();
    const static unsigned char sand_base_colors[]=
    { 152, 137, 107,
       170, 156, 128,
       139, 125, 95,
       129, 115, 85 };

    t->Noise();
    unsigned char k;
    unsigned int add;
    for( unsigned int i= 0; i< t->SizeX(); i++ )
        for( unsigned int j= 0; j< t->SizeY(); j++ )
        {
            k= 3*(data[ add= (i + (j<<t->SizeXLog2()))<<2 ]&3);
            for( int m= 0; m< 3; m++ )
                data[add+m]= sand_base_colors[ m + k ];
            data[add+3]= 255;
        }

        t->Smooth();
        
     Texture t2(t->SizeXLog2(), t->SizeYLog2() );
     t2.Noise();
     t2.Mul(64);
     t2.Add(190);

     t->Mul(&t2);
}



static Texture* tex;
void GenSpruceBranch_r( float size, float ang, float* pos, int depth )
{
    static const unsigned char color[]= { 100, 60, 10, 255 };
    static const unsigned char needle_color[]= { 5, 80, 5, 255 };
    unsigned char final_color[4];
    final_color[3]= 255;
    static const float triangle[12]= { -16.0f, 0.0f, 0.0f,
                                     -6.0f, 1012.0f, 0.0f,
                                      6.0f, 1012.0f, 0.0f,
                                      16.0f, 0.0f,  0.0f };
    static const float needle_triangle[]= { -2.0f, 0.0f, 0.0f,
                                            2.0f, 0.0f, 0.0f,
                                            0.0f, 20.0f, 0.0f };
    float trans_triangle[12];
    short s_triangle[8];
    

    float m[16], m_s[16], m_r[16], m_t[16];

    Mat4Identity(m_s);
    //Mat4Identity(m_r);
    Mat4Identity(m_t);

    m_s[0]= m_s[5]= size;

    m_t[12]= pos[0];
    m_t[13]= pos[1];

    /*m_r[0]= cos(ang);
    m_r[1]= sin(ang);
    m_r[4]= -m_r[1];
    m_r[5]= m_r[0];*/
    Mat4RotateXY( m_r, ang );


    Mat4Mul( m_r, m_s, m );
    Mat4Mul( m, m_t );

    for( int i= 0; i< 4; i++ )
    {
          Vec3Mat4Mul( triangle + 3*i, m, trans_triangle + 3*i );
          s_triangle[i*2]= short(trans_triangle[i*3] );
          s_triangle[i*2+1]= short(trans_triangle[i*3+1] );
    }
    tex->DrawTriangle( s_triangle, color );
    s_triangle[2]= s_triangle[6];
    s_triangle[3]= s_triangle[7];
    tex->DrawTriangle( s_triangle, color );

  
    const float needle_step= 6.0f;
    int n= int( 1012.0f * size / needle_step);
    float pos2[2]={pos[0],pos[1]};
    float width= size * 16.0f;
    float dw= size * ( 16.0f - 6.0f ) / float(n);
    for( int i= 0; i< n; i++ , width-=dw )
    {

        pos2[0]+=  needle_step * cos(ang+MW_PI2);
        pos2[1]+=  needle_step * sin(ang+MW_PI2);

        Mat4Identity(m_s);
        //Mat4Identity(m_r);
        Mat4Identity(m_t);

       
            unsigned char color_delta= 16 * rand()/ RAND_MAX;
       for( int i= 0; i< 3; i++ )
            final_color[i]= needle_color[i] + color_delta;

        float da= -MW_PI3;
        for( int j= 0; j< 2; j++, da+= MW_2PI3 )
        {
            /*m_r[0]= cos(ang+da);
            m_r[1]= sin(ang+da);
            m_r[4]= -m_r[1];
            m_r[5]= m_r[0];*/
            Mat4RotateXY( m_r, ang + da );

            m_t[12]= pos2[0]-width*m_r[1];
            m_t[13]= pos2[1]+width*m_r[0];

            Mat4Mul( m_r, m_t, m );
            //Mat4Mul( m, m_t );

            for( int k= 0; k< 3; k++ )
            {
                Vec3Mat4Mul( needle_triangle + 3*k, m, trans_triangle );
                s_triangle[k*2]= short(trans_triangle[0] );
                s_triangle[k*2+1]= short(trans_triangle[1] );
            }
                 tex->DrawTriangle( s_triangle, final_color );
        }
    }


    if( depth != 0 )
    {
        float pos2[]= { pos[0], pos[1] };
        //float step= size * 0.6666f;
        while( size > 0.1f )
        {

            pos2[0]+= size * 0.3333f * 1024.0f * cos(ang+MW_PI2);
            pos2[1]+= size * 0.3333f * 1024.0f * sin(ang+MW_PI2);

            GenSpruceBranch_r( size * 0.3333f , ang +  MW_PI3, pos2, depth-1 );
            GenSpruceBranch_r( size * 0.3333f, ang -   MW_PI3, pos2, depth-1 );

            size*= 0.6666f;
        }
        
        /*float db= 80.0f;
        int steps= int(size * 1012.0f / db);
        float pos2[2]={pos[0],pos[1]};
        size*= 0.5f;
        float ds= size / float(steps);
        size-= ds;
        for( int i= 1; i< steps; i++ )
        {
               pos2[0]+= db * cos(ang+MW_PI*0.5f);
               pos2[1]+= db* sin(ang+MW_PI*0.5f);
               GenSpruceBranch_r( size, ang +  0.333333f * MW_PI, pos2, depth-1 );
               GenSpruceBranch_r( size , ang - 0.333333f * MW_PI, pos2, depth - 1 );
               size-= ds;
        }*/
    }

}

void GenSpruceBranch(Texture* t)
{
    static const unsigned char color[]= { 5, 88, 5, 64 };
    t->Clear(color);
    tex= t;
    float pos[]= { 512.0f, 0.0f }; 
    GenSpruceBranch_r( 1.0f, 0.0f, pos, 3 );

    pos[1]= 80.0f;
    GenSpruceBranch_r( 0.45f, MW_PI3, pos, 2 );
    GenSpruceBranch_r( 0.45f, -MW_PI3, pos, 2 );

   
}


void GenTreeBark(Texture* t)
{
   static const unsigned char color[]= { 91, 46, 6, 255 };
    t->Noise();
    t->DownScaleX();
    t->DownScaleX();
    t->SetAlpha(255);
    t->Mul( color );
}


void GenAchtungMines( Texture* t )
{
    static unsigned char table_color[]= { 192, 192, 200, 255 };
    static unsigned char color[]= { 240, 32, 32, 255 };
   
    t->Noise();
    t->Mul(32);
    t->Add(224);
    t->Mul(table_color);
    

    #define ACHTUNG_MINES "Achtung, Mines!"
    const unsigned int text_len= sizeof(ACHTUNG_MINES);
    const unsigned int text_size= 8;

    short x= t->SizeX()/2 - (15 * text_size/2) * LETTER_WIDTH;
    short y= t->SizeY()/2 - LETTER_HEIGHT * text_size/2;

    t->DrawText( x, y, text_size, color, ACHTUNG_MINES );
    t->SetAlpha(255);
}

void GenMetal( Texture* t )
{
    t->Noise();
    t->Mul(64);
    t->Add(128);
}


/*
default cell texture 1024x1024
red channel - distance to nearest cell nucleus
green channel - cell edges
blue chanell - ddistance to nearest edge
alpha - number of cell
*/
static Texture* CELL_TEXTURE= NULL;


void GenCellTexture()
{
    if( CELL_TEXTURE != NULL  )
        return;

    CELL_TEXTURE= new Texture(10,10);

    Texture* t= CELL_TEXTURE;

#define CELLS 8
#define POINTS_PER_CELL 1
#define CELL_SIZE  128
    short cell_points[CELLS][CELLS][ 2 * POINTS_PER_CELL];// points in cell
    for( unsigned int i= 0; i< CELLS; i++ )
        for( unsigned int j= 0; j< CELLS; j++ )
        {
            for( unsigned int k= 0; k< POINTS_PER_CELL; k++ )
            {
                //cell_points[i][j][k*2]= CELL_SIZE * rand()/(RAND_MAX+1);
               // cell_points[i][j][k*2+1]= CELL_SIZE * rand()/(RAND_MAX+1);
                cell_points[i][j][k*2]= 3*CELL_SIZE/4 * rand()/(RAND_MAX+1);
                cell_points[i][j][k*2+1]= 3*CELL_SIZE/4 * rand()/(RAND_MAX+1);
               
            }
        }

    unsigned char* data= (unsigned char*) t->Data();

    float dst[2];
    unsigned char result;
    int dst2_min[2], dst2;
    short nearest_points[2][2];
    unsigned char nearest_point_id;
    short center_point[2];
    short dx, dy;
    short x, y;
    for( int i= 0; i< (int)t->SizeX(); i++ )
        for( int j= 0; j< (int)t->SizeY(); j++ )
        {
            /*find 2 nearest points*/
            dst2_min[0]= dst2_min[1]= INT_MAX;
            for( int c_i= i/CELL_SIZE-1; c_i<= i/CELL_SIZE+1; c_i++ )
                for(  int c_j= j/CELL_SIZE-1; c_j<= j/CELL_SIZE+1; c_j++ )
                {
                    for( unsigned int k= 0; k< POINTS_PER_CELL; k++ )
                    {
                        x= cell_points[c_i&(CELLS-1)] [c_j&(CELLS-1)][k*2];
                        y= cell_points[c_i&(CELLS-1)] [c_j&(CELLS-1)][k*2+1];
                        x+= c_i * CELL_SIZE;
                        y+= c_j * CELL_SIZE;
                        x-=i;
                        y-=j;
                        dst2= x * x + y * y;
                        if( dst2 < dst2_min[0] )
                        {
                            dst2_min[1]= dst2_min[0];
                            dst2_min[0]= dst2;
                            nearest_points[1][0]= nearest_points[0][0];
                            nearest_points[1][1]= nearest_points[0][1];
                            nearest_points[0][0]= x+i;
                            nearest_points[0][1]= y+j;
                            nearest_point_id= (c_i&(CELLS-1)) + (c_j&(CELLS-1))*CELLS;
                        }
                        else if( dst2< dst2_min[1] )
                        {
                            dst2_min[1]= dst2;
                            nearest_points[1][0]= x+i;
                            nearest_points[1][1]= y+j;
                        }
                    }
                }
            /*find 2 nearest points*/

            //center point between 2 nearest points
            center_point[0]= (nearest_points[0][0] + nearest_points[1][0])>>1;
            center_point[1]= (nearest_points[0][1] + nearest_points[1][1])>>1;



            //dx= nearest_points[0][0] - center_point[0];
            //dy= nearest_points[0][1] - center_point[1];
            //dst[0]= sqrt( float(dx*dx+dy*dy) );//dsitance between nearest point and edge
            dx= nearest_points[1][0] - i;
            dy= nearest_points[1][1] - j;
            dst[0]= sqrt( float( dx*dx+ dy*dy) );

            dx= nearest_points[0][0] - i;
            dy= nearest_points[0][1] - j;//distance between nearst point and current point
            dst[1]=  sqrt( float( dx*dx+ dy*dy) );

            unsigned int k= (t->SizeX() * j + i )*4;

            data[k]= (unsigned char)(1.5f*dst[1]);
           
            result= (unsigned char)( dst[0]- dst[1] );
            data[k+1]= (result < 2 ) ? 255 : 0;

            data[k+2]= 255 - (unsigned char)fabs( (dst[0]- dst[1])*1.5f );
            data[k+3]= nearest_point_id;
            
           
            //data[k]= data[k+1]= data[k+2]= result;
            //data[k+3]= 255;

        }
}

void GenCacodemonSkin( Texture* t )
{
    GenCellTexture();
    t->Copy( CELL_TEXTURE );
 
    t->DownScaleX();
    t->DownScaleY();

    const static unsigned char color0[]= { 255, 32, 32, 0 };
    const static unsigned char color1[]= { 32, 32, 96, 0 };

    unsigned char* data= (unsigned char*) t->Data();
    unsigned int i_end= t->SizeX() * t->SizeY() * 4;
    unsigned char k0, k1;
    for( unsigned int i= 0; i< i_end; i+=4 )
    {
        k0= data[i];
        k1= ~k0;
        for( unsigned int j= 0; j< 3; j++ )
            data[i+j]= (k0 *color1[j] + k1 * color0[j])>>8;
        data[i+3]= 255;
    }

    //eyes
    static const unsigned char eye_color0[]= { 16, 16, 16, 255 };
    static const unsigned char eye_color1[]= { 16, 128, 48, 255 };
    t->DrawCircle( -64, 768-128, 32, eye_color0 );
    t->DrawCircle( 64, 768-128, 32, eye_color0 );
    t->FillRect( 64-7, 768-128 - 17, 64+7, 768-128+17, eye_color1 );
    t->FillRect( -64-7, 768-128 - 17, -64+7, 768-128+17, eye_color1 );



    static const short mouth_coord[]= { -96, 502,
                                        0, 470,
                                        0, 450,
                                        96, 502 };

    t->DrawTriangle( mouth_coord, eye_color1 );
    t->DrawTriangle( mouth_coord+2, eye_color1 );
}


void GenPavingStone( Texture* t )
{
    GenCellTexture();
    //t->Copy( CELL_TEXTURE );
   // t->Smooth();
   // t->Smooth();
   // return;
    static const unsigned char clear_color[]= { 32, 32, 32 };
    static const unsigned char stone_color[]= { 118, 88, 68 };
    
    unsigned char colors[64][3];
    unsigned char delta;
    for( int i= 0; i< 64; i++ )
    {
        delta= 32 * rand() /RAND_MAX;
        colors[i][0]= stone_color[0]+delta;
        colors[i][1]= stone_color[1]+delta;
        colors[i][2]= stone_color[2]+delta;
    }

    unsigned char* data= (unsigned char*) t->Data();
    unsigned char* in_data= (unsigned char*) CELL_TEXTURE->Data();
    unsigned int i_end= t->SizeX() * t->SizeY() * 4;
    unsigned char k0, k1;
    unsigned char bright;
    unsigned char stone_id;
    for( unsigned int i= 0; i< i_end; i+=4 )
    {
        stone_id= in_data[i+3];
        k0= in_data[i+1];
        bright= ~in_data[i+2];
        bright= min( bright*6, 255 );
        if( k0 > 1 ) k0= 255;else k0= 0;
        k1= ~k0;
        for( unsigned int j= 0; j< 3; j++ )
            data[i+j]= (k0 * clear_color[j] + (bright * k1 * colors[stone_id][j])/256 )/256;
        data[i+3]= 255;
        
    }

    Texture t2( t->SizeXLog2(), t->SizeYLog2() );
    t2.Noise();
    t2.DownScaleX();
    t2.DownScaleY();
    t2.Mul(128);
    t2.Add(128);

    t->Mul(&t2);

}


void GenHealthpackTexture( Texture* t )
{
    static const unsigned char  clear_color[]= { 240, 240, 255, 255 };
    t->Clear( clear_color );

    static const unsigned char color[]= { 255, 32, 32, 255 };
    //t->FillRect( 384, 128, 640, 896, color );
    //t->FillRect( 128, 384, 896, 640, color );

    t->FillRect( 448, 320, 576, 704, color );
    t->FillRect( 320, 448, 704, 576, color );
}


void GenItemGlow( Texture* t )
{
   // t->Noise();
    //t->Mul(128);
    //t->Add(128);
    static unsigned char color[]= { 255, 255, 64, 255 };

    unsigned char* d= (unsigned char*) t->Data();
    unsigned int center_x= t->SizeX()/2, center_y= t->SizeY()/2;
    int dx, dy;
    int l;
    unsigned int k;
    unsigned int bright;
    int l_max=  t->SizeX() * t->SizeX()/4 - 4;
    for( unsigned int x= 0; x< t->SizeX(); x++ )
        for( unsigned int y= 0; y< t->SizeY(); y++ )
        {
            dx= x - center_x;
            dy= y - center_y;
            l= dx*dx + dy*dy;
            k= (x + y * t->SizeX())*4;
      
            //d[k]=   (d[k] * l )>>t->SizeXLog2();
           // d[k+1]= (d[k] * l )>>t->SizeXLog2();
           // d[k+2]= (d[k] * l )>>t->SizeXLog2();
           // l= l * d[k] / 256;
            if( l < l_max )
                bright= 255 - 255 * l / l_max;
            else bright= 0;
            d[k]=   color[0] * bright / 256;
            d[k+1]= color[1] * bright / 256;
            d[k+2]= color[2] * bright / 256;
        }
}


void GenWood( Texture* t )
{
    t->Noise();
    t->DownScaleX();
    t->DownScaleX();
   // t->Mul(128);
  //  t->Add(127);

    unsigned char* data= (unsigned char*) t->Data();
    unsigned int i_end= t->SizeX() * t->SizeY() *4;

    static const unsigned char color0[]= { 146, 137, 29 };
    static const unsigned char color1[]= { 236, 237, 128 };
    unsigned char d;
    for( unsigned int i= 0; i< i_end; i+=4 )
    {
        d= data[i]&0xFC;
        d*=5;

        for( unsigned int k= 0; k< 3; k++ )
            data[i+k]= (d * color0[k] + (255-d) * color1[k])/256;

        data[i+3]= 255;
    }

}

void (*texture_gen_func[])(Texture* t)={
    GenGrass, GenTile, GenBrick, GenSand, GenDirt, GenSpruceBranch, GenTreeBark, GenAchtungMines,
    GenMetal, GenCacodemonSkin, GenPavingStone, GenHealthpackTexture, GenWood };
