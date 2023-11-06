#include "PixelVector.h"


/////////////////////////
// constructors & destructor

// default
PixelVector::PixelVector() {
    length = 0;
    capacity = 2;
    vec = new Pixel[capacity];
    componentsUsed = GR;
}

// destructor
PixelVector::~PixelVector() {
    delete[] vec;
}

// specified capacity and components
PixelVector::PixelVector(unsigned capacity, channel_t componentsUsed) {
    vec = nullptr;
    this->capacity = capacity;
    vec = new Pixel[capacity];
    length = 0;
    this->componentsUsed = componentsUsed;
}

// copy ctor
PixelVector::PixelVector(const PixelVector &that) {
    this->capacity = that.capacity;
    this->length = that.length;
    this->componentsUsed = that.componentsUsed;
    vec = new Pixel[capacity];
    for (unsigned p = 0; p < capacity; p++)
        this->vec[p] = that.vec[p];
}

/////////////////////////
// overloaded ops
// = (copy assignment)
PixelVector& PixelVector::operator=(const PixelVector &that) {
    if (this != &that) {
        delete[] this->vec;
        this->capacity = that.capacity;
        this->length = that.length;
        this->componentsUsed = that.componentsUsed;
        this->vec = new Pixel[capacity];
        for (unsigned p = 0; p < capacity; p++)
            this->vec[p] = that.vec[p];
    }
    return *this;
}

// []
Pixel& PixelVector::operator[](unsigned index) {
    if (index < length)
        return vec[index];
}

// <<
ostream& operator<<(ostream& out, const PixelVector& v) {
    for(unsigned i = 0; i < v.length; i++) {
        out << v.pixelAt(i) << " ";
    }
    return out;
}


/////////////////////////
// transformations

void PixelVector::resize(unsigned delta, unsigned offset) {
    if (delta != 0 && offset <= delta) {
        Pixel* temp = new Pixel[capacity + delta];
        for (unsigned p = 0; p < this->capacity; p++)
            temp[p + offset] = this->vec[p];
        delete[] this->vec;
        this->vec = temp;
        this->capacity += delta;
    }
}

// push pixel to back of vec
void PixelVector::pushBack(const Pixel &pixel) {
    if (length >= capacity)
        this->resize(capacity, 0);
    vec[length] = pixel;
    length++;
}

// reverse this vec
void PixelVector::reverse() {
    Pixel* rev = new Pixel [this->capacity];
    for (unsigned p = 0; p < this->length; p++)
        rev[p] = this->vec[this->length - p - 1];
    delete[] this->vec;
    this->vec = rev;
}

/////////////////////////
// getters

// const ref to pixel at index
Pixel& PixelVector::pixelAt(unsigned index) const {
    if (index < length)
        return vec[index];
}