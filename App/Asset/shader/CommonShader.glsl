#type vertex
#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 texCoord;
uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ModelMatrix;

void main(){
	gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(aPos.xyz, 1.0);
	texCoord = aTexCoord;
}

#type fragment
#version 450 core
in vec2 texCoord;
out vec4 pixelColor;
		
uniform sampler2D u_texture;
uniform vec4 u_Color;

void main(){
	pixelColor = texture(u_texture, texCoord) * u_Color;
}	
