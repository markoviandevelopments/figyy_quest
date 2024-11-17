#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec3 vertexNormal;

// Output to fragment shader
out vec3 fragPosition;
out vec3 fragNormal;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Transform the vertex position
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
    
    // Pass transformed position and normal to fragment shader
    fragPosition = vec3(model * vec4(vertexPosition, 1.0));
    fragNormal = normalize(mat3(transpose(inverse(model))) * vertexNormal);
}
