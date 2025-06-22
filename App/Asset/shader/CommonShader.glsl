#type vertex
#version 450 core
layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord; 
layout(location = 3) in float aTexIndex; 
layout(location = 4) in float aTillingFactor; 
layout(location = 5) in vec4 aTintColor; 

out vec2 texCoord;
out vec4 color;
flat out float texIndex;
out float tillingFactor; 
out vec4 tintColor;
uniform mat4 u_ViewProjectionMatrix;

void main(){
	gl_Position = u_ViewProjectionMatrix * aPos;
	texCoord = aTexCoord;
	color = aColor;
	texIndex = aTexIndex;
	tillingFactor =  aTillingFactor;
	tintColor = aTintColor;
}

#type fragment
#version 450 core
in vec2 texCoord;
in vec4 color;
flat in float texIndex;
in float tillingFactor;
in vec4 tintColor;

out vec4 pixelColor;
uniform sampler2D u_Textures[32];

void main(){
	int index = int(texIndex);
	pixelColor = texture(u_Textures[index], texCoord * tillingFactor) * color * tintColor;
}	
