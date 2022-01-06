#shader vertex
#version 460

// read from vertex buffer, location 0 -> attrib pointer 0
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_tex_coord;

// sent to subsequent fragment shader invocations
out vec3 position;
out vec2 tex_coord;

// uniforms shared by all vertices
uniform mat4 mvp;

void main()
{
  tex_coord = vertex_tex_coord;                   // out tex_coor 
  gl_Position = mvp * vec4(vertex_position, 1.0); // the clip-space output position of the current vertex
}

#shader fragment
#version 460

in vec3 position;
in vec2 tex_coord;

layout( location = 0 ) out vec4 frag_color;
layout( binding = 0 ) uniform sampler2DArray player_tex; // The texture sampler object, requires #version 420

uniform int frame_ID;

void main()
{
  frag_color = texture(player_tex, vec3(tex_coord, frame_ID));
}
#end