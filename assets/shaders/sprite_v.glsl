// sprite_v.glsl
#version 120

varying out vec4 vert_pos;

void main()
{
    // Set position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // Set texture coord
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // Set default color
    gl_FrontColor = gl_Color;

    // Set output
    vert_pos = gl_ModelViewProjectionMatrix * gl_Vertex;
}
