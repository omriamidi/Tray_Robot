// version 2 explanation - 
// The movement of the robot is carried out by control from the network.
// The component I'm using now is an ESP32 that allows control by wifi, so make sure you connect to the component's network.
// In this file, an HTML file is attached along with javascript, the purpose of which is to link the commands given through 
// the network with the activation of the motors.

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESP32Servo.h>

Servo myservo;
Servo myservo2;

int pos = 0;
int pos2 = 0; 
const char* ssid = "SHoSH";
const char* password = "123456789";
String state = "off";
WebServer server(80);
int servoDelay = 15; // Time delay for servo movement in milliseconds
int targetPos = 0;
int targetPos2 = 0; 
unsigned long lastServoMoveTime = 0;

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

  server.on("/1", []() {
    targetPos = 0;
    Serial.println("1");
    handleRoot();
  });
  server.on("/2", []() {
    targetPos = 90;
    Serial.println("2");
    handleRoot();
  });
  server.on("/3", []() {
    targetPos = 180;
    Serial.println("3");
    handleRoot();
  });
  server.on("/4", []() { 
    targetPos2 = 0; 
    Serial.println("4");
    handleRoot(); 
}); 

server.on("/5", []() { 
    targetPos2 = 180; 
    Serial.println("5");
    handleRoot();
}); 

  server.onNotFound(handleNotFound);
    myservo.attach(13);
    myservo2.attach(12); 

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2); //allow the cpu to switch to other tasks
  moveServo();
}

void handleRoot() {
String htmlContent = "<!DOCTYPE html>\n";
htmlContent += "<html lang=\"en\">\n";
htmlContent += "<head>\n";
htmlContent += "    <meta charset=\"UTF-8\">\n";
htmlContent += "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
htmlContent += "    <title>SHOSH</title>\n";
htmlContent += "    <style>\n";
htmlContent += "        body {\n";
htmlContent += "            font-family: Arial, sans-serif;\n";
htmlContent += "            text-align: center;\n";
htmlContent += "            background-color: #f0f0f0;\n";
htmlContent += "            margin: 0;\n";
htmlContent += "            padding: 0;\n";
htmlContent += "        }\n";
htmlContent += "\n";
htmlContent += "        h1 {\n";
htmlContent += "            color: #333;\n";
htmlContent += "            margin-top: 20px;\n";
htmlContent += "        }\n";
htmlContent += "\n";
htmlContent += "        button {\n";
htmlContent += "            font-size: 18px;\n";
htmlContent += "            padding: 10px 20px;\n";
htmlContent += "            margin: 10px;\n";
htmlContent += "            cursor: pointer;\n";
htmlContent += "            border: none;\n";
htmlContent += "            border-radius: 5px;\n";
htmlContent += "            transition: all 0.6s; /* Add a smooth transition effect for size change */\n";
htmlContent += "        }\n";
htmlContent += "\n";
htmlContent += "        .button-group {\n";
htmlContent += "            margin-top: 20px;\n";
htmlContent += "        }\n";
htmlContent += "\n";
htmlContent += "        .button-group button {\n";
htmlContent += "            background-color: grey;\n";
htmlContent += "        }\n";
htmlContent += "\n";
htmlContent += "        .button-group button.active {\n";
htmlContent += "            background-color: green;\n";
htmlContent += "            transform: scale(2.0); /* Increase size when active */\n";
htmlContent += "        }\n";
htmlContent += "    </style>\n";
htmlContent += "</head>\n";
htmlContent += "<body>\n";
htmlContent += "    <h1>SHOSH - TRAIL ROBOT!</h1>\n";

// Spin move buttons
htmlContent += "    <div class=\"button-group\">\n";
htmlContent += "        <h2>Spin Move:</h2>\n";
htmlContent += "        <button id=\"button1\" onclick=\"toggleButton(this, 3)\">left: (-120 degrees) </button>\n";
htmlContent += "        <button id=\"button2\" onclick=\"toggleButton(this, 2)\">middle: (0 degrees)</button>\n";
htmlContent += "        <button id=\"button3\" onclick=\"toggleButton(this, 1)\">right: (120 degrees)</button>\n";
htmlContent += "    </div>\n";

// Linear move buttons
htmlContent += "    <div class=\"button-group\">\n";
htmlContent += "        <h2>Linear Move:</h2>\n";
htmlContent += "        <button id=\"button4\" onclick=\"toggleButton(this, 4)\">Forward</button>\n";
htmlContent += "        <button id=\"button5\" onclick=\"toggleButton(this, 5)\">Backwards</button>\n";
htmlContent += "    </div>\n";

htmlContent += "    <script>\n";
htmlContent += "        function toggleButton(button, id) {\n";
htmlContent += "            // Reset the style of all buttons\n";
htmlContent += "            document.getElementById('button1').classList.remove('active');\n";
htmlContent += "            document.getElementById('button2').classList.remove('active');\n";
htmlContent += "            document.getElementById('button3').classList.remove('active');\n";
htmlContent += "            document.getElementById('button4').classList.remove('active');\n";
htmlContent += "            document.getElementById('button5').classList.remove('active');\n";
htmlContent += "            // Set the active style for the clicked button\n";
htmlContent += "            button.classList.add('active');\n";
htmlContent += "            console.log(id);\n";
htmlContent += "            window.location.href = `/${id}`;\n";
htmlContent += "        }\n";
htmlContent += "    </script>\n";
htmlContent += "</body>\n";
htmlContent += "</html>";
  server.send(404, "text/html", htmlContent);
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
  // Check if it's time to move the servo again
  if ((millis() - lastServoMoveTime >= servoDelay) && (pos != targetPos || pos2 != targetPos2)) {
    lastServoMoveTime = millis(); // Update the last time the servo moved - instead of using delay function!

    if (pos != targetPos) { 
      if(pos > targetPos){
        pos = pos - 1;
      }
      if(pos < targetPos){
        pos = pos + 1;
      }
      myservo.write(pos);
    }

    if (pos2 != targetPos2) { 
      if (pos2 > targetPos2) {  
          pos2 = pos2 - 1; 
      }
      if (pos2 < targetPos2) {
          pos2 = pos2 + 1;
      }
      myservo2.write(pos2);
    }
  }
}
