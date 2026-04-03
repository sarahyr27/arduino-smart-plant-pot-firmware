# Smart Plant Pot Firmware

This firmware integrates data from three sensors into the system logic, allowing the user to assess the current health of the plant. By pressing a button, one of three LEDs (green, yellow, or red) lights up to indicate the plant’s condition.

The project was designed and developed from scratch as the final assignment for the Tangible Interaction course in the Multimedia Degree at Universitat Oberta de Catalunya.

Additionally, the sensor data was used as input for a simple Java application, which, in conjunction with the Twilio API and the PanamaHitek library, enabled the system to send a test WhatsApp message containing sensor data. This feature was not fully developed, as it was slightly outside the project's scope, but it was included as a proof of concept to demonstrate how a complete version of the system could behave.

## Electronic components

    - DHT22 (AM2302) Temperature sensor and air humidity sensor
    - GY-302 (BH1750) Light sensor
    - Soil Moisture Sensor V1.2
    - Button and LED diodes
    - Arduino Uno R3

## Tinkercad model

<img width="803" height="723" alt="Image" src="https://github.com/user-attachments/assets/68ffada8-fcf3-4227-89ce-e921dbfd6c8d" />

## Video presentation of the prototype

If you would like to see the prototype in action, a [video presentation](https://www.youtube.com/watch?v=PIjZ2ib99bc) is available on my academic YouTube channel.
