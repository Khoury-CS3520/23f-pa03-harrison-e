#ifndef IMAGELIB_IMAGE_H
#define IMAGELIB_IMAGE_H
#include <cmath>
#include <stdlib.h>
#include "PixelVector.h"

using std::string, std::min, std::max, std::rand;

/**
 * @enum representing image file formats (for exporting images)
 *  PNG     represents a .png image
 *  JPG     represents a .jpg/.jpeg image
 *  BMP     represents a .bmp image
 */
typedef enum imageFormat {
    PNG,
    JPG,
    BMP
} format_t;

/**
 * Represents an Image
 *
 * @param x                 The width of the image (# rows)
 * @param y                 The height of the image (# columns)
 * @param componentsUsed    How many components are used in the image
 * @param inputFilepath     The filepath to the input image (relative to this file)
 * @param outputFilepath    The filepath to the output image (relative to this file)
 * @param pixels            Pointer to the first PixelVector in the PixelMatrix
 */
class Image {
private:
    unsigned x; // width
    unsigned y; // height
    channel_t componentsUsed;
    string inputFilepath;
    string outputFilepath;

    // CharArray is an alias for a pointer to an unsigned char
    // Used to interact with STBI
    typedef unsigned char* charArray;

    // PixelMatrix is an alias for a pointer to a PixelVector
    // A.K.A.: a pointer to a pointer to a Pixel
    // Represents, in data, the column of rows of pixels
    typedef PixelVector* PixelMatrix;

    // pointer to array of pixel vectors
    PixelMatrix pixels;

public:
    /////////////////////////
    // constructors (except file read constructor)

    // default
    Image() : x{}, y{}, componentsUsed{GR}, inputFilepath{}, outputFilepath{}, pixels{nullptr} {}

    // constructor for grey gradient (for testing)
    Image(unsigned X, unsigned Y);

    // constructor for solid color image
    Image(unsigned X, unsigned Y, const Pixel& P);

    // destructor
    ~Image();


    /////////////////////////
    // file i/o

    // constructor reads from filepath
    // implies dimensions and components with stbi_info()
    explicit Image(string filepath);

    // (old) constructor reads from ../sampleImages/filename w/ spec components, encodes with specified dimensions & # components
    Image(string filename, unsigned X, unsigned Y, channel_t encodedComponents, channel_t desiredComponents);

    // write an image to ../sampleImages/filename, in specified format
    // NOTE this function adds an appropriate file extension to `filename`
    void write(string filename, format_t format);

    /////////////////////////
    // conversion between uchar[] and pixel[][]

    // read chars from `data` into `this->pixels`
    void expandImage(charArray data);

    // flatten `this->pixels` into `data` to write
    void flattenImage(charArray data, unsigned dataLength);


    /////////////////////////
    // transformations
    void flipHorizontal();
    void flipVertical();
    void rotatePos90();
    void rotateNeg90();
    void addBorder(Pixel borderPx, unsigned width);
    void pointillize();


    /////////////////////////
    // resize / crop
    void resize(unsigned newX, unsigned newY);
    void square();


    /////////////////////////
    // convenience

    // fetch a row at a given X
    // NOTE: columnAt creates a new PixelVector
    // make sure to DELETE this if necessary
    PixelVector & columnAt(unsigned X);

    // fetch a column at a given Y
    PixelVector & rowAt(unsigned Y);
    // const version
    PixelVector & constRowAt(unsigned Y) const;

    unsigned getWidth() const { return this->x; }
    unsigned getHeight() const { return this->y; }
    channel_t getCompUsed() const { return this->componentsUsed; }

    // return average properties of this image
    const Pixel average() const;
    // ranged version
    const Pixel average(unsigned fromX, unsigned toX,
                        unsigned fromY, unsigned toY) const;


    /////////////////////////
    // overloaded operators
    // <<
    friend std::ostream & operator<<(std::ostream& out, const Image& img);
    // []
    PixelVector & operator[](unsigned index);
    // !
    bool operator!();
    // assignment
    Image & operator=(const Image &that);
    // copy ctor
    Image(const Image &that);
};

#endif //IMAGELIB_IMAGE_H
