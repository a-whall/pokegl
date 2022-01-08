#shader vertex
#version 460

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_texcoord;

out vec3 Position;
out vec2 tex_coord;

uniform mat4 mvp;

void main() {
    tex_coord = vertex_texcoord;
    gl_Position = mvp * vec4(vertex_position, 1.0);
}

#shader fragment
#version 460

in vec3 Position;
in vec2 tex_coord;

layout( location = 0 ) out vec4 frag_color;

layout( binding = 0 ) uniform sampler2D text_box_texture;

void main() {
    frag_color = vec4( texture(text_box_texture,tex_coord).rgb, 1.0 );
}
#end