
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "filter.hpp"


int main(int argc, char* argv[])
{
    if (argc < 12) {
        std::cerr << "Usage: " << argv[0] << " <image> <output> <# row samples> <# col samples> <hx> <hy> <# sinkhorn iterations> <# eigen vectors> <weight 1> <weight 2> <weight 3> <weight 4>" << std::endl;
        return 0;
    }

    std::string imagePath { argv[1] };
    std::string outputPath { argv[2] };
    int nRowSamples = std::stoi(argv[3]);
    int nColSamples = std::stoi(argv[4]);
    double hx = std::stod(argv[5]);
    double hy = std::stod(argv[6]);
    int nSinkhornIter = std::stoi(argv[7]);
    int nEigenVectors = std::stoi(argv[8]);

    double sigmaColor = std::stod(argv[9]);
    double sigmaSpace = std::stod(argv[10]);
    double shrinkFactor = std::stod(argv[11]);

    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Failed to read file from " << imagePath << std::endl;
        return 0;
    }

    auto filter = nle::NLEFilter();
    filter.trainForDenoise(image, nRowSamples, nColSamples, hx, hy, nSinkhornIter, nEigenVectors,
                           sigmaColor, sigmaSpace);
    cv::Mat result = filter.denoise(image, shrinkFactor, sigmaColor, sigmaSpace);
    std::cout << "Done. Press any key in result window to exit." << std::endl;

    cv::imwrite(outputPath, result);
    cv::imshow("Result", result);
    cv::waitKey(-1);

    return 0;
}
