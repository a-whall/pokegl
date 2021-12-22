#shader vertex
#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;

out vec3 Position;
out vec2 TexCoord;

uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main() {
    TexCoord = VertexTexCoord;
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}

#shader fragment
#version 460

in vec3 Position;
in vec2 TexCoord;

layout( location = 0 ) out vec4 FragColor;
layout( binding = 0 ) uniform sampler2DArray PlayerTex; // The texture sampler object, requires #version 420

uniform int frameID;

void main()
{
  FragColor = texture(PlayerTex, vec3(TexCoord, frameID));
}
#end