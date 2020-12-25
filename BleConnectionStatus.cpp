#include "BleConnectionStatus.h"

BleConnectionStatus::BleConnectionStatus(void) {
  sem = xSemaphoreCreateBinary();
}

BLE2902 *BleConnectionStatus::descriptor() {
  return (BLE2902*) this->inputGamepad->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
}

void BleConnectionStatus::onConnect(BLEServer* pServer) {
  this->connected = true;
  descriptor()->setNotifications(true);
  xSemaphoreGive(sem);
}

void BleConnectionStatus::onDisconnect(BLEServer* pServer) {
  this->connected = false;
  descriptor()->setNotifications(false);
  xSemaphoreTake(sem, portMAX_DELAY);
}
