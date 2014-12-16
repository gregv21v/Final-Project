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

int currTex = 1;
const int maxTextures = 3;
uniform sampler2D tex[maxTextures];

/*
in vec4 world_pos;
in vec4 vertPosition;
in vec4 vertColor;
in vec3 vertNormal;
in vec2 vertTexCoord;
in vec2 vertTexCoord_xy;
in vec2 vertTexCoord_zy;
in vec2 vertTexCoord_xz;
in int vertIsTextured;
in vec4 shadow_coord[maxLights];
*/

in GS_FS_INTERFACE {
	vec4 shadow_coord[maxLights];
	vec4 vertColor;
	vec3 vertNormal;
	vec2 vertTexCoord;
	vec2 vertTexCoord_xy;
	vec2 vertTexCoord_zy;
	vec2 vertTexCoord_xz;
	vec4 vertPosition;
	vec4 world_pos;
	flat int vertIsTextured;
} fragment;

uniform sampler2D modelTex;

uniform bool IsTerrain;

uniform float Shininess;
uniform float Strength;
uniform vec3 EyeDirection;
uniform vec3 Ambient;
uniform bool LightingOn;
uniform bool ShadowsOn;
uniform int Type;

uniform bool HeightTexturing;
uniform bool SlopeTexturing;
uniform bool TriPlanarTexturing;

out vec4 fragColor;

