#shader vertex
#version 460

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_texcoord;

out vec2 tex_coord;

uniform mat4 transform_to_menu_position;

void main()
{
  tex_coord = vertex_texcoord;
  gl_Position = vec4(vertex_position, 1.0);
}

#shader fragment
#version 460

in vec2 tex_coord;

layout( location = 0 ) out vec4 textbox_frag_color;

layout( binding = 0 ) uniform sampler2D textbox_texture;

void main()
{
  textbox_frag_color = texture(textbox_texture,tex_coord);
}
#end