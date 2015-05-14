#ifndef BLOCKDEF_H
#define BLOCKDEF_H

#include "vector.h"

enum Direction {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    FRONT,
    BACK
};

struct Block {
    Block() : type(0), mask(0xFF) {}
    Block(char type, bool transparent, bool passable) : type(type) {
        mask = 0xFF & (transparent ? 0xFF : ~TRANS) & (passable ? 0xFF : ~PASS);
    }

    virtual ~Block() {}

    inline bool isTransparent() const { return mask & TRANS; }
    inline bool isPassable() const { return mask & PASS; }

    inline void setTransparent(bool b) {
        if (b) mask |= TRANS;
        else mask &= ~TRANS;
    }
    inline void setPassable(bool b) {
        if (b) mask |= PASS;
        else mask &= ~PASS;
    }

    inline bool shouldDrawTop() const { return mask & TOP; }
    inline bool shouldDrawBottom() const { return mask & BOTTOM; }
    inline bool shouldDrawLeft() const { return mask & LEFT; }
    inline bool shouldDrawRight() const { return mask & RIGHT; }
    inline bool shouldDrawFront() const { return mask & FRONT; }
    inline bool shouldDrawBack() const { return mask & BACK; }

    inline void setDrawTop(bool b) {
        if (b) mask |= TOP;
        else mask &= ~TOP;
    }
    inline void setDrawBottom(bool b) {
        if (b) mask |= BOTTOM;
        else mask &= ~BOTTOM;
    }
    inline void setDrawLeft(bool b) {
        if (b) mask |= LEFT;
        else mask &= ~LEFT;
    }
    inline void setDrawRight(bool b) {
        if (b) mask |= RIGHT;
        else mask &= ~RIGHT;
    }
    inline void setDrawFront(bool b) {
        if (b) mask |= FRONT;
        else mask &= ~FRONT;
    }
    inline void setDrawBack(bool b) {
        if (b) mask |= BACK;
        else mask &= ~BACK;
    }

    unsigned char type;
    unsigned char mask;

private:
    static const unsigned char
        TOP = 0x01, BOTTOM = 0x02,
        LEFT = 0x04, RIGHT = 0x08,
        FRONT = 0x10, BACK = 0x20,
        TRANS = 0x40, PASS = 0x80;
};

struct BlockDef {
    BlockDef() : drawn(false), block(Block()) {}
    BlockDef(const Block &block) : drawn(false), block(block) {}

    BlockDef(const Block &block, const Vector4 &coords) : drawn(true), block(block) {
        for (int i = 0; i < 6; i++) {
            textureCoords[i] = coords;
        }
    }

    BlockDef(const Block &block, const Vector4 &top, const Vector4 &bottom, const Vector4 &side) : drawn(true), block(block) {
        textureCoords[TOP] = top;
        textureCoords[BOTTOM] = bottom;
        for (int i = LEFT; i < 6; i++) {
            textureCoords[i] = side;
        }
    }

    bool drawn;
    Block block;
    Vector4 textureCoords[6];
};

#endif // BLOCKDEF_H
