#version 330

// Inputs from vertex shader
in vec3 fragPosition;
in vec3 fragNormal;

// Uniforms
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 baseColor;

// Output color
out vec4 finalColor;

void main()
{
    // Normalize vectors
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPosition - fragPosition);

    // Compute diffuse lighting
    float diff = max(dot(norm, lightDir), 0.0);

    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Combine results
    vec3 result = (ambient + diff * lightColor) * baseColor;
    finalColor = vec4(result, 1.0);
}
