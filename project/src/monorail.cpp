#include "monorail.h"

#include "graphics.h"
#include "biometile.h"
#include "player.h"
#include "voxelsystem.h"
#include "biomemanager.h"
#include "noise.h"

Monorail::Monorail(VoxelSystem *_voxel, McChunkFactory *_factory, Player *_passenger, Vector3 _pos) : Entity(_pos, Vector3(0, 0, 0))
{
    m_collidesTerrain = false;
    m_gravable = false;

    factory = _factory;
    voxel = _voxel;
    currentGoalPointIndex = 0;
    
    
    speed = 10;
    baseDirection = Vector3(1, 0, 0);
    timeToNextGoal = 0;

    noise = new Noise(1928345);

    passenger = _passenger;
    passenger->m_gravable = false;
    passenger->m_collidesTerrain = false;
    m_pos.y = factory->getBiomeInfo(m_pos)->primaryBlockHeight + 20;

    Vector3 _startPosition = m_pos;
    Vector3 _startDirection = Vector3(1, 1, 0);

    monorailGoalPoints.push_back(_startPosition);
    monorailGoalPoints.push_back(_startPosition + _startDirection*16);

    addToPath();
    
}

void Monorail::addToPath(){
    if (monorailGoalPoints.size() < 2) {
        cerr << "monorail path doesn't have enough points to extrapolate" << endl;
        return;
    }

    Vector3 _lastPoint = monorailGoalPoints.back();
    Vector3 _secondToLastPoint = monorailGoalPoints.at(monorailGoalPoints.size()-2);

    int _pointsSinceTurn = 0;
    for (int i = 0; i < 20; i++){
        _pointsSinceTurn++;
        Vector3 _nextPoint = _lastPoint + baseDirection * 6;
        
        if (_pointsSinceTurn > 3){
            int _noise = (int)(noise->perlin2(Vector2(_nextPoint.x, _nextPoint.z), 1))%10 - 5;
            _nextPoint += (Vector3(1, 0, 1) - baseDirection)*_noise;
            _pointsSinceTurn = 0;
        }

        _nextPoint.y = factory->getBiomeInfo(_nextPoint)->primaryBlockHeight;
        for (int ty = 0; ty < 100; ty++){
            bool _passable = voxel->queryBlock(Vector3(_nextPoint.x, _nextPoint.y + ty, _nextPoint.z)).isPassable();
            if (_passable){
                _nextPoint.y += ty + 20;
                break;
            }
        }
        if (_nextPoint.y < 30) _nextPoint.y = 30;

//        cout << "adding goal " << _nextPoint << endl;

        monorailGoalPoints.push_back(_nextPoint);
        _secondToLastPoint = _lastPoint;
        _lastPoint = _nextPoint;
        
    }
}


void Monorail::tick(float _seconds){

    if (passenger != NULL){
        timeToNextGoal -= _seconds;
        Vector3 _currentGoal = monorailGoalPoints.at(currentGoalPointIndex);
        if (timeToNextGoal <= 0 || m_pos.distanceSquared(_currentGoal) < 1){
//            cout << "moving on when "<<m_pos.distanceTo(_currentGoal) << " dist away from goal" << endl;
            currentGoalPointIndex++;
            if (currentGoalPointIndex >= monorailGoalPoints.size()){
                addToPath();
            }
            Vector3 _nextGoal = monorailGoalPoints.at(currentGoalPointIndex);
            float _distBetweenGoals = _nextGoal.distanceTo(_currentGoal);
            m_vel = (_nextGoal - _currentGoal)/_distBetweenGoals * speed; // normalize direction then multiply by speed
            timeToNextGoal = (_distBetweenGoals/m_vel.length())+.5;
//            cout << "next goal: " << _nextGoal << endl;
        }
        
        m_pos += m_vel*_seconds;
        passenger->m_pos = m_pos + passenger->m_dim;
        passenger->m_pos.y += m_dim.y/2;
    }
    
}

void Monorail::draw(Graphics *g){
    return;
    glPushMatrix();
    glColor3f(1, 1, 1);
    g->setupTexture("particle");
    g->translate(m_pos);
    g->scale(Vector3(5, 1, 5));
    g->renderCube();
    glPopMatrix();


    glPushMatrix();
    glColor3f(1, 1, 1);
    g->setupTexture("particle");
    g->translate(monorailGoalPoints.at(currentGoalPointIndex));
    g->scale(Vector3(1, 1, 1));
    g->renderCube();
    glPopMatrix();

}
