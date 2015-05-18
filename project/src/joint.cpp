#include "joint.h"

#include "graphics.h"

Joint::Joint(string _id, Vector3 _relativePosition, Rotation *_rotation1, Rotation *_rotation2, Rotation *_rotation3, bool _isRoot){
    isRoot = _isRoot;

    id = _id;

    relativePos = _relativePosition;
    rotation1 = _rotation1;
    rotation2 = _rotation2;
    rotation3 = _rotation3;

    if (isRoot){
        rootListOfChildren = new map<string, Joint*>();
    } else {
        rootListOfChildren = NULL;
    }
}

Joint::~Joint(){
    delete rotation1;
    delete rootListOfChildren;

    for (int i = 0; i < children.size(); i++){
        delete children.at(i);
    }
}

void Joint::paint(Graphics *g){
    glPushMatrix();

    g->translate(relativePos);

    if (rotation1 != NULL){
        glRotatef(rotation1->degrees, rotation1->axis.x, rotation1->axis.y, rotation1->axis.z);
    }

    if (rotation2 != NULL){
        glRotatef(rotation2->degrees, rotation2->axis.x, rotation2->axis.y, rotation2->axis.z);
    }

    if (rotation3 != NULL){
        glRotatef(rotation3->degrees, rotation3->axis.x, rotation3->axis.y, rotation3->axis.z);
    }

    paintAttachedObjects(g);

    for (int i = 0; i < children.size(); i++){
        children.at(i)->paint(g);
    }

    glPopMatrix();
}

void Joint::paintAttachedObjects(Graphics *g){

}

Joint* Joint::addChild(Joint *_child){
    if (_child == this){ // still can't prevent indirect circular paths
        cerr << "--Joint Error: cannot add joint to its own children." << endl;
        return NULL;
    }
    children.push_back(_child);

    if (isRoot){
        (*rootListOfChildren)[_child->id] =  _child;
    } else {
        // should be adding to root's list...
    }

    return _child; // to allow joints to be added to rigs and lists at the same time
}
