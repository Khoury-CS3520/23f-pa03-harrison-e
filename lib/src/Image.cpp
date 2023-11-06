#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../stb/stb_image.h"
#include "../stb/stb_image_write.h"
#include "../stb/stb_image_resize2.h"


/////////////////////////
// constructors & destructor

Image::~Image() {
    delete[] pixels;
}

// make a grey gradient image of specified dimensions
Image::Image(const unsigned X, const unsigned Y) {
    this->x = X;
    this->y = Y;
    this->componentsUsed = GR;
    this->pixels = new PixelVector [Y];
    Pixel p = Pixel(100);
    Pixel* pp = nullptr;

    for (unsigned y = 0; y < this->y; y++) {
        Pixel pn = Pixel(p.getR() + 5*y);
        pp = &pn;
        for (unsigned x = 0; x < this->x; x++) {
            auto pnn = Pixel(pp->getR() + 5);
            pp = &pnn;
            pixels[y].pushBack(*pp);
        }
    }
}

// make an image of specified dimensions, with specified pixel
Image::Image(const unsigned X, const unsigned Y, const Pixel& P) {
    this->x = X;
    this->y = Y;
    this->componentsUsed = RGB;
    this->pixels = new PixelVector [Y];

    for (unsigned y = 0; y < this->y; y++) {
        for (unsigned x = 0; x < this->x; x++) {
            pixels[y].pushBack(P);
        }
    }
}


/////////////////////////
// file i/o
// read constructor (old)
Image::Image(string filename, const unsigned X, const unsigned Y, const channel_t encodedComponents, const channel_t desiredComponents) {
    int width = int(X);
    int height = int(Y);
    int encoded = int(encodedComponents);
    int n = int(desiredComponents);
    inputFilepath = "../sampleImages/" + filename;
    charArray data = stbi_load(inputFilepath.c_str(), &width, &height, &encoded, n);

    if (!data) {
        cerr << "Could not read image from file: " << inputFilepath << endl;
        this->pixels = nullptr;
    } else {
        this->x = X;
        this->y = Y;
        this->componentsUsed = desiredComponents;
        this->pixels = new PixelVector [Y];
        expandImage(data);
    }

    stbi_image_free(data);
}

// 1-arg read ctor
Image::Image(string filepath) {
    int w, h, comp;
    inputFilepath = filepath;
    int info = stbi_info(inputFilepath.c_str(), &w, &h, &comp);
    if (info != 1) {
        cerr << "info: " << info << endl;
        cerr  << "File format not supported: " << inputFilepath << endl;
        pixels = nullptr;
    } else {
        charArray data = stbi_load(inputFilepath.c_str(), &w, &h, &comp, comp);
        if (!data) {
            cerr << "Could not read image from file: " << inputFilepath << endl;
            this->pixels = nullptr;
        } else {
            // img properties read well, img loaded well
            this->x = w;
            this->y = h;
            this->componentsUsed = channel_t (comp);
            this->pixels = new PixelVector [h];
            expandImage(data);
        }
        stbi_image_free(data);
    }
}

void Image::write(string filepath, format_t format) {
    outputFilepath = filepath;
    int X = int(x);
    int Y = int(y);
    int N = int(componentsUsed);
    int stride = N * X;
    unsigned dataLength = X * Y * N;
    charArray data = new unsigned char [dataLength];
    flattenImage(data, dataLength);

    // convert from pixels[][] to data[]
    switch (format) {
        case PNG:
            stbi_write_png((outputFilepath + ".png").c_str(), X, Y, N, data, stride);
            break;
        case JPG:
            stbi_write_jpg((outputFilepath + ".jpg").c_str(), X, Y, N, data, 100);
            break;
        case BMP:
            stbi_write_bmp((outputFilepath + ".bmp").c_str(), X, Y, N, data);
            break;
    }

    delete[] data;
}



/////////////////////////
// transformations
void Image::flipHorizontal() {
    for (unsigned y = 0; y < this->y; y++) {
        PixelVector* r = &this->rowAt(y);
        r->reverse();
    }
}

void Image::flipVertical() {
    // very similar implementation to PixelVector::reverse()
    PixelVector* rev = new PixelVector [this->y];
    for (unsigned y = 0; y < this->y; y++)
        rev[y] = this->constRowAt(this->y - y - 1);
    delete[] this->pixels;
    this->pixels = rev;
}

void Image::rotatePos90() {
    unsigned temp_y = this->x;
    PixelVector* temp = new PixelVector [temp_y];
    for (unsigned i = 0; i < temp_y; i++) {
        temp[i] = this->columnAt(i);
        temp[i].reverse();
    }
    this->x = this->y;
    this->y = temp_y;
    delete[] this->pixels;
    this->pixels = temp;
}

void Image::rotateNeg90() {
    unsigned temp_y = this->x;
    PixelVector* temp = new PixelVector [temp_y];
    for (unsigned i = 0; i < temp_y; i++)
        temp[i] = this->columnAt(temp_y - i - 1);
    this->x = this->y;
    this->y = temp_y;
    delete[] this->pixels;
    this->pixels = temp;
}

