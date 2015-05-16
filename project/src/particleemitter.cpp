#include "particleemitter.h"

ParticleEmitter::ParticleEmitter(Vector3 location, Vector3 color, Vector3 velocity,
                                 Vector3 force, float scale, float fuzziness, unsigned maxParticles) :
                    m_maxParticles(maxParticles), m_pos(location),
                    m_fuzziness(fuzziness), m_scale(scale), m_color(color), m_vel(velocity),
                    m_force(force), m_toggle(true) {
    m_particles = new Particle[maxParticles];

    for (unsigned i = 0; i < m_maxParticles; ++i)
        m_particles[i].active = false;
}

ParticleEmitter::~ParticleEmitter() {
    if (m_particles) {
        delete[] m_particles;
        m_particles = 0;
    }
}

void ParticleEmitter::resetParticles() {
    for (unsigned i = 0; i < m_maxParticles; i++)
        resetParticle(i);
}

void ParticleEmitter::draw(Graphics *g) {
//    GLint min, mag;
//    glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &min);
//    glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &mag);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    g->setupTexture("particle");
    glDepthMask(false);
    glDisable(GL_LIGHTING);
    g->enableBlend();
    glDisable(GL_CULL_FACE);

    float m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    Vector3 dx = Vector3(m[0], m[4], m[8]); // left-right
    Vector3 dy = Vector3(m[1], m[5], m[9]); // up-down
    Vector3 dz = Vector3(m[2], m[6], m[10]); // front-back

    glBegin(GL_QUADS);
    for (unsigned i = 0; i < m_maxParticles; i++) {
        Particle &p = m_particles[i];
        if (!p.active && m_toggle) {
            p.active = true;
            resetParticle(i);
        }

        if (p.active) {
            p.p = p.p + p.d * .017;
            p.d = p.d + p.f * .017;
            p.life = p.life - p.decay;

            if (p.life < 0) {
                p.active = false;
            }

            glColor4f(p.c.x, p.c.y, p.c.z, sqrt(p.life));
            float scale = m_scale * (p.life * p.life + .2);
            Vector3 ddx = dx * scale;
            Vector3 ddy = dy * scale;

            glTexCoord2f(0,1);
            glVertex3fv((p.p-ddx+ddy).xyz);
            glTexCoord2f(1,1);
            glVertex3fv((p.p+ddx+ddy).xyz);
            glTexCoord2f(1,0);
            glVertex3fv((p.p+ddx-ddy).xyz);
            glTexCoord2f(0,0);
            glVertex3fv((p.p-ddx-ddy).xyz);
        }
    }
    glEnd();

    glEnable(GL_CULL_FACE);
    g->disableBlend();
    glEnable(GL_LIGHTING);
    glDepthMask(true);
    g->teardownTexture();
}
