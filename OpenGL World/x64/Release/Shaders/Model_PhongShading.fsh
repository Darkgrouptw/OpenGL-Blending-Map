#version 430 core
struct MaterialInfo
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
};

in vec3 SurfaceNormal;
in vec3 ToLightVector;
in vec2 UV;
in float Visibility;

out vec4 FragColor;

uniform MaterialInfo Material;
uniform sampler2D texture;

//Lighting Color
uniform vec4 AmbientColor;
uniform vec4 DiffuseColor;   
uniform vec4 SpecularColor;
uniform vec4 SkyColor;

float Shininess = 128.0;//for material specular

void main()
{
	vec4 TextureColor = texture2D(texture , UV);
	vec4 OutColor = vec4(0, 0, 0, 1);
	if(TextureColor.a < 0.5)
		discard;
		
	// Dot product gives us diffuse intensity
	float diff = max(0.0, dot(normalize(SurfaceNormal),
					normalize(ToLightVector)));
					
	// Multiply intensity by diffuse color, force alpha to 1.0
	OutColor = diff * DiffuseColor * vec4(Material.Kd,1) +
				diff * DiffuseColor * TextureColor;
				
	// Add in ambient light
	OutColor += AmbientColor * vec4(Material.Ka, 1) +
				AmbientColor * TextureColor;
	
	// Specular Light
	vec3 vReflection = normalize(reflect(-normalize(ToLightVector),
								normalize(SurfaceNormal)));				//反射角
	float spec = max(0.0, dot(normalize(SurfaceNormal), vReflection));
	
	if(diff != 0) 
	{
		spec = pow(spec, Shininess);
		OutColor += SpecularColor * vec4(Material.Ks,1) * spec +
					SpecularColor * TextureColor * spec;
	}
	
	FragColor = mix(SkyColor, OutColor, Visibility);					//0 是skyColor，1 是OutColor，取決於Visibilty
}