#ifndef PHOTOMOSAIC_PHOTOMOSAIC_H
#define PHOTOMOSAIC_PHOTOMOSAIC_H
#include <filesystem>
#include "Image.h"
#include "AVLTree.h"
namespace fs = std::filesystem;
using fs::path;

/**
 * Represents a Photomosaic
 *
 * @param mosaic                  The Image object holding the mosaic
 * @param tilesetTree             The AVL tree containing the tileset for the mosaic
 * @param tileSize                The size of an individual tile in the mosaic
 * @param inputPath               The path to the base image for the mosaic
 * @param tilesetDirectoryPath    The path to the directory containing the tileset
 * @param outputPath              The path to write the mosaic image to
 */
class Photomosaic {
private:
    //// photomosaic data members ////
    // mosaic image
    Image mosaic;
    // tiles
    AVLTree<Pixel, Image> tilesetTree;
    // tile size
    unsigned tileSize;

    // filepaths:
    path inputPath;
    path tilesetDirectoryPath;
    path outputPath;

    //// private helper functions ////
    // validate that a filepath exists
    void validateInputPath(path p, bool isDir);
    // check if a string is a valid image file extension
    bool isImageExt(string extension);

public:
    // instantiates a Photomosaic object, preprocesses tileset dir
    Photomosaic(string baseImagePath,
                string tilesetDirectory,
                unsigned tileSize,
                string outputImage);

    // reload the base image without remaking tilesetTree
    void changeBaseImg(string baseImagePath);

    // tile base image, making it into a mosaic
    void tile();

    // write whatever is in `mosaic` to outputPath, whether or not mosaic has been tiled
    void writeOut(format_t fmt);
};

#endif //PHOTOMOSAIC_PHOTOMOSAIC_H