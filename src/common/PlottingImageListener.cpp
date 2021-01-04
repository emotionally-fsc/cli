/**
 * @author Affectiva, heavily modified by Andrea Esposito.
 */

#include "PlottingImageListener.hpp"

#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/timer/timer.hpp>

#include <rapidjson/stringbuffer.h>

#include <rapidjson/writer.h>
#include "Visualizer.h"

#include "ImageListener.h"

using namespace affdex;

PlottingImageListener::PlottingImageListener()
        : mStartT(std::chrono::system_clock::now()),
          mCaptureLastTS(-1.0f), mCaptureFPS(-1.0f),
          mProcessLastTS(-1.0f), mProcessFPS(-1.0f)
{
    document.SetArray();
}

cv::Point2f PlottingImageListener::minPoint(VecFeaturePoint points)
{
    VecFeaturePoint::iterator it = points.begin();
    FeaturePoint ret = *it;
    for (; it != points.end(); it++)
    {
        if (it->x < ret.x) ret.x = it->x;
        if (it->y < ret.y) ret.y = it->y;
    }
    return cv::Point2f(ret.x, ret.y);
}

cv::Point2f PlottingImageListener::maxPoint(VecFeaturePoint points)
{
    VecFeaturePoint::iterator it = points.begin();
    FeaturePoint ret = *it;
    for (; it != points.end(); it++)
    {
        if (it->x > ret.x) ret.x = it->x;
        if (it->y > ret.y) ret.y = it->y;
    }
    return cv::Point2f(ret.x, ret.y);
}

double PlottingImageListener::getProcessingFrameRate()
{
    std::lock_guard <std::mutex> lg(mMutex);
    return mProcessFPS;
}

double PlottingImageListener::getCaptureFrameRate()
{
    std::lock_guard <std::mutex> lg(mMutex);
    return mCaptureFPS;
}

int PlottingImageListener::getDataSize()
{
    std::lock_guard <std::mutex> lg(mMutex);
    return mDataArray.size();

}

std::pair <Frame, std::map<FaceId, Face>> PlottingImageListener::getData()
{
    std::lock_guard <std::mutex> lg(mMutex);
    std::pair <Frame, std::map<FaceId, Face>> dpoint = mDataArray.front();
    mDataArray.pop_front();
    return dpoint;
}

void PlottingImageListener::onImageResults(std::map <FaceId, Face> faces, Frame image)
{
    std::lock_guard <std::mutex> lg(mMutex);
    mDataArray.push_back(std::pair < Frame, std::map < FaceId, Face >> (image, faces));
    std::chrono::time_point <std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::chrono::milliseconds milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - mStartT);
    double seconds = milliseconds.count() / 1000.f;
    mProcessFPS = 1.0f / (seconds - mProcessLastTS);
    mProcessLastTS = seconds;
}

void PlottingImageListener::onImageCapture(Frame image)
{
    std::lock_guard <std::mutex> lg(mMutex);
    mCaptureFPS = 1.0f / (image.getTimestamp() - mCaptureLastTS);
    mCaptureLastTS = image.getTimestamp();
}

rapidjson::Value
PlottingImageListener::printFeatures(rapidjson::Document &document, float *features,
                                     const std::vector <std::string> &viz)
{
    rapidjson::Value temp;
    temp.SetObject();
    for (const std::string &property : viz)
    {
        temp.AddMember(rapidjson::Value().SetString(property.c_str(), property.size()), rapidjson::Value(*features).Move(), document.GetAllocator());
        features++;
    }
    return temp;
}

