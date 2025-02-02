#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
	vec3 lightPosition;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);

	float intensity = max(dot(inNormal, vec3(gl_Position) - ubo.lightPosition), 0.0);
	intensity = 1-intensity;
	
	if(intensity > 0.1)
		fragColor = inColor * (intensity);
	else
		fragColor = inColor * 0.1;

	fragTexCoord = inTexCoord;
}