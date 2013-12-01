#include "model.h"
#include "math.h"
#include "texture_generation.h"
#include "model_generation.h"

//static unsigned int segments= 24;//global value for cylinder, cone, sphere generation
//static unsigned int partition=8;
//static bool use_caps= false;

void GenCylinder(Model* m, unsigned int segments, bool use_caps )
{
    unsigned int i_count= segments * 6 , v_count= segments * 2 + 2 ;
    if( use_caps )
    {
        i_count+= segments*3 *2;
        v_count+= (segments+2)*2;
    }
    if( m->vertices == NULL )
    {
        m->indeces= new unsigned short[ i_count ];
        m->vertices= new ModelVertex[ v_count ];
    }
    unsigned short* ind= m->indeces;
    ModelVertex* v= m->vertices;

    float x, y;
    float a= 0.0f;
    float da= MW_2PI  / float( segments );
    short t= 0;
    
    unsigned int s= segments*2;
    for( unsigned int j=0; j<= s; j+=2, a+= da )
    {
        y= sin(a);
        x= cos(a);
        y*= 256.0f;
        x*= 256.0f;

        v[j].pos[0]= (short)(x);
        v[j].pos[1]= (short)(y);
        v[j].normal[0]= (v[j].pos[0]-1)/2;
        v[j].normal[1]= (v[j].pos[1]-1)/2;
        v[j].normal[2]= 0;

        v[j].tex_coord[0]= j * 4096 / s;

        v[j+1]= v[j];

        v[j].pos[2]= 0;
        v[j+1].pos[2]= 256;//1.0f

        v[j].tex_coord[1]= 0;
        v[j+1].tex_coord[1]= 4096;//1.0f
    }

    for( unsigned int i= 0, j=0; i< s; i+=2, j+=6 )
    {
        ind[j]= i;
        ind[j+1]=i+1;
        ind[j+2]=i+3;//

        ind[j+3]=i;
        ind[j+4]=i+3;//
        ind[j+5]=i+2;//
    }

    if( use_caps )
    {
        v= m->vertices + segments*2+2;
        ind= m->indeces + segments*6;
        for( unsigned int k= 0; k< 2; k++ )
        {
            v[0].pos[0]= 0;
            v[0].pos[1]= 0;
            v[0].pos[2]= k*256;
            v[0].normal[0]= 0;
            v[0].normal[1]= 0;
            v[0].normal[2]= (k == 0) ? -127 : 127;
            v[0].tex_coord[0]= 2048;
            v[0].tex_coord[1]= 2048;
            v++;

            a= 0.0f;
            da= MW_2PI  / float( segments );
            for( unsigned int i= 0; i<= segments; i++, a+=da )
            {
                y= sin(a);
                x= cos(a);
                y*= 256.0f;
                x*= 256.0f;

                v[i].pos[0]= (short)(x);
                v[i].pos[1]= (short)(y);
                v[i].pos[2]=  v[-1].pos[2];
                v[i].normal[0]= 0;
                v[i].normal[1]= 0;
                v[i].normal[2]= v[-1].normal[2];

                v[i].tex_coord[0]= v[i].pos[0] * 2048/256 + 2048;
                v[i].tex_coord[1]= v[i].pos[1] * 2048/256 + 2048;
            }
   
            unsigned int i0= v - 1 - m->vertices;
            for( unsigned int i= v - m->vertices, i_end= i + segments, j=0; i< i_end; i++, j+=3 )
            {
                ind[j]= i0;
                ind[j+1]= i;
                ind[j+2]= i+1;
            }
            ind+= segments*3;
            v+= segments+1;
        }//top and bottom cap

        
    }//use caps

    m->vertex_count= v_count;
    m->index_count= i_count;
    m->primitive_type= GL_TRIANGLES;
}


void GenCone( Model* m, unsigned int segments )
{
    if( m->vertices == NULL )
    {
        m->indeces= new unsigned short[ segments * 3 ];
        m->vertices= new ModelVertex[ segments * 2 + 2 ];
    }
    unsigned short* ind= m->indeces;
    ModelVertex* v= m->vertices;


    float x, y;
    float a= 0;
    float da= MW_2PI  / float( segments );

    
    unsigned int s= segments*2;
    for( unsigned int j=0; j<= s; j+=2, a+= da )
    {
        y= sin(a);
        x= cos(a);
        y*= 256.0f;
        x*= 256.0f;

        v[j].pos[0]= (short)(x);
        v[j].pos[1]= (short)(y);
        v[j].normal[0]= (v[j].pos[0]-1)/2;
        v[j].normal[1]= (v[j].pos[1]-1)/2;
        v[j].normal[2]= 127;

        v[j].tex_coord[0]= j * 2048 / s;

        v[j+1]= v[j];
        v[j+1].pos[0]= v[j+1].pos[1]= 0;

        v[j].pos[2]= 0;
        v[j+1].pos[2]= 256;//1.0f

        v[j].tex_coord[1]= 0;
        v[j+1].tex_coord[1]= 4096;//1.0f
    }

    for( unsigned int i= 0, j=0; i< segments * 3; i+=3, j+= 2 )
    {
        ind[i]= j;
        ind[i+1]= j+1;
        ind[i+2]= j+2;
    }

     m->vertex_count= segments * 2 + 2;
    m->index_count= segments * 3;
    m->primitive_type= GL_TRIANGLES;

}


void GenSphere( Model* m, unsigned int segments, unsigned int partition )
{
    
    if( m->vertices == NULL )
    {
        m->indeces= new unsigned short[ segments * partition * 6 ];
        m->vertices= new ModelVertex[ (segments + 1 ) * (partition+1) ];
    }
    unsigned short* ind= m->indeces;
    ModelVertex* v= m->vertices;

    float vec[3];
    float xy, z;
    float a, da= MW_2PI / float( segments );
    float phi= -MW_PI2, d_phi= MW_PI / float(partition);
    unsigned int i, j, k;
    for( j= 0, k=0; j<= partition; j++, phi+= d_phi )
    {
        xy= cos(phi) * 256.0f;
        z= sin(phi) * 256.0f;
        for( i= 0, a= 0.0f; i<= segments; i++, a+=da, k++ )
        {
            vec[0]= xy * cos(a);
            vec[1]= xy * sin(a);
          
            v[k].pos[0]= (short)(vec[0]);
            v[k].pos[1]= (short)(vec[1]);
            v[k].pos[2]= (short)(z);
            v[k].normal[0]= (v[k].pos[0]-1)/2;
            v[k].normal[1]= (v[k].pos[1]-1)/2;
            v[k].normal[2]= (v[k].pos[2]-1)/2;

            v[k].tex_coord[0]= 4096 * i / segments;
            v[k].tex_coord[1]= 4096 * j / partition;
            //v[k].tex_coord[2]= TEXTURE_GRASS;

        }
    }

     for( j= 0, k=0; j< partition; j++  )
        for( i= 0, a= 0.0f; i< segments; i++, a+=da, k+=6 )
        {
            ind[k]= i + j * (segments + 1 );
            ind[k+1]= ind[k] + segments + 1;
            ind[k+2]= ind[k+1] + 1;

            ind[k+3]=ind[k];
            ind[k+4]= ind[k+2];
            ind[k+5]= ind[k]+1;
        }
    
    m->vertex_count= (segments + 1) * (partition+1);
    m->index_count= segments * partition * 6;
    m->primitive_type= GL_TRIANGLES;
}

