#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"

int main()
{
    gl::init("Particules!");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);


    // TODO: create an array of particles
    // Définition de la structure Particle.
    struct Particle {
        glm::vec2 position; // Position 2D de la particule (x, y)
        glm::vec2 velocity; // Vitesse 2D (vx, vy)
        float life;         // Durée de vie de la particule
        glm::vec4 color;    // Couleur (R, G, B, Alpha)
    };

    // Nombre maximal de particules
    const int MAX_PARTICLES = 1000;

    // Tableau de particules
    std::vector<Particle> particles(MAX_PARTICLES);

    // Initialisation des particules
    for (auto& particle : particles) {
        particle.position = glm::vec2(    // Position aléatoire dans l'écran
            static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,   // X entre -1 et 1
            static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f    // Y entre -1 et 1
        );
        particle.velocity = glm::vec2(   // Vitesse aléatoire
            static_cast<float>(rand()) / RAND_MAX * 0.2f - 0.1f,   // Vx entre -0.1 et 0.1
            static_cast<float>(rand()) / RAND_MAX * 0.2f - 0.1f    // Vy entre -0.1 et 0.1
        );
        particle.life = static_cast<float>(rand()) / RAND_MAX * 5.0f;   // Vie entre 0 et 5 secondes
        particle.color = glm::vec4(      // Couleur aléatoire translucide
            static_cast<float>(rand()) / RAND_MAX, // Rouge
            static_cast<float>(rand()) / RAND_MAX, // Vert
            static_cast<float>(rand()) / RAND_MAX, // Bleu
            1.0f                              // Alpha initial à 1.0 (complètement opaque)
        );
    }

    //-----------------------

    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO update particles
        // TODO render particles

        // Afficher toutes les particules
        for (const auto& particle : particles) {
            // Valeur hardcodée pour le rayon
            const float radius = 0.01f;
            utils::draw_disk(particle.position, radius, particle.color);
        }
    }
}