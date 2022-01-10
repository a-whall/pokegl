#shader vertex
#version 460

uniform mat4 mapsprite_mvp;

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_texcoord;

out vec2 tex_coord; // out variables that are not flat are automatically interpolated between nearby verts

void main()
{
  tex_coord = vertex_texcoord;
  gl_Position = mapsprite_mvp * vec4(vertex_position, 1.0);
}
#shader fragment
#version 460

in vec2 tex_coord;
layout( location = 0 ) out vec4 map_frag_color;
layout( binding = 0 ) uniform sampler2D map_tex; // the texture sampler object, requires #version 420

void main()
{
  map_frag_color = vec4( texture(map_tex,tex_coord).rgb, 1.0 );
}
#end