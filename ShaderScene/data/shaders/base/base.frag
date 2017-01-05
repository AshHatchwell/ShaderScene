#version 410

in vec4 vPosition;
in vec4 vColor;
in vec4 vNormal;

out vec4 fColor;

uniform vec4 LightPosition;
uniform vec4 LightColor;
uniform vec4 CameraPos;

uniform mat4 ModelMatrix;
uniform mat4 ProjectionViewMatrix;
uniform mat4 ViewMatrix;

vec4 calculateLight() {
	// Calculate light direction
	vec4 lightDirection = normalize(LightPosition - vPosition);

	// Diffuse
	float d = max(0, dot(normalize(vNormal.xyz), lightDirection.xyz));
	// Specular
	vec3 e = normalize(CameraPos.xyz - vPosition.xyz);
	vec3 r = reflect(-lightDirection.xyz, vNormal.xyz);
	float spec = max(0, dot(e, r));
	spec = pow(spec, 16.f) * 0.6f;	// 16.f = Specular power, and the other 0.6f = specular intensity
	vec4 appliedLight = (LightColor * d + LightColor * spec);
	// If diffuse is below/at ambient, cut off all applied light to prevent spec in ambient
	appliedLight = d > 0.15f ? appliedLight : vec4(0.f);
	// Add ambient - 0.15 for now
	appliedLight = max(vec4(0.15f, 0.15f, 0.15f, 1f), appliedLight);
	return appliedLight;
}

void main() {
	fColor = vColor * calculateLight();
}
