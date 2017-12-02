# NodeMCU
Control LED light by calling API from cloud server
API:
POST http://39.106.107.244:8080/springTest/user?id=1&name=on
name == on, LED is on;
name == off, LED is off;
name == other value, LED is off.