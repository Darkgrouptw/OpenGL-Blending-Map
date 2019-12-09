#version 430 core
layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec2 VertexUV;

out vec3 SurfaceNormal;
out vec3 ToLightVector;
out vec2 UV;
out float Visibility;

uniform mat4 ProjectionM;
uniform mat4 ViewM;
uniform vec3 LightPos;

const float density = 0.007;
const float gradient = 1.5;
void main()
{
    vec4 ToCameraPos = ViewM * vec4(VertexPos,1);			//Relate to Camera
	
	//設定光的參數
	SurfaceNormal = vec3(0, 1, 0);
	ToLightVector = LightPos - VertexPos;
	
	//設定UV
	UV = VertexUV;
	
	//設定Visibility
	float distance = length(ToCameraPos.xyz);
	Visibility = exp(-pow((distance * density), gradient));
	Visibility = clamp(Visibility, 0, 1);
		
	gl_Position = ProjectionM * ViewM * vec4(VertexPos, 1);
}