void Image::addBorder(Pixel borderPx, unsigned width) {
    unsigned new_y = width + this->y + width;
    unsigned new_x = width + this->x + width;
    PixelVector* temp = new PixelVector [new_y];
    for (unsigned i = 0; i < new_y; i++) {
        for (unsigned j = 0; j < new_x; j++) {
            if (i < width || j < width
            ||  i > (new_y - width - 1) || j > (new_x - width - 1)) {
                temp[i].pushBack(borderPx);
            } else {
                temp[i].pushBack(this->pixels[i - width][j - width]);
            }
        }
    }
    this->x = new_x;
    this->y = new_y;
    delete[] this->pixels;
    this->pixels = temp;
}

void Image::pointillize() {
    // calculate radius range and # dots based on image size
    unsigned int nDots = (x * y) / 12;
    unsigned int rMax  = min(int(min(x, y)) / 15, 99);
    unsigned int rMin  = max(int(rMax) / 2, 3);
    float intensityToR = (float(rMax) - float(rMin)) / 255;

    PixelVector* temp = new PixelVector [y];    // pointillism "canvas"
    for (unsigned y = 0; y < this->y; y++)
        temp[y] = this->rowAt(y);

    // randomly place dots, dot radius based on range & intensity
    for (int dot = 0; dot < nDots; dot++) {
        unsigned dotX = rand() % x;
        unsigned dotY = rand() % y;
        Pixel dotCopy = pixels[dotY][dotX];
        unsigned intensity = max(int(min(int(dotCopy.getIntensity()), 223)), 111);
        unsigned dotR = unsigned (intensityToR * intensity);
        for (int pointY = dotY - dotR; pointY <= dotY + dotR; pointY++) {
            for (int pointX = dotX - dotR; pointX <= dotX + dotR; pointX++) {
                float distance = sqrt(pow((float(pointX) - float(dotX)), 2) + pow((float(pointY) - float(dotY)), 2));
                if (pointX >= 0 && pointY >= 0 && pointX <= x - 1 && pointY <= y - 1
                && distance <= dotR) {
                    temp[pointY][pointX] = dotCopy;
                }
            }
        }
    }
    delete[] this->pixels;
    this->pixels = temp;
}

void Image::resize(unsigned newX, unsigned newY) {
    int oldX = int(x);
    int oldY = int(y);
    int N = int(componentsUsed);
    int oldStride = N * oldX;
    unsigned oldDataLength = N * oldX * oldY;
    charArray oldData = new unsigned char [oldDataLength];
    flattenImage(oldData, oldDataLength);  // flatten original image
    int newStride = N * newX;
    charArray newData = stbir_resize_uint8_srgb(oldData, oldX, oldY, oldStride,
                                                0, newX, newY, newStride,
                                                (stbir_pixel_layout) N);
    if (!newData) {
        cerr << "Could not resize image" << endl;
    } else {
        delete[] pixels;
        x = newX;
        y = newY;
        pixels = new PixelVector [newY];
        expandImage(newData);
        delete[] newData;
    }
    delete[] oldData;
}

void Image::square() {
    if (x != y) {
        unsigned sq_len = min(x, y);
        PixelVector* square = new PixelVector [sq_len];
        unsigned diff = max(x, y) - sq_len;
        unsigned offset = diff / 2;
        if (x > y) {
            for (unsigned ny = 0; ny < y; ny++) {
                for (unsigned nx = 0; nx < x - diff; nx++) {
                    square[ny].pushBack(pixels[ny][nx + offset]);
                }
            }
        } else {
            for (unsigned ny = 0; ny < y - diff; ny++) {
                for (unsigned nx = 0; nx < x; nx++) {
                    square[ny].pushBack(pixels[ny + offset][nx]);
                }
            }
        }
        x = sq_len;
        y = sq_len;
        delete[] pixels;
        pixels = square;
    }
}


/////////////////////////
// convenience

PixelVector & Image::columnAt(unsigned X) {
    PixelVector* col = nullptr;
    if (X < this->x) {
        col = new PixelVector(this->y, this->componentsUsed);
        for (unsigned y = 0; y < this->y; y++) {
            col->pushBack(this->pixels[y].operator[](X));
        }
    } else {
        // throw exception
        cerr << "Column out of bounds" << endl;
    }
    return *col;
}

PixelVector & Image::rowAt(unsigned Y) {
    PixelVector* mt = nullptr;
    if (Y < this->y) {
        mt = &this->pixels[Y];
    } else {
        // throw exception
        cerr << "Row out of bounds" << endl;
    }
    return *mt;
}

PixelVector & Image::constRowAt(unsigned Y) const {
    if (Y < this->y) {
        return this->pixels[Y];
    } else {
        // throw exception
        cerr << "Row out of bounds" << endl;
        PixelVector* mt = nullptr;
        return *mt;
    }
}

