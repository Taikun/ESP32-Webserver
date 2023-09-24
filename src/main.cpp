#include <Arduino.h>
#include <WiFi.h>

#include <esp_http_server.h>
#include <credentials.h>




/**
 * HTTP server.
 * Works with ESP-IDF library internally.
 * 
 * Usage:
 * First call http.StartServer()
 * Then call http.RegisterHandler(...) for all handlers
 */
class HTTPServer {
public:
    HTTPServer(): conf(HTTPD_DEFAULT_CONFIG()) {
    }

    void StartServer() {
        if (httpd_start(&server, &conf) != ESP_OK) {
            ESP_LOGE("HTTP server", "Error starting server!");
        }
    }

    void RegisterHandler(const httpd_uri_t *uri_handler) {
        httpd_register_uri_handler(this->server, uri_handler);
    }

    httpd_handle_t server = nullptr;
    httpd_config_t conf;
};

static const httpd_uri_t rootHandler = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = [](httpd_req_t *req) {
        httpd_resp_set_type(req, "text/html");
        httpd_resp_send(req, "<h1>Hello Compulabers!!</h1>", HTTPD_RESP_USE_STRLEN);

        return ESP_OK;
    }
};

HTTPServer http;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    // Connect Wifi, restart if not connecting
    // https://techoverflow.net/2021/01/21/how-to-fix-esp32-not-connecting-to-the-wifi-network/
    uint32_t notConnectedCounter = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.println("Wifi connecting...");
        notConnectedCounter++;
        if(notConnectedCounter > 300) { // Reset board if not connected after 30s
            Serial.println("Resetting due to Wifi not connecting...");
            ESP.restart();
        }
    }
    Serial.print("Wifi connected, IP address: ");
    Serial.println(WiFi.localIP());

    http.StartServer();
    http.RegisterHandler(&rootHandler);
}

void loop() {
    // Nothing to do here.
    delay(1000);
}