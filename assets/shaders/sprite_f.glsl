// sprite_f.glsl
#version 120

varying in vec4 vert_pos;

uniform bool uUsingTexture;
uniform sampler2D uTexture;

uniform vec2 uLightPos;

void main()
{
    // Sample texture
    vec4 pixel = uUsingTexture ? texture2D(uTexture, gl_TexCoord[0].xy): vec4(1.0);
    
    // Calculate light
    vec4 ambient = vec4(0.05, 0.05, 0.05, 1.0);
    vec2 lightPos = (gl_ModelViewProjectionMatrix * vec4(uLightPos, 0, 1)).xy;
    
    vec2 lightToFrag = lightPos - vert_pos.xy;
	lightToFrag.y = lightToFrag.y / 1.7;

	float vecLength = clamp(length(lightToFrag) * 2, 0, 1);
    float intensity = 1;

    // Set output color
    gl_FragColor = gl_Color * pixel * max(ambient, vec4(1-vecLength, 1-vecLength, 1-vecLength, 1) * intensity);
}
