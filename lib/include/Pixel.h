#ifndef IMAGELIB_PIXEL_H
#define IMAGELIB_PIXEL_H
#include <iostream>
#include <string>
using std::cerr; // for debugging
using std::endl, std::ostream;
using std::string;

/**
 * @enum representing how many components are used in a pixel, or an image
 *  GR   represents a 1-component, grayscale pixel/image
 *  GRa  represents a 2-component, grayscale pixel/image with opacity
 *  RGB  represents a 3-component, color pixel/image
 *  RGBa represents a 4-component, color pixel/image with opacity
 */
typedef enum channelType {
    GR   = 1,
    GRa  = 2,
    RGB  = 3,
    RGBa = 4
} channel_t;

/**
 * Represents a single pixel of an Image
 *
 * @param r The red (R) value of a RGB(a) pixel, or the gray value of an GR(a) pixel
 * @param g The green (G) value of a RGB(a) pixel
 * @param b The blue (B) value of a RGB(a) pixel
 * @param a The alpha (a) value of a GRa/RGBa pixel
 * @param componentsUsed The type of channel (number of components) used in this pixel
 */
class Pixel {
private:
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    channel_t componentsUsed;
public:
    // constructors
    Pixel() = default;
    explicit Pixel(unsigned char gr);
    Pixel(unsigned char gr, unsigned char a);
    Pixel(unsigned char r, unsigned char g,
          unsigned char b);
    Pixel(unsigned char r, unsigned char g,
          unsigned char b, unsigned char a);

    // copy constructor
    Pixel(const Pixel &copyMe);

    // destructor
    ~Pixel() = default;

    // copy assignment operator
    Pixel & operator=(const Pixel &);

    // getters
    unsigned char getR() const { return r; }
    unsigned char getG() const { return g; }
    unsigned char getB() const { return b; }
    unsigned char getA() const { return a; }

    // returns intensity of this pixel (for pointillization)
    unsigned char getIntensity() const;

    // comparison
    bool operator==(const Pixel& that) const;
    bool operator<(const Pixel& that) const;
    bool operator>(const Pixel& that) const;

    // color difference using redmean algorithm
    double operator-(const Pixel& that) const;

    // stream insertion (for display)
    friend std::ostream& operator<<(std::ostream& out, const Pixel& px);
};

#endif //IMAGELIB_PIXEL_H
