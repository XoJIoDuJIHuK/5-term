const http = require("http");
const fs = require("fs");

const host = 'localhost';
const port = 5001;

const server = http.createServer((request, response) => {
    // Проверяем, что запрос по адресу /html
    if (request.url === '/jquery') {
        // Читаем файл index.html
        fs.readFile('jquery.html', (error, data) => {
            // Если ошибка, то выводим ее
            if (error) {
                console.error(error);
                response.writeHead(500); // Код ошибки сервера
                response.end('Server error');
            } else {
                // Если нет ошибки, то отправляем файл в ответе
                response.writeHead(200, {'Content-Type': 'text/html'}); // Код успешного ответа и тип контента
                response.end(data); // Отправляем содержимое файла
            }
        });
    } else {
        // Если запрос не по адресу /html, то отправляем сообщение об ошибке
        response.writeHead(404); // Код ошибки клиента
        response.end('Not found');
    }
});

// Запускаем сервер на порту 5000
server.listen(port, host, () => {
    console.log(`Server is running on http://${host}:${port}`);
});