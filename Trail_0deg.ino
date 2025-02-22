// Version 4 explanation 
// This is the final version! :) 
// Here you can check all the gestures and UI code - with it you can control the robot by buttons

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESP32Servo.h>

Servo myservo;
Servo myservo2;
Servo myservo3; 


int pos = 0;
int pos2 = 0; 
int pos3 = 0; 
const char* ssid = "TRAY";
const char* password = "123456789";
String state = "off";
WebServer server(80);
int servoDelay = 14; // delay Time for servo movement in milliseconds
int targetPos = 0;
int targetPos2 = 0; 
int targetPos3 = 0;
unsigned long lastServoMoveTime = 0;
const int motorSyncDelay = 80;

void setup(void) {
  Serial.begin(115200);

// Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.on("/", handleRoot);

// Original movement - starting from 0 degree
  targetPos = targetPos2 = targetPos3 = 0;
  
server.on("/1", []() { // Forward
    targetPos = 80;
    targetPos2 = 100;
    Serial.println("1");
    handleRoot();
}); 

  server.on("/2", []() { // Middle Linear
    targetPos = 130;
    targetPos2 = 50;
    Serial.println("2");
    handleRoot();
  });

  server.on("/3", []() { // Backwards  
    targetPos = 190; 
    targetPos2 = -10;
    targetPos3 = 90;
    Serial.println("3");
    handleRoot(); 
});

  server.on("/4", []() { // Left (Short)  
    targetPos3 = 60;
    Serial.println("4");
    handleRoot();
  });

  server.on("/5", []() { // Right (Short) 
    targetPos3 = 120;
    Serial.println("5");
    handleRoot();
  });

  server.on("/6", []() { // Middle spin
    targetPos3 = 90;
    Serial.println("6");
    handleRoot();
  });
    server.on("/7", []() { // Left (Long)
    targetPos3 = 180;
    Serial.println("7");
    handleRoot();
  });

  server.on("/8", []() { // Right (Long)
  targetPos3 = 0;
  Serial.println("8");
  handleRoot();
});

server.on("/9", []() { // Left & Forward (Short)
    targetPos = 100;
    targetPos2 = 80;
    targetPos3 = 60;
    Serial.println("9");
    handleRoot();
}); 

server.on("/10", []() { // Right & Forward (Short) 
    targetPos = 80;
    targetPos2 = 100;
    targetPos3 = 120;
    Serial.println("10");
    handleRoot();
}); 

server.on("/11", []() { // Left & Forward (Long)
    targetPos = 100;
    targetPos2 = 80;
    targetPos3 = 180;
    Serial.println("11");
    handleRoot();
}); 

server.on("/12", []() { // Right & Forward (Long)
    targetPos = 80;
    targetPos2 = 100;
    targetPos3 = 0;
    Serial.println("12");
    handleRoot();
});

  server.onNotFound(handleNotFound);
    myservo.attach(13);
    myservo2.attach(12);
    myservo3.attach(14);
     

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2); // allow the cpu to switch to other tasks
  moveServo();
}

