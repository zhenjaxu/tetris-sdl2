#version 330

in vec2 fragTexCoord;

out vec4 outColor;

uniform sampler2D uTexture;
uniform vec4 uColor;
uniform bool uUseTexture;

void main()
{
    if (uUseTexture) {
        outColor = texture(uTexture, fragTexCoord);
    } else {
        outColor = uColor;
    }
}