#include "ADS1256.h"

#define STATUS 0x00
#define MUX 0x01
#define ADCON 0x02
#define DRATE 0x03
#define IO 0x04

#define CH1 0x01
#define CH2 0x23
#define CH3 0x45
#define CH4 0x67

#define PGA1 0x00
#define PGA2 0x01
#define PGA4 0x02
#define PGA8 0x03
#define PGA16 0x04
#define PGA32 0x05
#define PGA64 0x06

#define SPS2000 0xb0
#define SPS1000 0xa1
#define SPS500 0x92
#define SPS100 0x82
#define SPS50 0x63
#define SPS10 0x23
#define SPS5 0x13

#define WAKEUP 0x00
#define RDATA  0x01
#define RDATAC 0x03
#define SDATAC 0x0f
#define RREG 0x10
#define WREG 0x50
#define SELFCAL 0xf0
#define SELFOCAL 0xf1
#define SELFGCAL 0xf2
#define SYSOCAL 0xf3
#define SYSGCAL 0xf4
#define SYNC 0xfc
#define STANDBY 0xfd
#define RESET 0xfe

#define VRef 2.5




ADS1256::ADS1256() {
  PIN_RDY = 6;
  PIN_Reset = 7;
  PIN_CS = 10;
  PGA = 1;
  SPS = 1000;
  Channel = 1;
}

ADS1256::~ADS1256() {
  SPI.end();
}

void ADS1256::OpenDevice() {

  pinMode(PIN_RDY, INPUT);
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_Reset, OUTPUT);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);	//MSB First
  SPI.setClockDivider(SPI_CLOCK_DIV16);	//
  SPI.setDataMode(SPI_MODE1);	//涓婂崌娌跨疆浣嶏紝涓嬮檷娌块噰鏍凤紝SCK闂叉椂涓�0

  digitalWrite(PIN_Reset, LOW);	//Low to Reset�
  delayMicroseconds(1);
  digitalWrite(PIN_Reset, HIGH);

  digitalWrite(PIN_CS, LOW);	//Low to select

  write_register(STATUS, 0x04);        //杈撳嚭楂樹綅鏁版嵁鍦ㄥ墠锛屽叧闂紦鍐�
  write_register(MUX, CH1);           //宸垎閫氶亾1
  write_register(ADCON, PGA1);         //gain=1
  write_register(DRATE, SPS1000);         //1000psp
  write_register(IO, 0x00);	//IO閮介厤缃负杈撳嚭
  delay(1);
//  SPI.transfer(SELFCAL);
//  delay(1000);

  digitalWrite(PIN_CS, HIGH);

}

void ADS1256::CloseDevice() {
  SPI.end();
}

float ADS1256::GetAnalog(int ch) {

  long x;
  float y;

  switch (ch) {
    case 1:
      digitalWrite(PIN_CS, LOW);
      Channel = ch;
      write_register(MUX, CH1);
      break;
    case 2:
      digitalWrite(PIN_CS, LOW);
      Channel = ch;
      write_register(MUX, CH2);
      break;
    case 3:
      digitalWrite(PIN_CS, LOW);
      Channel = ch;
      write_register(MUX, CH3);
      break;
    case 4:
      digitalWrite(PIN_CS, LOW);
      Channel = ch;
      write_register(MUX, CH4);
      break;
    default:
      return 0;
  }
  SPI.transfer(SYNC);    //
  SPI.transfer(WAKEUP);  //
  while (digitalRead(PIN_RDY) == HIGH); //
  SPI.transfer(RDATA);                 //
  delayMicroseconds(10);

  x = read_data();

  if (x > 0x007fffff) {
    x -= 0x1000000;
  }

  digitalWrite(PIN_CS, HIGH);

  y = x;
  y = 2 * VRef * y / 0x007fffff / PGA;

  return y;

}

float ADS1256::GetAnalogC() {

  long x;
  float y;

  digitalWrite(PIN_CS, LOW);

  while (digitalRead(PIN_RDY) == HIGH); //

  x = read_data();

  digitalWrite(PIN_CS, HIGH);

  if (x > 0x007fffff) {
    x -= 0x1000000;
  }

  y = x;
  y = 2 * VRef * y / 0x007fffff / PGA;

  return y;

}

void ADS1256::StartAnalogC(int ch) {

  switch (ch) {
    case 1:
      digitalWrite(PIN_CS, LOW);
      Channel = ch;
      write_register(MUX, CH1);
      break;
    case 2:
      digitalWrite(PIN_CS, LOW);
      Channel = ch;
      write_register(MUX, CH2);
      break;
    case 3:
      digitalWrite(PIN_CS, LOW);
      Channel = ch;
      write_register(MUX, CH3);
      break;
    case 4:
      digitalWrite(PIN_CS, LOW);
      Channel = ch;
      write_register(MUX, CH4);
      break;
    default:
      return;
  }
  SPI.transfer(SYNC);    //
  SPI.transfer(WAKEUP);  //
  while (digitalRead(PIN_RDY) == HIGH); //
  SPI.transfer(RDATAC);                 //
  delayMicroseconds(10);

  digitalWrite(PIN_CS, HIGH);

}