void handleRoot() {



// User interface - buttons

String htmlContent = "<!DOCTYPE html>\n";
htmlContent += "<html lang=\"en\">\n";
htmlContent += "<head>\n";
htmlContent += "    <meta charset=\"UTF-8\">\n";
htmlContent += "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
htmlContent += "    <title>TRAY</title>\n";
htmlContent += "    <style>\n";
htmlContent += "        body {\n";
htmlContent += "            display: flex;\n";
htmlContent += "            justify-content: center;\n";
htmlContent += "            align-items: center;\n";
htmlContent += "            height: 100vh;\n";
htmlContent += "            font-family: 'Times New Roman', serif;\n";
htmlContent += "            margin: 0;\n";
htmlContent += "            padding: 0;\n";
htmlContent += "            background-color: #C8C1C1;\n";
htmlContent += "        }\n";
htmlContent += "        .container {\n";
htmlContent += "            text-align: center;\n";
htmlContent += "        }\n";
htmlContent += "        h1 {\n";
htmlContent += "            font-size: 35px;\n"; // Increased from 48px to 72px
htmlContent += "            color: #2c3e50;\n";
htmlContent += "            margin-bottom: 30px;\n"; // Increased from 40px to 60px
htmlContent += "        }\n";
htmlContent += "        h2 {\n";
htmlContent += "            font-size: 20px;\n"; // Increased from 36px to 54px
htmlContent += "            color: #2c3e50;\n";
htmlContent += "            margin-bottom: 20px;\n"; // Increased from 30px to 45px
htmlContent += "        }\n";
htmlContent += "        button {\n";
htmlContent += "            font-size: 24px;\n"; // Increased from 24px to 36px
htmlContent += "            padding: 20px 40px;\n"; // Increased padding
htmlContent += "            margin: 20px;\n"; // Increased margin
htmlContent += "            cursor: pointer;\n";
htmlContent += "            border: 2px solid #2c3e50;\n";
htmlContent += "            border-radius: 10px;\n";  
htmlContent += "            background-color: #ecf0f1;\n";
htmlContent += "            color: #2c3e50;\n";
htmlContent += "            transition: all 0.3s ease;\n";
htmlContent += "        }\n";
htmlContent += "        button:hover {\n";
htmlContent += "            background-color: #bdc3c7;\n";
htmlContent += "        }\n";
htmlContent += "        .button-group {\n";
htmlContent += "            margin-top: 30px;\n"; // Increased from 40px to 60px
htmlContent += "        }\n";
htmlContent += "        .button-group button.active {\n";
htmlContent += "            background-color: #2980b9;\n";
htmlContent += "            color: #ffffff;\n";
htmlContent += "            transform: scale(1.2);\n";
htmlContent += "        }\n";
htmlContent += "    </style>\n";
htmlContent += "</head>\n";
htmlContent += "<body>\n";
htmlContent += "    <div class=\"container\">\n";
htmlContent += "        <h1>ROBOTIC  TRAY</h1>\n";
htmlContent += "        <div class=\"button-group\">\n";
htmlContent += "            <button id=\"button1\" onclick=\"toggleButton(this, 1)\">Forward</button>\n";
htmlContent += "            <button id=\"button2\" onclick=\"toggleButton(this, 2)\">Middle Linear</button>\n";
htmlContent += "            <button id=\"button3\" onclick=\"toggleButton(this, 3)\">Backwards</button>\n";
htmlContent += "        </div>\n";
htmlContent += "        <div class=\"button-group\">\n";
htmlContent += "            <button id=\"button4\" onclick=\"toggleButton(this, 4)\">Left (Short)</button>\n";
htmlContent += "            <button id=\"button5\" onclick=\"toggleButton(this, 5)\">Right (Short)</button>\n";
htmlContent += "        </div>\n";
htmlContent += "        <div class=\"button-group\">\n";
htmlContent += "            <button id=\"button6\" onclick=\"toggleButton(this, 6)\">Middle Spin</button>\n";
htmlContent += "        </div>\n";
htmlContent += "        <div class=\"button-group\">\n";
htmlContent += "            <button id=\"button7\" onclick=\"toggleButton(this, 7)\">Left (Long)</button>\n";
htmlContent += "            <button id=\"button8\" onclick=\"toggleButton(this, 8)\">Right (Long)</button>\n";
htmlContent += "        </div>\n";
htmlContent += "        <div class=\"button-group\">\n";
htmlContent += "            <button id=\"button9\" onclick=\"toggleButton(this, 9)\">Left & Forward (Short)</button>\n";
htmlContent += "            <button id=\"button10\" onclick=\"toggleButton(this, 10)\">Right & Forward (Short)</button>\n";
htmlContent += "            <button id=\"button11\" onclick=\"toggleButton(this, 11)\">Left & Forward (Long)</button>\n";
htmlContent += "            <button id=\"button12\" onclick=\"toggleButton(this, 12)\">Right & Forward (Long)</button>\n";
htmlContent += "        </div>\n";
htmlContent += "    </div>\n";
htmlContent += "    <script>\n";
htmlContent += "        function toggleButton(button, id) {\n";
htmlContent += "            var buttons = document.querySelectorAll('.button-group button');\n";
htmlContent += "            buttons.forEach(function(btn) {\n";
htmlContent += "                btn.classList.remove('active');\n";
htmlContent += "            });\n";
htmlContent += "            button.classList.add('active');\n";
htmlContent += "            console.log(id);\n";
htmlContent += "            window.location.href = `/${id}`;\n";
htmlContent += "        }\n";
htmlContent += "    </script>\n";
htmlContent += "</body>\n";
htmlContent += "</html>";


server.send(200, "text/html", htmlContent);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}


void moveServo() {
  // Check if it's time to move the servos again
  if ((millis() - lastServoMoveTime >= servoDelay) && 
      (pos != targetPos || pos2 != targetPos2 || pos3 != targetPos3)) {
    
    lastServoMoveTime = millis(); // Update the last time the servo moved

    // Move the first servo towards the target position
    if (pos != targetPos) {
      if (pos > targetPos) {
        pos = pos - 1;
      } else {
        pos = pos + 1;
      }
      myservo.write(pos);
    }

    // Move the second servo towards the target position
    if (pos2 != targetPos2) {
      if (pos2 > targetPos2) {
        pos2 = pos2 - 1;
      } else {
        pos2 = pos2 + 1;
      }
      myservo2.write(pos2);
    }

    // Move the third servo towards the target position
    if (pos3 != targetPos3) {
      if (pos3 > targetPos3) {
        pos3 = pos3 - 1;
      } else {
        pos3 = pos3 + 1;
      }
      myservo3.write(pos3);
    }
  }
}
