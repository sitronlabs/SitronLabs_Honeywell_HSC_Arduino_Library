/* Self header */
#include "hsc.h"

/**
 *
 * @param[in] i2c_library
 * @param[in] i2c_address
 */
int hsc_i2c::setup(TwoWire &i2c_library, uint8_t i2c_address) {

    /* Save i2c settings */
    m_i2c_library = &i2c_library;
    m_i2c_address = i2c_address;

    /* Return success */
    return 0;
}

/**
 *
 * @param[in] spi_library
 * @param[in] spi_pin_cs
 * @param[in] spi_speed
 */
int hsc_spi::setup(SPIClass &spi_library, const int spi_pin_cs, const int spi_speed) {

    /* Ensure spi speed is within supported range */
    if (spi_speed < 50000 || spi_speed > 800000) {
        return -EINVAL;
    }

    /* Save spi settings */
    m_spi_library = &spi_library;
    m_spi_pin_cs = spi_pin_cs;
    m_spi_speed = spi_speed;

    /* Setup pin */
    digitalWrite(m_spi_pin_cs, HIGH);
    pinMode(m_spi_pin_cs, OUTPUT);

    /* Return success */
    return 0;
}

/**
 *
 * @return 0 if the sensor returned fresh data,
 *         2 if the sensor returned stale data,
 *         1 if the sensor is in factory programming mode,
 *         3 if the sensor reports a diagnostic error,
 *         Or a negative error code otherwise.
 */
int hsc_i2c::data_get(uint16_t &pressure_raw, uint16_t &temperature_raw) {

    /* Ensure i2c library has been configures */
    if (!m_i2c_library) {
        return -EINVAL;
    }

    /* Perform a 4-byte data readout using i2c */
    uint8_t buf[4] = {0};
    if (Wire.requestFrom(m_i2c_address, 4, true) != 4) {
        return -EIO;
    }
    for (uint8_t i = 0; i < 4; i++) buf[i] = Wire.read();

    /* Extract status from bits 31:30 */
    uint8_t status = ((buf[0] >> 6) & 0b11);

    /* Extract pressure information from bits 29:16 */
    pressure_raw = ((((uint16_t)buf[0]) << 8) & 0x3F00) |
                   ((((uint16_t)buf[1]) >> 0) & 0x00FF);

    /* Extract temperature information from bits 15:5 */
    temperature_raw = ((((uint16_t)buf[2]) << 3) & 0x07F8) |
                      ((((uint16_t)buf[3]) >> 5) & 0x0007);

    /* Return status */
    return status;
}

/**
 *
 * @return 0 if the sensor returned fresh data,
 *         2 if the sensor returned stale data,
 *         1 if the sensor is in factory programming mode,
 *         3 if the sensor reports a diagnostic error,
 *         Or a negative error code otherwise.
 */
int hsc_spi::data_get(uint16_t &pressure_raw, uint16_t &temperature_raw) {

    /* Ensure spi library has been configures */
    if (!m_spi_library) {
        return -EINVAL;
    }

    /* Perform a 4-byte data readout using spi */
    uint8_t buf[4] = {0};
    SPI.beginTransaction(SPISettings(m_spi_speed, MSBFIRST, SPI_MODE1));
    digitalWrite(m_spi_pin_cs, LOW);
    SPI.transfer(buf, 4);
    digitalWrite(m_spi_pin_cs, HIGH);
    SPI.endTransaction();
    if (buf[0] == 0xFF && buf[1] == 0xFF && buf[2] == 0xFF && buf[3] == 0xFF) {
        return -EIO;
    }

    /* Extract status from bits 31:30 */
    uint8_t status = ((buf[0] >> 6) & 0b11);

    /* Extract pressure information from bits 29:16 */
    pressure_raw = ((((uint16_t)buf[0]) << 8) & 0x3F00) |
                   ((((uint16_t)buf[1]) >> 0) & 0x00FF);

    /* Extract temperature information from bits 15:5 */
    temperature_raw = ((((uint16_t)buf[2]) << 3) & 0x07F8) |
                      ((((uint16_t)buf[3]) >> 5) & 0x0007);

    /* Return status */
    return status;
}
