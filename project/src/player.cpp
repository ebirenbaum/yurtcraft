#include "player.h"
#include "fireball.h"

Player::Player(const Vector3 &pos, VoxelSystem *system, VrCamera *cam, VrData *data)
    : Entity(pos, Vector3(WIDTH, HEIGHT, WIDTH)), m_voxels(system), m_camera(cam), m_data(data) {
    m_canJump = false;
    m_block = GRASS;
    m_speed = 12;
    m_life = 5;
}

Player::~Player() {
    delete m_camera;
}

void Player::tick(float seconds) {
    Entity::tick(seconds);
    m_camera->setEye(m_pos + Vector3(0, HEIGHT * .8f, 0));
}

void Player::draw(Graphics *g) {
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1,1,1);
    glScalef(.01,.01,.01);

    glTranslatef(1.3,-.8,-3);
    glScalef(.8,.8,.8);
    BlockDef chosen = m_voxels->getBlockDef(m_block);
    Vector4 *vecs = new Vector4[6];
    for (int i = 0; i < 6; i++) {
        vecs[i] = chosen.textureCoords[i];
    }

    glDisable(GL_LIGHTING);
    //g->renderTexturedCube("atlas", vecs);
    glEnable(GL_LIGHTING);

    delete[] vecs;

    glPopMatrix();
}

void Player::drawHUD(Graphics *g, const Vector2 &dim) {
    Block bl = m_voxels->queryBlock(m_camera->getEye());
    if (bl.type == WATER) {
        g->enableBlend();
        glColor4f(0,0,1,.5);
        glBegin(GL_QUADS);
        glVertex2f(0,0);
        glVertex2f(dim.x,0);
        glVertex2f(dim.x, dim.y);
        glVertex2f(0,dim.y);
        glEnd();
        g->disableBlend();
    }

    glColor3f(1,1,1);
    g->renderCompass(getHorizontalLook(), dim);
}

void Player::determineVelocity(float seconds) {
    Entity::determineVelocity(seconds);
    float move = 0, strafe = 0;

    if (m_life >= 0) {
        if (m_heldKeys.find("W") != m_heldKeys.end()) move += 1;
        if (m_heldKeys.find("S") != m_heldKeys.end()) move -= 1;
        if (m_heldKeys.find("A") != m_heldKeys.end()) strafe += 1;
        if (m_heldKeys.find("D") != m_heldKeys.end()) strafe -= 1;
    }

    Vector3 aim = m_camera->getHorizontalLook();
    Vector3 horizontal = Vector3(aim.z, 0, -aim.x);

    Vector3 goalVel = move * aim + strafe * horizontal;
    if (goalVel != Vector3(0,0,0)) {
        goalVel = goalVel.unit();
    }

    goalVel *= m_speed;
    m_vel.x = goalVel.x; m_vel.z = goalVel.z;

    //    float speed = 10;
    //    if (_canJump) {
    //        goalVel *= speed;
    //        Vector3 curr = Vector3(_vel.x, 0, _vel.z);

    //        accelerate((goalVel - curr) * speed);
    //    } else if (goalVel.dot(Vector3(_vel.x, 0, _vel.z)) < 10) {
    //        accelerate(goalVel * speed);
    //    }

    if (m_heldKeys.find("SPACE") != m_heldKeys.end() && m_canJump && m_life >= 0) {
        m_vel.y += m_speed;
        m_canJump = false;
        m_heldKeys.erase("SPACE");
    }
}

void Player::resetJump() {
    m_canJump = true;
}


void Player::collideVoxel(const VoxelCollision &voxel) {
    if (voxel.mtv == Vector3(0,1,0)) {
        resetJump();
    }
}

void Player::mousePressed(MouseEvent *event) {
}

void Player::mouseReleased(MouseEvent *event) {
}

void Player::mouseMoved(const Vector2 &delta) {
    if (delta.x) m_camera->yaw(delta.x * .2);
    if (delta.y) m_camera->pitch(delta.y * .2);
}

void Player::mouseWheeled(int delta) {
    m_block += (delta > 0 ? -1 : 1);
    if (m_block <= (int)WATER) m_block = (int)WATER + 1;
    if (m_block >= (int)NUM_BLOCK_TYPES) m_block = (int)NUM_BLOCK_TYPES - 1;
}

void Player::keyPressed(const string &key) {
    m_heldKeys.insert(key);

    if (key == "H") {
        m_speed = 36 - m_speed;
    }
}

void Player::wandButtonPressed(WandButton button)
{
	
}

void Player::wandButtonReleased(WandButton button)
{

}

void Player::collideCylinder(const Vector3 &mtv, Entity *other)
{
	if (Fireball *fireball = dynamic_cast<Fireball *>(other)) {

		if (!fireball->m_friendly) {
			m_life--;
//cout << "player hit" << endl;
		}
	}
}

void Player::keyReleased(const string &key) {
    m_heldKeys.erase(key);
}

void Player::resize(float aspectRatio) {
//    m_camera->setAspectRatio(aspectRatio);
}

Vector3 Player::getHorizontalLook() {
    return m_camera->getHorizontalLook();
}

Ray Player::getRay() {
    return Ray(m_camera->getEye(), m_camera->getLook());
}