void GenSpruce(Model* m )
{
    unsigned int segments = 12;
    //segments = 12;
    const unsigned int branches= 16;
    m->vertices= new ModelVertex[ segments * 2 + 2 +   branches * 4 * 4 ];
    m->indeces= new unsigned short[ segments * 6 +  branches * 8 * 3 ];
    GenCylinder(m,segments);
    m->SetTexture(TEXTURE_TREE_BARK);
    static const short tex_scale[]= { 4096/*2^12*/, 32767 };
    m->ScaleTexture(tex_scale);
    
    short vec[]= { 0, 0, 256 };
    vec[0]= 1<<8;
    vec[1]= 1<<8;
    vec[2]= 19<<8;//16 metters
    m->Scale( vec );

    for( unsigned int i= 0; i<= segments; i++ )
    {
         m->vertices[i*2].pos[0]>>=1;
        m->vertices[i*2].pos[1]>>=1;

        m->vertices[i*2+1].pos[0]>>=4;
        m->vertices[i*2+1].pos[1]>>=4;
    }

    static const float branch_v[]= {
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.2f,
    -0.433012701892219323381861585376473f/*sqrt(3)/4*/, 0.25f, 0.1f,
    0.433012701892219323381861585376473f/*sqrt(3)/4*/, 0.25f, 0.1f
    };
    static const unsigned short bransh_i[]= { 0,2,1, 0,1,3 };

    static const char branch_normals[]= {
        0, 0, 127,
        0, 0, 127,
        32, 0, 123,
        -32, 0, 123,

        0, 0, 127,
        0, 0, 127,
        0, 32, 123,
        0, -32, 123,

        0, 0, 127,
        0, 0, 127,
        -32, 0, 123,
        32, 0, 123,

         0, 0, 127,
        0, 0, 127,
        0, -32, 123,
        0, 32, 123
    };

    ModelVertex* v= m->vertices + segments * 2 + 2;
    unsigned short* ind= m->indeces + segments * 6;
    unsigned short v_shift= segments * 2 + 2;

    float transform_v[3];

    ModelVertex final_v;
    

    const short branch_tc[]= {
    2048/*0.5 * 2^12*/, 0,
    2048/*0.5 * 2^12*/, 4096/*2^12*/, 
    274/**/, 1024/*2^12/4*/,
    3822, 1024
    };

    float  m_s[16], m_t[16], m_r[16];
    Mat4Identity(m_t);

    float scale= 6.0f;
    float ds= scale / float(branches);
    float a;
    float h= 3.0f, dh= 16.0f / float(branches);
    float width= 0.5f, dw= 0.5f/float(branches);
    for( unsigned int i= 0; i< branches; i++, scale-=ds, h+=dh, width-=dw )//tree
    {
        if(i&1) a= 0.0f;
        else a= MW_PI4;
        a+= MW_PI4* (0.0625f * float(rand())/float(RAND_MAX) - 0.03125f);

        for( unsigned int j= 0; j< 4; j++, a+= MW_PI2)//branches
        {
            Mat4Identity(m_s);
            //Mat4Identity(m_r);
            
            m_t[12]= -width*sin(a);
            m_t[13]= width*cos(a);
            m_t[14]= h + float(j&1)*0.1f;

            m_s[0]= m_s[5]= m_s[10]= scale;

            /*m_r[0]= cos(a);
            m_r[1]= sin(a);
            m_r[4]= -m_r[1];
            m_r[5]= m_r[0];*/
            Mat4RotateXY( m_r, a );

            Mat4Mul( m_s, m_r );
            Mat4Mul( m_s, m_t );

            short tex_scale= 4096 * (branches - i ) / branches;
            for( unsigned int k= 0; k< 4; k++ )//branch vertices
            {
                Vec3Mat4Mul( branch_v + k*3, m_s, transform_v  );
                final_v.pos[0]= short( transform_v[0] * 256.0f );
                final_v.pos[1]= short( transform_v[1] * 256.0f );
                final_v.pos[2]= short( transform_v[2] * 256.0f );
               // final_v.tex_coord[0]= branch_tc[k*2];
               // final_v.tex_coord[1]= branch_tc[k*2+1];
                final_v.tex_coord[0]= ((branch_tc[k*2]-2048)*tex_scale)/4096 + 2048;
                final_v.tex_coord[1]= ((branch_tc[k*2+1]-4096)*tex_scale)/4096 + 4096;
                final_v.tex_coord[2]= TEXTURE_SPRUCE_BRANCH;

                final_v.normal[0]= branch_normals[k*3+ 12*j + 0];
                final_v.normal[1]= branch_normals[k*3+ 12*j + 1];
                final_v.normal[2]= branch_normals[k*3+ 12*j + 2];

                *v= final_v;
                v++;
            }
            for( unsigned int k= 0; k< 6; k++ )
                ind[k]= bransh_i[k] + v_shift;
            ind+=6;
            
            v_shift+= 4;//add 4 vetices
            

        }
    }


                                                       
    m->vertex_count= segments * 2 + 2 +   branches * 4 * 4;
    m->index_count= segments * 6 +  branches * 8 * 3;

}


void GenSpruceLod( Model* m )
{
    unsigned int segments = 5;
    const unsigned int branches= 3;
    m->vertices= new ModelVertex[ segments * 2 + 2 +   branches * 3 ];
    m->indeces= new unsigned short[ segments * 6 +  branches * 3 ];
    GenCylinder(m,segments);
    m->SetTexture(TEXTURE_TREE_BARK);
    static const short tex_scale[]= { 4096/*2^12*/, 32767 };
    m->ScaleTexture(tex_scale);
    
    short vec[]= { 0, 0, 256 };
    vec[0]= 1<<8;
    vec[1]= 1<<8;
    vec[2]= 19<<8;//16 metters
    m->Scale( vec );

    for( unsigned int i= 0; i<= segments; i++ )
    {
        m->vertices[i*2].pos[0]>>=1;
        m->vertices[i*2].pos[1]>>=1;

        m->vertices[i*2+1].pos[0]>>=4;
        m->vertices[i*2+1].pos[1]>>=4;
    }

    ModelVertex* v=  m->vertices + segments * 2 + 2;

    float a= 0.0f, da= MW_PI / float( branches );
    for( unsigned int i= 0; i< branches; i++, a+= da, v+=3 )
    {
        float x, y;
        x= 1.1f* 6.0f*cos(a);
        y= 1.1f* 6.0f*sin(a);

        v[0].pos[0]= short(x*256.0f);
        v[0].pos[1]= short(y*256.0f);
        v[0].pos[2]= short(3.0f*256.0f);
        v[0].tex_coord[0]= 4096;
        v[0].tex_coord[1]= 0;

        v[1].pos[0]= short(-x*256.0f);
        v[1].pos[1]= short(-y*256.0f);
        v[1].pos[2]= short(3.0f*256.0f);
        v[1].tex_coord[0]= 0;
        v[1].tex_coord[1]= 0;

        v[2].pos[0]= 0;
        v[2].pos[1]= 0;
        v[2].pos[2]= short(19.0f*256.0f);
        v[2].tex_coord[0]= 2048;
        v[2].tex_coord[1]= 4096;

        v[0].tex_coord[2]= v[1].tex_coord[2]= v[2].tex_coord[2]= TEXTURE_SPRUCE_BRANCH;

        v[0].normal[0]= v[0].normal[1]= v[1].normal[0]= v[1].normal[1]= v[2].normal[0]= v[2].normal[1]= 0;
        v[0].normal[2]= v[1].normal[2]= v[2].normal[2]= 127;
    }
    unsigned short* ind= m->indeces + segments * 6;
    for( unsigned int i= 0, j= segments * 2 + 2; i< 3*branches; i+=3, j+=3 )
    {
        ind[i]= j;
        ind[i+1]= j+1;
        ind[i+2]= j+2;
    }

   m->vertex_count= segments * 2 + 2 +   branches * 3;
    m->index_count= segments * 6 +  branches * 3;
}

static const short cube_vertices[]= {
   256, -256, -256,
   256, 256, -256,
   256, 256, 256,
   256, -256, 256,//+x
   -256, -256, -256,
   -256, 256, -256,
   -256, 256, 256,
   -256, -256, 256,//-x

   -256, 256, -256,
   256, 256, -256,
   256, 256, 256,
   -256, 256, 256,//+y
   -256, -256, -256,
   256, -256, -256,
   256, -256, 256,
   -256, -256, 256,//-y

   -256, -256, 256,
   256, -256, 256,
   256, 256, 256,
   -256, 256, 256,//+z
   -256, -256, -256,
   256, -256, -256,
   256, 256, -256,
   -256, 256, -256//-z
};

static const char cube_normals[]={
127,0,0,
127,0,0,
127,0,0,
127,0,0,//+x
-127,0,0,
-127,0,0,
-127,0,0,
-127,0,0,//-x

0,127,0,
0,127,0,
0,127,0,
0,127,0,//+y
0,-127,0,
0,-127,0,
0,-127,0,
0,-127,0,//-y

0,0,127,
0,0,127,
0,0,127,
0,0,127,//+z
0,0,-127,
0,0,-127,
0,0,-127,
0,0,-127//-z
};

static const short cube_tex_coord[]={
0,0,
4096,0,
4096,4096,
0,4096
};


void GenCube( Model* m )
{
     if( m->vertices == NULL )
    {
        m->indeces= new unsigned short[ 6 * 6 ];
        m->vertices= new ModelVertex[ 6 * 4 ];
    }
    unsigned short* ind= m->indeces;
    ModelVertex* v= m->vertices;
    for( unsigned int i= 0; i< 6*4; i++ )
    {
        v[i].pos[0]= cube_vertices[3*i];
        v[i].pos[1]= cube_vertices[3*i+1];
        v[i].pos[2]= cube_vertices[3*i+2];

        v[i].tex_coord[0]= cube_tex_coord[ (i&3)*2 ];
        v[i].tex_coord[1]= cube_tex_coord[ (i&3)*2+1 ];

        v[i].normal[0]= cube_normals[3*i];
        v[i].normal[1]= cube_normals[3*i+1];
        v[i].normal[2]= cube_normals[3*i+2];
    }

    for( unsigned int i= 0, j=0; i< 6*6; i+=6, j+=4 )
    {
        ind[i]= j;
        ind[i+1]= j+3;
        ind[i+2]= j+2;

        ind[i+3]= j;
        ind[i+4]= j+1;
        ind[i+5]= j+2;
    }

    m->vertex_count= 6 * 4;
    m->index_count= 6 * 6;
}

