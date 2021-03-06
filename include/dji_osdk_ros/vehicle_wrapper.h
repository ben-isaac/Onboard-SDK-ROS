/** @file vehicle_wrapper.hpp
 *  @version 4.0
 *  @date May 2020
 *
 *  @brief layer of modules of osdk ros 4.0.Encapsulate the interface of osdk.
 *
 *  @Copyright (c) 2020 DJI
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef __VEHICLE_WRAPPER_HH__
#define __VEHICLE_WRAPPER_HH__

// Header include
#include <dji_setup_helpers.hpp>
#include <dji_vehicle.hpp>
#include <dji_osdk_ros/common_type.h>
#include <string>
#include <cmath>
#include <vector>
#ifdef ADVANCED_SENSING
#include <dji_camera_image.hpp>
#endif
#include <mutex>

const double C_EARTH = 6378137.0;
const double DEG2RAD = 0.01745329252;
// Declaration
namespace dji_osdk_ros
{
  using namespace DJI::OSDK;
  using namespace Telemetry;
  class VehicleWrapper : private Setup
  {
    public:
      VehicleWrapper(int app_id,
                     const std::string& env_key,
                     const std::string& dev_acm_name,
                     const std::string& dev_name,
                     unsigned int baud_rate,
                     bool enableAdvancedSensing = false);
      virtual ~VehicleWrapper() = default;
    
    public:
      void setupEnvironment(bool enable_advanced_sensing);
      bool initVehicle();
      ErrorCode::ErrorCodeType initGimbalModule(dji_osdk_ros::PayloadIndex index,
                                                const char* name);
      ErrorCode::ErrorCodeType initCameraModule(dji_osdk_ros::PayloadIndex index,
                                                const char* name);

    public:
      /*! Parts of Camera */
      bool setEV(const PayloadIndex& payloadIndex, const ExposureCompensation& exposureCompensation);
      bool setExposureMode(const PayloadIndex& index, const ExposureMode& dataTarget);
      bool setISO(const PayloadIndex& payloadIndex, const ISO& ios);
      bool setShutterSpeed(const PayloadIndex& payloadIndex, const ShutterSpeed& shutterSpeed);
      bool setAperture(const PayloadIndex& payloadIndex, const Aperture& aperture);
      bool setFocusPoint(const PayloadIndex& payloadIndex,const float& x, const float& y);
      bool setTapZoomPoint(const PayloadIndex& payloadIndex,const uint8_t& multiplier, const float& x,const float& y);
      bool startZoom(const PayloadIndex& payloadIndex,const uint8_t& direction, const uint8_t& speed);
      bool stopZoom(const PayloadIndex& payloadIndex);
      bool startShootSinglePhoto(const PayloadIndex& payloadIndex);
      bool startShootBurstPhoto(const PayloadIndex& payloadIndex, const PhotoBurstCount& photoBurstCount);
      bool startShootAEBPhoto(const PayloadIndex& index,  const PhotoAEBCount& photoAebCount);
      bool startShootIntervalPhoto(const PayloadIndex& payloadIndex, const PhotoIntervalData& photoIntervalData);
      bool shootPhotoStop(const PayloadIndex& payloadIndex);
      bool startRecordVideo(const PayloadIndex& payloadIndex);
      bool stopRecordVideo(const PayloadIndex& payloadIndex);
      /*! Parts of Gimbal */
      GimbalSingleData getGimbalData(const PayloadIndex& payloadIndex);
      bool rotateGimbal(const PayloadIndex& PayloadIndex, const GimbalRotationData& rotationData);
      bool resetGimbal(const PayloadIndex& payloadIndex);

      /*! Parts of Flight Control*/
      bool checkActionStarted(uint8_t mode);
      bool setNewHomeLocation(int timeout = 1);
      bool setHomeAltitude(uint16_t altitude, int timeout = 1);
      bool goHome(ACK::ErrorCode& ack, int timeout);
      bool goHomeAndConfirmLanding(int timeout);
      bool setAvoid(bool enable);
      bool setUpwardsAvoidance(bool enable);

      bool monitoredTakeoff(ACK::ErrorCode& ack, int timeout);
      bool monitoredLanding(ACK::ErrorCode& ack, int timeout);
      bool moveByPositionOffset(ACK::ErrorCode& ack, int timeout, MoveOffset& p_offset);

      /*! Parts of mfio */
      uint8_t outputMFIO(uint8_t mode, uint8_t channel, uint32_t init_on_time_us, uint16_t freq, bool block, uint8_t gpio_value);
      uint32_t inputMFIO(uint8_t mode, uint8_t channel, bool block);
      uint8_t stopMFIO(uint8_t mode, uint8_t channel);

      /*! Parts of mobile device */
      void sendDataToMSDK(uint8_t* data, uint8_t len);

      /*! Parts of payload device */
      void sendDataToPSDK(uint8_t* data, uint8_t len);

      /*! Parts of advanced_sendsing */
