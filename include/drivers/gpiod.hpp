#pragma once

struct gpiod_chip;
struct gpiod_line;

class GpioManager{
    private:
    gpiod_chip* chip_;
    gpiod_line* line_start_;
    gpiod_line* line_drop_;

    public:
    GpioManager(const char* chipName, int startLine, int dropLine);
    ~GpioManager();

    void setStart(bool ready);
    void setDrop();
};