void GenAchtungTable( Model* m )
{
    static const short quad_vertices[]= {
        -128, -34, 256, 
        128, -34, 256,
        128, -34, 512,
        -128, -34, 512 };
       
        

    m->vertex_count= 4 * 6 + 4;//1 cube + 1 quad
    m->index_count= 6 * 6 + 6;

    m->vertices= new ModelVertex[ m->vertex_count ];
    m->indeces= new unsigned short[ m->index_count ];

    ModelVertex* v= m->vertices;
    for( unsigned int i= 0; i< 4 * 6; i++ )
    {
        
        v[i].pos[0]= cube_vertices[i*3];
        v[i].pos[1]= cube_vertices[i*3+1];
        v[i].pos[2]= cube_vertices[i*3+2];

        v[i].tex_coord[0]= cube_tex_coord[(i&3)*2];
        v[i].tex_coord[1]= cube_tex_coord[(i&3)*2 + 1];
        v[i].tex_coord[2]= TEXTURE_TREE_BARK;

        v[i].normal[0]= cube_normals[i*3];
        v[i].normal[1]= cube_normals[i*3+1];
        v[i].normal[2]= cube_normals[i*3+2];
       
    }


    short vec[]= { 0, 0, 256 };
    m->Move( vec );
    vec[0]= 256/8;
    vec[1]= 256/8;
    vec[2]= 256;
    m->Scale( vec );

    for( unsigned int i= 4 * 6, j= 0; j < 4; i++, j++ )
    {
        v[i].pos[0]= quad_vertices[j*3];
        v[i].pos[1]= quad_vertices[j*3+1];
        v[i].pos[2]= quad_vertices[j*3+2];

        v[i].tex_coord[0]= cube_tex_coord[(i&3)*2];
        v[i].tex_coord[1]= cube_tex_coord[(i&3)*2 + 1];
        v[i].tex_coord[2]= TEXTURE_ACHTUNG_MINES;

        /*v[i].normal[0]= cube_normals[i*3+8];
        v[i].normal[1]= cube_normals[i*3+1+8];
        v[i].normal[2]= cube_normals[i*3+2+8];*/
         v[i].normal[0]= cube_normals[3*12];
        v[i].normal[1]= cube_normals[3*12+1];
        v[i].normal[2]= cube_normals[3*12+2];
    }

    for( unsigned int i=0, j=0; i< 6 * 6 + 6; i+=6, j+=4 )
    {
        m->indeces[i]= j;
        m->indeces[i+1]= j+3;
        m->indeces[i+2]= j+2;

        m->indeces[i+3]= j;
        m->indeces[i+4]= j+2;
        m->indeces[i+5]= j+1;
    }

    m->primitive_type= GL_TRIANGLES;

}


void GenShovel( Model* m )
{
    static const short shovell_vertices[]={
    -32, 0, 0+192,
    -32, 0, 48+192,
    -24, 0, 64+192,
    24, 0, 64+192,
    32, 0, 48+192,
    32, 0, 0+192
    };
    static const char shovell_normal[]= { 0, -127, 0 };

   // segments= 12;

   unsigned int segments= 12;
    m->vertices= new ModelVertex[ segments * 2 + 2 + 6 ];
    m->indeces= new unsigned short[ segments * 6 + 3*4 ];
    GenCylinder(m,12);
    m->vertex_count= segments * 2 + 2 + 6 ;//1 cylinder (n*2+2) + shovell model
    m->index_count= segments * 6 + 3 * 4;

    short vec[]= { 8, 8, 192 };
    m->Scale( vec );
    vec[0]= 0;
    vec[1]= 0;
    vec[2]= -256;
   // m->Move(vec);
    m->SetTexture( TEXTURE_WOOD );


    ModelVertex* v= m->vertices;
    for( unsigned int i= segments * 2 + 2, j= 0; j < 6; i++, j++ )
    {
        v[i].pos[0]= shovell_vertices[j*3];
        v[i].pos[1]= 0;//shovell_vertices[j*3+1];
        v[i].pos[2]= shovell_vertices[j*3+2];

        v[i].tex_coord[0]= shovell_vertices[j*3]<<4;
        v[i].tex_coord[1]= shovell_vertices[j*3+2]<<4;
        v[i].tex_coord[2]= TEXTURE_METAL;

        v[i].normal[0]= shovell_normal[0];
        v[i].normal[1]= shovell_normal[1];
        v[i].normal[2]= shovell_normal[2];
    }

    unsigned short* ind= m->indeces;
    unsigned int v0= segments * 2 + 2;
    for( unsigned int i= segments * 6, j= segments * 2 + 2 + 1;
        i< segments * 6 + 3 * 4;
        i+=3, j++ )
    {
        ind[i]= v0;
        ind[i+1]= j;
        ind[i+2]= j+1;
    }

    //all model is one bne - 0
    m->SetBone(0);
    GenShovelAnimations(m);
}

void GenShovelAnimations( Model* m )
{
    m->animation_count= 1;
    #define FRAMES 18

    ModelAnimation* anim= & (m->animations[0]);
    m->is_animation[0]= true;


    anim->bone_count= 1;
    for( unsigned int i= 1; i< MAX_MODEL_BONES; i++ )
        anim->bones[i]= false;
    anim->bones[0]= true;
    anim->duration= 0.3f;
    anim->frames= FRAMES;

    anim->v_matrices= new float[ FRAMES * 16 ];
    anim->n_matrices= new float[ FRAMES * 9 ];

    float mat[16];
    float a=0.0f, da= -3.0f*MW_PI4 / float(FRAMES);
    for( unsigned int i= 0; i< FRAMES; i++, a+=da )
    {
        if( i == FRAMES/2 )
              da= -da;
        Mat4Identity(mat);

        mat[5]= cos(a);
        mat[6]= sin(a);
        mat[9]= - mat[6];
        mat[10]= mat[5];

        memcpy( (void*)(anim->v_matrices + i*16), mat, 16*sizeof(float) );
        Mat4ToMat3( mat );
        memcpy( (void*)(anim->n_matrices + i*9), mat, 9*sizeof(float) );
    }



#undef FRAMES
}

void GenCacodemon( Model* m )
{
   // segments= 16;
    //partition= 8;
    GenSphere(m,16,8);
    m->SetTexture( TEXTURE_CACODEMON_SKIN );


    Model cone;
   // segments= 10;
    GenCone( &cone,10 );
    cone.SetTexture( TEXTURE_METAL );

    float mat[16], m_t[16], m_s[16], m_r[16];
    Mat4Identity(mat);
    Mat4Identity(m_s);
    Mat4Identity(m_t);
    Mat4Identity(m_r);
    m_s[0]= 0.2f;
    m_s[5]= 0.2f;
    m_s[10]= 0.6f;
     
    m_t[14]= 0.9f;

    m_r[5]= cos( - MW_PI6 );
    m_r[6]= sin(  - MW_PI6 );
    m_r[9]= - m_r[6];
    m_r[10]= m_r[5];
    Mat4Mul( m_s, m_t, mat );
    Mat4Mul( mat, m_r );
    cone.Transform( mat );
    m->AddModel( &cone );

    m_r[5]= cos( MW_PI3 );
    m_r[6]= sin( MW_PI3 );
    m_r[9]= - m_r[6];
    m_r[10]= m_r[5];
    Mat4Mul( mat, m_r );
    cone.Transform( m_r );
    m->AddModel( &cone );

    Mat4RotateXY( m_r, MW_PI2 );
    m->Transform(m_r);
    m->TransformNormals(m_r);

    m->animation_count= 0;
    m->SetBone(0);

}

void GenPistol( Model* m )
{
   // segments= 12;
    GenCylinder(m,12);

    const static short scale_vec0[]= { 16, 16, 96 };
    m->Scale( scale_vec0 );
    m->SetTexture( TEXTURE_DARK_METAL );

    Model m2;
    GenCube( &m2 );
    const static short move_vec[]= { 0, 224, 0 };
    m2.Move( move_vec );
    const static short scale_vec1[]= { 16, 48, 16 };
    m2.Scale( scale_vec1 );

    float mat[16];
    Mat4Identity(mat);
    mat[6]= -0.15f;
    m2.Transform( mat );
    m2.SetTexture( TEXTURE_WOOD );
    m->AddModel( &m2 );


    GenCube( &m2 );
    m2.Scale( 18 );
    m2.SetTexture( TEXTURE_DARK_METAL );
    m->AddModel(&m2);

    GenCube(&m2);
    static const short scale_vec2[]= { 6, 6, 12 };
    m2.Scale( scale_vec2 );
    static const short move_vec2[]= { 0, -16, 96-12 };
    m2.Move(move_vec2);
    m2.SetTexture( TEXTURE_DARK_METAL );
    m->AddModel(&m2);

    Mat4Identity(mat);
    mat[5]= 0.0f;
    mat[6]= -1.0f;
    mat[9]= 1.0f;
    mat[10]= 0.0f;
    m->Transform(mat);
    m->TransformNormals(mat);

    m->SetBone(0);

    GenPistolAnimations(m);
}

