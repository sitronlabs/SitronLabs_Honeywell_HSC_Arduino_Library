#ifndef HSCMRNV160MG2A3_H
#define HSCMRNV160MG2A3_H

/* Library headers */
#include "hsc.h"

/**
 *
 */
class hscmrnv160mg2a3 : public hsc_i2c {
   public:
    int range_get(double &pressure_min_pa, double &pressure_max_pa) {
        pressure_min_pa = m_pressure_min_pa;
        pressure_max_pa = m_pressure_max_pa;
        return 0;
    }
    using hsc_i2c::data_get;
    int data_get(double &pressure_pa, double &temperature_c) {
        uint16_t pressure_raw;
        uint16_t temperature_raw;
        int status = hsc_i2c::data_get(pressure_raw, temperature_raw);
        pressure_pa = ((((double)pressure_raw - m_raw_min) * (m_pressure_max_pa - m_pressure_min_pa)) / (m_raw_max - m_raw_min)) + m_pressure_min_pa;
        temperature_c = (((double)temperature_raw / 2047.0) * 200.0) - 50.0;
        return status;
    }

   protected:
    const double m_pressure_min_pa = 0;          //!< 0 mmHg
    const double m_pressure_max_pa = 21331.579;  //!< 160 mmHg
};

#endif
