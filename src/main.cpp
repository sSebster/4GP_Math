#include "glm/ext/scalar_constants.hpp"
#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"


struct IntersectionResult {
    glm::vec2 point;
    float t, s;
};

std::optional<IntersectionResult> intersect_segments(const glm::vec2& A, const glm::vec2& B,
                                                     const glm::vec2& C, const glm::vec2& D)
{
    glm::vec2 u = B - A;
    glm::vec2 v = D - C;
    glm::vec2 w = C - A;
    // Matrice M = [u, -v]
    glm::mat2 M(u, -v);
    float det = glm::determinant(M);

    if (glm::abs(det) < 1e-6f)
        return std::nullopt;  // Parallèles ou colinéaires

    glm::mat2 invM = glm::inverse(M);
    glm::vec2 ts = invM * w;
    float t = ts.x;
    float s = ts.y;

    if (t >= 0.0f && t <= 1.0f && s >= 0.0f && s <= 1.0f) {
        glm::vec2 P = A + t * u;
        return IntersectionResult{P, t, s};
    }

    return std::nullopt;
}


float easeInOut(float x, float power)
{
    if (x < 0.5)
    {
        return 0.5 * pow(2 * x, power);
    }
    else
    {
        return 1 - 0.5 * pow(2 * (1 - x), power);
    }
}

struct Particle {
    glm::vec2 position{
        utils::rand(-gl::window_aspect_ratio(), +gl::window_aspect_ratio()),
        utils::rand(-1.f, +1.f),
    };

    glm::vec2 velocity;

    float mass{utils::rand(1.f, 2.f)};

    float age{0.f};
    float lifespan{utils::rand(5.f, 15.f)};

    glm::vec3 start_color{
        utils::rand(0.f, 1.f),
        utils::rand(0.f, 1.f),
        utils::rand(0.f, 1.f),
    };
    glm::vec3 end_color{
        utils::rand(0.f, 1.f),
        utils::rand(0.f, 1.f),
        utils::rand(0.f, 1.f),
    };

    glm::vec3 color() const
    {
        return start_color;
    }

    float radius() const
    {
        return 0.015f;
    }

    float relative_age() const
    {
        return age / lifespan;
    }

    Particle()
    {
        float const initial_angle = utils::rand(0.f, 2.f * glm::pi<float>());

        velocity = {
            0.2f * std::cos(initial_angle),
            0.2f * std::sin(initial_angle),
        };
    }
};

struct Segment
{
    glm::vec2 A;
    glm::vec2 B;

    glm::vec2 normal() const
    {
        glm::vec2 dir = B - A;
        glm::vec2 n = glm::normalize(glm::vec2(-dir.y, dir.x)); // Perpendicular
        return n;
    }
};

int main()
{
    gl::init("Particules!");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    std::vector<Particle> particles(3000);
    std::vector<Segment> segments;


    /*for (int i = 0; i < 5; ++i)
    {
        glm::vec2 A{
            utils::rand(-gl::window_aspect_ratio(), +gl::window_aspect_ratio()),
            utils::rand(-1.f, +1.f),
        };
        glm::vec2 B{
            utils::rand(-gl::window_aspect_ratio(), +gl::window_aspect_ratio()),
            utils::rand(-1.f, +1.f),
        };
        segments.push_back({A, B});
    }*/

    for (auto& particle : particles)
    {
        glm::vec2 xV(0.1f,0.f);
        glm::vec2 yV(0.85f,-0.2f);
        particle.position = xV*utils::rand(-1.f, 1.f) + yV*utils::rand(-1.f, 1.f);

            //glm::vec2(utils::rand(0,1)*xV.x,utils::rand(0,1)*yV.y);
    }

    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (auto const& particle : particles) utils::draw_disk(particle.position, particle.radius(), glm::vec4{particle.color(), 1.f});
        for (auto& particle : particles)
        {
            //glm::vec2 previous = particle.position;
            //particle.age += gl::delta_time_in_seconds();

            //auto forces = glm::vec2{0.f};

            // Gravity
            // forces += glm::vec2{0.f, -1.f} * particle.mass;

            // Air friction
            //forces += -particle.velocity * 1.f;

            // Follow mouse
            // forces += (gl::mouse_position() - particle.position);


            /*glm::vec2 new_pos = particle.position + particle.velocity * gl::delta_time_in_seconds();
            for (auto const &seg : segments)
            {
                // Utilisation correcte de intersect_segments pour détecter une collision avec un segment
                if (auto hit = intersect_segments(previous, new_pos, seg.A, seg.B))
                {
                    // Calcul de la normale du segment
                    glm::vec2 n = seg.normal();

                    // Réflexion de la vélocité de la particule par rapport à la normale
                    particle.velocity = glm::reflect(particle.velocity, glm::normalize(-n));

                    // Mise à jour de la position après la collision, tout en respectant la direction et le temps écoulé
                    new_pos = hit->point + particle.velocity * gl::delta_time_in_seconds();
                }
                particle.position = new_pos;
                particle.velocity += forces / particle.mass * gl::delta_time_in_seconds();
                particle.position += particle.velocity * gl::delta_time_in_seconds();
            }*/

            //std::erase_if(particles, [&](Particle const& particle) { return particle.age > particle.lifespan; });


            /*auto A = glm::vec2(-1, 0);
            auto B = glm::vec2(1, 0);
            auto C = glm::vec2(0, -0.75f);
            auto D = gl::mouse_position();*/

            //if (auto res = intersect_segments(A, B, C, D)) {
                // draw_point(glm::vec2 pos, float size, glm::vec4 const& color)
                //utils::draw_disk(res->point, 0.02f, glm::vec4{0.f, 1.f, 0.f, 1.f});
            //}
        }

        //Line
        //draw_line(glm::vec2 start, glm::vec2 end, float thickness, glm::vec4 const& color)
        //utils::draw_line(glm::vec2(-1,0), glm::vec2(1,0), 0.01, glm::vec4{1.f, 0.f, 0.f, 1.f});
        //utils::draw_line(glm::vec2(0,-0.75), gl::mouse_position(), 0.01, glm::vec4{1.f, 1.f, 1.f, 1.f});
    }
}