//#include "Arduino.h"
#include "SPI.h"

class ADS1256
{
  private:
    int PIN_RDY;
    int PIN_CS;
    int PIN_Reset;
    int PGA;
    int SPS;
    int Channel;
    //    float CF;  //Cutoff Frequency
  public:
    ADS1256();
    ~ADS1256();
    void OpenDevice();
    void CloseDevice();
    float GetAnalog(int ch);
    float GetAnalogC();
    void StartAnalogC(int ch);
    void StopAnalogC();
    bool SetPGA(int pga);
    bool SetSPS(int sps);
    bool SetPINRDY(int rdy);
    bool SetPINCS(int cs);
    bool SetPINReset(int reset);
    //    bool SetFilter(float cutoff);
    int GetPGA();
    int GetSPS();
    int GetPINRDY();
    int GetPINCS();
    int GetPINReset();
  private:
    void write_register(unsigned char reg, unsigned char wdata);
    long read_data();
    //    float ButterworthFilter(float data);  //4-Order Low-Pass ButterworthFilter
};