void GenPistolAnimations( Model* m )
{
#define FRAMES 15

    m->animation_count= 1;

    ModelAnimation* anim= & (m->animations[0]);
    m->is_animation[0]= true;


    anim->bone_count= 1;
    for( unsigned int i= 1; i< MAX_MODEL_BONES; i++ )
        anim->bones[i]= false;
    anim->bones[0]= true;
    anim->duration= 0.25f;
    anim->frames= FRAMES;

    anim->v_matrices= new float[ FRAMES * 16 ];
    anim->n_matrices= new float[ FRAMES * 9 ];

    float mat[16];
    float a=0.0f, da= -MW_PI / float( FRAMES );
    for( unsigned int i= 0; i< FRAMES; i++, a+=da )
    {

        Mat4Identity(mat);
       // mat[13]= -0.15f * sin(a)*sin(a);
        mat[13]= -0.15f * fabs(sin(a));

        memcpy( (void*)(anim->v_matrices + i*16), mat, 16*sizeof(float) );
        Mat4ToMat3( mat );
        memcpy( (void*)(anim->n_matrices + i*9), mat, 9*sizeof(float) );
    }
#undef FRAMES
}

/*void GenMachineGun(Model* m)
{
    segments= 12;
    GenCylinder(m);//center cylinder



    const static short scale_vec0[]= { 20, 20, 256 };
    m->Scale( scale_vec0 );


    Model m2;
    GenCylinder(&m2);
    m2.Scale( scale_vec0 );
    const static short shift_vec0[]={ 44, 0, 0 };
    const static short shift_vec1[]={ -88, 0, 0 };
    m2.Move( shift_vec0 );

    m->AddModel(&m2);//left cylinder

    m2.Move( shift_vec1 );
    m->AddModel(&m2);//rgiht cylinder


    GenCube( &m2 );
    const static short scale_vec1[]= { 66, 22, 22 };
    m2.Scale( scale_vec1 );

    m->AddModel(&m2);//model body
     m->SetTexture( TEXTURE_METAL );

    GenCube( &m2 );
    const static short scale_vec2[]= { 22, 22, 64 };
    m2.Scale( scale_vec2 );
    const static short shift_vec2[]={ 0, 0, -64 };
    m2.Move( shift_vec2 );

     float mat[16];
    Mat4Identity(mat);
    mat[9]= -0.35f;
    m2.Transform( mat );//handle
    m2.SetTexture( TEXTURE_WOOD );
    m->AddModel(&m2);

    Mat4Identity(mat);
    mat[5]= 0.0f;
    mat[6]= -1.0f;
    mat[9]= 1.0f;
    mat[10]= 0.0f;
    m->Transform(mat);
    m->TransformNormals(mat);

    m->SetBone(0);
    GenMachineGunAnimations(m);
}

void GenMachineGunAnimations( Model* m )
{
    GenPistolAnimations(m);
    m->animations[0].duration= 0.125f;
}
*/

void GenSuperMachineGunAnimations(Model* m )
{
    #define FRAMES 8

    m->animation_count= 1;

    ModelAnimation* anim= & (m->animations[0]);
    m->is_animation[0]= true;

    anim->bone_count= 1;
    for( unsigned int i= 0; i< MAX_MODEL_BONES; i++ )
        anim->bones[i]= false;
    anim->bones[1]= true;
    anim->duration= 0.125f;
    anim->frames= FRAMES;

    anim->v_matrices= new float[ FRAMES * 16 ];
    anim->n_matrices= new float[ FRAMES * 9 ];

    float mat[16];
    float a=0.0f, da= MW_PI2 / float( FRAMES );
    for( unsigned int i= 0; i< FRAMES; i++, a+=da )
    {
        Mat4Identity(mat);
        mat[0]= cos(a);
        mat[2]= sin(a);
        mat[8]= - mat[2];
        mat[10]= mat[0];

        memcpy( (void*)(anim->v_matrices + i*16), mat, 16*sizeof(float) );
        Mat4ToMat3( mat );
        memcpy( (void*)(anim->n_matrices + i*9), mat, 9*sizeof(float) );
    }
#undef FRAMES
}

void GenSuperMachineGun( Model* m )
{
    /*
    bone 0 - handle
    bone 1 - guns
    */

   // segments= 16;
    //use_caps= true;
    GenCylinder( m, 16, true );
    //use_caps= false;

    const static short scale_vec0[]= { 60, 60, 22 };
    m->Scale(scale_vec0);//body


    Model m2;
   // segments= 12;
    GenCylinder( &m2, 12 );
    const static short scale_vec1[]= { 19, 19, 256 };
    m2.Scale( scale_vec1 );
    Model m3;
   // segments= 10;use_caps= true;
    GenCylinder(&m3, 10, true );//use_caps= false;
    static const short scale_vec_m[]= { 22, 22, 42 };
    m3.Scale( scale_vec_m);
    static const short move_vec_m[]= { 0, 0, 250 };
    m3.Move( move_vec_m );m3.index_count-= 10*3;
    m2.AddModel(&m3);
    static const  short move_vec[]= { 
         0, -30, 0 ,
         30, 30, 0,
         -30, 30, 0,
        -30, -30, 0  };
    for( unsigned int i= 0; i< 4; i++ )
    {
        m2.Move( move_vec + i*3 );
        m->AddModel(&m2);
    }
    m->SetBone(1);
    m->SetTexture( TEXTURE_METAL );


   // delete[] m2.vertices, m2.indeces;
   // m2.vertices= NULL;
   // segments= 16;
   // use_caps= true;
   // GenCylinder(&m2);
  //  use_caps= false;
    GenCube(&m2);
    const static short scale_vec2[]= { 52, 52, 48 };
    const static short move_vec1[]= { 0, 0, -48 };
    m2.Scale(scale_vec2);//body
    m2.Move(move_vec1);
    m2.SetBone(0);
    m2.SetTexture( TEXTURE_METAL );
    m->AddModel(&m2);


    GenCube( &m2 );
    const static short scale_vec3[]= { 28, 28, 72 };
    m2.Scale( scale_vec3 );
    const static short shift_vec2[]={ 0, -24, -72-48-24 };
    m2.Move( shift_vec2 );
    float mat[16];
    Mat4Identity(mat);
    mat[9]= -0.25f;
    m2.Transform( mat );//handle
    m2.SetTexture( TEXTURE_WOOD );
    m2.SetBone(0);
    m->AddModel(&m2);


    Mat4Identity(mat);
    mat[5]= 0.0f;
    mat[6]= -1.0f;
    mat[9]= 1.0f;
    mat[10]= 0.0f;
    m->Transform(mat);
    m->TransformNormals(mat);

    GenSuperMachineGunAnimations(m);
}
void GenSmallHealthPack( Model* m )
{
  //  segments= 12;
  //  partition= 7;

   GenSphere(m, 12, 7 );

   for( unsigned int i= 0; i< 13*4; i++ )
       m->vertices[i].pos[2]-= 384;

   m->Scale(32);

   float mat[16], m_t[16], m_r[16];
   Mat4Identity( m_t );
   Mat4Identity( m_r );

   m_t[14]= 0.09375f;

    m_r[5]= cos( - MW_PI6 );
    m_r[6]= sin( - MW_PI6 );
    m_r[9]= - m_r[6];
    m_r[10]= m_r[5];
    Mat4Mul( m_t, m_r, mat );
    
    m->Transform( mat );

    static const short tex_scale_vec[]= { 4096, 4096*2 };
    m->ScaleTexture( tex_scale_vec );
    static const short shift_vec[]= { 0, 2048  };
    m->ShiftTexture(shift_vec);


   m->SetTexture( TEXTURE_HEALTHPACK );
 
}

void GenMediumHealthPack( Model* m )
{
  //  segments= 14;
  //  partition= 8;

    GenSphere(m, 14, 8 );
    m->Scale( 96 );

    const static short tex_scale_vec[]={ 8192, 4096 };
    m->ScaleTexture( tex_scale_vec );


    m->SetTexture( TEXTURE_HEALTHPACK );
}

void GenLargeHealthPack( Model* m )
{
    GenCube(m);
    m->Scale( 144 );

    static const short tex_scale[]= { 2048, 2048 };
    m->ScaleTexture( tex_scale );
    static const short tex_move[]= { 1024, 1024 };
    m->ShiftTexture( tex_move );

    m->SetTexture( TEXTURE_HEALTHPACK );
}

