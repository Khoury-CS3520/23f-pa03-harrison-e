#include "Photomosaic.h"
#include <cstring>
using std::cout, std::cin;

// photomosaic driver
int main(int argc, char* argv[]) {
    string flag = "-t";

    if (argc == 4) {
        // 3-arg mode
        cout << "Processing your photomosaic...";
        Photomosaic m = Photomosaic(argv[1], argv[2], 10, argv[3]);
        m.tile();
        m.writeOut(JPG);
        cout << "Mosaic image written to: " << argv[3];
        return 0;

    } else if (argc == 2 && strcmp(flag.c_str(), argv[1]) == 0) {
        // 1-arg mode
        string cats = "../../cats";
        string dogs = "../../dogs";
        string emojis = "../../emojis/image/Apple";
        string natural = "../../natural";
        string flowers = "../../flowers";
        string colors = "../../colors";
        string sample = "../sampleImages/sampleTiles";

        string baymax = "../sampleImages/baymax.jpeg";
        string blern = "../sampleImages/blern.jpg";
        unsigned tilesize = 5;
        string out = "../sampleImages/blernMosaic";

        Photomosaic m = Photomosaic(blern, colors, tilesize, out);
        m.tile();
        m.writeOut(JPG);
        return 0;

    } else if (argc == 1) {
        // interactive mode
        string baseImg;
        string tileDir;
        string outPath;
        unsigned tileSize;
        cout << "Welcome to Photomosaic interactive mode!" << endl;
        cout << "Enter the path to the base image for your mosaic: ";
        cin >> baseImg;
        cin.ignore(10, '\n');
        cout << "Enter the path to the directory of tile images: ";
        cin >> tileDir;
        cin.ignore(10, '\n');
        cout << "Enter the size (in pixels) of each tile: ";
        cin >> tileSize;
        cin.ignore(10, '\n');
        cout << "Enter the path where your final mosaic will be written (without file extension): ";
        cin >> outPath;
        cin.ignore(10, '\n');

        cout << "Processing your photomosaic..." << endl;
        Photomosaic m = Photomosaic(baseImg, tileDir, tileSize, outPath);
        m.tile();
        m.writeOut(JPG);
        cout << "Mosaic image written to: " << outPath;
        return 0;

    } else {
        cerr << "Invalid argument configuration: ";
        for (int arg = 0; arg < argc; arg++) {
            cerr << argv[arg] << " ";
        }
        cerr << endl
             << "Please see README.md for instructions on how to run this program. Bye bye!" << endl;
        return -1;
    }
}