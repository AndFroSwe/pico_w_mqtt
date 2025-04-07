#include "lwip/apps/mqtt.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <pico/time.h>
#include <stdio.h>

#define WIFI_SSID "your_ssid"
#define WIFI_PASSWORD "your_password"
#define _MQTT_BROKER_IP "192.168.1.100"
#define _MQTT_PORT 1883

static int inpub_id = -1;

// Forward declarations
void example_do_connect(mqtt_client_t *client);
static void mqtt_connection_cb(mqtt_client_t *client, void *arg,
                               mqtt_connection_status_t status);
static void mqtt_sub_request_cb(void *arg, err_t result);
static void mqtt_incoming_publish_cb(void *arg, const char *topic,
                                     u32_t tot_len);
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len,
                                  u8_t flags);
static void mqtt_pub_request_cb(void *arg, err_t result);
void example_publish(mqtt_client_t *client, void *arg);

// Definitions
void connect_wifi() {
  if (cyw43_arch_init()) {
    printf("Wi-Fi init failed\n");
    return;
  }
  cyw43_arch_enable_sta_mode();
  while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD,
                                            CYW43_AUTH_WPA2_AES_PSK, 10000)) {
    printf("Failed to connect. Retrying...\n");
  }
  printf("Connected to Wi-Fi\n");
}

void example_do_connect(mqtt_client_t *client) {
  struct mqtt_connect_client_info_t ci;
  err_t err;
  ip_addr_t ip_addr;

  // Setup an empty client info structure
  memset(&ci, 0, sizeof(ci));
  ci.client_id = "lwip_test";

  // Set the IP address of the MQTT broker
  ip4addr_aton(_MQTT_BROKER_IP, &ip_addr);

  // Connect to MQTT broker
  err = mqtt_client_connect(client, &ip_addr, _MQTT_PORT, mqtt_connection_cb,
                            NULL, &ci);

  if (err != ERR_OK) {
    printf("mqtt_connect return %d\n", err);
  }
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg,
                               mqtt_connection_status_t status) {
  err_t err;

  if (status == MQTT_CONNECT_ACCEPTED) {
    printf("mqtt_connection_cb: Successfully connected\n");

    // Setup callback for incoming messages
    mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb,
                            mqtt_incoming_data_cb, arg);

    // Subscribe to a topic
    err = mqtt_subscribe(client, "subtopic", 1, mqtt_sub_request_cb, arg);
    if (err != ERR_OK) {
      printf("mqtt_subscribe return: %d\n", err);
    }
  } else {
    printf("mqtt_connection_cb: Disconnected, reason: %d\n", status);
    example_do_connect(client);
  }
}

static void mqtt_sub_request_cb(void *arg, err_t result) {
  printf("Subscribe result: %d\n", result);
}

static void mqtt_incoming_publish_cb(void *arg, const char *topic,
                                     u32_t tot_len) {
  printf("Incoming publish at topic %s with total length %u\n", topic,
         (unsigned int)tot_len);

  if (strcmp(topic, "print_payload") == 0) {
    inpub_id = 0;
  } else if (topic[0] == 'A') {
    inpub_id = 1;
  } else {
    inpub_id = 2;
  }
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len,
                                  u8_t flags) {
  printf("Incoming publish payload with length %d, flags %u\n", len,
         (unsigned int)flags);

  if (flags & MQTT_DATA_FLAG_LAST) {
    if (inpub_id == 0) {
      if (data[len - 1] == 0) {
        printf("mqtt_incoming_data_cb: %s\n", (const char *)data);
      }
    } else if (inpub_id == 1) {
      // handle A-case
    } else {
      printf("mqtt_incoming_data_cb: Ignoring payload...\n");
    }
  }
}

void example_publish(mqtt_client_t *client, void *arg) {
  const char *pub_payload = "PubSubHubLubJub";
  err_t err;
  u8_t qos = 2;
  u8_t retain = 0;

  err = mqtt_publish(client, "pub_topic", pub_payload, strlen(pub_payload), qos,
                     retain, mqtt_pub_request_cb, arg);
  if (err != ERR_OK) {
    printf("Publish err: %d\n", err);
  }
}

static void mqtt_pub_request_cb(void *arg, err_t result) {
  if (result != ERR_OK) {
    printf("Publish result: %d\n", result);
  }
}

int main(void) {
  stdio_init_all();
  connect_wifi();

  mqtt_client_t *client = mqtt_client_new();
  if (client != nullptr) {
    example_do_connect(client);
  }
  // Your main loop or OS scheduler should run here
  int i = 0;
  while (1) {
    sleep_ms(1000);
    printf("Cycle number %d", i++);
  }

  return 0;
}