void GenRobot( Model* m )
{
    /*
    bone 0 - head + body
    bone 1 - hand
    bone 2 - hand with hammer ( right hand )
    bone 3 -left leg
    bone 4 - right leg
    */

    //head
   // segments= 12;
    //partition= 7;
    GenCube(m);
    m->Scale( 33 );//make head size 67
    static const short head_move[]={ 0, 0, 412 };
    m->Move( head_move );
    m->SetBone(0);
    m->SetTexture( TEXTURE_METAL );
    for( int i= 8; i< 12; i++ )
        m->vertices[i].tex_coord[2]= TEXTURE_ROBOT_HEAD;

     //legs
    Model m2;
   // segments= 24;
    GenCylinder(&m2, 24);
   // segments= 10;
    GenCylinder(&m2, 10);
    static const short leg_scale[]= { 24, 24, 190 };
    m2.Scale(leg_scale);
    static const short leg_move0[]= { -32, 0, 0 };
    m2.Move( leg_move0 );
    m2.SetBone(3);
    m2.SetTexture( TEXTURE_METAL );
    m->AddModel(&m2);
    static const short leg_move1[]= { 64, 0, 0 };
    m2.Move( leg_move1 );
    m2.SetBone(4);
    m2.SetTexture( TEXTURE_METAL );
    m->AddModel(&m2);

     //hands
    GenCylinder(&m2, 10);
    static const short hand_scale[]= { 20, 20, 210 };
    m2.Scale( hand_scale );
    static const short hand_move0[]= { 86, 0, 170 };
    m2.Move( hand_move0 );
    m2.SetBone(1);
    m2.SetTexture( TEXTURE_METAL );
    m->AddModel(&m2);

    //hand with hammer
    Model m3;
    GenCube(&m3);
    m3.Scale(24);
    m3.Move( hand_move0 );
    m2.AddModel(&m3);
    static const short hand_move1[]= { -172 - 4, 0, 0 };
    m2.Move( hand_move1 );
    m2.SetBone(2);
    m2.SetTexture( TEXTURE_METAL );
    m->AddModel(&m2);

    //body
    GenCube(&m2);
    static const short body_scale[]= { 64, 30, 95 };
    m2.Scale( body_scale );
    static const short body_move[]= { 0, 0, 284 };
    m2.Move( body_move );
    m2.SetBone(0);
    m2.SetTexture( TEXTURE_METAL );
     for( int i= 8; i< 12; i++ )
     {
        m2.vertices[i].tex_coord[2]= TEXTURE_ROBOT_BODY;
        m2.vertices[i].tex_coord[0]= - m2.vertices[i].tex_coord[0];
     }
    m->AddModel(&m2);


   // m->SetTexture( TEXTURE_METAL );
    GenRobotAnimations(m);
    m->Scale( 256+128 );//hack for big size

}

void GenRobotAnimations( Model* m )
{
    m->animation_count= 2;
    ModelAnimation* anim= & (m->animations[ANIMATION_ATTACK0]);
    m->is_animation[ANIMATION_ATTACK0]= true;

    //hammer animation
    #define FRAME_COUNT 30

    anim->bone_count= 1;
    for( unsigned int i= 0; i< MAX_MODEL_BONES; i++ )
        anim->bones[i]= false;
     anim->bones[2]= true;

    anim->frames= FRAME_COUNT;
    anim->v_matrices= new float[ 16 * 1 * FRAME_COUNT ];//4x4matrix for FRAME_COUNT frames of 1 bone
    anim->n_matrices= new float[ 9 * 1 * FRAME_COUNT ];//3x3matrix for FRAME_COUNT frames of 1 bone
    anim->duration= 1.0f;

    float rot_mat[16], tr_mat0[16], tr_mat1[16], res_mat[16];
    Mat4Identity(tr_mat0);
    Mat4Identity(tr_mat1);
    tr_mat0[14]= -1.4453125f * 1.5f;//370/256
    tr_mat1[14]= 1.4453125f * 1.5f;

    float a= 0.0f, da= MW_2PI / float(FRAME_COUNT);
    for( unsigned int i= 0; i< FRAME_COUNT; i++, a-= da )
    {
        Mat4Identity(rot_mat);
        rot_mat[5]= cos(a);
        rot_mat[6]= sin(a);
        rot_mat[9]= - rot_mat[6];
        rot_mat[10]= rot_mat[5];

        Mat4Mul( tr_mat0, rot_mat, res_mat );
        Mat4Mul( res_mat, tr_mat1 );

        memcpy( (void*)(anim->v_matrices + i*16), res_mat, 16*sizeof(float) );
        Mat4ToMat3( rot_mat );
        memcpy( (void*)(anim->n_matrices + i*9), rot_mat, 9*sizeof(float) );

    }

    #undef FRAME_COUNT

    //walk animation
    #define FRAME_COUNT 60
    m->is_animation[ANIMATION_WALK0]= true;
    anim= & (m->animations[ANIMATION_WALK0]);
    anim->duration= 1.0f;
    anim->bone_count= 2;

     for( unsigned int i= 0; i< MAX_MODEL_BONES; i++ )
        anim->bones[i]= false;
      anim->bones[3]= anim->bones[4]= true;


     anim->frames= FRAME_COUNT;
    anim->v_matrices= new float[ 16 * 2 * FRAME_COUNT ];//4x4matrix for FRAME_COUNT frames of 2 bones
    anim->n_matrices= new float[ 9 * 2 * FRAME_COUNT ];//3x3matrix for FRAME_COUNT frames of 2 bones


    tr_mat0[14]= -0.703125f * 1.5f;//180/256
    tr_mat1[14]= 0.703125f * 1.5f;

    unsigned int step_end_time[]= { FRAME_COUNT/4, 3*FRAME_COUNT/4, FRAME_COUNT };
    float a0[]= { 0.0f, MW_PI6, -MW_PI6};
    float da0[]= { MW_PI6/float(FRAME_COUNT/4), -MW_PI3/float(FRAME_COUNT/2), MW_PI6/float(FRAME_COUNT/4) };
    float a2[2];
    for( unsigned int i= 0, j=0, step= 0; step< 3; step++ )
    {
        a2[0]= a0[step];
        a2[1]= -a0[step];
        da= da0[step];
        for( ; i< step_end_time[step]; i++, j+=2, a2[0]+= da, a2[1]-=da )
        {
            for( unsigned int k= 0; k< 2; k++ )
            {
                Mat4Identity(rot_mat);
                rot_mat[5]= cos(a2[k]);
                rot_mat[6]= sin(a2[k]);
                rot_mat[9]= - rot_mat[6];
                rot_mat[10]= rot_mat[5];

                Mat4Mul( tr_mat0, rot_mat, res_mat );
                Mat4Mul( res_mat, tr_mat1 );

                memcpy( (void*)(anim->v_matrices + j*16 +k*16), res_mat, 16*sizeof(float) );
                Mat4ToMat3( rot_mat );
                memcpy( (void*)(anim->n_matrices + j*9 + k*9), rot_mat, 9*sizeof(float) );
            }

        }
    }

    #undef FRAME_COUNT

    //remote attack 
    #define FRAME_COUNT 90
    m->is_animation[ANIMATION_ATTACK1]= true;
    anim= & (m->animations[ANIMATION_ATTACK1]);
    anim->duration= 1.5f;
    anim->bone_count= 1;

    anim->frames= FRAME_COUNT;
    anim->v_matrices= new float[ 16 * 1 * FRAME_COUNT ];//4x4matrix for FRAME_COUNT frames of 1 bone
    anim->n_matrices= new float[ 9 * 1 * FRAME_COUNT ];//3x3matrix for FRAME_COUNT frames of 1 bone

      for( unsigned int i= 0; i< MAX_MODEL_BONES; i++ )
        anim->bones[i]= false;
      anim->bones[1]= true;

    tr_mat0[14]= -1.4453125f * 1.5f;//370/256
    tr_mat1[14]= 1.4453125f * 1.5f;

    a= 0.0f, da= MW_2PI / float(FRAME_COUNT);
    for( unsigned int i= 0; i< FRAME_COUNT; i++, a+= da )
    {
        Mat4Identity(rot_mat);
        rot_mat[5]= cos(a);
        rot_mat[6]= sin(a);
        rot_mat[9]= - rot_mat[6];
        rot_mat[10]= rot_mat[5];

        Mat4Mul( tr_mat0, rot_mat, res_mat );
        Mat4Mul( res_mat, tr_mat1 );

        memcpy( (void*)(anim->v_matrices + i*16), res_mat, 16*sizeof(float) );
        Mat4ToMat3( rot_mat );
        memcpy( (void*)(anim->n_matrices + i*9), rot_mat, 9*sizeof(float) );

        if( i>=  FRAME_COUNT/4 && i<= 3*FRAME_COUNT/4 )
        {
            da= 0.0f;
        }
        else if( i> 3*FRAME_COUNT/4 )
        {
            da= -MW_2PI / float(FRAME_COUNT);
        }
    }
#undef FRAME_COUNT
}

