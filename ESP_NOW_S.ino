#include "Arduino.h"
#include <esp_now.h>
#include <WiFi.h>



// REPLACE WITH THE MAC Address of your receiver
//uint8_t broadcastAddress1[] = {0xBC, 0xDD, 0xC2, 0xC4, 0x87, 0xF0};
//uint8_t broadcastAddress2[] = {0x4C, 0x11, 0xAE, 0xEF, 0xDF, 0x14};
uint8_t broadcastAddress3[] = {0xBC, 0xDD, 0xC2, 0xC3, 0x58, 0x28};

int ID0;
int ID2;
int incomingData;
// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
  int id = 1;
  int x;
} struct_message;

typedef struct struct_message_r {
  int id;
  int x;
} struct_message_r;

// Create a struct_message to hold sensor readings
struct_message PIR;

// Create a struct_message to hold incoming sensor readings
struct_message_r incomingReadings0;
struct_message_r incomingReadings2;


;

//struct_message nodesStruct[3] = {node0,node1,node2};

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  
  memcpy(&incomingReadings0, incomingData, sizeof(incomingReadings0));
  memcpy(&incomingReadings2, incomingData, sizeof(incomingReadings2));
    
    ID0 = incomingReadings0.id;
    ID2 = incomingReadings2.id;
    

    Serial.printf("x value: %d  recieved from : node %d \n ", incomingReadings0.x,incomingReadings0.id);

    Serial.println();
    
    //Serial.printf("x value: %d  recieved from : node %d \n ", incomingReadings2.x,incomingReadings2.id);

 Serial.println();
    if ((ID0 == 111 && incomingReadings0.x == 1) || (ID2 == 0 && incomingReadings2.x == 1))
    {
      digitalWrite(14,HIGH);
      delay(2000);
    }
   
     else   
     {
      digitalWrite(14,LOW);
    }
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

 pinMode(12,INPUT);
 pinMode(14,OUTPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo;


    memcpy(peerInfo.peer_addr, broadcastAddress3, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("Failed to add peer");
      return;
    }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

  PIR.x = digitalRead(12);
  Serial.println();
  Serial.printf("The data read from PIR in node 1 is : %d",PIR.x);
  Serial.println();
  // Send message via ESP-NOW
  esp_err_t result1 = esp_now_send(broadcastAddress3, (uint8_t *) &PIR, sizeof(struct_message));

    if (result1 == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }

  delay(2000);
}
