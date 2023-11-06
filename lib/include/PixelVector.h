#ifndef IMAGELIB_PIXELVECTOR_H
#define IMAGELIB_PIXELVECTOR_H
#include "Pixel.h"

/**
 * Represents a vector of Pixels
 * In the context of an Image, can represent either a row or a column
 *
 * @param vec               A pointer to the first Pixel of the vector
 * @param length            How many elements are currently in the vector
 * @param capacity          How many elements can fit in the vector before resizing
 * @param componentsUsed    How many components are used in the vector
 */
class PixelVector {
private:
    Pixel* vec {};
    unsigned length;
    unsigned capacity;
    channel_t componentsUsed;

public:
    /////////////////////////
    // constructors & destructor
    PixelVector();
    ~PixelVector();

    // 2-arg ctor
    PixelVector(unsigned capacity, channel_t componentsUsed);

    // copy ctor
    PixelVector(const PixelVector &copyMe);

    /////////////////////////
    // overloaded ops
    PixelVector& operator=(const PixelVector&);
    Pixel& operator[](unsigned index);
    friend std::ostream& operator<<(std::ostream& out, const PixelVector& v);


    /////////////////////////
    // transformations
    // resize
    // delta -> how much to change capacity by
    // offset -> by how much do we shift existing elements
    void resize(unsigned delta, unsigned offset);

    // push to back
    void pushBack(const Pixel &pixel);

    // reverse this vec
    void reverse();

    /////////////////////////
    // getters

    // pixel at (const ref)
    Pixel& pixelAt(unsigned index) const;
};

#endif //IMAGELIB_PIXELVECTOR_H
