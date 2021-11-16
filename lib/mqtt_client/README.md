# MqttClient

A class for working with the MQTT protocol.

***

## API

- MqttClient()
- bool Init(String username, String host, String port, String password, std::function<void(char*, uint8_t*, unsigned int)> callback)
- void MqttLoop()
- bool Subscribe(const char * topic)
- bool Publish(const char * topic, const char * value, bool retained)
- bool IsConnected()
- bool IsReconnected()

***

**MqttClient()**

Creates an MQTT client object.

***

**bool Init(String username, String host, String port, String password, std::function<void(char\*, uint8_t\*, unsigned int)> callback)**

Saves the received parameters and initializes the MQTT client.

- username: user ID.
- host: the address where the MQTT broker is installed.
- port: the port on which the broker is located.
- password: password for connecting to the broker.
- callback: a function that will be called to accept new messages from the broker. 

Returns the initialization status.

***

**void MqttLoop()**

Checks the connection to the broker and the presence of new messages.

***

**bool Subscribe(const char \*topic)**

- topic: topic to subscribe to.

Returns the subscription status.

***

**bool Publish(const char \*topic, const char \*value, bool retained)**

Checks if the client is connected to the broker and publishes a message to the topic.

- topic: topic to subscribe to.
- value: message.
- retained: flag responsible for storing the message in the topic.

Returns the status of the publication.

***

**bool IsConnected()**

Returns the connection status.

***

**bool IsReconnected()**

Returns the reconnect flag.

***
