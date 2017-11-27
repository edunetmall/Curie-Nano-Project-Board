#include <CurieBLE.h>

#define LED 13

BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)
BLEService ControlLED("19B10010-E8F2-537E-4F6C-D104768A1214"); // BLE AnalogRead Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEUnsignedIntCharacteristic LEDStatus("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite  );

int incom = 0;
int last_incom = 0;

void setup() {
  Serial.begin(9600);
  // set Light pin to output mode
  // set advertised local name and service UUID:
  blePeripheral.setLocalName("ControlLED");
  blePeripheral.setAdvertisedServiceUuid(ControlLED.uuid());

  // add service and characteristic:
  blePeripheral.addAttribute(ControlLED);
  blePeripheral.addAttribute(LEDStatus);

  // begin advertising BLE Light service:
  blePeripheral.begin();

  Serial.println("BLE AnalogRead service.");

  pinMode(LED, OUTPUT);

}

void loop() {
  // listen for BLE peripherals to connect:

  BLECentral central = blePeripheral.central();
  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());


    // while the central is still connected to peripheral:
    while (central.connected()) {
      //Serial.println(LEDStatus.written());


      if (LEDStatus.written())
      {
        incom = LEDStatus.value();
        Serial.print("incom ");
        Serial.println(incom);
        if (incom != last_incom)
        {
          if (incom == 1)
          {
            Serial.println("LED On");

            digitalWrite(LED, HIGH);
          }
          else
          {
            Serial.println("LED Off");
            digitalWrite(LED, LOW);
          }
        }
        last_incom = incom;
      }
    }

    delay(100);
  }

  // when the central disconnects, print it out:

  Serial.print(F("Disconnected from central: "));
  Serial.println(central.address());
}

