#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Color;
layout(location=2) in vec4 Normal;

out vec4 vPosition;
out vec4 vColor;
out vec4 vNormal;

uniform mat4 ModelMatrix;
uniform mat4 ProjectionViewMatrix;
uniform mat4 ViewMatrix;

void main() {
	mat4 mvp = ProjectionViewMatrix * ModelMatrix;
    vColor = Color;

	// Rotate and scale normals - inverse transpose of non-translate mvp
	mat3 normalMatrix = mat3(ViewMatrix * ModelMatrix);
	normalMatrix = inverse(normalMatrix);
	normalMatrix = transpose(normalMatrix);
	vNormal = vec4(normalize(Normal.xyz * normalMatrix), 1.f);

	mat4 mv = ViewMatrix * ModelMatrix;
	vPosition = mv * Position;
    gl_Position = mvp * Position;
}
