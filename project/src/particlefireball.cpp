#include "particlefireball.h"

ParticleFireball::ParticleFireball(Vector3 location, Vector3 color, float radius)
    : ParticleEmitter(location, color, Vector3(), Vector3(0.0f, 5.f, 0.0f), .2f,
                      25.0f, 200), m_inner(radius * .2), m_outer(radius * 1.2), m_maxLife(1), m_exploded(false) {
}

ParticleFireball::~ParticleFireball() {
}

float ParticleFireball::radius() { return m_inner * 5; }

void ParticleFireball::setRadius(float radius){
    m_inner = radius * .2;
    m_outer = radius * 1.2;
}

void ParticleFireball::resetParticle(unsigned i) {
    Particle &p = m_particles[i];

    // Make particles spawn in a random sphere around the center location
    p.p = Vector3::randomDirection();
    p.p *= urand(m_inner, m_outer);

    p.f.x = -p.p.x * .3;
    p.f.y = m_exploded ? 7 : 0;
    p.f.z = -p.p.z * .3;

    p.d.x = m_exploded ? p.p.x * 7 : -p.p.z;
    p.d.y = m_exploded ? 5 : .3;
    p.d.z = m_exploded ? p.p.z * 7 : p.p.x;

    // Add the location of the center
    p.p += m_pos;

    p.life = m_maxLife;
    p.decay = urand(.04f, .1f);
    p.c = m_color;
}

void ParticleFireball::explode(){
    m_maxLife = 1.5;
    m_exploded = true;

    /*int num = 1000;
    Particle * moarParticles = new Particle[m_maxParticles + num];

    memcpy(moarParticles, m_particles, m_maxParticles * sizeof(Particle));

    for (unsigned i = m_maxParticles; i < m_maxParticles+num; ++i) {
        m_particles[i].active = false;
    }

    delete[] m_particles;
    m_particles = moarParticles;
    m_maxParticles += num;
*/
}