void ADS1256::StopAnalogC() {

  digitalWrite(PIN_CS, LOW);
  while (digitalRead(PIN_RDY) == HIGH);
  SPI.transfer(SDATAC);
  digitalWrite(PIN_CS, HIGH);

}

bool ADS1256::SetPGA(int pga) {

  bool result;
  switch (pga) {
    case 1:
      digitalWrite(PIN_CS, LOW);
      PGA = pga;
      write_register(ADCON, PGA1);
      digitalWrite(PIN_CS, HIGH);
      delay(1);
      result = true;
      break;
    case 2:
      digitalWrite(PIN_CS, LOW);
      PGA = pga;
      write_register(ADCON, PGA2);
      digitalWrite(PIN_CS, HIGH);
      delay(1);
      result = true;
      break;
    case 4:
      digitalWrite(PIN_CS, LOW);
      PGA = pga;
      write_register(ADCON, PGA4);
      digitalWrite(PIN_CS, HIGH);
      delay(1);
      result = true;
      break;
    case 8:
      digitalWrite(PIN_CS, LOW);
      PGA = pga;
      write_register(ADCON, PGA8);
      digitalWrite(PIN_CS, HIGH);
      delay(1);
      result = true;
      break;
    case 16:
      digitalWrite(PIN_CS, LOW);
      PGA = pga;
      write_register(ADCON, PGA16);
      digitalWrite(PIN_CS, HIGH);
      delay(1);
      result = true;
      break;
    case 32:
      digitalWrite(PIN_CS, LOW);
      PGA = pga;
      write_register(ADCON, PGA32);
      digitalWrite(PIN_CS, HIGH);
      delay(1);
      result = true;
      break;
    case 64:
      digitalWrite(PIN_CS, LOW);
      PGA = pga;
      write_register(ADCON, PGA64);
      digitalWrite(PIN_CS, HIGH);
      delay(1);
      result = true;
      break;
    default:
      result = false;
  }
  return result;

}

bool ADS1256::SetSPS(int sps) {

  bool result;
  switch (sps) {
    case 2000:
      digitalWrite(PIN_CS, LOW);
      SPS = sps;
      write_register(DRATE, SPS2000);
      digitalWrite(PIN_CS, HIGH);
      delay(1);
      result = true;
      break;
    case 1000:
      digitalWrite(PIN_CS, LOW);
      SPS = sps;
      write_register(DRATE, SPS1000);
      digitalWrite(PIN_CS, HIGH);
      delay(1);
      result = true;
      break;
    case 500:
      digitalWrite(PIN_CS, LOW);
      SPS = sps;
      write_register(DRATE, SPS500);
      digitalWrite(PIN_CS, HIGH);
      delay(1);
      result = true;
      break;
    case 100:
      digitalWrite(PIN_CS, LOW);
      SPS = sps;
      write_register(DRATE, SPS100);
      digitalWrite(PIN_CS, HIGH);
      delay(1);
      result = true;
      break;
    case 50:
      digitalWrite(PIN_CS, LOW);
      SPS = sps;
      write_register(DRATE, SPS50);
      digitalWrite(PIN_CS, HIGH);
      delay(1);
      result = true;
      break;
    case 10:
      digitalWrite(PIN_CS, LOW);
      SPS = sps;
      write_register(DRATE, SPS10);
      digitalWrite(PIN_CS, HIGH);
      delay(1);
      result = true;
      break;
    case 5:
      digitalWrite(PIN_CS, LOW);
      SPS = sps;
      write_register(DRATE, SPS5);
      digitalWrite(PIN_CS, HIGH);
      delay(1);
      result = true;
      break;
    default:
      result = false;
  }
  return result;

}

bool ADS1256::SetPINRDY(int rdy) {
  if (2 < rdy < 10) {
    PIN_RDY = rdy;
    return true;
  }
  else {
    return false;
  }
}

bool ADS1256::SetPINCS(int cs) {
  if (2 < cs <= 10) {
    PIN_CS = cs;
    return true;
  }
  else {
    return false;
  }
}

bool ADS1256::SetPINReset(int reset) {
  if (2 < reset < 10) {
    PIN_Reset = reset;
    return true;
  }
  else {
    return false;
  }
}

int ADS1256::GetPGA() {
  return PGA;
}

int ADS1256::GetSPS() {
  return SPS;
}

int ADS1256::GetPINRDY() {
  return PIN_RDY;
}

int ADS1256::GetPINCS() {
  return PIN_CS;
}

int ADS1256::GetPINReset() {
  return PIN_Reset;
}

void ADS1256::write_register(unsigned char reg, unsigned char wdata) {
  while (digitalRead(PIN_RDY) == HIGH);                    //DRDY low to available�
  SPI.transfer(WREG | reg);             //select register�
  SPI.transfer(0x00);                   //write 1 byte
  SPI.transfer(wdata);                  //write data
  //  delayMicroseconds(1000);	//data available when next DRDY cycle�
}

long ADS1256::read_data() {
  unsigned long rx_dat[3];
  long x;
  rx_dat[0] = SPI.transfer(0xff);
  rx_dat[1] = SPI.transfer(0xff);
  rx_dat[2] = SPI.transfer(0xff);
  x = (unsigned long)rx_dat[0] * 65536 + (unsigned long)rx_dat[1] * 256 + (unsigned long)rx_dat[2];
  return x;
}