void GenSnowManAnimations( Model* m )
{

    ModelAnimation* anim;
     //remote attack 

    #define FRAME_COUNT (60*8/10)

    m->is_animation[ANIMATION_ATTACK1]= true;
    anim= & (m->animations[ANIMATION_ATTACK1]);
    anim->duration= 0.8f;
    anim->bone_count= 1;

    anim->frames= FRAME_COUNT;
    anim->v_matrices= new float[ 16 * 1 * FRAME_COUNT ];//4x4matrix for FRAME_COUNT frames of 1 bone
    anim->n_matrices= new float[ 9 * 1 * FRAME_COUNT ];//3x3matrix for FRAME_COUNT frames of 1 bone

      for( unsigned int i= 1; i< MAX_MODEL_BONES; i++ )
        anim->bones[i]= false;
     anim->bones[0]= true;


    float rot_mat[16];
    float a, da;
    a= 0.0f, da= MW_PI4 / float(FRAME_COUNT);
    for( unsigned int i= 0; i< FRAME_COUNT; i++, a+= da )
    {
        if( i == FRAME_COUNT / 4 || i == FRAME_COUNT *3/4 )
            da= - da;

        Mat4Identity(rot_mat);
        rot_mat[5]= cos(a);
        rot_mat[6]= sin(a);
        rot_mat[9]= - rot_mat[6];
        rot_mat[10]= rot_mat[5];

        memcpy( (void*)(anim->v_matrices + i*16), rot_mat, 16*sizeof(float) );
        Mat4ToMat3( rot_mat );
        memcpy( (void*)(anim->n_matrices + i*9), rot_mat, 9*sizeof(float) );

    }
#undef FRAME_COUNT
}

void GenSnowMan( Model* m )
{
  //  segments= 16;
  //  partition= 10;

#define SNOWMAN_DEPTH 256 - 64
    
     //bottom sphere
    GenSphere(m, 16, 10 );
    static const short move_vec0[]= { 0, 0, SNOWMAN_DEPTH };
    m->Move(move_vec0);
    m->SetTexture( TEXTURE_SNOW );

    //middle sphere
    Model m2;
    GenSphere(&m2, 16, 10);
    m2.Scale(192);
    static const short move_vec1[]= { 0, 0, 256 +SNOWMAN_DEPTH };
    m2.Move( move_vec1 );
    m2.SetTexture( TEXTURE_SNOW );
    m->AddModel( &m2 );

    //top sphere - head
    GenSphere(&m2, 14, 8 );
    m2.Scale(128);
    static const short move_vec2[]= { 0, 0, 512 +SNOWMAN_DEPTH };
    m2.Move( move_vec2 );
    m2.SetTexture(TEXTURE_SNOWMAN_HEAD );
    m->AddModel( &m2 );

    //nose
    //segments= 10;
    GenCone(&m2, 8);
    m2.SetTexture( TEXTURE_TREE_BARK );
    float mat[16];
    Mat4Identity(mat);
    
    mat[5]= 0.0f;//cos( MW_PI2 );
    mat[6]= -1.0f;//sin( MW_PI2 );
    mat[9]= - mat[6];
    mat[10]= mat[5];

    m2.Transform( mat );
    m2.TransformNormals( mat );

    static const short scale_vec[]= { 16, 64, 16 };
    m2.Scale( scale_vec );
    static const short move_vec3[]= { 0, 128, 512 +SNOWMAN_DEPTH };
    m2.Move( move_vec3 );
    
    m->AddModel(&m2);

    //hand 0
    GenCone( &m2, 8 );
    static const short hand_scale_vec0[]= { 16, 16, 256 };
    m2.Scale( hand_scale_vec0 );
    Mat4Identity(mat);
    mat[0]= cos( MW_PI2+MW_PI6 );
    mat[2]= sin( MW_PI2+MW_PI6 );
    mat[8]= -mat[2];
    mat[10]= mat[0];

    m2.Transform( mat );
    m2.TransformNormals( mat );

    static const short move_vec4[]= { -128, 0, 256 +SNOWMAN_DEPTH };
    m2.Move( move_vec4 );
    m->AddModel(&m2);

     //hand 1
    GenCone( &m2, 8 );
    static const short hand_scale_vec1[]= { 16, 16, 256+64 };
    m2.Scale( hand_scale_vec1 );
    Mat4Identity(mat);
    mat[0]= cos( -MW_PI2-MW_PI6*0.5f );
    mat[2]= sin( -MW_PI2-MW_PI6*0.5f );
    mat[8]= -mat[2];
    mat[10]= mat[0];

    m2.Transform( mat );
    m2.TransformNormals( mat );

    static const short move_vec5[]= { 128, 0, 256 +SNOWMAN_DEPTH };
    m2.Move( move_vec5 );
    m->AddModel(&m2);


    m->SetBone(0);
    GenSnowManAnimations( m );
   // m->SetTexture( TEXTURE_SNOW );

#undef SNOWMAN_DEPTH
}


void GenChainBulletsAmmoBox( Model* m )
{
    GenCube(m);
    static const short scale_vec[]= { 96, 64, 72 };
    m->Scale(scale_vec);
    m->SetTexture( TEXTURE_CHAIN_BULLETS_AMMO_BOX );
}

void GenBulletsBox( Model* m )
{
    GenCube(m);
    static const short scale_vec[]= { 64, 64, 48 };
    m->Scale(scale_vec);
    m->SetTexture( TEXTURE_BULLETS_AMMO_BOX );
}

void GenRocketLauncherAnimations( Model* m )
{
    #define FRAMES (60*4/10)

    m->animation_count= 1;

    ModelAnimation* anim= & (m->animations[0]);
    m->is_animation[0]= true;


    anim->bone_count= 1;
    for( unsigned int i= 1; i< MAX_MODEL_BONES; i++ )
        anim->bones[i]= false;
    anim->bones[0]= true;
    anim->duration= 0.4f;
    anim->frames= FRAMES;

    anim->v_matrices= new float[ FRAMES * 16 ];
    anim->n_matrices= new float[ FRAMES * 9 ];

    float mat[16], m_t[16], m_r[16];
    float y=0.0f, dy= -MW_PI / float( FRAMES );
    float a= 0.0f, da=  MW_PI6 / float( FRAMES );
    for( unsigned int i= 0; i< FRAMES; i++, y+=dy, a+=da )
    {
        if( i == FRAMES/2 )
            da= -da;

        Mat4Identity(m_t);
        Mat4Identity(m_r);
        m_t[13]= -0.25f * fabs(sin(y));

        m_r[5]= cos(a);
        m_r[6]= sin(a);
        m_r[9]= - m_r[6];
        m_r[10]= m_r[5];

        Mat4Mul( m_t, m_r, mat );
        

        memcpy( (void*)(anim->v_matrices + i*16), mat, 16*sizeof(float) );
        Mat4ToMat3( m_r );
        memcpy( (void*)(anim->n_matrices + i*9), m_r, 9*sizeof(float) );
    }
#undef FRAMES
}

void GenRocketLauncher( Model* m )
{
    GenCylinder( m, 16 );
    static const short scale_vec0[]= { 48, 48, 256 };
    m->Scale( scale_vec0 );
    m->SetTexture( TEXTURE_METAL );

    Model m2;
    GenCylinder( &m2, 12, true );
    static const short scale_vec1[]= { 64, 64, 96 };
    static const short move_vec1[]= { 0, 12, -96 };
    m2.Scale( scale_vec1 );
    m2.Move( move_vec1 );
    m2.SetTexture( TEXTURE_DARK_METAL );
    m->AddModel(&m2);

    GenCube(&m2);
    static const short scale_vec2[]= { 6, 6, 12 };
    m2.Scale( scale_vec2 );
    static const short move_vec2[]= { 0, -48-8, 256-12 };
    m2.Move(move_vec2);
    m2.SetTexture( TEXTURE_DARK_METAL );
    m->AddModel(&m2);

    GenCube(&m2);
    m2.Scale( scale_vec2 );
    static const short move_vec3[]= { 16, -48-4, -18 };
    static const short move_vec3b[]= { -32, 0, 0 };
    m2.Move( move_vec3 );
    m->AddModel(&m2);
    m2.Move( move_vec3b );
    m->AddModel(&m2);


    GenCylinder(&m2, 12 );
    static const short scale_vec4[]= { 16, 16, 64 };
    m2.Scale( scale_vec4 );
    float mat[16];
    Mat4Identity(mat);
    mat[5]= 0.0f;
    mat[6]= -1.0f;
    mat[9]= 1.0f;
    mat[10]= 0.0f;
    m2.Transform( mat );
    m2.TransformNormals( mat );
    static const short move_vec4[]= { 0, 72, -48 };
    m2.Move( move_vec4 );
    m2.SetTexture( TEXTURE_WOOD );
    m->AddModel( &m2 );

    GenCube(&m2);
    static const short scale_vec5[]= { 24, 32, 96 };
    static const short move_vec5[]= { 0, 0, -96 - 96 };
    m2.Scale( scale_vec5 );
    m2.Move( move_vec5 );
    m2.SetTexture( TEXTURE_WOOD );
    Mat4Identity(mat);
    mat[9]= -0.15f;
    m2.Transform(mat);
    m->AddModel( &m2 );

    GenCylinder(&m2, 8, true );
    static const short scale_vec6[]= { 16, 16, 240 };
    static const short move_vec6[]= { 0, 48+16-2, 0 };
    m2.Scale( scale_vec6 );
    m2.Move( move_vec6 );
    m2.SetTexture( TEXTURE_METAL );
    m->AddModel( &m2 );

    Mat4Identity(mat);
    mat[5]= 0.0f;
    mat[6]= -1.0f;
    mat[9]= 1.0f;
    mat[10]= 0.0f;
    m->Transform(mat);
    m->TransformNormals(mat);

    m->SetBone(0);

    GenRocketLauncherAnimations(m);
}


