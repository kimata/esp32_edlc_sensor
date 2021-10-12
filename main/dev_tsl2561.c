#include "driver/i2c.h"

#include <math.h>

#include "i2c.h"

#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

#define TSL2561_DEV_ADDR        0x39

#define TSL2561_REG_TIMING      0x81
#define TSL2561_REG_CTRL        0x80
#define TSL2561_REG_DATA0       0xAC
#define TSL2561_REG_DATA1       0xAE

#define TSL2561_TIMING_1X_101MS 0x01

#define TSL2561_CTRL_ON         0x03
#define TSL2561_CTRL_OFF        0x00

float tsl2561_get_lux()
{
    float ch0;
    float ch1;

    if (i2c_read_byte(TSL2561_DEV_ADDR, TSL2561_REG_TIMING) != TSL2561_TIMING_1X_101MS) {
        i2c_write_byte(TSL2561_DEV_ADDR, TSL2561_REG_TIMING, TSL2561_TIMING_1X_101MS);
    }

    // ON
    i2c_write_byte(TSL2561_DEV_ADDR, TSL2561_REG_CTRL, TSL2561_CTRL_ON);

    vTaskDelay(101 / portTICK_RATE_MS);

    // Read
    ch0 = (float)i2c_read_word(TSL2561_DEV_ADDR, TSL2561_REG_DATA0);
    ch1 = (float)i2c_read_word(TSL2561_DEV_ADDR, TSL2561_REG_DATA1);

    // OFF
    i2c_write_byte(TSL2561_DEV_ADDR, TSL2561_REG_CTRL, TSL2561_CTRL_OFF);

    // GAIN = 1, INTEG = 101ms
    ch0 *= 16 * 322.0/81;
    ch1 *= 16 * 322.0/81;

    if (ch0 < 0.01) {
        return 0.0;
    } else if ((ch1/ch0) <= 0.52) {
        return 0.0304*ch0 - 0.062*ch0*pow(ch1/ch0, 1.4);
    } else if ((ch1/ch0) <= 0.65) {
        return 0.0224*ch0 - 0.031*ch1;
    } else if ((ch1/ch0) <= 0.80) {
        return 0.0128*ch0 - 0.0153*ch1;
    } else if ((ch1/ch0) <= 1.30) {
        return 0.00146*ch0 - 0.00112*ch1;
    } else {
        return 0.0;
    }
}
