#version 410

layout(location = 0) in vec2 in_position;
uniform mat4 view_projection_matrix;


void main()
{
    gl_Position = view_projection_matrix * vec4(in_position, 0., 1.);
}


/**
2D
void main()
{

uniform vec2 moveCube; // Vous pouvez mettre le type que vous voulez, et le nom que vous voulez
uniform float aspect_ratio = 1;
uniform bool fullScreen = true;

    vec2 position = in_position;
    if(!fullScreen){
        position += moveCube;
        gl_Position = vec4(position.x / aspect_ratio, position.y, 0., 1.);
        return;
    }
    gl_Position = vec4(position, 0., 1.);

}
*/