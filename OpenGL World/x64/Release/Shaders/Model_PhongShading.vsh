#version 430 core
layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec2 VertexUV;
layout(location = 2) in vec3 VertexNormal;

out vec3 SurfaceNormal;
out vec3 ToLightVector;
out vec2 UV;
out float Visibility;

//PVM
uniform mat4 ProjectionM;
uniform mat4 ViewM;
uniform mat4 ModelM;

//useFakeLight => obj's normal to top
uniform int useFakeLight;

const float density = 0.007;
const float gradient = 1.5;

uniform vec3 LightPosition;
void main()
{
	vec4 WorldPos = ModelM * vec4(VertexPos, 1);
    vec4 ToCameraPos = ViewM * ModelM * vec4(VertexPos,1);			//Relate to Camera
	
	// Get vector to light source
    ToLightVector = LightPosition - WorldPos.xyz;
	
	//Use FakeLight to create a normal to top
	vec3 actualNormal = VertexNormal;
	if(useFakeLight == 1)
		actualNormal = vec3(0, 1, 0);
		
    // Get surface normal in eye coordinates
	SurfaceNormal = (ModelM * vec4(actualNormal, 0)).xyz;
	
	float distance = length(ToCameraPos.xyz);
	Visibility = exp(-pow((distance * density), gradient));
	Visibility = clamp(Visibility, 0, 1);							//介於0~1之間
	
	UV = VertexUV;
    // Don't forget to transform the geometry!
    gl_Position = ProjectionM * ViewM * ModelM * vec4(VertexPos,1);
}