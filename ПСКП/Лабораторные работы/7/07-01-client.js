const http = require('http');

const options = {
    hostname: 'localhost',
    port: 8000,
    path: '/movies.json',
    method: 'GET'
};

const request = http.request(options, response => {
    console.log(`Статус ответа: ${response.statusCode}`);
    console.log(`Сообщение к статусу ответа: ${response.statusMessage}`);
    console.log(`IP-адрес удаленного сервера: ${response.socket.remoteAddress}`);
    console.log(`Порт удаленного сервера: ${response.socket.remotePort}`);

    response.on('data', d => {
        process.stdout.write(d);
    });
});

request.on('error', error => {
    console.error(error);
});

request.end();
