#version 330 core
out vec4 FragColor;



 float near_plane = 0.1f;
 float far_plane = 10.f;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main()
{
    FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}
