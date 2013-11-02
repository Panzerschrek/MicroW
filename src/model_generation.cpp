#include "model.h"
#include "math.h"
#include "texture_generation.h"
#include "model_generation.h"

static unsigned int SEGMENTS= 24;//global value for cylinder, cone, sphere generation
static unsigned int PARTITION=8;

void GenCylinder(Model* m )
{
    if( m->vertices == NULL )
    {
        m->indeces= new unsigned short[ SEGMENTS * 6 ];
        m->vertices= new ModelVertex[ SEGMENTS * 2 + 2 ];
    }
     unsigned short* ind= m->indeces;
    ModelVertex* v= m->vertices;

    float x, y;
    float a= 0;
    float da= MW_2PI  / float( SEGMENTS );
    short t= 0;
    
    unsigned int s= SEGMENTS*2;
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

    m->vertex_count= SEGMENTS * 2 + 2;
    m->index_count= SEGMENTS * 6;
    m->primitive_type= GL_TRIANGLES;
}


void GenCone( Model* m )
{
    if( m->vertices == NULL )
    {
        m->indeces= new unsigned short[ SEGMENTS * 3 ];
        m->vertices= new ModelVertex[ SEGMENTS * 2 + 2 ];
    }
    unsigned short* ind= m->indeces;
    ModelVertex* v= m->vertices;


    float x, y;
    float a= 0;
    float da= MW_2PI  / float( SEGMENTS );

    
    unsigned int s= SEGMENTS*2;
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

    for( unsigned int i= 0, j=0; i< SEGMENTS * 3; i+=3, j+= 2 )
    {
        ind[i]= j;
        ind[i+1]= j+1;
        ind[i+2]= j+2;
    }

     m->vertex_count= SEGMENTS * 2 + 2;
    m->index_count= SEGMENTS * 3;
    m->primitive_type= GL_TRIANGLES;

}


void GenSphere( Model* m )
{
    
    if( m->vertices == NULL )
    {
        m->indeces= new unsigned short[ SEGMENTS * PARTITION * 6 ];
        m->vertices= new ModelVertex[ (SEGMENTS + 1 ) * (PARTITION+1) ];
    }
    unsigned short* ind= m->indeces;
    ModelVertex* v= m->vertices;

    float vec[3];
    float xy, z;
    float a, da= MW_2PI / float( SEGMENTS );
    float phi= -MW_PI2, d_phi= MW_PI / float(PARTITION);
    unsigned int i, j, k;
    for( j= 0, k=0; j<= PARTITION; j++, phi+= d_phi )
    {
        xy= cos(phi) * 256.0f;
        z= sin(phi) * 256.0f;
        for( i= 0, a= 0.0f; i<= SEGMENTS; i++, a+=da, k++ )
        {
            vec[0]= xy * cos(a);
            vec[1]= xy * sin(a);
          
            v[k].pos[0]= (short)(vec[0]);
            v[k].pos[1]= (short)(vec[1]);
            v[k].pos[2]= (short)(z);
            v[k].normal[0]= (v[k].pos[0]-1)/2;
            v[k].normal[1]= (v[k].pos[1]-1)/2;
            v[k].normal[2]= (v[k].pos[2]-1)/2;

            v[k].tex_coord[0]= 4096 * i / SEGMENTS;
            v[k].tex_coord[1]= 4096 * j / PARTITION;
            //v[k].tex_coord[2]= TEXTURE_GRASS;

        }
    }

     for( j= 0, k=0; j< PARTITION; j++  )
        for( i= 0, a= 0.0f; i< SEGMENTS; i++, a+=da, k+=6 )
        {
            ind[k]= i + j * (SEGMENTS + 1 );
            ind[k+1]= ind[k] + SEGMENTS + 1;
            ind[k+2]= ind[k+1] + 1;

            ind[k+3]=ind[k];
            ind[k+4]= ind[k+2];
            ind[k+5]= ind[k]+1;
        }
    
    m->vertex_count= (SEGMENTS + 1) * (PARTITION+1);
    m->index_count= SEGMENTS * PARTITION * 6;
    m->primitive_type= GL_TRIANGLES;
}

