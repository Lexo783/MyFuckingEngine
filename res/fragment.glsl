
#version 410

uniform vec3 global_light;
uniform float global_light_intensity;
uniform vec3 point_light;
uniform float point_light_intensity;
uniform float global_illumination;
uniform sampler2D tex;

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec4 out_color;

// Je n'ai absolument rien compris je me suis fait aider mais tranquille !
void main()
{
    vec3 normalized_normal = normalize(normal);

    float global_intensity = max(0, -dot(global_light, normalized_normal));
    global_intensity *= global_light_intensity;

    vec3 point_light_vector = position - point_light;
    float point_light_length = length(point_light_vector);
    float point_intensity = max(0, -dot(normalize(position - point_light), normalized_normal)) / pow(point_light_length, 2);
    point_intensity *= point_light_intensity;

    float light_intensity = global_intensity + point_intensity;
    float light_with_global_illumination = light_intensity + global_illumination;

    vec4 tex_color = texture(tex, uv);
    vec4 tex_light_color =  vec4( vec3(tex_color) * light_with_global_illumination, 1.);

    out_color = tex_light_color;
}
