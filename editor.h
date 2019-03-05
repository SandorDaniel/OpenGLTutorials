#define sd_debugger
#include "Debug.h"


using namespace glm;


float angleBetweenVectorsInRadians(
	vec3 u, 
	vec3 v)
{
	return acos(dot(u, v) / (length(u) * length(v)));
}

bool preProcessPositionalLightData(
	vec3  positional_light_pos, // in
	vec3  positional_light_dir, // in
	float positional_light_angle_in_radians, // in
	vec3  out_vert_pos, // in
	vec3  out_vert_nor, // in
	float distance_factor, // out
	float positional_light_power, // inout
	vec3 light_dir) // out
{
	float distance = length(positional_light_pos - out_vert_pos);
	distance_factor = distance * distance;

	light_dir = normalize(positional_light_pos - out_vert_pos);
}

vec3 preProcessDirectionalLightData(
	vec3 directional_light_dir, // in
	float distance_factor, // out
	vec3 light_dir) // out
{
	distance_factor = 1.0f;

	light_dir = -normalize(directional_light_dir);
}

vec3 colFromPreProcessedLightData(
	float light_power,
	vec3 light_diffuse_col,
	vec3 light_specular_col,
	vec3 light_ambient_col,
	vec3 out_vert_pos,
	vec3 out_vert_nor,
	vec3 nor,
	vec3 material_diffuse_col,
	vec3 material_specular_col,
	vec3 material_ambient_col,
	vec3 cam_pos,
	vec3 light_dir,
	float distance_factor
)
{
	if (angleBetweenVectorsInRadians(out_vert_nor, light_dir) > 3.14f / 1.75f) // TODO: After implementing shadow mapping, rethig the neccesity of this block.
	{
		return material_ambient_col * light_ambient_col / distance_factor;
	}

	// Normal of the computed fragment, in camera space
	vec3 n = normalize(nor);
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize(light_dir);
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp(dot(n, l), 0, 1);

	// Eye vector (towards the camera)
	vec3 E = normalize(cam_pos - out_vert_pos);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l, n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp(dot(E, R), 0, 1);

	return
		// Ambient : simulates indirect lighting
		material_ambient_col * light_ambient_col / distance_factor
		// Diffuse : "color" of the object
		+ material_diffuse_col * light_diffuse_col  * light_power * cosTheta / distance_factor
		// Specular : reflective highlight, like a mirror
		+ material_specular_col * light_specular_col * light_power * pow(cosAlpha, 5) / distance_factor;
}

vec3 colFromPositionalLightData(
	vec3  positional_light_pos,
	vec3  positional_light_dir,
	float positional_light_angle_in_radians,
	float positional_light_power,
	vec3  positional_light_diffuse_col,
	vec3  positional_light_specular_col,
	vec3  positional_light_ambient_col,
	vec3  out_vert_pos,
	vec3  out_vert_nor,
	vec3  nor,
	vec3  material_diffuse_col,
	vec3  material_specular_col,
	vec3  material_ambient_col,
	vec3  cam_pos)
{
	vec3 light_dir;
	float distance_factor;
	preProcessPositionalLightData(
		positional_light_pos, // in
		positional_light_dir, // in
		positional_light_angle_in_radians, // in
		out_vert_pos, // in
		out_vert_nor, // in
		distance_factor, // out
		positional_light_power, // inout
		light_dir);

	return colFromPreProcessedLightData(
		positional_light_power,
		positional_light_diffuse_col,
		positional_light_specular_col,
		positional_light_ambient_col,
		out_vert_pos,
		out_vert_nor,
		nor,
		material_diffuse_col,
		material_specular_col,
		material_ambient_col,
		cam_pos,
		light_dir,
		distance_factor
	);
}

vec3 colFromDirectionalLightData(
	vec3  directional_light_dir,
	float directional_light_power,
	vec3  directional_light_diffuse_col,
	vec3  directional_light_specular_col,
	vec3  directional_light_ambient_col,
	vec3  out_vert_pos,
	vec3  out_vert_nor,
	vec3  nor,
	vec3  material_diffuse_col,
	vec3  material_specular_col,
	vec3  material_ambient_col,
	vec3  cam_pos)
{
	vec3 light_dir;
	float distance_factor;
	preProcessDirectionalLightData(
		directional_light_dir, // in
		distance_factor, // out
		light_dir);

	return colFromPreProcessedLightData(
		directional_light_power,
		directional_light_diffuse_col,
		directional_light_specular_col,
		directional_light_ambient_col,
		out_vert_pos,
		out_vert_nor,
		nor,
		material_diffuse_col,
		material_specular_col,
		material_ambient_col,
		cam_pos,
		light_dir,
		distance_factor
	);
}









