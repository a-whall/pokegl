#shader vertex
#version 440

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 vertex_texcoord;

out vec2 tex_coord;

uniform mat2 adj= mat2(2, 0,  0, .8);

void main()
{
  tex_coord= vertex_texcoord;
  vec2 adj_pos= adj * vertex_position;
  adj_pos -= vec2(0, .6);
  gl_Position= vec4(adj_pos, 0, 1);
}
#shader fragment
#version 440

in vec2 tex_coord;

layout( location = 0 ) out vec4 textbox_frag_color;

layout( binding = 0 ) uniform sampler2D textbox_texture;

void main()
{
  textbox_frag_color = texture( textbox_texture, tex_coord );
}
#end