void GenRocket( Model* m )
{
    GenSphere( m, 10, 8 );
    static const short scale_vec0[]= { 40, 40, 72 };
    static const short move_vec0[]= { 0, 0, 192-32 };
    m->Scale( scale_vec0 );
    m->Move( move_vec0 );
    m->SetTexture( TEXTURE_DARK_METAL );

    Model m2;
    GenCylinder( &m2, 10, true );
    static const short scale_vec1[]= { 40, 40, 192-32 };
    m2.Scale( scale_vec1 );
    m2.SetTexture( TEXTURE_METAL );
    m->AddModel( &m2 );

    m->SetBone(0);
}

void GenRocketAmmoBox( Model* m )
{
    static const short move_vec[]={
    -40, -40, 0,
    80, 0, 0,
    0, 80, 0,
    -80, 0, 0 };

    Model m2;
    GenRocket(&m2);
    m->index_count= m->vertex_count= 0;
    for( unsigned int i= 0; i< 4; i++ )
    {
        m2.Move( move_vec + i*3 );
        m->AddModel( &m2 );
    }

    static const short move_vec1[]= { 0, 0, -72 };
    m->Move( move_vec1 );
}


void GenSnowBall( Model* m )
{
    GenSphere( m, 10, 7 );
    m->Scale( 64 );
    m->SetTexture( TEXTURE_SNOW );
}

void GenFireBall( Model* m )
{
    GenSphere( m, 10, 7 );
    m->Scale( 96 );
    m->SetTexture( TEXTURE_FIRE_BALL );
}

void GenGaussGunAnimations( Model* m )
{
    #define FRAMES 12

    m->animation_count= 1;

    ModelAnimation* anim= & (m->animations[0]);
    m->is_animation[0]= true;


    anim->bone_count= 2;
    for( unsigned int i= 0; i< MAX_MODEL_BONES; i++ )
        anim->bones[i]= false;
    anim->bones[0]= anim->bones[1]= true;
    anim->duration= 0.1875f;
    anim->frames= FRAMES;

    anim->v_matrices= new float[ FRAMES * 16 * 2 ];
    anim->n_matrices= new float[ FRAMES * 9 * 2 ];

    float mat[16];
    float a=0.0f, da= -MW_PI / float( FRAMES );
    for( unsigned int i= 0; i< FRAMES; i++, a+=da )
    {

        for( unsigned int k= 0; k< 2; k++ )
        {
            Mat4Identity(mat);
       // mat[13]= -0.15f * sin(a)*sin(a);
            mat[13]= (-0.1f -float(k)*0.1f )* fabs(sin(a));

            memcpy( (void*)(anim->v_matrices + i*16*2 +k*16), mat, 16*sizeof(float) );
            Mat4ToMat3( mat );
            memcpy( (void*)(anim->n_matrices + i*9*2 +k*9), mat, 9*sizeof(float) );
        }
    }
#undef FRAMES
}

void GenGaussGun( Model* m )
{
    GenCube(m);
    static const short scale_vec0[]= { 28 ,28, 72 };
    m->Scale( scale_vec0 );
    m->SetTexture( TEXTURE_DARK_METAL );



    Model m2;
    GenCylinder( &m2, 16 );
    static const short scale_vec1[]= { 22, 22, 160 };
    static const short move_vec1[]= { 0, 0, 72 };
    m2.Scale( scale_vec1 );
    m2.Move( move_vec1 );
    m2.SetTexture( TEXTURE_COOPER_WIRE );
    m2.SetBone(0);
    m->AddModel(&m2);

    GenCylinder( &m2, 12 );
    static const short scale_vec1b[]= { 18, 18, 200 };
    m2.Scale( scale_vec1b );
    m2.Move( move_vec1 );
    m2.SetTexture( TEXTURE_DARK_METAL );
    m2.SetBone(1);
    m->AddModel(&m2);

    GenCube(&m2);
    static const short scale_vec2[]= { 34, 34, 12 };
    static const short move_vec2[]= { 0, 0, 72 };
    static const short move_vec2b[]= { 0, 0, -48 };
    m2.Scale( scale_vec2 );
    m2.Move( move_vec2 );
    m2.SetTexture( TEXTURE_METAL );
    m2.SetBone(0);
    for( unsigned int i= 0; i< 3; i++ )
    {
        m2.Move( move_vec2b );
        m->AddModel(&m2);
    }

    Model m3;
    GenSphere( &m3, 16, 8 );
    m3.Scale( 16 );
    static const short move_vec3[]= { 0, -72, 72-16 };
    m3.Move( move_vec3 );
    m3.SetTexture( TEXTURE_DARK_METAL );
   
    GenCylinder( &m2, 12 );
    float mat[16];
    Mat4Identity( mat );
    mat[5]= 0.0f;
    mat[6]= -1.0f;
    mat[9]= 1.0f;
    mat[10]= 0.0f;
    m2.Transform( mat );
    m2.TransformNormals( mat );
    static const short scale_vec3c[]= { 10, 64, 10 };
    m2.Scale( scale_vec3c );
    static const short move_vec3c[]= { 0, -72, 72-16 };
    m2.Move( move_vec3c );
    m2.SetTexture( TEXTURE_COOPER_WIRE );

    m2.AddModel(&m3);
    m2.SetBone(0);

    float a= 0.0f, da= MW_PI2; 
    for( unsigned int i= 0; i< 3; i++, a+=da )
    {
       Mat4RotateXY( mat, a );
       m2.Transform( mat );
       m2.TransformNormals( mat );
        m->AddModel(&m2);
    }


    GenCube( &m2 );
    static const short scale_vec4[]= { 64, 12, 12 };
    static const short move_vec4[]= { 64, 0, -32 };
    m2.Scale( scale_vec4 );
    m2.Move( move_vec4 );
    m2.SetTexture( TEXTURE_WOOD );
    m->AddModel(&m2);

    GenCube(&m2);
    static const short scale_vec5[]= { 12, 64, 12 };
    static const short move_vec5[]= { 0, 64, -72 };
    m2.Scale( scale_vec5 );
    m2.Move( move_vec5 );
    m->AddModel(&m2);



    GenCube(&m2);
    static const short scale_vec6[]= { 20, 28, 88 };
    static const short move_vec6[]= { 0, 0, -96 - 80 };
    m2.Scale( scale_vec6 );
    m2.Move( move_vec6 );
    //m2.SetTexture( TEXTURE_WOOD );
    Mat4Identity(mat);
    mat[9]= -0.15f;
    m2.Transform(mat);
    m->AddModel( &m2 );

   // float mat[16];
    Mat4Identity(mat);
    mat[5]= 0.0f;
    mat[6]= -1.0f;
    mat[9]= 1.0f;
    mat[10]= 0.0f;
    m->Transform(mat);
    m->TransformNormals(mat);

   // m->SetTexture( TEXTURE_METAL );
  //  m->SetBone( 0 );
    GenGaussGunAnimations(m);

}

void GenGaussBolts( Model* m )
{
    GenCylinder( m, 6, true );
    static const short scale_vec[]= { 64, 64, 128 };
    static const short move_vec[]= { 0,0,-64 };
    m->Scale( scale_vec );
    m->Move( move_vec );
    m->SetTexture( TEXTURE_COOPER_WIRE );

    Model m2;
    float a= 0.0f, da= MW_2PI/6.0f;
    for( unsigned int i= 0 ;i< 6; i++, a+= da )
    {
        GenCylinder( &m2, 8, true );
        static const short scale_vec1[]= { 14, 14, 32 };
        short move_vec1[]= { short(cos(a)*34.0f),short(sin(a)*34.0f),64 };
        m2.Scale( scale_vec1 );
        m2.Move( move_vec1 );
        m2.SetTexture( TEXTURE_DARK_METAL );
        m->AddModel(&m2);
    }
}




/**************************
**TREE GENERATION SECTION**
**************************/

const float TREE_SIZE= 15.0f;
const float TREE_WIDTH= 0.5f;
const unsigned int TREE_DEPTH= 3;
const unsigned int MAX_BRANCHES= 6;
const unsigned int MIN_BRANCHES= 3;
const float TREE_K= 0.5f;

struct TreeBranch
{
    float pos;//z position of branch in relative coordinates of parent branch
    float z_ang, xy_ang;//angles of branch relative of parent branch
    float size;
    unsigned int depth;
   // TreeBranch* child_branches[8];
    TreeBranch* parent_branch;

