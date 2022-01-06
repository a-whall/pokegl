#shader vertex
#version 460

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_tex_coord;
layout (std430, binding = 0) buffer char_buffer {
  int char_at[];
};

out vec3 position;
out vec2 tex_coord;
flat out int char_data;

uniform mat4 mvp;
uniform float left_margin= 0.001;
uniform float text_size= 0.12;
uniform float char_spacing= -0.01;
uniform float line_spacing= 0.1;

void main()
{
  tex_coord = vertex_tex_coord;
  char_data = gl_InstanceID + 7;
  int col = gl_InstanceID % 18;
  int row = gl_InstanceID / 18;
  float x = -1.0 + left_margin + vertex_position.x * text_size + col * (text_size + char_spacing);
  float y = vertex_position.y * text_size - (0.5 + row * (text_size + line_spacing));
  float z = vertex_position.z;
  gl_Position = vec4(x, y, z, 1.0);
}

#shader fragment
#version 460

in vec3 position;
in vec2 tex_coord;
flat in int char_data;

layout( location = 0 ) out vec4 frag_color;

layout( binding = 0 ) uniform sampler2DArray char_dict; // The texture sampler object, requires > #version 420

uniform int index = 8;

void main()
{
  frag_color = texture(char_dict, vec3(tex_coord, char_data));
}
#end