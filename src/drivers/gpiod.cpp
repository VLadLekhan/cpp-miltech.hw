#include "../../include/drivers/gpiod.hpp"
#include <gpiod.h>
#include <unistd.h>
#include <string>
#include <stdexcept>

GpioManager::GpioManager(const char* chipName, int startLine, int dropLine)
    : startLine_((unsigned int)startLine), dropLine_((unsigned int)dropLine) {

    // v2 хоче повний шлях до пристрою, а не просто ім'я чипа (як було в v1)
    std::string path = std::string("/dev/") + chipName;

    gpiod_chip* chip = gpiod_chip_open(path.c_str());
    if (!chip) throw std::runtime_error("Не вдалося відкрити GPIO chip: " + path);

    gpiod_line_settings* settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);

    gpiod_line_config* lineCfg = gpiod_line_config_new();
    unsigned int offsets[2] = { startLine_, dropLine_ };
    gpiod_line_config_add_line_settings(lineCfg, offsets, 2, settings);

    gpiod_request_config* reqCfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(reqCfg, "drone");

    request_ = gpiod_chip_request_lines(chip, reqCfg, lineCfg);

    // після request_ чіп більше не потрібен - request_ вже незалежний від нього
    gpiod_request_config_free(reqCfg);
    gpiod_line_config_free(lineCfg);
    gpiod_line_settings_free(settings);
    gpiod_chip_close(chip);

    if (!request_) throw std::runtime_error("Не вдалося зареєструвати GPIO лінії START/DROP");

    // одразу піднімаємо START, як і раніше
    gpiod_line_request_set_value(request_, startLine_, GPIOD_LINE_VALUE_ACTIVE);
}

GpioManager::~GpioManager() {
    if (request_) gpiod_line_request_release(request_);
}

void GpioManager::setStart(bool ready) {
    gpiod_line_request_set_value(request_, startLine_,
        ready ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
}

void GpioManager::setDrop() {
    gpiod_line_request_set_value(request_, dropLine_, GPIOD_LINE_VALUE_ACTIVE);
    usleep(80000);
    gpiod_line_request_set_value(request_, dropLine_, GPIOD_LINE_VALUE_INACTIVE);
}