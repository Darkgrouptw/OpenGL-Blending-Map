#version 430 core
in vec3 SurfaceNormal;
in vec3 ToLightVector;
in vec2 UV;
in float Visibility;

out vec4 FragColor;

//Multitexture
uniform sampler2D BlendMap;
uniform sampler2D BgTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;

//Lighting Color
uniform vec4 AmbientColor;
uniform vec4 DiffuseColor;   
uniform vec4 SpecularColor;
uniform vec4 SkyColor;

const float Shininess = 128.0;//for material specular

void main()
{
	//Get BlendMap Color
	vec4 BlendMapColor = texture2D(BlendMap, UV);
	
	// Get Background Color
	float BgTextureAmount = 1 - (BlendMapColor.r + BlendMapColor.g + BlendMapColor.b);
	vec2 TiltUV = UV * 40;
	vec4 BgTextureColor = texture2D(BgTexture, TiltUV) * BgTextureAmount;
	
	//Get Other Color
	vec4 rTextureColor = texture2D(rTexture, TiltUV) * BlendMapColor.r;
	vec4 gTextureColor = texture2D(gTexture, TiltUV) * BlendMapColor.g;
	vec4 bTextureColor = texture2D(bTexture, TiltUV) * BlendMapColor.b;
	
	//TotalColor
	vec4 TotalColor = BgTextureColor + rTextureColor + gTextureColor + bTextureColor;
	vec4 OutColor = vec4(0, 0, 0, 1);
	
	// Dot product gives us diffuse intensity
	float diff = max(0.0, dot(normalize(SurfaceNormal),
					normalize(ToLightVector)));
					
	// Multiply intensity by diffuse color, force alpha to 1.0
	OutColor = diff * DiffuseColor * TotalColor;
				
	// Add in ambient light
	OutColor += AmbientColor * TotalColor;
	
	// Specular Light
	vec3 vReflection = normalize(reflect(-normalize(ToLightVector),
								normalize(SurfaceNormal)));				//反射角
	float spec = max(0.0, dot(normalize(SurfaceNormal), vReflection));
	
	if(diff != 0) 
	{
		spec = pow(spec, Shininess);
		OutColor += SpecularColor * TotalColor * spec;
	}
	FragColor = mix(SkyColor, OutColor, Visibility);
}
