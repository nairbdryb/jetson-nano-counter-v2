#include "LoRaWan_APP.h"
#include "Arduino.h"
#define Sensor GPIO5 // Sensor Input Pin
int sensorval = 0;  // Sensor Value
int human_count = 0;
int timer_count = 0;
int x;

/*
 * set LoraWan_RGB to Active,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */

/* OTAA para*/
uint8_t devEui[] = { 0x22, 0x32, 0x33, 0x00, 0x00, 0x88, 0x88, 0x03 }; 
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x66, 0x01 };

/* ABP para*/
uint8_t nwkSKey[] = { 0x15, 0xb1, 0xd0, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 0x18, 0x1e, 0x1e, 0xc7, 0xda,0x85 };
uint8_t appSKey[] = { 0xd7, 0x2c, 0x78, 0x75, 0x8c, 0xdc, 0xca, 0xbf, 0x55, 0xee, 0x4a, 0x77, 0x8d, 0x16, 0xef,0x67 };
uint32_t devAddr =  ( uint32_t )0x007e6ae1;

/*LoraWan channelsmask, default channels 0-7*/ 
uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = LORAWAN_CLASS;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 15000;

/*OTAA or ABP*/
bool overTheAirActivation = LORAWAN_NETMODE;

/*ADR enable*/
bool loraWanAdr = LORAWAN_ADR;

/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool keepNet = LORAWAN_NET_RESERVE;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = LORAWAN_UPLINKMODE;

/* Application port */
uint8_t appPort = 2;
/*!
* Number of trials to transmit the frame, if the LoRaMAC layer did not
* receive an acknowledgment. The MAC performs a datarate adaptation,
* according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
* to the following table:
*
* Transmission nb | Data Rate
* ----------------|-----------
* 1 (first)       | DR
* 2               | DR
* 3               | max(DR-1,0)
* 4               | max(DR-1,0)
* 5               | max(DR-2,0)
* 6               | max(DR-2,0)
* 7               | max(DR-3,0)
* 8               | max(DR-3,0)
*
* Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
* the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t confirmedNbTrials = 4;

/* Prepares the payload of the frame */
static void prepareTxFrame( uint8_t port )
{
  /*appData size is LORAWAN_APP_DATA_MAX_SIZE which is defined in "commissioning.h".
  *appDataSize max value is LORAWAN_APP_DATA_MAX_SIZE.
  *if enabled AT, don't modify LORAWAN_APP_DATA_MAX_SIZE, it may cause system hanging or failure.
  *if disabled AT, LORAWAN_APP_DATA_MAX_SIZE can be modified, the max value is reference to lorawan region and SF.
  *for example, if use REGION_CN470, 
  *the max value for different DR can be found in MaxPayloadOfDatarateCN470 refer to DataratesCN470 and BandwidthsCN470 in "RegionCN470.h".
  */
    appDataSize = 1;
    appData[0] = human_count;
    //appData[1] = 0x01;
    //appData[2] = 0x02;
    //appData[3] = 0x47;
}

void lora_send(){
    switch( deviceState )
  {
    case DEVICE_STATE_INIT:
    {
#if(LORAWAN_DEVEUI_AUTO)
      LoRaWAN.generateDeveuiByChipID();
#endif
#if(AT_SUPPORT)
      getDevParam();
#endif
      printDevParam();
      LoRaWAN.init(loraWanClass,loraWanRegion);
      deviceState = DEVICE_STATE_JOIN;
      break;
    }
    case DEVICE_STATE_JOIN:
    {
      LoRaWAN.join();
      break;
    }
    case DEVICE_STATE_SEND:
    {
      prepareTxFrame( appPort );
      LoRaWAN.send();
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
      // Schedule next packet transmission
      txDutyCycleTime = appTxDutyCycle + randr( 0, APP_TX_DUTYCYCLE_RND );
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;
    }
    case DEVICE_STATE_SLEEP:
    {
      LoRaWAN.sleep();
      break;
    }
    default:
    {
      deviceState = DEVICE_STATE_INIT;
      break;
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
#if(AT_SUPPORT)
  enableAt();
#endif
  deviceState = DEVICE_STATE_INIT;
  LoRaWAN.ifskipjoin();
  pinMode (Sensor, INPUT);  // Sensor as input
}

void loop() {
  while (!Serial.available());
  x = Serial.readString().toInt();
  human_count = x;
  lora_send();
}