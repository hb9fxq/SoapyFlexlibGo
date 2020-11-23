#include <SoapySDR/Device.hpp>
#include <SoapySDR/Registry.hpp>
#include <string>
#include <SoapySDR/Logger.h>
#include <SoapySDR/Formats.h>
#include <libiqtransfer.h>


/***********************************************************************
 * Device interface
 **********************************************************************/
class FlexlibWrapperDevice : public SoapySDR::Device
{

public:
    FlexlibWrapperDevice(const SoapySDR::Kwargs &args){
        SoapySDR_logf(SOAPY_SDR_DEBUG, "\tctor flexlib-go wrapper");
    }

    int activateStream(SoapySDR::Stream *stream, const int flags, const long long int timeNs,
                       const size_t numElems) override {
        GoString radio = {"192.168.92.8", 12};
        GoString myudp = {"5777", 4};
        GoString channel = {"1", 1};
        GoString rate = {"192000", 6};

        InitRadio(radio,myudp,channel, rate);


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

    void setAntenna(const int direction, const size_t channel, const std::string &name) override {

    }

    std::string getAntenna(const int direction, const size_t channel) const override {
        return "ANT";
    }

    bool hasDCOffsetMode(const int direction, const size_t channel) const override {
        return false;
    }

    bool hasFrequencyCorrection(const int direction, const size_t channel) const override {
        return true;
    }



    double getFrequencyCorrection(const int direction, const size_t channel) const override {
        return 0.0;
    }

    double getFrequency(const int direction, const size_t channel) const override {

        return lastSet;

    }

    double lastSet = 7100000;
    void setFrequency(const int direction, const size_t channel, const double frequency,
                      const SoapySDR::Kwargs &args) override {
    lastSet = frequency;

    long long newFreq = (long long)frequency;

    SetFrequency(newFreq);

    }

    double getSampleRate(const int direction, const size_t channel) const override {
        return 192000;
    }

    bool getGainMode(const int direction, const size_t channel) const override {
        return true;
    }

    double getGain(const int direction, const size_t channel, const std::string &name) const override {
        return 1;
    }

    SoapySDR::Range getGainRange(const int direction, const size_t channel, const std::string &name) const
    {

         return SoapySDR::Range(-3, 6);

    }


    std::vector<std::string> listFrequencies(const int direction, const size_t channel) const
    {
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
        return SoapySDR::Device::getBandwidth(direction, channel);
    }

    std::vector<double> listBandwidths(const int direction, const size_t channel) const
    {
        std::vector<double> results;

        return results;
    }

    std::vector<std::string> getStreamFormats(const int direction, const size_t channel) const {
        std::vector<std::string> formats;

        formats.push_back(SOAPY_SDR_CF32);
        fprintf(stderr,"getStreamFormats\n");

        return formats;
    }

    std::string getNativeStreamFormat(const int direction, const size_t channel, double &fullScale) const {

        fullScale = 1.0;
        fprintf(stderr,"getNativeStreamFormat\n");
        return SOAPY_SDR_CF32;
    }

    size_t getStreamMTU(SoapySDR::Stream *stream) const
    {
        return 1024;
    }


    SoapySDR::Stream *setupStream(const int direction, const std::string &format, const std::vector<size_t> &channels,
                                  const SoapySDR::Kwargs &args) override {
        std::printf("SetupStream called\n");
    }

    int
    readStream(SoapySDR::Stream *stream, void *const *buffs, const size_t numElems, int &flags, long long int &timeNs,
               const long timeoutUs) override {

        int readSize = numElems * sizeof(std::complex<float>); // byte

        ReadStream3_return res = ReadStream3(readSize);


        unsigned unsigned char *out=(unsigned char*)buffs[0];


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
    return new FlexlibWrapperDevice(args);
}


/***********************************************************************
 * Registration
 **********************************************************************/
static SoapySDR::Registry registerMyDevice("flexlib-go-wrapper", &findMyDevice, &makeMyDevice, SOAPY_SDR_ABI_VERSION);