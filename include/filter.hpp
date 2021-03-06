#pragma once
#ifndef FILTER_HPP
#define FILTER_HPP

#include <vector>
#include <opencv2/core.hpp>
#include <Eigen/Core>

namespace nle {
    using Vec = Eigen::VectorXd;
    using Mat = Eigen::MatrixXd;
    using DType = double;
    const auto OPENCV_MAT_TYPE = CV_64F;
    const double EPS = 1e-10;
    struct Point {
        int row;
        int col;
    };

    std::tuple<Eigen::PermutationMatrix<Eigen::Dynamic, Eigen::Dynamic>, Mat, Mat> 
    computeKernel(const cv::Mat& mat, int nRowSamples, int nColSamples, DType hx, DType hy);

    std::pair<Mat, Vec> 
    eigenDecomposition(const Mat& M, DType eps=EPS);

    std::pair<Vec, Mat> 
    nystromApproximation(const Mat& Ka, const Mat& Kab);

    std::pair<Mat, Mat>
    sinkhorn(const Mat& phi, const Vec& eigvals, int maxIter=10);

    std::pair<Mat, Vec> 
    orthogonalize(const Mat& Wa, const Mat& Wab, int nEigVectors=5, DType eps=EPS);

    class NLEFilter {
        public:
            void trainForEnhancement(const cv::Mat& image, int nRowSamples, int nColSamples,
                                     DType hx, DType hy, int nSinkhornIter=10, int nEigenVectors=5);

            void trainForDenoise(const cv::Mat& image, int nRowSamples, int nColSamples,
                                 DType hx, DType hy, int nSinkhornIter, int nEigenVectors,
                                 int sigmaColor=10, int sigmaSpace=10);

            cv::Mat enhance(const cv::Mat& I, const std::vector<DType>& weights) const;
            cv::Mat denoise(const cv::Mat& I, DType k, int sigmaColor=10, int sigmaSpace=10) const;

        private:
            cv::Mat apply(const cv::Mat& channel, const Vec& transformedEigVals) const;
            void trainFilter(const cv::Mat& channel, int nRowSamples, int nColSamples,
                             DType hx, DType hy, int nSinkhornIter, int nEigenVectors);

            Mat m_eigvecs;
            Vec m_eigvals;
    };
}

#endif
