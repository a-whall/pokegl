#shader vertex
#version 460

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_tex_coord;
layout (std430, binding = 0) buffer char_buffer {
  int char_at[];
};

out vec2 tex_coord;     // interpolated by fragment
flat out int char_data; // uninterpolated data exclusive to this batch of vertices and uniform to any fragments that get the output of those vertices

uniform float left_margin= 0.06;   // starting distance from the left edge of the window
uniform float text_size= 0.14;     // character scaling
uniform float char_spacing= -0.01; // width between textures in the x
uniform float line_spacing= 0.1;   // width between textures in the y

void main()
{
  int col = 0, row = 0; // position of this character from the start of the text (-1.0 + left margin, -0.55f)

  tex_coord = vertex_tex_coord;       // texture coordinate to be interpolated by fragment processing
  char_data = char_at[gl_InstanceID]; // this shader is meant to print the int stored in this buffer as a character

  if (char_data == 94)
    char_data= 0; // change this chars data to 0 to print space as there is no character texture for newline.

  // count new lines in the buffer up to this characters index
  // the new line character code is 94 which is 1 more than the last ascii character '~'
  for (int i = 0; i < gl_InstanceID; ++i)
  {
    if (char_at[i] != 94)
      ++col;
    else {
      col= 0;
      ++row;
    }
  }

  // compute the position of a vertex of the quad which will host a single character of text from the char buffer
  float x = -1.0 + left_margin + vertex_position.x * text_size + col * (text_size + char_spacing);
  float y = vertex_position.y * text_size - (0.55 + row * (text_size + line_spacing));
  float z = vertex_position.z;

  // pass the position to opengl to perform clipping with
  gl_Position = vec4(x, y, z, 1.0);
}
#shader fragment
#version 460

in vec2 tex_coord;
flat in int char_data; // char data is the index into the texture array. ordered as ascii starting at space = 0

layout( location = 0 ) out vec4 text_frag_color;
layout( binding = 0 ) uniform sampler2DArray char_dict; // The texture sampler object, requires > #version 420

void main()
{
  text_frag_color = texture(char_dict, vec3(tex_coord, char_data));
}
#end