//text shader
const char* text_shader_v=
"#version 330\n"
"in vec2 v;"//in position
"in vec2 tc;"//in texture coord
"in vec4 c;"//in color
"out vec4 vc;"//out color
"out vec2 vtc;"//out texture coord
"void main(void)"
"{"
"vc=c;"
"vtc=tc*vec2(1.0/96.0,1.0);"
"gl_Position=vec4(v,0.0,1.0);"
"}";

const char* text_shader_f=
"#version 330\n"
"in vec4 vc;"
"in vec2 vtc;"
"out vec4 oc;"
"uniform sampler2D tex;"
"void main(void)"
"{"
"float c=texture2D(tex,vtc).x;"
"oc=vec4(vc.xyz*c,max(vc.a,c));"
"}";




const char* landscape_shader_v=
"#version 330\n"
"uniform sampler2D hm;"
"uniform vec3 ps;"//position shift
"uniform mat4 m;"
"uniform float ls= 1.0/1024.0;"//landscape size
"uniform float a;"//landscape ampitude
"in vec2 p;"
"out vec3 n;"//normal
"out vec2 fp;"
"void main(void)"
"{"
"vec2 tc= (p+ps.xy)*ls;"
"fp= tc*256.0;"
//"float fc= texelFetch( hm, ivec2(ps.xy+p),0 ).x;"
"float fc=textureLod(hm,tc,0.0).x;"
"vec3 nv;"
"nv.x=textureLod(hm,tc+vec2(-ls,0.0),1.0).x-textureLod( hm,tc+vec2(ls,0.0),1.0).x;"
"nv.y=textureLod(hm,tc+vec2(0.0,-ls),1.0).x-textureLod( hm,tc+vec2(0.0,ls),1.0).x;"
"nv.z= 1.0;"
"nv.xy*=a;" 
"n= nv;"
"vec4 pos=vec4(p+ps.xy,a*fc,1.0);"
"gl_Position=m*pos;"
"}"
;

const char* landscape_shader_f=
"#version 330\n"
"in vec3 n;"
"uniform sampler2DArray tex;"//texture
"uniform sampler2D tm;"//texture map
"in vec2 fp;"
"out vec4 fc;"
"uniform vec3 sun;"
"void main(void)"
"{"
"float l= max( dot(sun,normalize(n)),0.2);"
"float h= texture(tm,fp/256.0f).x*255.0;"
"fc= l*texture(tex,vec3(fp,h));"
"}";


const char* model_shader_v= 
"#version 330\n"
"in vec3 v;"//position
"in vec3 tc;"//texture cooord
"in vec3 n;"//normal
"uniform mat4 m;"//transform matrix
"out vec3 fn;"//out normal
"out vec3 ftc;"//out texture coord
"void main(void)"
"{"
"ftc=tc*vec3(0.000244140625,0.000244140625,1.0);"
"fn=n;"
"gl_Position=m*vec4(v*0.00390625,1.0);"
"}";


const char* model_animation_shader_v= 
"#version 330\n"
"in vec3 v;"//position
"in vec3 tc;"//texture cooord
"in vec3 n;"//normal
"in int b;"//bone id
//"uniform mat4 m;"//transform matrix
"uniform mat3 nm[8];"//normal transform matices
"uniform mat4 vm[8];"//vertex transform matrix
"out vec3 fn;"//out normal
"out vec3 ftc;"//out texture coord
"void main(void)"
"{"
"ftc=tc*vec3(0.000244140625,0.000244140625,1.0);"
"fn=nm[b]*n;"
"vec4 v2=vec4(v*0.00390625,1.0);"
"gl_Position=vm[b]*v2;"
"}";

const char* model_shader_f=
"#version 330\n"
"in vec3 fn;"
"in vec3 ftc;"
"uniform sampler2DArray tex;"
"out vec4 c;"//out color
"uniform vec3 sun;"
"void main(void)"
"{"
"vec4 c2= texture(tex,ftc);"
"if(c2.a<0.5)discard;"
"c=c2 * max(0.2,dot(sun,normalize(fn)));"
//"c= vec4(fn,1.0);"
"}";


const char* item_glow_shader_v=
"#version 330\n"
"in vec4 v;"//position
"uniform mat4 m;"//transform matrix
"void main(void)"
"{"
"vec4 p2=m*vec4(v.xyz,1.0);"
"gl_PointSize= v.w/p2.w;"
"gl_Position=p2;"
"}";

const char* item_glow_shader_f=
"#version 330\n"
"out vec4 c;"
"uniform sampler2D tex;"
"void main(void)"
"{"
"c=texture(tex,gl_PointCoord);"
"}";

const char* sky_shader_v=
"#version 330\n"
"in vec3 v;"
"out vec3 fv;"
"uniform mat4 m;"
"void main(void)"
"{"
"fv=v;"
"gl_Position=m*vec4(v*10,1.0);"
"}";

const char* sky_shader_f=
"#version 330\n"
"out vec4 c;"
"in vec3 fv;"
"void main(void)"
"{"
//"float z=(3.0-normalize(fv).z)*0.5;"
"float z=(3.0-normalize(fv).z)*0.4;"
"c=vec4(0.019,0.654,0.811,0.0)*z;"
"}";