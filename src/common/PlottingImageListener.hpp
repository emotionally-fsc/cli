/**
 * @author Affectiva, heavily modified by Andrea Esposito.
 */

#ifndef AFFECTIVA_PLOTTING_IMAGE_LISTENERS_HPP
#define AFFECTIVA_PLOTTING_IMAGE_LISTENERS_HPP

#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/timer/timer.hpp>

#include <rapidjson/writer.h>
#include <rapidjson/document.h>

#include "Visualizer.h"
#include "ImageListener.h"

class PlottingImageListener : public affdex::ImageListener
{
private:
    std::mutex mMutex;
    std::deque <std::pair<affdex::Frame, std::map < affdex::FaceId, affdex::Face>> >
    mDataArray;

    double mCaptureLastTS;
    double mCaptureFPS;
    double mProcessLastTS;
    double mProcessFPS;
    std::chrono::time_point <std::chrono::system_clock> mStartT;
    const int spacing = 20;
    const float font_size = 0.5f;
    const int font = cv::FONT_HERSHEY_COMPLEX_SMALL;
    Visualizer viz;

    rapidjson::Document document;

public:

    PlottingImageListener();

    cv::Point2f minPoint(affdex::VecFeaturePoint points);

    cv::Point2f maxPoint(affdex::VecFeaturePoint points);

    double getProcessingFrameRate();

    double getCaptureFrameRate();

    int getDataSize();

    std::pair <affdex::Frame, std::map<affdex::FaceId, affdex::Face>> getData();

    void onImageResults(std::map <affdex::FaceId, affdex::Face> faces, affdex::Frame image) override;

    void onImageCapture(affdex::Frame image) override;

    static rapidjson::Value printFeatures(rapidjson::Document &document, float *features,
                                          const std::vector <std::string> &viz);

    void addResult(const std::map <affdex::FaceId, affdex::Face> &faces, const double timeStamp);

    void outputToFile(std::ostream &file);

    std::vector <cv::Point2f> CalculateBoundingBox(affdex::VecFeaturePoint points);

    void draw(const std::map <affdex::FaceId, affdex::Face> faces, affdex::Frame image);

};

#endif // AFFECTIVA_PLOTTING_IMAGE_LISTENERS_HPP
