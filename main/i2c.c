#include "driver/i2c.h"

uint8_t i2c_read_byte(uint8_t dev_addr, uint8_t reg_addr)
{
    i2c_cmd_handle_t cmd;
    uint8_t reg_value;
    
    cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (dev_addr<<1)|I2C_MASTER_WRITE, I2C_MASTER_ACK));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, reg_addr, I2C_MASTER_ACK));
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (dev_addr<<1)|I2C_MASTER_READ, I2C_MASTER_ACK));
    ESP_ERROR_CHECK(i2c_master_read_byte(cmd, &reg_value, I2C_MASTER_ACK));
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_RATE_MS));
    i2c_cmd_link_delete(cmd);

    return reg_value;
}

uint16_t i2c_read_word(uint8_t dev_addr, uint8_t reg_addr)
{
    i2c_cmd_handle_t cmd;
    uint8_t reg_value[2];
    
    cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (dev_addr<<1)|I2C_MASTER_WRITE, I2C_MASTER_ACK));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, reg_addr, I2C_MASTER_ACK));
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (dev_addr<<1)|I2C_MASTER_READ, I2C_MASTER_ACK));
    ESP_ERROR_CHECK(i2c_master_read_byte(cmd, &reg_value[0], I2C_MASTER_ACK));
    ESP_ERROR_CHECK(i2c_master_read_byte(cmd, &reg_value[1], I2C_MASTER_ACK));
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_RATE_MS));
    i2c_cmd_link_delete(cmd);

    return reg_value[0] | (((uint16_t)reg_value[1]) << 8);
}

void i2c_write_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t value)
{
    i2c_cmd_handle_t cmd;
    
    cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (dev_addr<<1)|I2C_MASTER_WRITE, I2C_MASTER_ACK));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, reg_addr, I2C_MASTER_ACK));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, value, I2C_MASTER_ACK));
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_RATE_MS));
    i2c_cmd_link_delete(cmd);
}
