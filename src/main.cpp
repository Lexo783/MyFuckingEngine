#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut

    auto camera = gl::Camera{};
    gl::set_events_callbacks({camera.events_callbacks()});

    glm::mat4 const projection_matrix = glm::infinitePerspective(1.f /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.001f /*near plane*/);

    auto const rectangle_mesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position3D{0}, gl::VertexAttribute::UV{1}, gl::VertexAttribute::Normal3D{2}},
            .data   = {
                    // Positions            // UVs         // Normales
                    // Face gauche
                    -1.0f, -1.0f, -1.0f,    0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
                    -1.0f, -1.0f,  1.0f,    1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
                    -1.0f,  1.0f,  1.0f,    1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
                    -1.0f,  1.0f, -1.0f,    0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
                    // Face droite
                    1.0f, -1.0f, -1.0f,    0.0f, 0.0f,    1.0f,  0.0f,  0.0f,
                    1.0f, -1.0f,  1.0f,    1.0f, 0.0f,    1.0f,  0.0f,  0.0f,
                    1.0f,  1.0f,  1.0f,    1.0f, 1.0f,    1.0f,  0.0f,  0.0f,
                    1.0f,  1.0f, -1.0f,    0.0f, 1.0f,    1.0f,  0.0f,  0.0f,
                    // Face arrière
                    -1.0f, -1.0f, -1.0f,    0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
                    1.0f, -1.0f, -1.0f,    1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
                    1.0f,  1.0f, -1.0f,    1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
                    -1.0f,  1.0f, -1.0f,    0.0f, 1.0f,    0.0f,  0.0f, -1.0f,
                    // Face avant
                    -1.0f, -1.0f,  1.0f,    0.0f, 0.0f,    0.0f,  0.0f,  1.0f,
                    1.0f, -1.0f,  1.0f,    1.0f, 0.0f,    0.0f,  0.0f,  1.0f,
                    1.0f,  1.0f,  1.0f,    1.0f, 1.0f,    0.0f,  0.0f,  1.0f,
                    -1.0f,  1.0f,  1.0f,    0.0f, 1.0f,    0.0f,  0.0f,  1.0f,
                    // Face du bas
                    -1.0f, -1.0f, -1.0f,    0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
                    1.0f, -1.0f, -1.0f,    1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
                    1.0f, -1.0f,  1.0f,    1.0f, 1.0f,    0.0f, -1.0f,  0.0f,
                    -1.0f, -1.0f,  1.0f,    0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
                    // Face du haut
                    -1.0f,  1.0f, -1.0f,    0.0f, 0.0f,    0.0f,  1.0f,  0.0f,
                    1.0f,  1.0f, -1.0f,    1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
                    1.0f,  1.0f,  1.0f,    1.0f, 1.0f,    0.0f,  1.0f,  0.0f,
                    -1.0f,  1.0f,  1.0f,    0.0f, 1.0f,    0.0f,  1.0f,  0.0f
                    },
                    }},
                    .index_buffer   = {
                    // Indices pour chaque face
                    0, 1, 2,  0, 2, 3,    // Face gauche
                    4, 5, 6,  4, 6, 7,    // Face droite
                    8, 9, 10, 8, 10,11,   // Face arrière
                    12,13,14, 12,14,15,   // Face avant
                    16,17,18, 16,18,19,   // Face du bas
                    20,21,22, 20,22,23    // Face du haut
        }
    }};

    auto const shader = gl::Shader{{
        .vertex   = gl::ShaderSource::File{"res/vertex.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
        }};

    auto const texture = gl::Texture{
            gl::TextureSource::File{ // Peut être un fichier, ou directement un tableau de pixels
                    .path           = "res/texture.png",
                    .flip_y         = true, // Il n'y a pas de convention universelle sur la direction de l'axe Y. Les fichiers (.png, .jpeg) utilisent souvent une direction différente de celle attendue par OpenGL. Ce booléen flip_y est là pour inverser la texture si jamais elle n'apparaît pas dans le bon sens.
                    .texture_format = gl::InternalFormat::RGBA8, // Format dans lequel la texture sera stockée. On pourrait par exemple utiliser RGBA16 si on voulait 16 bits par canal de couleur au lieu de 8. (Mais ça ne sert à rien dans notre cas car notre fichier ne contient que 8 bits par canal, donc on ne gagnerait pas de précision). On pourrait aussi stocker en RGB8 si on ne voulait pas de canal alpha. On utilise aussi parfois des textures avec un seul canal (R8) pour des usages spécifiques.
            },
            gl::TextureOptions{
                    .minification_filter  = gl::Filter::Linear, // Comment on va moyenner les pixels quand on voit l'image de loin ?
                    .magnification_filter = gl::Filter::Linear, // Comment on va interpoler entre les pixels quand on zoom dans l'image ?
                    .wrap_x               = gl::Wrap::Repeat,   // Quelle couleur va-t-on lire si jamais on essaye de lire en dehors de la texture ?
                    .wrap_y               = gl::Wrap::Repeat,   // Idem, mais sur l'axe Y. En général on met le même wrap mode sur les deux axes.
            }
    };

    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 1.f, 1.f); // Choisis la couleur à utiliser. Les paramètres sont R, G, B, A avec des valeurs qui vont de 0 à 1
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Vient remplacer glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 const rotation = glm::rotate(glm::mat4{1.f}, gl::time_in_seconds() /*angle de la rotation*/, glm::vec3{0.f, 0.f, 1.f} /* axe autour duquel on tourne */);
        glm::mat4 const translation = glm::translate(glm::mat4{1.f}, glm::vec3{0.f, 1.f, 0.f} /* déplacement */);
        shader.bind();

        shader.set_uniform("m_texture", texture);

        // on peut récupérer la matrice de vue avec :
        glm::mat4 const view_matrix = camera.view_matrix();
        shader.set_uniform("view_projection_matrix", (projection_matrix * view_matrix) * ( rotation));
        shader.set_uniform("color", glm::vec4{.1, 0.5f, .5f, 1.});

        rectangle_mesh.draw();
    }
}

