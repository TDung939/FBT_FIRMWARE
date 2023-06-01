#include <U8g2lib.h>
#include <Arduino_GFX_Library.h>
Arduino_DataBus *bus = new Arduino_HWSPI(A3 /* DC */, A5 /* CS */);
Arduino_GFX *mylcd = new Arduino_ILI9341(bus, A4 /* RST */);
#define Forte_Green 0x25F8
// temp probe
#include <OneWire.h>
#include <DallasTemperature.h>
#define PIN_ONE_WIRE_BUS A1
OneWire oneWire(PIN_ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#define HEAT A2



float
ctemp = 0,
heatTime = 0;
unsigned int heatTemp = 39;


void setup() {
  // mosfets
  pinMode(HEAT, OUTPUT);
  mylcd->begin();
  mylcd->setRotation(1);
  mylcd->setUTF8Print(true);
  mylcd->setFont(u8g2_font_unifont_t_vietnamese1);
  mylcd->fillScreen(BLACK);
  mylcd->setTextSize(3);
  mylcd->setTextColor(0x25F8);
  mylcd->setCursor(5, 100);
  mylcd->println("FORTE BIOTECH");
  mylcd->setTextSize(1);
  mylcd->setCursor(80, 130);
  mylcd->print("Test prawns with RAPID");
  mylcd->setCursor(100, 150);
  mylcd->print("Profit no Limit");
  mylcd->setCursor(5, 240);
  mylcd->print("H1.0");
  delay(4000);
  preheat();
}

void preheat() {
  mylcd->fillScreen(BLACK);
  float current_temp = 0;
  bool hit = false;

  mylcd->setTextSize(2);
  mylcd->setTextColor(WHITE);
  mylcd->setCursor(5, 30);
  mylcd->print("Hot air ");
  mylcd->setTextColor(Forte_Green);
  mylcd->setTextSize(2);
  mylcd->setCursor(160, 30);
  mylcd->print(heatTemp);
  while (not hit) {
    sensors.requestTemperatures();  // Send the command to get temperature readings
    ctemp = sensors.getTempCByIndex(0);
    if (ctemp != current_temp) {
      current_temp = ctemp;
      mylcd->fillRect(90, 100, 300, 50, BLACK);
      mylcd->setTextSize(4);
      mylcd->setCursor(90, 150);
      mylcd->print(String(ctemp));
      ////////////////////////////////////////////////////////////////
      mylcd->setCursor(200, 150);
      mylcd->print("℃");
      ////////////////////////////////////////////////////////////////
    }
    if ((ctemp > heatTemp) && (ctemp < heatTemp + 0.5)) {
      hit = true;
    }
    if (ctemp > heatTemp + 0.2) {
      digitalWrite(HEAT, LOW);
    } else if (ctemp < heatTemp) {
      digitalWrite(HEAT, HIGH);
    }
    delay(500);
  }
  digitalWrite(HEAT, LOW);
}
//duy tri nhiet do
void heat_maintain(float heatTemp) {
  sensors.requestTemperatures();  // Send the command to get temperature readings
  ctemp = sensors.getTempCByIndex(0);
  if (ctemp < heatTemp) {
    digitalWrite(HEAT, HIGH);
  } else {
    digitalWrite(HEAT, LOW);
  }
}


void loop() {
  heat_maintain(heatTemp);
  mylcd->fillRect(90, 100, 300, 50, BLACK);
  mylcd->setTextSize(4);
  mylcd->setCursor(90, 150);
  mylcd->print(String(ctemp));
  ////////////////////////////////////////////////////////////////
  mylcd->setCursor(200, 150);
  mylcd->print("℃");
  ////////////////////////////////////////////////////////////////
}
