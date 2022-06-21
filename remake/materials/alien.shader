// NOTE: Shader automatically converted from Godot Engine 3.5.rc3's SpatialMaterial.

shader_type spatial;
render_mode async_visible,depth_draw_always,cull_back,diffuse_burley;
uniform vec4 highlight : hint_color;
uniform float disintegrate: hint_range(0, 1) = 0.0;
uniform float light : hint_range(0, 1) = 1.0;
uniform float fade : hint_range(0, 1) = 1.0;
uniform sampler2D texture_albedo : hint_albedo;
uniform sampler2D noise : hint_albedo;
uniform float roughness : hint_range(0,1);
uniform sampler2D texture_roughness : hint_white;
uniform vec4 roughness_texture_channel;
uniform sampler2D texture_ambient_occlusion : hint_white;
uniform vec4 ao_texture_channel;
uniform float ao_light_affect;

void fragment() {
	vec2 base_uv = UV;
	vec4 albedo_tex = texture(texture_albedo,base_uv);
	ALBEDO = highlight.rgb + albedo_tex.rgb * vec3(light);
	float roughness_tex = dot(texture(texture_roughness,base_uv),roughness_texture_channel);
	ROUGHNESS = mix(1.0, roughness_tex * roughness, light);
	AO = dot(texture(texture_ambient_occlusion,base_uv),ao_texture_channel);
	AO_LIGHT_AFFECT = ao_light_affect;
	ALPHA = ((disintegrate * (texture(noise, vec2(cos(VERTEX.x), sin(VERTEX.y * 5.0 + TIME * 1.5))).r * 1.5)) + (1.0 - disintegrate)) * fade;
}
