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
"uniform sampler2D sm;"//shadow map
"uniform vec3 ps;"//position shift
"uniform mat4 m;"
"uniform float ls= 1.0/1024.0;"//landscape size
"uniform float a;"//landscape ampitude
"in vec2 p;"
"out vec3 n;"//normal
"out vec2 fp;"
"out float fs;"//fragment shadow
"void main(void)"
"{"
"vec2 tc= (p+ps.xy)*ls;"
"fp= tc*256.0;"
"float fc=texelFetch(hm,ivec2(ps.xy+p),0).x;"
"fs=texelFetch(sm,ivec2(ps.xy+p),0).x;"
//"float fc=textureLod(hm,tc,0.0).x;"
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
"in float fs;"//shadow 
"out vec4 fc;"
"uniform vec3 sun;"
"void main(void)"
"{"
"float l=max(fs*dot(sun,normalize(n)),0.2);"
"float h=texture(tm,fp/256.0f).x*255.0;"
"fc=l*texture(tex,vec3(fp,h));"
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
"vec4 c2=texture(tex,ftc);"
"if(c2.a<0.5)discard;"
"vec3 n=normalize(fn);"
"float l=max(0.2,dot(sun,n));"
"c=c2*l;"
//"c=vec4(n,1.0);"
"}";


const char* weapon_shader_v= 
"#version 330\n"
"in vec3 v;"//position
"in vec3 tc;"//texture cooord
"in vec3 n;"//normal
"in int b;"//bone id
"uniform mat3 nm[8];"//normal transform matices
"uniform mat4 vm[8];"//vertex transform matrix - to global space
"uniform mat4 ivm;"//inverse view matirx
"uniform mat4 pm;"//projection matrix
"uniform vec3 cam;"
"out vec3 fn;"//out normal
"out vec3 ftc;"//out texture coord
"out vec3 fv;"//camera position - gloabal vertex position
"void main(void)"
"{"
"ftc=tc*vec3(0.000244140625,0.000244140625,1.0);"
"fn=nm[b]*n;"
"vec4 v2=vec4(v*0.00390625,1.0);"
"vec4 v3=vm[b]*v2;"
"gl_Position=pm*v3;"
"fv=cam-(ivm*v3).xyz;"
"}";

const char* weapon_shader_f=
"#version 330\n"
"in vec3 fn;"
"in vec3 ftc;"
"in vec3 fv;"
"uniform sampler2DArray tex;"
"out vec4 c;"
"uniform vec3 sun;"
"void main(void)"
"{"
"vec4 c2= texture(tex,ftc);"
"vec3 n= normalize(fn);"
"float l=max(0.2,dot(sun,n));"
"float s= max(dot(reflect(normalize(fv),n),normalize(sun)),0.01);"
"l+=step(0.21,l)*step(7.5,ftc.z)*step(ftc.z,9.5)*pow(s,6.0)*0.5;"
"c=c2*l;"
"}";




const char* item_glow_shader_v=
"#version 330\n"
"in vec4 v;"//position
"uniform mat4 m;"//transform matrix
"void main(void)"
"{"
"vec4 p2=m*vec4(v.xyz,1.0);"
"gl_PointSize=v.w/p2.w;"
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
"out vec2 tc;"
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
"uniform vec3 sun;"
"uniform sampler2D tex;"
"uniform vec3 cs;"//cloud shift
"void main(void)"
"{"
"vec3 n=normalize(fv);"
"float l=(3.0-n.z)*0.4;"
"l+=0.6*pow(max(dot(n,sun),0.1),32.0);"
"c=mix(vec4(0.019,0.654,0.811,0.0)*l,vec4(1.0,1.0,0.8,0.0),step(1.58,l));"
"n/=abs(n.z)+0.02;"
"c=mix(c,vec4(1.0,1.0,1.0,1.0),texture(tex,n.xy*0.25+cs.xy).x);"
"}";


const char* particle_shader_v=
"#version 330\n"
"in vec4 v;"//position
"uniform mat4 m;"//transform matrix
"out float t;"//texture id
"void main(void)"
"{"
"t=mod(v.w,16.0);"
"vec4 p2=m*vec4(v.xyz,1.0);"
"gl_PointSize=(v.w-t)*0.0625/p2.w;"
"gl_Position=p2;"
"}";