void GenTree(Model* m )
{
  
    SEGMENTS = 12;
    const unsigned int branches= 16;
    m->vertices= new ModelVertex[ SEGMENTS * 2 + 2 +   branches * 4 * 4 ];
    m->indeces= new unsigned short[ SEGMENTS * 6 +  branches * 8 * 3 ];
    GenCylinder(m);
    m->SetTexture(TEXTURE_TREE_BARK);
    static const short tex_scale[]= { 4096/*2^12*/, 32767 };
    m->ScaleTexture(tex_scale);
    
    short vec[]= { 0, 0, 256 };
    vec[0]= 1<<8;
    vec[1]= 1<<8;
    vec[2]= 19<<8;//16 metters
    m->Scale( vec );

    for( unsigned int i= 0; i<= SEGMENTS; i++ )
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

    ModelVertex* v= m->vertices + SEGMENTS * 2 + 2;
    unsigned short* ind= m->indeces + SEGMENTS * 6;
    unsigned short v_shift= SEGMENTS * 2 + 2;

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
            for( unsigned int k= 0; k< 4; k++ )//branch vertices
            {
                Vec3Mat4Mul( branch_v + k*3, m_s, transform_v  );
                final_v.pos[0]= short( transform_v[0] * 256.0f );
                final_v.pos[1]= short( transform_v[1] * 256.0f );
                final_v.pos[2]= short( transform_v[2] * 256.0f );
                final_v.tex_coord[0]= branch_tc[k*2];
                final_v.tex_coord[1]= branch_tc[k*2+1];
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


                                                       
    m->vertex_count= SEGMENTS * 2 + 2 +   branches * 4 * 4;
    m->index_count= SEGMENTS * 6 +  branches * 8 * 3;

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

    SEGMENTS= 12;

    m->vertices= new ModelVertex[ SEGMENTS * 2 + 2 + 6 ];
    m->indeces= new unsigned short[ SEGMENTS * 6 + 3*4 ];
    GenCylinder(m);
    m->vertex_count= SEGMENTS * 2 + 2 + 6 ;//1 cylinder (n*2+2) + shovell model
    m->index_count= SEGMENTS * 6 + 3 * 4;

    short vec[]= { 8, 8, 192 };
    m->Scale( vec );
    vec[0]= 0;
    vec[1]= 0;
    vec[2]= -256;
   // m->Move(vec);
    m->SetTexture( TEXTURE_TREE_BARK );


    ModelVertex* v= m->vertices;
    for( unsigned int i= SEGMENTS * 2 + 2, j= 0; j < 6; i++, j++ )
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
    unsigned int v0= SEGMENTS * 2 + 2;
    for( unsigned int i= SEGMENTS * 6, j= SEGMENTS * 2 + 2 + 1;
        i< SEGMENTS * 6 + 3 * 4;
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
    SEGMENTS= 16;
    PARTITION= 8;
    GenSphere(m);
    m->SetTexture( TEXTURE_CACODEMON_SKIN );


    Model cone;
    SEGMENTS= 10;
    GenCone( &cone );
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
    m_r[6]= sin( - MW_PI6 );
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
    SEGMENTS= 12;
    GenCylinder(m);

    const static short scale_vec0[]= { 16, 16, 96 };
    m->Scale( scale_vec0 );
    m->SetTexture( TEXTURE_METAL );

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
    m2.SetTexture( TEXTURE_TREE_BARK );

    m->AddModel( &m2 );


    GenCube( &m2 );
    m2.Scale( 18 );
    m2.SetTexture( TEXTURE_METAL );
    
    m->AddModel(&m2);

    Mat4Identity(mat);
    mat[5]= 0.0f;
    mat[6]= -1.0f;
    mat[9]= 1.0f;
    mat[10]= 0.0f;
    m->Transform(mat);
    m->TransformNormals(mat);

    m->SetBone(0);
}

void GenMachineGun(Model* m)
{
    SEGMENTS= 12;
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
    m2.SetTexture( TEXTURE_TREE_BARK );
    m->AddModel(&m2);

    Mat4Identity(mat);
    mat[5]= 0.0f;
    mat[6]= -1.0f;
    mat[9]= 1.0f;
    mat[10]= 0.0f;
    m->Transform(mat);
    m->TransformNormals(mat);

    m->SetBone(0);
}

void GenSmallHealthPack( Model* m )
{
    SEGMENTS= 12;
    PARTITION= 7;

   GenSphere(m);

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
    SEGMENTS= 14;
    PARTITION= 8;

    GenSphere(m);
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
    SEGMENTS= 12;
    PARTITION= 7;
    GenSphere(m);
    m->Scale( 33 );//make head size 67
    static const short head_move[]={ 0, 0, 412 };
    m->Move( head_move );
    m->SetBone(0);

     //legs
    Model m2;
    SEGMENTS= 24;
    GenCylinder(&m2);
    SEGMENTS= 10;
    GenCylinder(&m2);
    static const short leg_scale[]= { 24, 24, 190 };
    m2.Scale(leg_scale);
    static const short leg_move0[]= { -32, 0, 0 };
    m2.Move( leg_move0 );
    m2.SetBone(3);
    m->AddModel(&m2);
    static const short leg_move1[]= { 64, 0, 0 };
    m2.Move( leg_move1 );
    m2.SetBone(4);
    m->AddModel(&m2);

     //hands
    GenCylinder(&m2);
    static const short hand_scale[]= { 20, 20, 210 };
    m2.Scale( hand_scale );
    static const short hand_move0[]= { 86, 0, 170 };
    m2.Move( hand_move0 );
    m2.SetBone(1);
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
    m->AddModel(&m2);

    //body
    GenCube(&m2);
    static const short body_scale[]= { 64, 34, 95 };
    m2.Scale( body_scale );
    static const short body_move[]= { 0, 0, 284 };
    m2.Move( body_move );
    m2.SetBone(0);
    m->AddModel(&m2);


    m->SetTexture( TEXTURE_METAL );
    GenRobotAnimations(m);
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
    tr_mat0[14]= -1.4453125f;//370/256
    tr_mat1[14]= 1.4453125f;

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


    tr_mat0[14]= -0.703125f;//180/256
    tr_mat1[14]= 0.703125f;

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

    #define FRAME_COUNT 60
    m->is_animation[ANIMATION_ATTACK1]= true;
    anim= & (m->animations[ANIMATION_ATTACK1]);
    anim->duration= 2.0f;
    anim->bone_count= 1;

    anim->frames= FRAME_COUNT;
    anim->v_matrices= new float[ 16 * 1 * FRAME_COUNT ];//4x4matrix for FRAME_COUNT frames of 1 bone
    anim->n_matrices= new float[ 9 * 1 * FRAME_COUNT ];//3x3matrix for FRAME_COUNT frames of 1 bone

      for( unsigned int i= 0; i< MAX_MODEL_BONES; i++ )
        anim->bones[i]= false;
      anim->bones[1]= true;

    tr_mat0[14]= -1.4453125f;//370/256
    tr_mat1[14]= 1.4453125f;

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
}