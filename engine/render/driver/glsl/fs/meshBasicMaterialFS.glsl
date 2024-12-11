
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
#ifdef USE_TEXTURE
	FragColor = texture(texture1, TexCoord);
#else
	FragColor = vec4(ourColor, 1.0f);
#endif
}