void main()
{
/*		**** NO GEOM SHADER ****
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

	float grass_factor = 1;
	float rock_factor = 1;
	float snow_factor = 1;
	
	// figure out height based texturing
	if(world_pos.y >= 60)
	{
		snow_factor = 1;
		rock_factor = 0;
		grass_factor = 0;
	}
	else
	{
		if(world_pos.y >= 40)
		{
			snow_factor = 1 - (abs(60 - world_pos.y) / 20);
			rock_factor = 1 - (abs(40 - world_pos.y) / 20);
			grass_factor = 0; 
		}
		else
		{
			if(world_pos.y >= 25)
			{
				snow_factor = 0;
				rock_factor = 1;
				grass_factor = 0;
			}
			else
			{
				if(world_pos.y >= 10)
				{
					snow_factor = 0;
					rock_factor = 1 - (abs(25 - world_pos.y) / 15);
					grass_factor = 1 - (abs(10 - world_pos.y) / 15);
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

	float grass_factor_xz = grass_factor;
	float rock_factor_xz = rock_factor;
	float snow_factor_xz = snow_factor;
	float grass_factor_xy = grass_factor;
	float rock_factor_xy = rock_factor;
	float snow_factor_xy = snow_factor;
	float grass_factor_zy = grass_factor;
	float rock_factor_zy = rock_factor;
	float snow_factor_zy = snow_factor;

	rock_factor_xy = abs(dot(vertNormal,vec3(0,0,1)));
	grass_factor_xy = (1 - rock_factor_xy) * grass_factor;
	snow_factor_xy = (1 - rock_factor_xy) * snow_factor;

	rock_factor_zy = abs(dot(vertNormal,vec3(1,0,0)));
	grass_factor_zy = (1 - rock_factor_zy) * grass_factor;
	snow_factor_zy = (1 - rock_factor_zy) * snow_factor;

	vec3 net_texture = (
		abs(dot(vertNormal,vec3(0,1,0))) * (
											grass_factor_xz * texture(tex[0],vertTexCoord_xz).rgb +
											rock_factor_xz * texture(tex[1],vertTexCoord_xz).rgb +
											snow_factor_xz * texture(tex[2],vertTexCoord_xz).rgb) + 
		abs(dot(vertNormal,vec3(0,0,1))) * (
											grass_factor_xy * texture(tex[0],vertTexCoord_xy).rgb +
											rock_factor_xy * texture(tex[1],vertTexCoord_xy).rgb +
											snow_factor_xy * texture(tex[2],vertTexCoord_xy).rgb) + 
		abs(dot(vertNormal,vec3(1,0,0))) * (
											grass_factor_zy * texture(tex[0],vertTexCoord_zy).rgb +
											rock_factor_zy * texture(tex[1],vertTexCoord_zy).rgb +
											snow_factor_zy * texture(tex[2],vertTexCoord_zy).rgb));

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
				vec3 rgb = vec3(1.0);

				switch(Type)
				{
				case 0: // SKY
					rgb = min(texture(modelTex,vertTexCoord).rgb, vec3(1.0));
					fragColor = vec4(rgb, vertColor.a * texture(modelTex, vertTexCoord).a);
					break;
				case 1: // FLOOR
					rgb = min(max(Ambient * texture(modelTex,vertTexCoord).rgb,texture(modelTex, vertTexCoord).rgb * scatteredLight + reflectedLight), vec3(1.0));
					fragColor = vec4(rgb, texture(modelTex, vertTexCoord).a);
					break;
				case 2:	// TERRAIN
					rgb = min(max(Ambient * net_texture, net_texture * scatteredLight + reflectedLight), vec3(1.0));
					fragColor = vec4(rgb, net_alpha);
					break;
				default:
					break;
				}
			}
			else 
			{
				vec3 rgb = min(max(Ambient, vertColor.rgb * scatteredLight + reflectedLight), vec3(1.0));
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
	*/

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
			lightDirection = lightDirection - vec3(fragment.vertPosition);
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
		
		float diffuse = max(0.0, dot(fragment.vertNormal, lightDirection));
		float specular = max(0.0, dot(fragment.vertNormal, halfVector));

		if(diffuse == 0.0)
			specular = 0.0;
		else
			specular = pow(specular,Shininess) * Strength;

		scatteredLight += Lights[light].ambient * attenuation +
							Lights[light].color * diffuse * attenuation;

		reflectedLight += Lights[light].color * specular * attenuation;

		if(Lights[light].isShadowMapEnabled)
		{
			float shadow = textureProj(Lights[light].depth_texture,fragment.shadow_coord[light]);
			net_shadow += shadow * Lights[light].color;
			numShadowsEnabled ++;
		}
	}

	net_shadow = net_shadow / numShadowsEnabled;

	//-----------------------------------------------------------------
	// HEIGHT BASED TEXTURING
	//-----------------------------------------------------------------
	float grass_factor = 1;
	float rock_factor = 0;
	float snow_factor = 0;
	
	if(HeightTexturing)
	{
		// figure out height based texturing
		if(fragment.world_pos.y >= 60)
		{
			snow_factor = 1;
			rock_factor = 0;
			grass_factor = 0;
		}
		else
		{
			if(fragment.world_pos.y >= 40)
			{
				snow_factor = 1 - (abs(60 - fragment.world_pos.y) / 20);
				rock_factor = 1 - (abs(40 - fragment.world_pos.y) / 20);
				grass_factor = 0; 
			}
			else
			{
				if(fragment.world_pos.y >= 25)
				{
					snow_factor = 0;
					rock_factor = 1;
					grass_factor = 0;
				}
				else
				{
					if(fragment.world_pos.y >= 10)
					{
						snow_factor = 0;
						rock_factor = 1 - (abs(25 - fragment.world_pos.y) / 15);
						grass_factor = 1 - (abs(10 - fragment.world_pos.y) / 15);
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
	}
	//-----------------------------------------------------------------
	
	//-----------------------------------------------------------------
	// SLOPE BASED TEXTURING
	//-----------------------------------------------------------------
	float grass_factor_xz = grass_factor;
	float rock_factor_xz = rock_factor;
	float snow_factor_xz = snow_factor;
	float grass_factor_xy = grass_factor;
	float rock_factor_xy = rock_factor;
	float snow_factor_xy = snow_factor;
	float grass_factor_zy = grass_factor;
	float rock_factor_zy = rock_factor;
	float snow_factor_zy = snow_factor;

	if(SlopeTexturing)
	{
		rock_factor_xy = pow(abs(dot(fragment.vertNormal,vec3(0,0,1))),.01);
		grass_factor_xy = (1 - rock_factor_xy) * grass_factor;
		snow_factor_xy = (1 - rock_factor_xy) * snow_factor;

		rock_factor_zy = pow(abs(dot(fragment.vertNormal,vec3(1,0,0))),.01);
		grass_factor_zy = (1 - rock_factor_zy) * grass_factor;
		snow_factor_zy = (1 - rock_factor_zy) * snow_factor;
	}
	//-----------------------------------------------------------------

	//-----------------------------------------------------------------
	// TRI-PLANAR TEXTURING
	//-----------------------------------------------------------------
	float xz_factor = 1;
	float xy_factor = 0;
	float yz_factor = 0;

	if(TriPlanarTexturing)
	{
		xz_factor = abs(dot(fragment.vertNormal,vec3(0,1,0)));
		xy_factor = abs(dot(fragment.vertNormal,vec3(0,0,1)));
		yz_factor = abs(dot(fragment.vertNormal,vec3(1,0,0)));
	}
	//-----------------------------------------------------------------

	//-----------------------------------------------------------------
	// COMBINE TEXTURINGS
	//-----------------------------------------------------------------
	vec3 net_texture = (
		xz_factor * (
				grass_factor_xz * texture(tex[0],fragment.vertTexCoord_xz).rgb +
				rock_factor_xz * texture(tex[1],fragment.vertTexCoord_xz).rgb +
				snow_factor_xz * texture(tex[2],fragment.vertTexCoord_xz).rgb) + 
		xy_factor * (
				grass_factor_xy * texture(tex[0],fragment.vertTexCoord_xy).rgb +
				rock_factor_xy * texture(tex[1],fragment.vertTexCoord_xy).rgb +
				snow_factor_xy * texture(tex[2],fragment.vertTexCoord_xy).rgb) + 
		yz_factor * (
				grass_factor_zy * texture(tex[0],fragment.vertTexCoord_zy).rgb +
				rock_factor_zy * texture(tex[1],fragment.vertTexCoord_zy).rgb +
				snow_factor_zy * texture(tex[2],fragment.vertTexCoord_zy).rgb));

	float net_alpha = (
		xz_factor * (
				grass_factor_xz * texture(tex[0],fragment.vertTexCoord_xz).a +
				rock_factor_xz * texture(tex[1],fragment.vertTexCoord_xz).a +
				snow_factor_xz * texture(tex[2],fragment.vertTexCoord_xz).a) + 
		xy_factor * (
				grass_factor_xy * texture(tex[0],fragment.vertTexCoord_xy).a +
				rock_factor_xy * texture(tex[1],fragment.vertTexCoord_xy).a +
				snow_factor_xy * texture(tex[2],fragment.vertTexCoord_xy).a) + 
		yz_factor * (
				grass_factor_zy * texture(tex[0],fragment.vertTexCoord_zy).a +
				rock_factor_zy * texture(tex[1],fragment.vertTexCoord_zy).a +
				snow_factor_zy * texture(tex[2],fragment.vertTexCoord_zy).a));
	//-----------------------------------------------------------------
	

	if(LightingOn) 
	{
		if(ShadowsOn)
		{
			if(fragment.vertIsTextured == 1)
			{
				vec3 rgb = min(Ambient * texture(tex[currTex],fragment.vertTexCoord).rgb + net_shadow * (texture(tex[currTex], fragment.vertTexCoord).rgb * scatteredLight + reflectedLight), vec3(1.0));
				fragColor = vec4(rgb, texture(tex[currTex], fragment.vertTexCoord).a);
			}
			else 
			{
				vec3 rgb = min(Ambient + net_shadow * (fragment.vertColor.rgb * scatteredLight + reflectedLight), vec3(1.0));
				fragColor = vec4(rgb, fragment.vertColor.a);
			}
		}
		else
		{
			if(fragment.vertIsTextured == 1)
			{
				vec3 rgb = vec3(1.0);

				switch(Type)
				{
				case 0: // SKY
					rgb = min(texture(modelTex,fragment.vertTexCoord).rgb, vec3(1.0));
					fragColor = vec4(rgb, fragment.vertColor.a * texture(modelTex, fragment.vertTexCoord).a);
					break;
				case 1: // FLOOR
					rgb = min(max(Ambient * texture(modelTex,fragment.vertTexCoord).rgb,texture(modelTex, fragment.vertTexCoord).rgb * scatteredLight + reflectedLight), vec3(1.0));
					fragColor = vec4(rgb, texture(modelTex, fragment.vertTexCoord).a);
					break;
				case 2:	// TERRAIN
					rgb = min(max(Ambient * net_texture, net_texture * scatteredLight + reflectedLight), vec3(1.0));
					fragColor = vec4(rgb, net_alpha);
					break;
				default:
					break;
				}
			}
			else 
			{
				vec3 rgb = min(max(Ambient, fragment.vertColor.rgb * scatteredLight + reflectedLight), vec3(1.0));
				fragColor = vec4(rgb, fragment.vertColor.a);
			}
		}
	} 
	else 
	{
		if(fragment.vertIsTextured == 1)
		{
			fragColor = texture(tex[currTex], fragment.vertTexCoord);		
		}
		else 
		{
			fragColor = fragment.vertColor;
		}
	}
}