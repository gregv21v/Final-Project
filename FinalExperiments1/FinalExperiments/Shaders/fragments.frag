#version 330 core
#pragma optionNV unroll all

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

int currTex = 1;
const int maxTextures = 3;
uniform sampler2D tex[maxTextures];

in vec4 geomWorld_pos;
in vec4 geomPosition;
in vec4 geomColor;
in vec3 geomNormal;
in vec2 geomTexCoord;
in vec2 geomTexCoord_xy;
in vec2 geomTexCoord_zy;
in vec2 geomTexCoord_xz;
flat in int geomIsTextured;

uniform sampler2D modelTex;

uniform int IsTerrain;

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
			lightDirection = lightDirection - vec3(geomPosition);
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
		
		float diffuse = max(0.0, dot(geomNormal, lightDirection));
		float specular = max(0.0, dot(geomNormal, halfVector));

		if(diffuse == 0.0)
			specular = 0.0;
		else
			specular = pow(specular,Shininess) * Strength;

		scatteredLight += Lights[light].ambient * attenuation +
							Lights[light].color * diffuse * attenuation;

		reflectedLight += Lights[light].color * specular * attenuation;

		if(Lights[light].isShadowMapEnabled)
		{
			float shadow = textureProj(Lights[light].depth_texture, shadow_coord[light]);
			net_shadow += shadow * Lights[light].color;
			numShadowsEnabled ++;
		}
	}

	net_shadow = net_shadow / numShadowsEnabled;

	float grass_factor = 1;
	float rock_factor = 1;
	float snow_factor = 1;
	
	// figure out height based texturing
	if(geomWorld_pos.y >= 40)
	{
		snow_factor = 1;
		rock_factor = 0;
		grass_factor = 0;
	}
	else
	{
		if(geomWorld_pos.y >= 20)
		{
			snow_factor = 1 - (abs(40 - geomWorld_pos.y) / 20);
			rock_factor = 1 - (abs(20 - geomWorld_pos.y) / 10);
			grass_factor = 0; 
		}
		else
		{
			if(geomWorld_pos.y >= 15)
			{
				snow_factor = 0;
				rock_factor = 1;
				grass_factor = 0;
			}
			else
			{
				if(geomWorld_pos.y >= 5)
				{
					snow_factor = 0;
					rock_factor = 1 - (abs(15 - geomWorld_pos.y) / 10);
					grass_factor = 1 - (abs(5 - geomWorld_pos.y) / 10);
				}
				else
				{
					snow_factor = 0;
					rock_factor = 0;
					grass_factor = 1;
				}
			}
		}
	}

	vec3 net_texture = (
		abs(dot(geomNormal,vec3(0,1,0))) * (
											grass_factor * texture(tex[0],geomTexCoord_xz).rgb +
											rock_factor * texture(tex[1],geomTexCoord_xz).rgb +
											snow_factor * texture(tex[2],geomTexCoord_xz).rgb) + 
		abs(dot(geomNormal,vec3(0,0,1))) * (
											grass_factor * texture(tex[0],geomTexCoord_xy).rgb +
											rock_factor * texture(tex[1],geomTexCoord_xy).rgb +
											snow_factor * texture(tex[2],geomTexCoord_xy).rgb) + 
		abs(dot(geomNormal,vec3(1,0,0))) * (
											grass_factor * texture(tex[0],geomTexCoord_zy).rgb +
											rock_factor * texture(tex[1],geomTexCoord_zy).rgb +
											snow_factor * texture(tex[2],geomTexCoord_zy).rgb));

	float net_alpha = (
		abs(dot(geomNormal,vec3(0,1,0))) * (
											grass_factor * texture(tex[0],geomTexCoord_xz).a +
											rock_factor * texture(tex[1],geomTexCoord_xz).a +
											snow_factor * texture(tex[2],geomTexCoord_xz).a) + 
		abs(dot(geomNormal,vec3(0,0,1))) * (
											grass_factor * texture(tex[0],geomTexCoord_xy).a +
											rock_factor * texture(tex[1],geomTexCoord_xy).a +
											snow_factor * texture(tex[2],geomTexCoord_xy).a) + 
		abs(dot(geomNormal,vec3(1,0,0))) * (
											grass_factor * texture(tex[0],geomTexCoord_zy).a +
											rock_factor * texture(tex[1],geomTexCoord_zy).a +
											snow_factor * texture(tex[2],geomTexCoord_zy).a));

	net_alpha = 1;

	if(LightingOn) 
	{
		if(ShadowsOn)
		{
			if(geomIsTextured == 1)
			{
				vec3 rgb = min(Ambient * texture(tex[currTex],geomTexCoord).rgb + net_shadow * (texture(tex[currTex], geomTexCoord).rgb * scatteredLight + reflectedLight), vec3(1.0));
				fragColor = vec4(rgb, texture(tex[currTex], geomTexCoord).a);
			}
			else 
			{
				vec3 rgb = min(Ambient + net_shadow * (geomColor.rgb * scatteredLight + reflectedLight), vec3(1.0));
				fragColor = vec4(rgb, geomColor.a);
			}
		}
		else
		{
			if(geomIsTextured == 1)
			{
				//vec3 rgb = min(min(Ambient * texture(tex[currTex],geomTexCoord).rgb,texture(tex[currTex], geomTexCoord).rgb * scatteredLight + reflectedLight), vec3(1.0));
				//fragColor = vec4(rgb, texture(tex[currTex], geomTexCoord).a);
				if(IsTerrain == 1)
				{
					vec3 rgb = min(max(Ambient * net_texture, net_texture * scatteredLight + reflectedLight), vec3(1.0));
					fragColor = vec4(rgb, net_alpha);
					//fragColor = vec4(1,0,1,1);
				}
				else
				{
					vec3 rgb = min(texture(modelTex,geomTexCoord).rgb, vec3(1.0));
					fragColor = vec4(rgb, texture(modelTex, geomTexCoord).a);
				}
			}
			else 
			{
				vec3 rgb = min(Ambient + (geomColor.rgb * scatteredLight + reflectedLight), vec3(1.0));
				fragColor = vec4(rgb, geomColor.a);
			}
		}
	} 
	else 
	{
		if(geomIsTextured == 1)
		{
			fragColor = texture(tex[currTex], geomTexCoord);		
		}
		else 
		{
			fragColor = geomColor;
		}
	}
}