#include "../../include/drivers/gpiod.hpp"
#include <gpiod.h>
#include <unistd.h>

GpioManager::GpioManager(const char* chipName, int startLine, int dropLine){
chip_  = gpiod_chip_open_by_name(chipName);
line_start_ = gpiod_chip_get_line(chip_, startLine);   // напр. 24
line_drop_  = gpiod_chip_get_line(chip_, dropLine);	// напр. 23
gpiod_line_request_output(line_start_, "drone", 0);
gpiod_line_request_output(line_drop_,  "drone", 0);

 gpiod_line_set_value(line_start_, 1);
}

void GpioManager::setDrop() {
    gpiod_line_set_value(line_drop_, 1);
    usleep(80000);
    gpiod_line_set_value(line_drop_, 0);
}

GpioManager::~GpioManager() {
    if (chip_) gpiod_chip_close(chip_);
}

void GpioManager::setStart(bool ready) {
    gpiod_line_set_value(line_start_, ready ? 1 : 0);
}