void PlottingImageListener::addResult(const std::map <FaceId, Face> &faces, const double timeStamp)
{
    auto &allocator = document.GetAllocator();
    rapidjson::Value v;
    v.SetObject();

    v.AddMember("timestamp", rapidjson::Value(timeStamp).Move(), allocator);

    // NOTE: To save all faces, decomment this 'for' (and comment the 'if') and
    // change the definition of f (see the comment), alongside with the
    // structure of the JSON 
    //
    // for (auto &face_id_pair : faces)
    if (!faces.empty())
    {
        Face f = faces.begin()->second; // To save all faces, change 'faces.begin()->' to 'face_id_pair.'

        v.AddMember("faceId", rapidjson::Value(f.id).Move(), allocator);
        const std::string dominantEmoji = affdex::EmojiToString(f.emojis.dominantEmoji);
        v.AddMember("dominantEmoji", rapidjson::Value().SetString(dominantEmoji.c_str(), dominantEmoji.size()), allocator);

        rapidjson::Value measurements;
        measurements.SetObject();
        measurements.AddMember("interocularDistance", rapidjson::Value(f.measurements.interocularDistance).Move(), allocator);
        measurements.AddMember("orientation", printFeatures(document, (float *) &f.measurements.orientation, viz.HEAD_ANGLES), allocator);
        v.AddMember("measurements", measurements,allocator);

        rapidjson::Value appearance;
        appearance.SetObject();
        appearance.AddMember("glasses", rapidjson::Value(viz.GLASSES_MAP[f.appearance.glasses]).Move(), allocator);
        appearance.AddMember("age", rapidjson::Value().SetString(viz.AGE_MAP[f.appearance.age].c_str(), viz.AGE_MAP[f.appearance.age].size()), allocator);
        appearance.AddMember("ethnicity", rapidjson::Value().SetString(viz.ETHNICITY_MAP[f.appearance.ethnicity].c_str(), viz.ETHNICITY_MAP[f.appearance.ethnicity].size()), allocator);
        appearance.AddMember("gender", rapidjson::Value().SetString(viz.GENDER_MAP[f.appearance.gender].c_str(), viz.GENDER_MAP[f.appearance.gender].size()), allocator);
        v.AddMember("appearance", appearance, allocator);

        rapidjson::Value emotions = printFeatures(document, (float *) &f.emotions, viz.EMOTIONS);
        v.AddMember("emotions", emotions, allocator);
        rapidjson::Value expressions = printFeatures(document, (float *) &f.emotions, viz.EXPRESSIONS);
        v.AddMember("expressions", expressions, allocator);
        rapidjson::Value emojis = printFeatures(document, (float *) &f.emojis, viz.EMOJIS);
        v.AddMember("emojis", emojis, allocator);
    }
    document.PushBack(v, allocator);
}

void PlottingImageListener::outputToFile(std::ostream &file)
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    file << buffer.GetString() << std::endl;
}

std::vector <cv::Point2f> PlottingImageListener::CalculateBoundingBox(VecFeaturePoint points)
{

    std::vector <cv::Point2f> ret;

    //Top Left
    ret.push_back(minPoint(points));

    //Bottom Right
    ret.push_back(maxPoint(points));

    //Top Right
    ret.push_back(cv::Point2f(ret[1].x,
                              ret[0].y));
    //Bottom Left
    ret.push_back(cv::Point2f(ret[0].x,
                              ret[1].y));

    return ret;
}

void PlottingImageListener::draw(const std::map <FaceId, Face> faces, Frame image)
{

    const int left_margin = 30;

    cv::Scalar clr = cv::Scalar(0, 0, 255);
    cv::Scalar header_clr = cv::Scalar(255, 0, 0);

    std::shared_ptr<unsigned char> imgdata = image.getBGRByteArray();
    cv::Mat img = cv::Mat(image.getHeight(), image.getWidth(), CV_8UC3, imgdata.get());
    viz.updateImage(img);

    for (auto &face_id_pair : faces)
    {
        Face f = face_id_pair.second;
        VecFeaturePoint points = f.featurePoints;
        std::vector <cv::Point2f> bounding_box = CalculateBoundingBox(points);

        // Draw Facial Landmarks Points
        //viz.drawPoints(points);

        // Draw bounding box
        viz.drawBoundingBox(bounding_box[0], bounding_box[1], f.emotions.valence);

        // Draw a face on screen
        viz.drawFaceMetrics(f, bounding_box);
    }

    viz.showImage();
    std::lock_guard <std::mutex> lg(mMutex);
}
