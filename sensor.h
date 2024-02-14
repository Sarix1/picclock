#ifndef SENSOR_H
#define	SENSOR_H

struct sensor_data_s
{
    union
    {
        uint32_t bits;
        struct
        {
            uint8_t humidity_int;
            uint8_t humidity_dec ;
            uint8_t temperature_int;
            uint8_t temperature_frac;
        };
    };
};

#endif	/* SENSOR_H */

