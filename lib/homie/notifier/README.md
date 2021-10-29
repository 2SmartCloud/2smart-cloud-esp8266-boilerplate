# Notifier

Класс для работы с уведомлениями.

***
## API

- Notifier(MqttClient* mqtt_client)
- void SetUserHash(String user_hash)
- bool CreateNotification(const char* text)

***

**Notifier(MqttClient\* mqtt_client)**

Создает объект модуля для уведомлений.

- mqtt_client: указатель на объект [MQTT клиента](../../mqtt_client/src/README.md).

***

**void SetUserHash(const char\* user_hash)**

Запоминает идентификатор пользователя.

- user_hash: идентификатор пользователя.

***

**bool CreateNotification(const char\* text)**

Публикует уведомление в топик `<user_hash>/notifications/create` 

- text: JSON строка с параметрами уведомления.

Возвращает статус публикации уведомлений.