#ifdef ADVANCED_SENSING
      bool startStream(bool is_h264, uint8_t request_view);
      bool stopStream(bool is_h264, uint8_t request_view);

      CameraRGBImage& getCameraImage();
      std::vector<uint8_t>& getCameraRawData();
      void setCameraRawData(uint8_t* rawData, int bufLen);
      void setCameraImage(const CameraRGBImage& img);
      void setAcmDevicePath(const std::string& acm_path);
#endif
      bool isM100();
      bool isM300();
      void setUpM100DefaultFreq(uint8_t freq[16]);
      void setUpA3N3DefaultFreq(uint8_t freq[16]);
      ACK::ErrorCode setBroadcastFreq(uint8_t* dataLenIs16, int timeout);
      uint16_t getPassFlag();
      Telemetry::RC getRC();
      Telemetry::Quaternion getQuaternion();
      Telemetry::Vector3f getAcceleration();
      Telemetry::Vector3f getAngularRate();
      Telemetry::GlobalPosition getGlobalPosition();
      Telemetry::Vector3f getVelocity();
      Telemetry::Battery getBatteryInfo();
      Telemetry::Status getStatus();
      Telemetry::Gimbal getGimbal();
      void setUserBroadcastCallback(VehicleCallBack callback, UserData userData);
      void setFromMSDKCallback(VehicleCallBack callback, UserData userData);
      void setFromPSDKCallback(VehicleCallBack callback, UserData userData);
      void subscribeNMEAMsgs(VehicleCallBack cb, void *userData);
      void subscribeUTCTime(VehicleCallBack cb, void *userData);
      void subscribeFCTimeInUTCRef(VehicleCallBack cb, void *userData);
      void subscribePPSSource(VehicleCallBack cb, void *userData);
      void unsubscribeNMEAMsgs();
      void unsubscribeUTCTime();
      void unsubscribeFCTimeInUTCRef();
      void unsubscribePPSSource();

      /*! Parts of data subscription*/
      bool setUpSubscription(int pkgIndex, int freq,TopicName* topicList,
                           uint8_t topicSize, int timeout);
      bool teardownSubscription(const int pkgIndex, int timeout);
      ACK::ErrorCode verify(int timeout);
      bool initPackageFromTopicList(int packageID, int numberOfTopics,TopicName* topicList,
                                    bool sendTimeStamp, uint16_t freq);
      ACK::ErrorCode startPackage(int packageID, int timeout);
      ACK::ErrorCode removePackage(int packageID, int timeout);
      void registerUserPackageUnpackCallback(int packageID, VehicleCallBack userFunctionAfterPackageExtraction,
                                             UserData userData);
      Version::FirmWare getFwVersion() const;
      char* getHwVersion() const;
  protected:
      bool startGlobalPositionBroadcast();
      Telemetry::Vector3f toEulerAngle(void* quaternionData);
      void localOffsetFromGpsOffset(Telemetry::Vector3f& deltaNed, void* target, void* origin);
    
    public:
      Vehicle::ActivateData *getActivateData()
      {
        return &this->activate_data_;
      }
    
      Vehicle *getVehicle()
      {
        return vehicle;
      }
    
      Linker *getLinker()
      {
        return linker;
      }
    
    private:
      uint32_t timeout_;
      Vehicle::ActivateData activate_data_;
      int          app_id_;
      std::string  enc_key_;
      std::string  device_acm_;
      std::string  device_;
      unsigned int baudrate_;
      std::string  sample_case_;
      const static unsigned int default_acm_baudrate = 230400;


#ifdef ADVANCED_SENSING
      CameraRGBImage image_from_camera_;
      std::vector<uint8_t> raw_data_from_camera_;
      std::mutex camera_data_mutex_;
#endif
};
}

#endif // __VEHICLE_WRAPPER_HH__

