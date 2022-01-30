#shader vertex
#version 460

layout (location = 0)
in vec4 vbuf_data;

out vec2 tex_coord;

uniform mat4 player_mvp;

void main()
{
  vec2 pos = vbuf_data.xy;
  tex_coord = vbuf_data.zw;
  gl_Position = player_mvp * vec4(pos, 0, 1); // NDC output position of the current vertex
}



#shader fragment
#version 460

in vec2 tex_coord;

layout (location = 0)
out vec4 player_frag_color;

layout (binding = 0)
uniform sampler2DArray player_tex; // texture sampler object, requires #version 420

uniform int player_frame_ID = 2; // 2 corresponds to the frame idle_d

void main()
{
  player_frag_color = texture( player_tex, vec3(tex_coord, player_frame_ID) );
}
#end