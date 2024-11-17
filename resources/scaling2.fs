#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0; // Diffuse texture
uniform float textureScale; // Scaling factor

void main()
{
    vec2 scaledTexCoord = fragTexCoord * textureScale;
    finalColor = texture(texture0, scaledTexCoord);
}
