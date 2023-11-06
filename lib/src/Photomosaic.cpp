#include "Photomosaic.h"


Photomosaic::Photomosaic(string baseImagePath,
                         string tilesetDirectory,
                         unsigned tileSize,
                         string outputImage) {
    inputPath = path(baseImagePath);
    validateInputPath(inputPath, false);
    tilesetDirectoryPath = path(tilesetDirectory);
    validateInputPath(tilesetDirectoryPath, true);
    outputPath = path(outputImage);

    this->tileSize = tileSize;

    // read base image from input path
    mosaic = Image(inputPath);

    // build tree from specified dir
    for (const fs::directory_entry& entry : fs::recursive_directory_iterator(tilesetDirectoryPath)) {
        if (entry.is_regular_file()) {
            if (isImageExt(entry.path().extension().string())) {
                // read image, add to tree
                Image file_as_img = Image(entry.path().string());
                file_as_img.square();
                file_as_img.resize(tileSize, tileSize);
                Pixel img_avg = file_as_img.average();
                tilesetTree.insert(img_avg, file_as_img);
            }
        }
    }
}

void Photomosaic::changeBaseImg(string baseImagePath) {
    inputPath = path(baseImagePath);
    validateInputPath(inputPath, false);
    mosaic = Image(inputPath);
}

void Photomosaic::tile() {
    unsigned x = mosaic.getWidth();
    unsigned y = mosaic.getHeight();
    unsigned xTiles = x / tileSize;
    unsigned yTiles = y / tileSize;
    unsigned nTiles = xTiles * yTiles;
    unsigned xLeftover = x - (xTiles * tileSize);
    unsigned yLeftover = y - (yTiles * tileSize);
    mosaic.resize(x - xLeftover, y - yLeftover);

    for (unsigned t = 0; t < nTiles; t++) {
        unsigned tx = t % xTiles;
        unsigned ty = t / xTiles;
        unsigned tileXStart = tx * tileSize;
        unsigned tileYStart = ty * tileSize;

        Pixel tileAvg = mosaic.average(tileXStart, tileXStart + tileSize,
                                       tileYStart, tileYStart + tileSize);
        Image tileBasis = tilesetTree.search(tileAvg);

        unsigned by {};
        for (unsigned my = tileYStart; my < tileYStart + tileSize; my++) {
            unsigned bx {};
            for (unsigned mx = tileXStart; mx < tileXStart + tileSize; mx++) {
                mosaic[my][mx] = tileBasis[by][bx];
                bx++;
            }
            by++;
        }
    }
}

void Photomosaic::writeOut(format_t fmt) {
    string out = outputPath.string();
    if (!out.empty()) {
        mosaic.write(out, fmt);
    }
}

void Photomosaic::validateInputPath(path p, bool isDir) {
    if (!fs::exists(p))
        cerr << p.string() << " does not exist" << endl;
    if (isDir) {
        if (!fs::is_directory(p))
            cerr << p.string() << " is not a directory" << endl;
    }
}

bool Photomosaic::isImageExt(string extension) {
    return extension == ".jpg"
           ||  extension == ".jpeg"
           ||  extension == ".png"
           ||  extension == ".bmp";
}