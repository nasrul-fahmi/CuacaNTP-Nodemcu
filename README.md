# Prakiraan cuaca NodeMCU
Prakiraan cuaca satu jam kedepan dan jam waktu akurat dari NTP (Network Time Protocol).\
Dilengkapi dengan suhu dan tanggal dari internet

## Wiring
SCL = D1\
SDA = D2\
_Jangan gunakan pin 3V karena kontras LCD akan redup Gunakan pin Vin dari NodeMCU ke I2C LCD, supaya daya LCD tidak kurang_

![Wiring](https://tutorkeren.com/sites/default/files/gambar_inline/arduino_-_i2c_lcd_wiring_pengkabelan.jpg)

## Library
| Nama library | Author                                                       |
|--------------|--------------------------------------------------------------|
| Arduino JSON |[@Benoit Blanchon](https://github.com/bblanchon)|
| Liquid Crystal I2C|[@John Rickman](https://github.com/johnrickman)|
| NTP Client | [@Fabrice](https://github.com/FWeinb)|
| Time | [@Paul Stoffregen](https://github.com/PaulStoffregen)|

### Embeded Library
- ESP8266HTTPClient
- WiFiClient
- ESP8266WiFi
## API key accuweather

| Parameter | Description                |
| :-------- | :------------------------- |
| `{apikey}`pada URL |  **Dibutuhkan**. API key mu |

_Daftar ke website [AccuWeather](https://developer.accuweather.com/) untuk mendapatkan API key_


  
