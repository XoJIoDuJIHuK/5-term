const http = require("http");
const fs = require("fs");

const host = 'localhost';
const port = 5000;

const server = http.createServer((request, response) => {
    if (request.url === '/api/name') {
        response.writeHead(200, {'Content-Type': 'text/plain', 'Access-Control-Allow-Origin' : '*'});
        response.end("Олег Точило сын Вячеславович");
    } else {
        response.writeHead(404);
        response.end('Not found');
    }
});

server.listen(port, host, () => {
    console.log(`Server is running on http://${host}:${port}`);
});