    void GenTransformMatrix( float* mat )
    {
        float m_r[16], mr_z[16], m_t[16], m_parent[16];
        Mat4RotateXY( m_r, xy_ang );
        Mat4Identity( m_t );
        Mat4Identity( mr_z );
        m_t[14]= pos;

        mr_z[5]= cos( z_ang );
        mr_z[6]= sin( z_ang );
        mr_z[9]= - mr_z[6];
        mr_z[10]= mr_z[5];


        Mat4Mul( mr_z, m_r, mat );
        Mat4Mul( mat, m_t );
        if( parent_branch != NULL )
        {
            parent_branch->GenTransformMatrix( m_parent );
            Mat4Mul( mat, m_parent );
        }
    }

    void GenNormalMatrix( float* mat )
    {
        float m_r[16], mr_z[16], m_parent[16];
        Mat4RotateXY( m_r, xy_ang );
        Mat4Identity( mr_z );

        mr_z[5]= cos( z_ang );
        mr_z[6]= sin( z_ang );
        mr_z[9]= - mr_z[6];
        mr_z[10]= mr_z[5];


        Mat4Mul( mr_z, m_r, mat );
        if( parent_branch != NULL )
        {
            parent_branch->GenNormalMatrix( m_parent );
            Mat4Mul( mat, m_parent );
        }
    }
};

static unsigned int branch_pull_shift= 0;
void GenTree_r( unsigned int depth, float size/*size of parent branch*/, TreeBranch* branch_pull, TreeBranch* parent_branch, float init_pos )//return pointer to branch
{

    TreeBranch* b= branch_pull + branch_pull_shift;
    branch_pull_shift++;

    b->xy_ang= randf( MW_2PI );
    if( depth == TREE_DEPTH )
    {
        b->z_ang= 0.0f;
        b->pos= 0.0f;
    }
    else
    {
        b->z_ang= randf( MW_PI6, MW_PI3 );
        b->pos= init_pos + randf(0.1f)*size;
    }
    b->parent_branch= parent_branch;
    b->size= size;
    b->depth= depth;

    if( depth != 0 )
    {
        unsigned int n= MIN_BRANCHES + rand()%( MAX_BRANCHES-MIN_BRANCHES+1 );
      //  b->child_branches_count= n;
        float p= 0.2f * size;
        float dp= 0.6f * size / float(n);
        for( unsigned int i= 0; i< n; i++, p+= dp )
        {
            float scale= 1.0f ;
            GenTree_r( depth-1, scale* size * TREE_K * randf( 0.8f, 1.2f ), branch_pull, b, p );
        }
    }
   // else
      //  b->child_branches_count= 0;

   // return b;
}



void GenTreeGreenery( Model* m, float size )
{
    size*= 2.0f;
    const unsigned int segments= 2;

    m->vertex_count= segments * 4;
    m->index_count= segments * 6;
    if( m->vertices == NULL )
    {
        m->vertices= new ModelVertex[ m->vertex_count ];
        m->indeces= new unsigned short[ m->index_count ];
    }

    float quad[]= { -0.5f,0.0f,  0.5f,0.0f,  0.5f,1.0f,  -0.5f,1.0f };
    short tc[]= { 0,0,  4096,0,  4096,4096, 0,4096 };  

    ModelVertex* v= m->vertices;
     float a= 0.0f, da= MW_PI / float(segments);
    for( unsigned int i= 0; i< segments; i++, a+= da )
    {
        float x= cos(a), y= sin(a);
        for( unsigned int j= 0; j< 4; j++, v++ )
        {
           v[0].pos[0]= short( quad[j*2]*x*256.0f * size );
           v[0].pos[1]= short( quad[j*2]*y*256.0f * size );
           v[0].pos[2]= short( quad[j*2+1]*256.0f * size );

           v[0].tex_coord[0]= tc[j*2];
           v[0].tex_coord[1]= tc[j*2+1];

           v[0].normal[0]= v[0].normal[1]= 0;
           v[0].normal[2]= 127;

        }
    }
    
    unsigned short* ind= m->indeces;
    for( unsigned int i= 0, j=0; i< segments * 6; i+= 6, j+=4 )
    {
        ind[i]= j;
        ind[i+1]= j+1;
        ind[i+2]= j+2;

        ind[i+3]= j;
        ind[i+4]= j+3;
        ind[i+5]= j+2;
    }
    
    m->SetTexture( TEXTURE_GREENERY );
}

void GenTree( Model* m )
{
    TreeBranch branches[1024];

    GenTree_r( TREE_DEPTH, TREE_SIZE, branches, NULL, 0.2f*TREE_SIZE );

    GenCylinder(m, 3 );
   // static const short scale_vec0[]= { 96, 96, short(TREE_SIZE*256.0f) };
   // m->Scale( scale_vec0 );
    m->vertex_count= m->index_count= NULL;


    float n_z= TREE_WIDTH;
    float n_xy= TREE_SIZE;
    float n_size= sqrt( n_z*n_z + n_xy*n_xy );
    n_z/= n_size;
    n_xy/= n_size;

    Model m2;
    Model m3;
    
    float mat[16];
    for( unsigned int i= 0; i< branch_pull_shift; i++ )
    {
        short segments= max( short( 10.0f * sqrt( branches[i].size / TREE_SIZE ) ), 3 );
        GenCone( &m2, segments );
        short width= short( TREE_WIDTH * 256.0f * branches[i].size / TREE_SIZE );
        short scale_vec1[]={ width, width, short(256.0f * branches[i].size ) };
        m2.Scale( scale_vec1 );
        
        branches[i].GenTransformMatrix( mat );
        m2.Transform( mat );
        m2.SetTexture( TEXTURE_WOOD );

        if( branches[i].depth == 0 )
        {
            GenTreeGreenery( &m3, branches[i].size );
            m3.Transform( mat );
        }

        Mat4Identity( mat );
        mat[0]= mat[5]= n_xy;
        mat[10]= n_z;

        float mat2[16];
        branches[i].GenNormalMatrix( mat2 );
        Mat4Mul( mat, mat2 );
        m2.TransformNormals( mat );

        if( branches[i].depth == 0 )
        {
            m3.TransformNormals( mat );
            m->AddModel( &m3 );
        }

        m->AddModel(&m2);
        
    }

   // m->SetTexture( TEXTURE_WOOD );
}

void GenTree0( Model* m )
{
    branch_pull_shift= 0;
    GenTree( m );
}
void GenTree1( Model* m )
{
    branch_pull_shift= 0;
    GenTree( m );
}

void GenBush( Model* m )
{
    float mat[16], mat_n[16], mat_r[16], mat_rz[16], mat_t[16], mat_s[16], ang_xy, ang_z, pos[3], size;

    GenCone( m, 3 );
    m->index_count= m->vertex_count= 0;

    Model m2, m3;
    for( unsigned int i= 0; i< 4; i++ )
    {
        pos[0]= randf( -1.0f, 1.0f );
        pos[1]= randf( -1.0f, 1.0f );
        pos[2]= 0.3f;
        size= randf( 1.7f, 2.2f );
        ang_xy= randf( MW_2PI );
        ang_z= randf( MW_PI4 );

        GenCone( &m2, 8 );
        static const short scale_vec[]= { 16, 16, 256 };
        m2.Scale( scale_vec );
      //  Mat4Identity( mat );
        
        GenTreeGreenery( &m3, size * 0.4f );
       
        Mat4Identity( mat_rz );
        Mat4Identity( mat_t );
        Mat4Identity( mat_s );
        Mat4RotateXY( mat_r, ang_xy );
        mat_rz[5]= cos( ang_z );
        mat_rz[6]= sin( ang_z );
        mat_rz[9]= - mat_rz[6];
        mat_rz[10]= mat_rz[5];
        mat_s[0]= mat_s[5]= mat_s[10]= size;
        mat_t[12]= pos[0];
        mat_t[13]= pos[1];
        mat_t[14]= pos[2];

        Mat4Mul( mat_rz, mat_r, mat );
        m2.TransformNormals( mat );
        m3.TransformNormals( mat );

        Mat4Mul( mat, mat_s );
        Mat4Mul( mat, mat_t );

        m2.Transform( mat );
        m3.Transform( mat );

        m2.SetTexture( TEXTURE_TREE_BARK );

        m->AddModel( &m2 );
        m->AddModel( &m3 );


    }
}

void (*static_objects_generation_func[])( Model* m )= { GenSpruce, GenTree0, GenTree1, GenBush, GenAchtungTable,
GenSmallHealthPack, GenMediumHealthPack, GenLargeHealthPack,
GenBulletsBox, GenChainBulletsAmmoBox, GenRocketAmmoBox, GenGaussBolts, 
GenShovel, GenPistol, /*GenMachineGun*/GenSuperMachineGun, GenRocketLauncher, GenGaussGun, 

GenRocket, GenSnowBall, GenFireBall,//not static objects - bullets
GenSpruceLod//hack for spruce - lod
};
