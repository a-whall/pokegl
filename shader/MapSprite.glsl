#shader vertex
#version 460

// per-vertex 2D position.
layout (location= 2)
in vec2 vpos;

// per-instance 2D scale and 2D shift, packed into a vec4.
layout (location= 3)
in vec4 ibuf;

// active multi-texture unit from which to sample.
flat
out int tex_unit;

// texture coordinates with which to sample.
out vec2 tex_coord;

// used to keep up with camera movement.
uniform vec2 cam_offset;


void main()
{
  vec2 s = ibuf.xy;
  vec2 t = ibuf.zw;
  vec2 pos = vec2( vpos.x * s.x /*- .25*/ , vpos.y * s.y /*- .25*/);
  pos.x -= cam_offset.x / 5;
  pos.y -= cam_offset.y / 5;
  tex_coord = vpos;
  if (tex_coord.y == 1) tex_coord.y = 0;
  else
  if (tex_coord.y == 0) tex_coord.y = 1;
  tex_unit = gl_InstanceID + 1;
  gl_Position = vec4(pos, 0.97, 1);
}



#shader fragment
#version 460

flat
in int tex_unit;
in vec2 tex_coord;

// pixel color.
layout (location= 0)
out vec4 map_frag_color;

// layout (binding=x) uniform sampler2D t; is the equivalent of getting the uniform location for "t" and setting its uniform value to "x". GLSL 4.2 required.
layout (binding= 1) uniform sampler2D t1;
layout (binding= 2) uniform sampler2D t2;
layout (binding= 3) uniform sampler2D t3;
layout (binding= 4) uniform sampler2D t4;
layout (binding= 5) uniform sampler2D t5;


void main()
{
  vec3 sampled_RGB;

  switch(tex_unit) {
    case 1: sampled_RGB= texture(t1, tex_coord).rgb; break;
    case 2: sampled_RGB= texture(t2, tex_coord).rgb; break;
    case 3: sampled_RGB= texture(t3, tex_coord).rgb; break;
    case 4: sampled_RGB= texture(t4, tex_coord).rgb; break;
    case 5: sampled_RGB= texture(t5, tex_coord).rgb; break;
    default:sampled_RGB= vec3(1); // blank white texture
  }

  map_frag_color= vec4(sampled_RGB, 1);
}
#end