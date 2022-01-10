#shader vertex
#version 460

out vec2 tex_coord;

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_tex_coord;

uniform mat4 player_mvp;
void main()
{
  tex_coord = vertex_tex_coord;
  gl_Position = player_mvp * vec4(vertex_position, 1.0); // clip-space output position of the current vertex
}

#shader fragment
#version 460

in vec2 tex_coord;

layout( location = 0 ) out vec4 player_frag_color;
layout( binding = 0 ) uniform sampler2DArray player_tex; // texture sampler object, requires #version 420

uniform int player_frame_ID;
void main()
{
  player_frag_color = texture(player_tex, vec3(tex_coord, player_frame_ID));
}
#end