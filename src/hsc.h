#ifndef HSC_H
#define HSC_H

/* Arduino libraries */
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

/* C/C++ libraries */
#include <errno.h>
#include <stdint.h>

/**
 *
 */
class hsc {
   public:
    virtual int data_get(uint16_t &pressure_raw, uint16_t &temperature_raw) = 0;

   protected:
    const double m_raw_min = 1638.3;   //!< 10% of 2^14 counts, or 0x0666
    const double m_raw_max = 14744.7;  //!< 90% of 2^14 counts, or 0x399A
};

/**
 *
 */
class hsc_spi : public hsc {
   public:
    int setup(SPIClass &spi_library, const int spi_pin_cs, const int spi_speed = 800000);
    int data_get(uint16_t &pressure_raw, uint16_t &temperature_raw);

   protected:
    SPIClass *m_spi_library = NULL;
    int m_spi_pin_cs = 0;
    int m_spi_speed;
};

/**
 *
 */
class hsc_i2c : public hsc {
   public:
    int setup(TwoWire &i2c_library, uint8_t i2c_address = 0x28);
    int data_get(uint16_t &pressure_raw, uint16_t &temperature_raw);

   protected:
    TwoWire *m_i2c_library = NULL;
    uint8_t m_i2c_address = 0;
};

#endif
