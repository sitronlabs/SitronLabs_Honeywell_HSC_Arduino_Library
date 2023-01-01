#ifndef HSCDANT001PGSA3_H
#define HSCDANT001PGSA3_H

/* Library headers */
#include "hsc.h"

/**
 *
 */
class hscdant001pgsa3 : public hsc_spi {
   public:
    int range_get(double &pressure_min_pa, double &pressure_max_pa) {
        pressure_min_pa = m_pressure_min_pa;
        pressure_max_pa = m_pressure_max_pa;
        return 0;
    }
    using hsc_spi::data_get;
    int data_get(double &pressure_pa, double &temperature_c) {
        uint16_t pressure_raw;
        uint16_t temperature_raw;
        int status = hsc_spi::data_get(pressure_raw, temperature_raw);
        pressure_pa = ((((double)pressure_raw - m_raw_min) * (m_pressure_max_pa - m_pressure_min_pa)) / (m_raw_max - m_raw_min)) + m_pressure_min_pa;
        temperature_c = (((double)temperature_raw / 2047.0) * 200.0) - 50.0;
        return status;
    }

   protected:
    const double m_pressure_min_pa = 0;        //!< 0 psi
    const double m_pressure_max_pa = 6894.76;  //!< 1 psi
};

#endif
