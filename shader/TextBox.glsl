#shader vertex
#version 440

layout (location = 0)
in vec4 vbuf_data;

out vec2 tex_coord;

uniform mat2 scale = mat2(2, 0, 0, .8);
uniform vec2 shift = vec2(0, -.6);


void main()
{
  vec2 position = vbuf_data.xy;
  tex_coord = vbuf_data.zw;
  vec2 adj = scale * position + shift;
  gl_Position = vec4(adj.xy, 0, 1);
}



#shader fragment
#version 440

in vec2 tex_coord;

layout (location= 0)
out vec4 textbox_frag_color;

layout (binding= 0)
uniform sampler2D textbox_texture;


void main()
{
  textbox_frag_color = texture( textbox_texture, tex_coord );
}
#end