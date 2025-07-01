#include <SoftwareSerial.h>
#include <AltSoftSerial.h>
#include <TinyGPS++.h>
#include <Wire.h>

// Nomor Telepon yang Diizinkan
const String AUTHORIZED_PHONE = "+6282199799982";

// Pin Konfigurasi
#define relayPin 5          // Relay utama (ON/OFF motor)
#define starterRelayPin 4   // Relay starter
#define vibrationPin 6      // Sensor getar
#define alarmRelayPin 12    // Relay alarm / buzzer
#define newRelayPin 7       // Relay tambahan (gunakan NO)

#define rxPin 10
#define txPin 11
SoftwareSerial sim800(rxPin, txPin);
#define gpsRXPin 9
#define gpsTXPin 8
AltSoftSerial gpsSerial(gpsRXPin, gpsTXPin);
TinyGPSPlus gps;

// Variabel
String sender_number, msg;
boolean ignition_status = false;
boolean anti_theft = false;

void setup() {
  Serial.begin(115200);
  sim800.begin(9600);
  gpsSerial.begin(9600);

  pinMode(relayPin, OUTPUT);
  pinMode(starterRelayPin, OUTPUT);
  pinMode(alarmRelayPin, OUTPUT);
  pinMode(newRelayPin, OUTPUT);
  pinMode(vibrationPin, INPUT);

  digitalWrite(relayPin, HIGH);
  digitalWrite(starterRelayPin, HIGH);
  digitalWrite(alarmRelayPin, HIGH);
  digitalWrite(newRelayPin, HIGH);

  initializeSIM800L();
}

void loop() {
  ignition_status = digitalRead(relayPin) == LOW;

  if (anti_theft && digitalRead(vibrationPin) == LOW) {
    digitalWrite(alarmRelayPin, LOW);
    delay(3000);
    digitalWrite(alarmRelayPin, HIGH);
  }

  if (sim800.available()) {
    String input = sim800.readString();
    parseData(input);
  }

  if (Serial.available()) {
    sim800.println(Serial.readString());
  }
}

void initializeSIM800L() {
  delay(7000);
  sendCommand("AT");
  sendCommand("ATE1");
  sendCommand("AT+CPIN?");
  sendCommand("AT+CMGF=1");
  sendCommand("AT+CNMI=1,2,0,0,0");
}

void sendCommand(String command) {
  sim800.println(command);
  delay(500);
  while (sim800.available()) {
    Serial.write(sim800.read());
  }
}

void parseData(String buff) {
  Serial.println("==> Data Masuk: " + buff);

  if (buff.indexOf("+CMT:") != -1) {
    int senderStart = buff.indexOf("+CMT: \"") + 7;
    int senderEnd = buff.indexOf("\",\"", senderStart);
    sender_number = buff.substring(senderStart, senderEnd);

    int msgStart = buff.indexOf("\n", senderEnd) + 1;
    msg = buff.substring(msgStart);
    msg.trim();
    msg.toLowerCase();

    Serial.println("Pengirim: " + sender_number);
    Serial.println("Isi Pesan: " + msg);

    if (sender_number != AUTHORIZED_PHONE) {
      sendSms("Nomor tidak diizinkan.");
      return;
    }

    if (msg == "get location") {
      handleLocationRequest();
    } else {
      doAction();
    }
  }
}

void doAction() {
  if (msg == "bike on") {
    if (digitalRead(newRelayPin) == HIGH) {  // Jika power utama ON (karena NO)
      digitalWrite(relayPin, LOW);
      sendSms("Motor dinyalakan.");
    } else {
      sendSms("Power utama belum ON.");
    }
  } else if (msg == "bike off") {
    digitalWrite(relayPin, HIGH);
    sendSms("Motor dimatikan.");
  } else if (msg == "bike start") {
    if (ignition_status) {
      startBike();
    } else {
      sendSms("Hidupkan motor dulu (bike on).");
    }
  } else if (msg == "anti theft on") {
    anti_theft = true;
    sendSms("Anti-pencurian aktif.");
  } else if (msg == "anti theft off") {
    anti_theft = false;
    sendSms("Anti-pencurian nonaktif.");
  } else if (msg == "on") {
    digitalWrite(newRelayPin, HIGH); // ON = HIGH karena NO
    sendSms("Power ON.");
  } else if (msg == "off") {
    digitalWrite(newRelayPin, LOW);  // OFF = LOW karena NO
    sendSms("Power OFF.");
  } else {
    sendSms("Perintah tidak dikenali.");
  }
}

void handleLocationRequest() {
  Serial.println("==> Mengambil lokasi GPS...");
  boolean newData = false;

  for (unsigned long start = millis(); millis() - start < 10000;) {
    while (gpsSerial.available()) {
      if (gps.encode(gpsSerial.read())) {
        newData = true;
      }
    }
  }

  if (newData && gps.location.isUpdated()) {
    String lokasi = "http://maps.google.com/maps?q=" +
                    String(gps.location.lat(), 6) + "," +
                    String(gps.location.lng(), 6);
    sendSms("Lokasi: " + lokasi);
  } else {
    sendSms("GPS tidak tersedia.");
  }
}

void startBike() {
  sendSms("Memulai starter...");
  digitalWrite(starterRelayPin, LOW);
  delay(3000);
  digitalWrite(starterRelayPin, HIGH);
  sendSms("Starter selesai.");
}

void sendSms(String text) {
  Serial.println("Mengirim SMS ke: " + sender_number);
  Serial.println("Isi: " + text);

  sim800.println("AT+CMGF=1");
  delay(1000);

  sim800.print("AT+CMGS=\"");
  sim800.print(sender_number);
  sim800.println("\"");
  delay(1000);

  sim800.print(text);
  delay(500);
  sim800.write(0x1A);  // Ctrl+Z
  delay(3000);

  Serial.println("SMS berhasil dikirim.");
}
