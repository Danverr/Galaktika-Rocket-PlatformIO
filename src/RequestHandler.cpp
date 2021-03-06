#include "RequestHandler.h"

RequestHandler::RequestHandler() : response({ 0, 0 }) {
}

void RequestHandler::read(byte b) {
    if (b == RF_ESCAPE_MARKER && !isEscaped) {
        isEscaped = true; // Если считали маркер экранирования, значит следующий байт не служебный
        return;
    }

    // Если есть место, читаем запрос
    if (currentByte != RF_REQUEST_SIZE) {
        buff[currentByte++] = b;

        // Прочитали весь запрос
        if (currentByte == RF_REQUEST_SIZE && buff[0] != lastRequestId) {
            // Тут делаем, что попросили
            // ...

            // Формируем ответ
            lastRequestId = buff[0];
            response[0] = (response[0] + 1) % RF_MAX_RESPONSE_ID; // ID ответа
            response[1] = 1;                                      // Сам ответ
        }
    } else if (b == RF_START_MARKER && !isEscaped) {
        currentByte = 0; // Если считали маркер начала, начнем записывать байты в буффер
    }

    isEscaped = false;
}

const byte* RequestHandler::getResponse() {
    return response;
}