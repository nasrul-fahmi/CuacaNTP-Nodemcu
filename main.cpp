#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

// Membuat object
LiquidCrystal_I2C lcd(0x27,16,2);
WiFiUDP udp;
WiFiClient klien;
HTTPClient http;
NTPClient waktu(udp, "asia.pool.ntp.org", utcOffsetInSeconds);
DynamicJsonDocument dokumen(512);

// Membuat global variabel & konstanta
const char* ssid = "Nasrul";
const char* pass = "????????";
const long utcOffsetInSeconds = 25200;
const char* fakeApi = "http://192.168.0.3/bio.php";
const char* host = "http://dataservice.accuweather.com/forecasts/v1/hourly/1hour/3460808?apikey={apikey}&language=id&details=true&metric=true";
const char namaHari[7][10] = {"Minggu,", "Senin,", "Selasa,", "Rabu,", "Kamis,", "Jum`at,", "Sabtu,"};
char namaBulan[12][5] = {"-Jan","-Feb","-Mar","-Apr","-Mei","-Jun","-Jul","-Agu","-Sep","-Okt","-Nov","-Des"};
time_t epoch;
unsigned long prevMsUpdate = 0;
unsigned long prevMsDisplay = 0;
boolean printCuaca = true;


// karakter suhu derajat
byte customChar[] = {
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

// menampilkan runing text di baris 2
void showLetters(int printStart, int startLetter, String data)
{
  lcd.setCursor(printStart, 1);
  for (int letter = startLetter; letter <= startLetter + 15; letter++)
  {
    lcd.print(data[letter]);
  }
  lcd.print(" ");
  delay(250);
}

// fungsi saat pertama kali dijalankan
void setup() 
{
    lcd.init();
    lcd.backlight();
    Serial.begin(9600);
    pinMode(D4, OUTPUT);

    // Menghubungkan ke WiFi
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        digitalWrite(D4, LOW);
        delay(100);
        digitalWrite(D4, HIGH);
        delay(100);
    }
    // Memulai layanan http
    http.begin(klien,host);
    http.GET();

    // Memulai NTP (Network Time Protocol)
    waktu.begin();

    String dataJson = http.getString();
    Serial.println("ags");

    // Mengatur waktu berdasarkan NTP
    waktu.update();
    setTime(waktu.getEpochTime());
    Serial.println(year(epoch));
    
    deserializeJson(dokumen,dataJson);

    // Membuat karakter yang telak didefinisikan
    lcd.createChar(0, customChar);

}
 
// fungsi yang dijalankan terus menerus setelah fungsi setup()
void loop() 
{
    int menitSekarang = waktu.getMinutes();
    int detikSekarang = waktu.getSeconds();

    // Setiap menit 6(satu jam) melakukan request ke web accuweather
    if (menitSekarang == 5 && detikSekarang == 59) {
        http.GET();
        String dataJson = http.getString();

        // Melaukan deserialisasi dari JSON ke variabel
        deserializeJson(dokumen, dataJson);
        delay(500);
    }
    // Menyimpan hasil dari internet yang berupa JSON ke variabel
    String cuaca = dokumen[0]["IconPhrase"];
    float nilaiSuhu = dokumen[0]["Temperature"]["Value"];
    float  nilaiSuhuAsli = dokumen[0]["RealFeelTemperature"]["Value"];
    unsigned long msSekarang = millis();

        // Mengupdate & mengatur waktu NTP setiap 5 menit
        if (msSekarang - prevMsUpdate >= 300000)//3600000)
        {
            prevMsUpdate = msSekarang;
            waktu.update();
            epoch = waktu.getEpochTime();
            setTime(epoch);
        }

        // Melakukan perubahan logika untuk pergantian layar setiap 5 detik
        if (msSekarang - prevMsDisplay >= 5000)
        {
            prevMsDisplay = msSekarang;
            if (printCuaca)
            {
                lcd.clear();
                printCuaca = false;
            } else {
                lcd.clear();
                printCuaca =true;
            }
        }
        
    // Melakukan pergantian layar LCD berdasarkan logika
    // Menampilkan perkiraan cuaca saat nilai printCuaca true 
    if (printCuaca)
        { 
            lcd.setCursor(0,0);
            lcd.print(nilaiSuhuAsli);
            lcd.print("/");
            lcd.print(nilaiSuhu);
            lcd.setCursor(14,0);

            // membuat karakter yang telak di definisikan ke LCD
            lcd.write(0);
            lcd.print("C");

            // Melakukan scroll ketika text > 16
            if (cuaca.length()>16)
            {
                for (int letter = 0; letter <= cuaca.length() - 16; letter++) //From 0 to upto n-16 characters supply to below function
                {
                    showLetters(0, letter,cuaca);
                }

            // Tidak mengscrooll ketika text < 16
            } else {
                lcd.setCursor(0,1);
                lcd.print(cuaca);
            }
        }

        // Menampilkan waktu ketika printCuaca false
        else {
            lcd.setCursor(4,0);
            lcd.print(waktu.getFormattedTime());
            String tanggal = String(namaHari[weekday()-1]) + String(day()) + String(namaBulan[month()-1]);
            lcd.setCursor(0,1);
            lcd.print(tanggal);
        }
}