const char* particle_shader_f=
"#version 330\n"
"out vec4 c;"
"uniform sampler2DArray tex;"
"in float t;"//texture id
"void main(void)"
"{"
"vec4 c2=texture(tex,vec3(gl_PointCoord,t));"
"if(c2.a<0.01)discard;"
"c=c2;"
"}";


const char* water_shader_v=
"#version 330\n"
"in vec2 v;"
"uniform float h;"
"uniform mat4 m;"
"uniform vec3 cam;"
"out vec3 tcv;"//vector to camera
"out vec2 tc;"
"void main(void)"
"{"
"tc=v*0.06125;"
"tcv=cam-vec3(v,h);"
"gl_Position=m*vec4(v,h,1.0);"
"}";

const char* water_shader_f=
"#version 330\n"
"in vec3 tcv;"
"in vec2 tc;"
"out vec4 c;"
"uniform vec3 sun;"
"uniform sampler2D tex;"
"uniform float t;"
"void main(void)"
"{"
"vec3 n;"
"vec2 mtc=vec2(tc.x + 0.06125*sin(8.0*tc.y+t), tc.y+0.06125*sin(8.0*tc.x+t));"
//"n.x=sin(3.14*textureLod(tex,mtc+vec2(1.0/256.0,0.0),2.0).x+t)-sin(3.14*textureLod(tex,mtc+vec2(-1.0/256.0,0.0),2.0).x+t);"
//"n.y=sin(3.14*textureLod(tex,mtc+vec2(0.0,1.0/256.0),2.0).x+t)-sin(3.14*textureLod(tex,mtc+vec2(0.0,-1.0/256.0),2.0).x+t);"
//"n.x=texture(tex,mtc+vec2(1.0/256.0,0.0)).x-texture(tex,mtc+vec2(-1.0/256.0,0.0)).x;"
//"n.y=texture(tex,mtc+vec2(0.0,1.0/256.0)).x-texture(tex,mtc+vec2(0.0,-1.0/256.0)).x;"
"n=texture(tex,mtc).yzw-vec3(0.5,0.5,0.0);"
"n=normalize(n);"
"vec3 tcvn=normalize(tcv);"
"float a=1.0-abs(dot(tcvn,n));"
"a=a*a*a;"
"float l= 0.8*pow( max( 0.1, dot( tcvn, -reflect(sun,n)) ), 64.0 );"
"c=vec4(vec3(0.09,0.327,0.405)+vec3(l,l,l),a);"
"}";


/*const char* water_shader_f=
"#version 330\n"
"in vec3 tcv;"
"in vec2 tc;"
"out vec4 c;"
"uniform vec3 sun;"
"uniform sampler2D tex;"
"uniform float t;"
"void main(void)"
"{"
"vec3 n=vec3(0.0,0.0,1.0);"
"vec2 mtc=vec2(tc.x + 0.125*sin(2.0*tc.y+t), tc.y+0.125*sin(2.0*tc.x+t));"
"n=normalize(n);"
"vec3 tcvn=normalize(tcv);"
"float a=1.0-abs(dot(tcvn,n));"
"a=a*a;"
"float l= 0.8*pow( max( 0.1, dot( tcvn, -reflect(sun,n)) ), 64.0 );"
"c=vec4(texture(tex,mtc).xyz+vec3(l,l,l),a);"
"}";*/

const char* blood_shader_v=
"#version 330\n"
"const vec2 v[6]=vec2[6](vec2(-1.0,-1.0),vec2(1.0f,-1.0),vec2(1.0,1.0),vec2(1.0,1.0),vec2(-1.0,1.0),vec2(-1.0,-1.0));"
"out vec2 ftc;"
"void main(void)"
"{"
"gl_Position=vec4(ftc=v[gl_VertexID],-0.0,1.0);"
"}";

const char* blood_shader_f=
"#version 330\n"
"out vec4 c;"
"in vec2 ftc;"
"uniform sampler2D tex;"//blood texture
"uniform float bf;"//blood factor
"void main(void)"
"{"
"float a= dot(ftc,ftc)*0.5+0.1;"
"c=vec4(1.0,0.1,0.1,a*bf*texture(tex,ftc*0.5+vec2(0.5,0.5)).x);"
"}";