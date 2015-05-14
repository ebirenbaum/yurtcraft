#ifndef PARTICLEFIREBALL_H
#define PARTICLEFIREBALL_H

#include "particleemitter.h"

class ParticleFireball : public ParticleEmitter {

public:
    ParticleFireball(Vector3 location = Vector3(),
                     Vector3 color = Vector3(1,.25,.05),
                     float radius = 0.4f);

    virtual ~ParticleFireball();
    void resetParticle(unsigned i);

    float radius();
    void setRadius(float radius);
    void explode();

private:
    float m_inner, m_outer, m_maxLife;
};

#endif // PARTICLEFIREBALL_H
