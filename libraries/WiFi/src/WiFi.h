#ifndef WiFi_h
#define WiFi_h

#include "api/Print.h"
#include <stdint.h>

#include "WiFiAP.h"
#include "WiFiGeneric.h"
#include "WiFiSTA.h"
#include "WiFiScan.h"
#include "WiFiType.h"

#include "WiFiClient.h"
#include "WiFiServer.h"
#include "WiFiUdp.h"

class WiFiClass : public WiFiGenericClass, public WiFiSTAClass, public WiFiScanClass, public WiFiAPClass {
private:
  bool prov_enable;

public:
  WiFiClass() { prov_enable = false; }

  using WiFiGenericClass::channel;

  using WiFiSTAClass::BSSID;
  using WiFiSTAClass::BSSIDstr;
  using WiFiSTAClass::RSSI;
  using WiFiSTAClass::SSID;

  using WiFiScanClass::SSID;
  // using WiFiScanClass::encryptionType;
  using WiFiScanClass::BSSID;
  using WiFiScanClass::BSSIDstr;
  using WiFiScanClass::channel;
  using WiFiScanClass::RSSI;

public:
  void printDiag(Print &dest);
  friend class WiFiClient;
  friend class WiFiServer;
  friend class WiFiUDP;
  void enableProv(bool status);
  bool isProvEnabled();
};

extern WiFiClass WiFi;

#endif
