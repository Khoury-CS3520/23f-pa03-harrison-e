#include "Pixel.h"
#include <cmath>
using std::max, std::min;

Pixel::Pixel(unsigned char gr) {
    this->r = this->b = this->g = gr;
    this->a = 255;
    this->componentsUsed = channel_t(1);
}

Pixel::Pixel(unsigned char gr,
             unsigned char a) {
    this->r = this->b = this->g = gr;
    this->a = a;
    this->componentsUsed = channel_t(2);
}

Pixel::Pixel(unsigned char r,
             unsigned char g,
             unsigned char b) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 255;
    this->componentsUsed = channel_t(3);
}

Pixel::Pixel(unsigned char r,
             unsigned char g,
             unsigned char b,
             unsigned char a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
    this->componentsUsed = channel_t(4);
}

Pixel::Pixel(const Pixel &copyMe) {
    this->r = copyMe.r;
    this->g = copyMe.g;
    this->b = copyMe.b;
    this->a = copyMe.a;
    this->componentsUsed = copyMe.componentsUsed;
}

unsigned char Pixel::getIntensity() const {
    switch (componentsUsed) {
        case GR:
            return r;
        case GRa:
            return (r + a) / 2;
        case RGB:
            return (r + g + b) / 3;
        case RGBa:
            return (r + g + b + a) / 4;
    }
}

Pixel & Pixel::operator=(const Pixel & p) {
    if (this != &p) {
        this->componentsUsed = p.componentsUsed;
        this->r = p.r;
        this->g = p.g;
        this->b = p.b;
        this->a = p.a;
    }

    return *this;
}

ostream& operator<<(ostream& out, const Pixel& px) {
    switch (px.componentsUsed) {
        case GR:
            out << +px.r;
            break;
        case GRa:
            out << +px.r << ", " << +px.a;
            break;
        case RGB:
            out << +px.r << ", " << +px.g << ", " << +px.b;
            break;
        case RGBa:
            out << +px.r << ", " << +px.g << ", " << +px.b << ", " << +px.a;
    }
    return out;
}

bool Pixel::operator<(const Pixel& that) const {
    return r < that.r
        || g < that.g
        || b < that.b;
}

bool Pixel::operator>(const Pixel& that) const {
    return r > that.r
        || g > that.g
        || b > that.b;
}

bool Pixel::operator==(const Pixel& that) const {
    return r == that.r
        && g == that.g
        && b == that.b
        && a == that.a;
}

double Pixel::operator-(const Pixel &that) const {
    double r_ = (double(this->r) + double(that.r)) / 2;
    double delta_R = (double(this->r) - that.r);
    double delta_G = (double(this->g) - that.g);
    double delta_B = (double(this->b) - that.b);
    double delta_C = sqrt(
                ((2 + (r_ / 256)) * pow(delta_R, 2)) +
                (4 * pow(delta_G, 2)) +
                ((2 + ((255 - r_) / 256)) * pow(delta_B, 2))
            );
    return delta_C;
}