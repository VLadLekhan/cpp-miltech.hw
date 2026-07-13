#include "../../include/drivers/uart_manager.hpp"
#include <cstdio>
#include <cstring>
#include <unistd.h>

UartManager::UartManager(const char* dev) {

  fd = init(dev);
}

int UartManager::init(const char* dev){
  int fd = open(dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (fd < 0) { perror("open"); return -1; }

  termios tio{};
  tcgetattr(fd, &tio);
  cfmakeraw(&tio);                     	// 8N1, без обробки символів
  cfsetispeed(&tio, B115200);
  cfsetospeed(&tio, B115200);          	// швидкість з обох боків однакова!
  tio.c_cflag |= (CLOCAL | CREAD);
  tcsetattr(fd, TCSANOW, &tio);
  return fd;
}

void UartManager::update() {
    uint8_t buf[256];
    int n = read(fd, buf, sizeof(buf));        // прочитати доступні байти
    uint8_t type, len, payload[260];
    for (int i = 0; i < n; i++){
        if (parser.feed(buf[i], type, payload, len)) { 	// зібрався цілий кадр
	        if (type == dlink::PKT_TELEMETRY) memcpy(&lastTelemetry_, payload, sizeof (lastTelemetry_)); 
          if (type == dlink::PKT_TARGET) memcpy(&lastTargetPos_, payload, sizeof (lastTargetPos_));
          if (type == dlink::PKT_AMMO) memcpy(&lastAmmoCfg_, payload, sizeof (lastAmmoCfg_));
          if (type == dlink::PKT_CONFIG)    memcpy(&droneConfig_, payload, sizeof(droneConfig_));
        }
    }
}


void UartManager::sendControl( float accel, float turnRate) {
  dlink::Control c{ accel, turnRate };        	// обидва float у [-1..1]
  uint8_t out[64];
  size_t m = encode(dlink::PKT_CONTROL, &c, sizeof c, out);
  write(fd, out, m);
}

