#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "graphics.h"

class ParticleEmitter {

public:
    ParticleEmitter(Vector3 location = Vector3(),
                    Vector3 color = Vector3(1.0f, 0.5f, 0.2f),
                    Vector3 velocity = Vector3(0.0f, 0.0f, 0.0f),
                    Vector3 force = Vector3(0.0f, 5.f, 0.0f),
                    float scale = .4f,
                    float fuzziness = 30.0f,
                    unsigned maxParticles = 1000);

    ~ParticleEmitter();

    // Particle generator logic
    void draw(Graphics *g);
    void resetParticles();
    virtual void resetParticle(unsigned i) = 0;

    inline void setPosition(const Vector3 &pos) { m_pos = pos; }
    inline void toggle(bool toggle) { m_toggle = toggle; }
	inline void setColor(const Vector3 &col) { m_color = col; }

protected:

    struct __attribute__ ((aligned (16))) Particle {
        bool active;
        float life;
        float decay;
        Vector3 c;
        Vector3 p;
        Vector3 d;
        Vector3 f;
    };

    Particle *m_particles;
    unsigned m_maxParticles;

    float m_fuzziness;
    float m_scale;
    Vector3 m_color;
    Vector3 m_vel;
    Vector3 m_force;
    Vector3 m_pos;
    bool m_toggle;
};


#endif // PARTICLEEMITTER_H
