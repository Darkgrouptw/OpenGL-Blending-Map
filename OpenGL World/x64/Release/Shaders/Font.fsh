#version 430 core
in vec2 UV;

out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
	vec4 TextureColor = texture2D(Texture, UV);
	if(TextureColor.a < 0.5f)
		discard;
	FragColor = TextureColor;
}
