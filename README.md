# TransportDisplay

Connecting to public transport APIs using a NodeMCU chip.

Project developed for TitanHacks event 2020.
Collaborative project with [PixelSergey](https://github.com/PixelSergey)
and [Kartik](https://github.com/hax0kartik)

## Notes:
- Run the command `git update-index --skip-worktree config.ino` to avoid committing
the modified config file when contributing

## Compilation
- Generate a DigiTransit API key using the instructions at <https://digitransit.fi/en/developers/api-registration/>
- Input this API key, as well as your WiFi SSID and password, into the `config.ino` file
- Compile through the Arduino IDE.
  - NodeMCU must be [set up as a board](https://www.instructables.com/id/Steps-to-Setup-Arduino-IDE-for-NODEMCU-ESP8266-WiF/)
  - This [Liquid Crystal I2C library](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library)
    must be installed to your `.../Arduino/libraries` folder
