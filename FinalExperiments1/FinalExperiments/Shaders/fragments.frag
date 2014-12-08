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

int currTex = 1;
const int maxTextures = 3;
uniform sampler2D tex[maxTextures];

in vec4 world_pos;
in vec4 vertPosition;
in vec4 vertColor;
in vec3 vertNormal;
in vec2 vertTexCoord;
in vec2 vertTexCoord_xy;
in vec2 vertTexCoord_zy;
in vec2 vertTexCoord_xz;
in int vertIsTextured;

//uniform sampler2D GrassTex;
//uniform sampler2D RockTex;
//uniform sampler2D SnowTex;

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

	float grass_factor = 0;
	float rock_factor = 0;
	float snow_factor = 0;
	
	// figure out height based texturing
	if(world_pos.y >= 40)
	{
		snow_factor = 1;
		rock_factor = 0;
		grass_factor = 0;
	}
	else
	{
		if(world_pos.y >= 20)
		{
			snow_factor = 1 - (abs(40 - world_pos.y) / 20);
			rock_factor = 1 - (abs(20 - world_pos.y) / 10);
			grass_factor = 0; 
		}
		else
		{
			if(world_pos.y >= 15)
			{
				snow_factor = 0;
				rock_factor = 1;
				grass_factor = 0;
			}
			else
			{
				if(world_pos.y >= 5)
				{
					snow_factor = 0;
					rock_factor = 1 - (abs(15 - world_pos.y) / 10);
					grass_factor = 1 - (abs(5 - world_pos.y) / 10);
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
		abs(dot(vertNormal,vec3(0,1,0))) * (
											grass_factor * texture(tex[0],vertTexCoord_xz).rgb +
											rock_factor * texture(tex[1],vertTexCoord_xz).rgb +
											snow_factor * texture(tex[2],vertTexCoord_xz).rgb) + 
		abs(dot(vertNormal,vec3(0,0,1))) * (
											grass_factor * texture(tex[0],vertTexCoord_xy).rgb +
											rock_factor * texture(tex[1],vertTexCoord_xy).rgb +
											snow_factor * texture(tex[2],vertTexCoord_xy).rgb) + 
		abs(dot(vertNormal,vec3(1,0,0))) * (
											grass_factor * texture(tex[0],vertTexCoord_zy).rgb +
											rock_factor * texture(tex[1],vertTexCoord_zy).rgb +
											snow_factor * texture(tex[2],vertTexCoord_zy).rgb));

	float net_alpha = (
		abs(dot(vertNormal,vec3(0,1,0))) * (
											grass_factor * texture(tex[0],vertTexCoord_xz).a +
											rock_factor * texture(tex[1],vertTexCoord_xz).a +
											snow_factor * texture(tex[2],vertTexCoord_xz).a) + 
		abs(dot(vertNormal,vec3(0,0,1))) * (
											grass_factor * texture(tex[0],vertTexCoord_xy).a +
											rock_factor * texture(tex[1],vertTexCoord_xy).a +
											snow_factor * texture(tex[2],vertTexCoord_xy).a) + 
		abs(dot(vertNormal,vec3(1,0,0))) * (
											grass_factor * texture(tex[0],vertTexCoord_zy).a +
											rock_factor * texture(tex[1],vertTexCoord_zy).a +
											snow_factor * texture(tex[2],vertTexCoord_zy).a));

	/*
	net_texture = (
		abs(dot(vertNormal,vec3(0,1,0))) * texture(tex[currTex],vertTexCoord_xz).rgb + 
		abs(dot(vertNormal,vec3(0,0,1))) * texture(tex[currTex],vertTexCoord_xy).rgb + 
		abs(dot(vertNormal,vec3(1,0,0))) * texture(tex[currTex],vertTexCoord_zy).rgb);
	//net_texture = net_texture / 3;
	net_alpha = (
		abs(dot(vertNormal,vec3(0,1,0))) * texture(tex[currTex],vertTexCoord_xz).a + 
		abs(dot(vertNormal,vec3(0,0,1))) * texture(tex[currTex],vertTexCoord_xy).a + 
		abs(dot(vertNormal,vec3(1,0,0))) * texture(tex[currTex],vertTexCoord_zy).a);
	//net_alpha = net_alpha / 3;
	
	*/

	net_alpha = 1;

	if(LightingOn) 
	{
		if(ShadowsOn)
		{
			if(vertIsTextured == 1)
			{
				vec3 rgb = min(Ambient * texture(tex[currTex],vertTexCoord).rgb + net_shadow * (texture(tex[currTex], vertTexCoord).rgb * scatteredLight + reflectedLight), vec3(1.0));
				fragColor = vec4(rgb, texture(tex[currTex], vertTexCoord).a);
			}
			else 
			{
				vec3 rgb = min(Ambient + net_shadow * (vertColor.rgb * scatteredLight + reflectedLight), vec3(1.0));
				fragColor = vec4(rgb, vertColor.a);
			}
		}
		else
		{
			if(vertIsTextured == 1)
			{
				//vec3 rgb = min(min(Ambient * texture(tex[currTex],vertTexCoord).rgb,texture(tex[currTex], vertTexCoord).rgb * scatteredLight + reflectedLight), vec3(1.0));
				//fragColor = vec4(rgb, texture(tex[currTex], vertTexCoord).a);

				vec3 rgb = min(max(Ambient * net_texture, net_texture * scatteredLight + reflectedLight), vec3(1.0));
				fragColor = vec4(rgb, net_alpha);
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
			fragColor = texture(tex[currTex], vertTexCoord);		
		}
		else 
		{
			fragColor = vertColor;
		}
	}
}