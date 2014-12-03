#version 330 core

struct LightProperties{
	bool isEnabled;
	bool isLocal;
	bool isSpot;
	vec3 ambient;
	vec3 color;
	vec3 position;
	vec3 halfVector;
	vec3 coneDirection;
	float spotCosCutoff;
	float spotExponent;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	bool isShadowMapEnabled;
	sampler2DShadow depth_texture;
};

const int maxLights = 3;		// *** Change this when adding lights ***
uniform LightProperties Lights[maxLights];

in vec4 shadow_coord[maxLights];

in vec4 vertPosition;
in vec4 vertColor;
in vec3 vertNormal;
in vec2 vertTexCoord;
flat in int vertIsTextured;

uniform sampler2D tex;

uniform float Shininess;
uniform float Strength;
uniform vec3 EyeDirection;
uniform vec3 Ambient;
uniform bool LightingOn;
uniform bool ShadowsOn;

out vec4 fragColor;

void main()
{
	vec3 scatteredLight = vec3(0.0);
	vec3 reflectedLight = vec3(0.0);

	float numShadowsEnabled = 0.0;
	vec3 net_shadow = vec3(0.0);

	for (int light = 0; light < maxLights; light++)
	{
		if (!Lights[light].isEnabled)
			continue;
		
		vec3 halfVector;
		vec3 lightDirection = normalize(Lights[light].position);
		float attenuation = 1.0;

		if(Lights[light].isLocal)
		{
			lightDirection = lightDirection - vec3(vertPosition);
			float lightDistance = length(lightDirection);
			lightDirection = lightDirection / lightDistance;

			attenuation = 1.0 / 
				(Lights[light].constantAttenuation
				+ Lights[light].linearAttenuation * lightDistance
				+ Lights[light].quadraticAttenuation * lightDistance * lightDistance);

			if(Lights[light].isSpot)
			{
				float spotCos = dot(lightDirection, -Lights[light].coneDirection);
				
				if(spotCos < Lights[light].spotCosCutoff)
				{
					attenuation = 0.0;
				}
				else
				{
					attenuation *= pow(spotCos, Lights[light].spotExponent);
				}
			}

			halfVector = normalize(lightDirection + EyeDirection);		
		}
		else
		{
			//halfVector = Lights[light].halfVector;	
			halfVector = normalize(lightDirection + EyeDirection);	
		}	
		
		float diffuse = max(0.0, dot(vertNormal, lightDirection));
		float specular = max(0.0, dot(vertNormal, halfVector));

		if(diffuse == 0.0)
			specular = 0.0;
		else
			specular = pow(specular,Shininess) * Strength;

		scatteredLight += Lights[light].ambient * attenuation +
							Lights[light].color * diffuse * attenuation;

		reflectedLight += Lights[light].color * specular * attenuation;

		if(Lights[light].isShadowMapEnabled)
		{
			float shadow = textureProj(Lights[light].depth_texture,shadow_coord[light]);
			net_shadow += shadow * Lights[light].color;
			numShadowsEnabled ++;
		}
	}

	net_shadow = net_shadow / numShadowsEnabled;
	
	if(LightingOn) 
	{
		if(ShadowsOn)
		{
			if(vertIsTextured == 1)
			{
				vec3 rgb = min(Ambient * texture(tex,vertTexCoord).rgb + net_shadow * (texture(tex, vertTexCoord).rgb * scatteredLight + reflectedLight), vec3(1.0));
				fragColor = vec4(rgb, texture(tex, vertTexCoord).a);
			}
			else 
			{
				vec3 rgb = min(Ambient + net_shadow * (vertColor.rgb * scatteredLight + reflectedLight), vec3(1.0));
				fragColor = vec4(rgb, vertColor.a);
				fragColor = vec4(1,1,1,1);
			}
		}
		else
		{
			if(vertIsTextured == 1)
			{
				vec3 rgb = min(Ambient * texture(tex,vertTexCoord).rgb + (texture(tex, vertTexCoord).rgb * scatteredLight + reflectedLight), vec3(1.0));
				fragColor = vec4(rgb, texture(tex, vertTexCoord).a);
			}
			else 
			{
				vec3 rgb = min(Ambient + (vertColor.rgb * scatteredLight + reflectedLight), vec3(1.0));
				fragColor = vec4(rgb, vertColor.a);
			}
		}
	} 
	else 
	{
		if(vertIsTextured == 1)
		{
			fragColor = texture(tex, vertTexCoord);		
		}
		else 
		{
			fragColor = vertColor;
		}
	}
}