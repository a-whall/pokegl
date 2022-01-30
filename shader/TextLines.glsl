#shader vertex
#version 460

layout (location = 0) in vec2 vert_pos;
layout (std430, binding = 0) buffer char_buffer {
  int char_at[];
};

out vec2 tex_coord;
flat out int char_data;

uniform int baseID         = 0;
uniform float top_margin   = +1.55;
uniform float left_margin  = +0.06;
uniform float text_size    = +0.13;
uniform float char_spacing = -0.01;
uniform float line_spacing = +0.10;

void main()
{
  int col = 0, row = 0;

  tex_coord = vert_pos;
  // switch the tex coord y position, this allows us to only store position in the buffer
  if (tex_coord.y == 1) tex_coord.y = 0;
  else
  if (tex_coord.y == 0) tex_coord.y = 1;

  char_data = char_at[baseID + gl_InstanceID];

  // 94 -> newline, 0 -> space
  if (char_data == 94)
    char_data= 0;

  for (int i = baseID; i < gl_InstanceID + baseID; ++i)
  {
    if (char_at[i] != 94)
      ++col;
    else {
      col= 0;
      ++row;
    }
  }

  float x = -1 + left_margin + vert_pos.x * text_size + col * (text_size + char_spacing);
  float y = +1 - top_margin + vert_pos.y * text_size - row * (text_size + line_spacing);

  gl_Position = vec4(x, y, -.1, 1);
}
#shader fragment
#version 460

in vec2 tex_coord;

flat in int char_data;

layout( location = 0 ) out vec4 text_frag_color;

layout( binding = 0 ) uniform sampler2DArray char_dict; // texture sampler object, requires at least #version 420.

void main()
{
  text_frag_color = texture( char_dict, vec3(tex_coord, char_data) );
}
#end

# How It Works In Depth:
------------------------

# vertex
--------
attribute: in
vert_pos: vertex data stream buffer which holds just the vertex position x, y. (vec2)
char_buffer: character buffer, basically a type to be used as a string. (int[])
attribute: out
tex_coord: the coordinates used by texture sampling performed by the fragment shader. (vec2)
char_data: the index which corresponds directly to a single character of the ascii texture array. (flat int)
         - accesses the char_buffer string at index baseID + gl_InstanceID.
uniform:
baseID: pointer to the starting index of the character buffer.
top_margin: starting distance from the top edge of the window.
left_margin: starting distance from the left edge of the window.
text_size: character scaling.
char_spacing: distance between neighboring instances of textured quads (x direction).
line_spacing: distance between neighboring instances of textured quads (y direction).
--------
The vertex shader begins by initializing the position of the character being rendered: col = row = 0;
I like think of it as Column and Row because the font is mono spaced, so it's grid-like.
This position is later adjusted by the uniform variables left_margin and top_margin.
The next thing that happens is defining the texture coordinate that should be output by this vertex.
That's done by quickly switching the y coordinate of vertex position from 0 to 1 or vice versa,
this is because texture (0,0) exists 'top left' and world coordinates have (0,0) as 'bottom left'.
Note that's this is what the mix function is doing, not interpolating anything. I couldn't get the ternary
operator to work with vector operands so using mix solved that problem but it's just the first thing that worked.
After that the out variable char_data is assigned by accessing the char_buffer at index gl_InstanceID.
Any newline characters are then switched to space since newline doesn't have an associated texture image.
Then, the for loop counts new lines stored inside char_buffer up to the current index (gl_InstanceID)
to compute the row and column where this character should be rendered. The x and y position are then
computed and I think the math of those statements should be fairly self explanatory with an understanding
of OpenGL screen-space. Finally gl_Position is assigned and the main function ends.

# shader:
---------
attribute: in
tex_coord: the auto-interpolated texture coordinate with which to sample. (vec2)
char_data: char data is the index into the texture array. order matches ascii starting at space = 0. (flat int)
attribute: out
text_frag_color: the pixel to be output by this fragment shader
uniform:
char_dict: the 2D texture array which stores ascii character images loaded from png (sampler2DArray)
---------
All the fragment shader does is sample a texture using the index into the texture array
that was passed by vertices of the quad this pipeline is rendering. It receives an interpolated
texture coordinate from those vertices.