void Image::expandImage(charArray data) {
    int n = int(componentsUsed);
    unsigned char gr, r, g, b, a {};
    Pixel p = Pixel();
    Pixel* pp = &p;
    // `i` indexes into `data`, increments by # of components
    for (unsigned i = 0; i < x * y * n; i += n) {
        unsigned py = i / (x * n);
        switch (this->componentsUsed) {
            case GR:
                gr = data[i];
                p = Pixel(gr);
                pixels[py].pushBack(*pp);
                break;
            case GRa:
                gr = data[i];
                a  = data[i+1];
                p = Pixel(gr, a);
                pixels[py].pushBack(*pp);
                break;
            case RGB:
                r = data[i];
                g = data[i+1];
                b = data[i+2];
                p = Pixel(r, g, b);
                pixels[py].pushBack(*pp);
                break;
            case RGBa:
                r = data[i];
                g = data[i+1];
                b = data[i+2];
                a = data[i+3];
                p = Pixel(r, g, b, a);
                pixels[py].pushBack(*pp);
                break;
        }
    }
}

void Image::flattenImage(charArray data, const unsigned dataLength) {
    int N = int(componentsUsed);
    for (unsigned i = 0; i < dataLength; i += N) {
        Pixel & currentPixel = pixels[i / (x * N)][(i / N) % x];
        switch (componentsUsed) {
            case GR:
                data[i] = currentPixel.getR();
                break;
            case GRa:
                data[i] = currentPixel.getR();
                data[i+1] = currentPixel.getA();
                break;
            case RGB:
                data[i] = currentPixel.getR();
                data[i+1] = currentPixel.getG();
                data[i+2] = currentPixel.getB();
                break;
            case RGBa:
                data[i] = currentPixel.getR();
                data[i+1] = currentPixel.getG();
                data[i+2] = currentPixel.getB();
                data[i+3] = currentPixel.getA();
                break;
        }
    }
}

const Pixel Image::average() const {
    if (pixels) {
        float averageR {};
        float averageG {};
        float averageB {};
        float averageA {};

        for (unsigned i = 0; i < x * y; i++) {
            unsigned px = i % x;
            unsigned py = i / x;
            Pixel& p = pixels[py][px];
            averageR = ((averageR * i) + p.getR()) / (i + 1);
            averageG = ((averageG * i) + p.getG()) / (i + 1);
            averageB = ((averageB * i) + p.getB()) / (i + 1);
            averageA = ((averageA * i) + p.getA()) / (i + 1);
        }

        Pixel average = Pixel(averageR, averageG, averageB, averageA);
        return average;
    } else {
        Pixel p = Pixel(0);
        return p;
    }
}

const Pixel Image::average(unsigned fromX, unsigned toX,
                           unsigned fromY, unsigned toY) const {
    toX = min(toX, x);
    toY = min(toY, y);
    unsigned width = toX - fromX;
    unsigned height = toY - fromY;
    if (pixels) {
        float averageR {};
        float averageG {};
        float averageB {};
        float averageA {};

        for (unsigned i = 0; i < width * height; i++) {
            unsigned px = (i % width) + fromX;
            unsigned py = (i / width) + fromY;
            Pixel& p = pixels[py][px];
            averageR = ((averageR * i) + p.getR()) / (i + 1);
            averageG = ((averageG * i) + p.getG()) / (i + 1);
            averageB = ((averageB * i) + p.getB()) / (i + 1);
            averageA = ((averageA * i) + p.getA()) / (i + 1);
        }

        Pixel average = Pixel(averageR, averageG, averageB, averageA);
        return average;
    } else {
        Pixel p = Pixel(0);
        return p;
    }
}

/////////////////////////
// overloaded operators
ostream & operator<<(ostream& out, const Image& img) {
    out << endl;
    for(unsigned y = 0; y < img.getHeight(); y++) {
        out << img.pixels[y] << endl;
    }
    return out;

}


PixelVector & Image::operator[](unsigned Y) {
    if (Y < y)
        return pixels[Y];
}

Image & Image::operator=(const Image & that) {
    if (this != &that) {
        delete[] this->pixels;
        this->x = that.x;
        this->y = that.y;
        this->componentsUsed = that.componentsUsed;
        this->inputFilepath = that.inputFilepath;
        this->outputFilepath = that.outputFilepath;
        this->pixels = new PixelVector [this->y];
        for (int y = 0; y < this->y; y++)
            this->pixels[y] = that.pixels[y];
    }
    return *this;
}

bool Image::operator!() {
    return this->pixels == nullptr;
}

Image::Image(const Image &that) {
    this->x = that.x;
    this->y = that.y;
    this->componentsUsed = that.componentsUsed;
    this->inputFilepath = that.inputFilepath;
    this->outputFilepath = that.outputFilepath;
    this->pixels = new PixelVector [this->y];
    for (unsigned y = 0; y < this->y; y++) {
        this->rowAt(y) = that.constRowAt(y);
    }
}