/***
 * Algo pour du 2D
 * @return Void main exécute le shader

int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut

    auto const shader = gl::Shader{{
        .vertex   = gl::ShaderSource::File{"res/vertex.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
        }};

    auto const background_mesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position2D{0}},
            .data   = {
                    -1.f,  -1.f, // Position2D du 1er sommet
                    +1.f,  -1.f, // Position2D du 2ème sommet
                    +1.f,  +1.f, // Position2D du 3ème sommet
                    -1.f, +1.f  // Position2D du 4ème sommet
                    },
                    }},
                    .index_buffer   = {
                0, 1, 2, // Indices du premier triangle : on utilise le 1er, 2ème et 3ème sommet
                0, 2, 3  // Indices du deuxième triangle : on utilise le 1er, 3ème et 4ème sommet
                },
                }};

    auto const rectangle_mesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position2D{0}},
            .data   = {
                    -0.5f, -0.5f, // Position2D du 1er sommet
                    +0.5f, -0.5f, // Position2D du 2ème sommet
                    +0.5f, +0.5f, // Position2D du 3ème sommet
                    -0.5f, +0.5f  // Position2D du 4ème sommet
                    },
                    }},
                    .index_buffer   = {
                0, 1, 2, // Indices du premier triangle : on utilise le 1er, 2ème et 3ème sommet
                0, 2, 3  // Indices du deuxième triangle : on utilise le 1er, 3ème et 4ème sommet
                },
                }};

    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 1.f, 1.f); // Choisis la couleur à utiliser. Les paramètres sont R, G, B, A avec des valeurs qui vont de 0 à 1
        //glClear(GL_COLOR_BUFFER_BIT); // Exécute concrètement l'action d'appliquer sur tout l'écran la couleur choisie au-dessus
        glEnable(GL_BLEND);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE); // On peut configurer l'équation qui mélange deux couleurs, comme pour faire différents blend mode dans Photoshop. Cette équation-ci donne le blending "normal" entre pixels transparents.

        shader.bind(); // On a besoin qu'un shader soit bind (i.e. "actif") avant de draw(). On en reparle dans la section d'après.
        shader.set_uniform("color", glm::vec4{1, 0.2, 0.3, 0.004});
        shader.set_uniform("fullScreen", true);
        background_mesh.draw(); // C'est ce qu'on appelle un "draw call" : on envoie l'instruction à la carte graphique de dessiner notre mesh.

        shader.bind(); // On a besoin qu'un shader soit bind (i.e. "actif") avant de draw(). On en reparle dans la section d'après.
        shader.set_uniform("moveCube", glm::vec2{cos(gl::time_in_seconds()), sin(gl::time_in_seconds())});
        shader.set_uniform("aspect_ratio", gl::framebuffer_aspect_ratio());
        shader.set_uniform("color", glm::vec4{.0, 0.2f, .7f, 1.});
        shader.set_uniform("fullScreen", false);

        rectangle_mesh.draw(); // C'est ce qu'on appelle un "draw call" : on envoie l'instruction à la carte graphique de dessiner notre mesh.
    }
}
*/