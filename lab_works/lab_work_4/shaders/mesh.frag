#version 450

layout( location = 0 ) out vec4 fragColor;

uniform vec3 uambiant; 
uniform vec3 udiffuse; 
uniform vec3 uspecular;
uniform float ushininess;

in vec3 normal;
in vec3 FragPos; 

void main()
{
	

	vec3 lightDirection = normalize(FragPos);												// lumière diffuse
	vec3 normalized = normalize(normal);
	float diff = max(dot(normalized, lightDirection), 0.f);
	vec3 diffuse = udiffuse * diff;

	vec3 viewDir = normalize(FragPos);														// lumière spéculaire
	vec3 reflectDir =  reflect(-lightDirection, normalized);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), ushininess);

	fragColor = vec4( uambiant + diffuse + spec , 1.f );
}
