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
glm::vec2 cacaprout(glm::vec2 origin, float radius)
{
    float x = utils::rand(-1.f, 1.f);
    float y = utils::rand(-1.f, 1.f);
    glm::vec2 offset = glm::vec2(x, y) * radius;

    if (glm::length(offset) <= radius)
    {
        return origin + offset;
    }
    else
    {
        return cacaprout(origin, radius); // récursion si en dehors du cercle
    }
}


void draw_parametric(std::function<glm::vec2(float)> const& parametric)
{
    const int N = 100; // nombre de segments
    const float thickness = .01f;
    const glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f}; // blanc opaque

    glm::vec2 prev = parametric(0.0f);
    for (int i = 1; i <= N; ++i)
    {
        float t = static_cast<float>(i) / N;
        glm::vec2 curr = parametric(t);

        utils::draw_line(prev, curr, thickness, color);
        prev = curr;
    }
}

glm::vec2 bezier3(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t)
{
    glm::vec2 a = glm::mix(p0, p1, t);
    glm::vec2 b = glm::mix(p1, p2, t);
    glm::vec2 c = glm::mix(p2, p3, t);

    glm::vec2 d = glm::mix(a, b, t);
    glm::vec2 e = glm::mix(b, c, t);

    glm::vec2 f = glm::mix(d, e, t);

    return f;
}

glm::vec2 bezier1_casteljau(glm::vec2 p0, glm::vec2 p1, float t)
{
    return glm::mix(p0, p1, t);
}
glm::vec2 bezier2_casteljau(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, float t)
{
    glm::vec2 a = glm::mix(p0, p1, t);
    glm::vec2 b = glm::mix(p1, p2, t);
    return glm::mix(a, b, t);
}
glm::vec2 bezier3_casteljau(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t)
{
    glm::vec2 a = glm::mix(p0, p1, t);
    glm::vec2 b = glm::mix(p1, p2, t);
    glm::vec2 c = glm::mix(p2, p3, t);

    glm::vec2 d = glm::mix(a, b, t);
    glm::vec2 e = glm::mix(b, c, t);

    return glm::mix(d, e, t);
}
glm::vec2 bezier1_bernstein(glm::vec2 p0, glm::vec2 p1, float t)
{
    return (1 - t) * p0 + t * p1;
}
glm::vec2 bezier2_bernstein(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, float t)
{
    float u = 1 - t;
    return u*u * p0 + 2*u*t * p1 + t*t * p2;
}
glm::vec2 bezier3_bernstein(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t)
{
    float u = 1 - t;
    float uu = u * u;
    float tt = t * t;

    return uu * u * p0
         + 3 * uu * t * p1
         + 3 * u * tt * p2
         + tt * t * p3;
}

int main()
{
    gl::init("Particules!");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    //std::vector<Particle> particles(5000);
    //std::vector<Segment> segments;


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

    /*for (auto& particle : particles)
    {
        glm::vec2 xV(0.1f,0.f);
        glm::vec2 yV(0.85f,-0.2f);

        //glm::vec2(utils::rand(0,1)*xV.x,utils::rand(0,1)*yV.y);

        //particle.position = xV*utils::rand(-1.f, 1.f) + yV*utils::rand(-1.f, 1.f);

        glm::vec2 ori (0.,0.);
        float angle = utils::rand(0,360);
        //float rad = utils::rand(0.f, 1.2f);
        float rad = 0.75f;

        glm::vec2 pos = ori + glm::vec2(glm::cos(angle)*rad, glm::sin(angle)*rad);
        particle.position = pos;
        particle.position = cacaprout(ori, rad);
    }*/

    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        /*draw_parametric([](float t) {
            return bezier3({-.3f, -.3f}, {-0.2f, 0.5f}, gl::mouse_position(), {.8f, .5f}, t);
        });
        draw_parametric([](float t) {
    return bezier3_casteljau({-.3f, -.3f}, {-0.2f, 0.5f}, gl::mouse_position(), {.8f, .5f}, t);
});
        draw_parametric([](float t) {
    return bezier1_casteljau(glm::vec2(-0.8f, -0.8f), glm::vec2(0.8f, 0.8f), t);
});
        draw_parametric([](float t) {
    return bezier1_bernstein(glm::vec2(-0.8f, -0.8f), glm::vec2(0.8f, 0.8f), t);
});
        draw_parametric([](float t) {
    glm::vec2 p0 = {-0.8f, -0.6f};
    glm::vec2 p1 = {-0.2f, 0.9f};
    glm::vec2 p2 = gl::mouse_position();
    return bezier2_casteljau(p0, p1, p2, t);
});
        draw_parametric([](float t) {
    glm::vec2 p0 = {-0.8f, -0.6f};
    glm::vec2 p1 = {-0.2f, 0.9f};
    glm::vec2 p2 = gl::mouse_position();
    return bezier2_bernstein(p0, p1, p2, t);
});
        draw_parametric([](float t) {
    glm::vec2 p0 = {-0.6f, -0.6f};
    glm::vec2 p1 = {-0.2f, 0.5f};
    glm::vec2 p2 = gl::mouse_position();
    glm::vec2 p3 = {0.8f, 0.5f};
    return bezier3_casteljau(p0, p1, p2, p3, t);
});*/
        draw_parametric([](float t) {
    glm::vec2 p0 = {-0.6f, -0.6f};
    glm::vec2 p1 = {-0.2f, 0.5f};
    glm::vec2 p2 = gl::mouse_position();
    glm::vec2 p3 = {0.8f, 0.5f};
    return bezier3_bernstein(p0, p1, p2, p3, t);
});


        //for (auto const& particle : particles) utils::draw_disk(particle.position, particle.radius(), glm::vec4{particle.color(), 1.f});
        //for (auto& particle : particles)
        //{
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
        //}

        //Line
        //draw_line(glm::vec2 start, glm::vec2 end, float thickness, glm::vec4 const& color)
        //utils::draw_line(glm::vec2(-1,0), glm::vec2(1,0), 0.01, glm::vec4{1.f, 0.f, 0.f, 1.f});
        //utils::draw_line(glm::vec2(0,-0.75), gl::mouse_position(), 0.01, glm::vec4{1.f, 1.f, 1.f, 1.f});
    }
}
