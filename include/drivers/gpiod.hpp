#pragma once

struct gpiod_line_request;

class GpioManager {
    private:
    gpiod_line_request* request_ = nullptr;
    unsigned int startLine_;
    unsigned int dropLine_;

    public:
    GpioManager(const char* chipName, int startLine, int dropLine);
    ~GpioManager();

    void setStart(bool ready);
    void setDrop();
};