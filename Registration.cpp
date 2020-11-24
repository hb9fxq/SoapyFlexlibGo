#include <SoapySDR/Device.hpp>
#include <SoapySDR/Registry.hpp>
#include <string>
#include <SoapySDR/Logger.h>
#include <SoapySDR/Formats.h>
#include <libiqtransfer.h>
#include <cstring>


/***********************************************************************
 * Device interface
 **********************************************************************/
class FlexlibWrapperDevice : public SoapySDR::Device
{

public:
    FlexlibWrapperDevice(const SoapySDR::Kwargs &args){
        std::printf(".ctor FlexlibWrapperDevice\n");
        //SoapySDR_logf(SOAPY_SDR_DEBUG, "\tctor flexlib-go wrapper");
        std::printf(".ctor END!!\n");
    }

    int activateStream(SoapySDR::Stream *stream, const int flags, const long long int timeNs,
                       const size_t numElems) override {
       
     

        std::printf("activateStream\n");
        GoString radio = {"192.168.92.8", 12};
        GoString myudp = {"5777", 4};
        GoString channel = {"1", 1};
        GoString rate = {"192000", 6};

        InitRadio(radio,myudp,channel, rate);
        std::printf("activateStream done\n");

    return 1;
    }

    std::string getDriverKey(void) const override {
        return "flexlib";
    }

    std::string getHardwareKey(void) const override {
        return "flexradio";
    }

    size_t getNumChannels(const int direction) const override {
        return 1;
    }

    bool getFullDuplex(const int direction, const size_t channel) const override {
        return false;
    }

    std::vector<std::string> listAntennas(const int direction, const size_t channel) const override {
        std::vector<std::string> antennas;
        antennas.push_back("ANT");
        return antennas;
    }

    std::vector<double> listSampleRates(const int direction, const size_t channel) const override {
        std::vector<double> res;
        res.push_back(192000.0);
        return res;

    }

    void setAntenna(const int direction, const size_t channel, const std::string &name) override {

    }

    std::string getAntenna(const int direction, const size_t channel) const override {
        std::printf("getAntenna\n");
        return "ANT";
    }

    bool hasDCOffsetMode(const int direction, const size_t channel) const override {
        std::printf("hasDCOffsetMode\n");
        return false;
    }

    bool hasFrequencyCorrection(const int direction, const size_t channel) const override {
        std::printf("hasFrequencyCorrection\n");
        return true;
    }



    double getFrequencyCorrection(const int direction, const size_t channel) const override {
        std::printf("getFrequency\n");
        return 0.0;
    }

    double getFrequency(const int direction, const size_t channel) const override {

        std::printf("getFrequency\n");
        return lastSet;

    }

    double lastSet = 7100000;
    void setFrequency(const int direction, const size_t channel, const double frequency,
                      const SoapySDR::Kwargs &args) override {
    std::printf("setFrequency\n");
    lastSet = frequency;

    long newFreq = (long)frequency;

    SetFrequency(newFreq);

     std::printf("setFrequency END\n");

    }

    double getSampleRate(const int direction, const size_t channel) const override {
        std::printf("getSampleRate\n");
        return 192000;
    }

    bool getGainMode(const int direction, const size_t channel) const override {
        std::printf("getSampleRate\n");
        return true;
    }

    double getGain(const int direction, const size_t channel, const std::string &name) const override {
        std::printf("getGain\n");
        return 1;
    }



    SoapySDR::Range getGainRange(const int direction, const size_t channel, const std::string &name) const
    {

    std::printf("getGainRange called\n");    
         return SoapySDR::Range(-3, 6);

    }


    std::vector<std::string> listFrequencies(const int direction, const size_t channel) const
    {
        std::printf("listFrequencies called\n");
        std::vector<std::string> names;
        names.push_back("RF");
        names.push_back("CORR");
        return names;
    }

    SoapySDR::RangeList getFrequencyRange(
            const int direction,
            const size_t channel,
            const std::string &name) const
    {
        std::printf("getFrequencyRange called\n");
        SoapySDR::RangeList results;
        if (name == "RF")
        {

                results.push_back(SoapySDR::Range(1000000, 2200000000));

        }
        if (name == "CORR")
        {
            results.push_back(SoapySDR::Range(-1000, 1000));
        }
        return results;
    }

    double getBandwidth(const int direction, const size_t channel) const
    {
        std::printf("getBandwidth called\n");
        return SoapySDR::Device::getBandwidth(direction, channel);
    }

    std::vector<double> listBandwidths(const int direction, const size_t channel) const
    {
       std::printf("listBandwidths called\n");
        std::vector<double> results;

        return results;
    }

    std::vector<std::string> getStreamFormats(const int direction, const size_t channel) const {
        std::printf("getStreamFormats called\n");
        std::vector<std::string> formats;

        formats.push_back(SOAPY_SDR_CF32);
        fprintf(stderr,"getStreamFormats\n");

        return formats;
    }

    std::string getNativeStreamFormat(const int direction, const size_t channel, double &fullScale) const {

        std::printf("getNativeStreamFormat called\n");
        fullScale = 1.0;
        fprintf(stderr,"getNativeStreamFormat\n");
        return SOAPY_SDR_CF32;
    }

    size_t getStreamMTU(SoapySDR::Stream *stream) const
    {
        return 1024;
    }


    //SoapySDR::Stream *setupStream(const int direction, const std::string &format, const std::vector<size_t> &channels,
    //                              const SoapySDR::Kwargs &args) override {
     //   std::printf("SetupStream called\n");
    //}

    int
    readStream(SoapySDR::Stream *stream, void *const *buffs, const size_t numElems, int &flags, long long int &timeNs,
               const long timeoutUs) override {

        int readSize = numElems * sizeof(std::complex<float>); // byte

        ReadStream3_return res = ReadStream3(readSize);


        unsigned char *out=(unsigned char*)buffs[0];


        const unsigned char *z = res.r1;

        std::memcpy(out, z, readSize);

        //std::printf("%i ", x);

        return numElems;
    }


};

/***********************************************************************
 * Find available devices
 **********************************************************************/
SoapySDR::KwargsList findMyDevice(const SoapySDR::Kwargs &args)
{

    std::printf("findMyDevice called\n");
    std::vector<SoapySDR::Kwargs> results;
    SoapySDR_logf(SOAPY_SDR_INFO, "\tadding flexlib-go wrapper device");
    SoapySDR::Kwargs devInfo;
    devInfo["product"] = "flexlib-go-wrapper-device";
    devInfo["manufacturer"] = "HB9FXQ";
    results.push_back(devInfo);

    return results;

}

/***********************************************************************
 * Make device instance
 **********************************************************************/
SoapySDR::Device *makeMyDevice(const SoapySDR::Kwargs &args)
{
    std::printf("makeMyDevice called!!++++\n");
    return new FlexlibWrapperDevice(args);
    std::printf("makeMyDevice completed!!\n");
}


/***********************************************************************
 * Registration
 **********************************************************************/
static SoapySDR::Registry registerMyDevice("flexlib-go-wrapper", &findMyDevice, &makeMyDevice, SOAPY_SDR_ABI_VERSION);