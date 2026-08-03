#version 330

in vec2 fragTexCoord;

out vec4 outColor;

uniform sampler2D uTexture;
uniform vec4 uColor;
uniform bool uUseTexture;

void main()
{
    vec4 color;
    
    if (uUseTexture) {
        color = texture(uTexture, fragTexCoord);
    } else {
        color = uColor;
    }
    
    outColor = color;
}