// Editar si es necesario ------------------------------

IPAddress Local_IP(192, 168, 1, 85);
IPAddress Gateway(192, 168, 1, 1);
IPAddress Subnet(255, 255, 255, 0);

const char* Ssid = "nombre de la wifi";
const char* Password = "contraseña de la wifi";

// End Editar si es necesario --------------------------

WiFiServer Server(80);
String header;

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;ing ResponseHeader;
String IndexFile;
String StyleFile;

typedef void (*ProcessRequestFn)(WiFiClient client, String header);
bool InitializeFileSystem();
void InitializeWebServer();
void ConnetToWifi();
String GetFile(String fileName);
void ServerLoop(ProcessRequestFn fn);
void SendToClient(WiFiClient client, String value);
bool RequestHas(String header, String value);

bool InitializeFileSystem()
{
    if (!SPIFFS.begin(true)) {
        Serial.println("Error al montar el sistema de archivos SPIFFS.");
        return false;
    }
    return true;
}

void InitializeWebServer()
{
    ResponseHeader = "HTTP/1.1 200 OK\r\nContent - type:text / html\r\nConnection : close\r\n\r\n";
    IndexFile = ResponseHeader + GetFile("/index.html");
    StyleFile = GetFile("/style.css");
}

void ConnetToWifi()
{
    bool ledStatus = true;

    if (!WiFi.config(Local_IP, Gateway, Subnet)) {
        Serial.println("Error al configurar la Wifi.");
    }

    WiFi.begin(Ssid, Password);
    Serial.print("Conectando a WiFi.");
    digitalWrite(BUILTIN_LED, ledStatus);
    while (WiFi.status() != WL_CONNECTED) {
        delay(250);
        Serial.print(".");
        ledStatus = !ledStatus;
        digitalWrite(BUILTIN_LED, ledStatus);
    }

    digitalWrite(BUILTIN_LED, LOW);
    Serial.println("");
    Serial.print("Conectado. IP: ");
    Serial.println(WiFi.localIP());

    Server.begin();
}

String GetFile(String fileName)
{
    File file = SPIFFS.open(fileName);
    if (!file) {
        Serial.println("Error al abrir " + fileName);
        return "";
    }

    char* buffer = new char[file.size()];
    int i = 0;
    while (file.available())
    {
        buffer[i++] = file.read();
    }
    file.close();
    buffer[i] = 0;

    return buffer;
}

void ServerLoop(ProcessRequestFn fn)
{
    WiFiClient client = Server.available();

    if (client) {
        currentTime = millis();
        previousTime = currentTime;
        String currentLine = "";
        while (client.connected() && currentTime - previousTime <= timeoutTime) {
            currentTime = millis();
            if (client.available()) {
                char c = client.read();
                header += c;
                if (c == '\n') {
                    if (currentLine.length() == 0) {

                        if (header.indexOf("GET / ") >= 0)
                        {
                            client.print(IndexFile);
                        }

                        else if (header.indexOf("GET /style.css") >= 0)
                        {
                            client.print(StyleFile);
                        }
                        else if (header.indexOf("GET /action?do=") >= 0)
                        {
                            fn(client, header);
                        }

                        client.println();

                        break; // while
                    }
                    else {
                        currentLine = "";
                    }
                }
                else if (c != '\r') {
                    currentLine += c;
                }
            }
        }
        header = "";
        client.stop();
    }
}

void SendToClient(WiFiClient client, String value)
{
    client.println(ResponseHeader + value);
}

bool RequestHas(String header, String value)
{
    return header.indexOf(value) >= 0;
}