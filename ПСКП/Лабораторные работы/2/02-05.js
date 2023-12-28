const http = require("http");
const fs = require("fs");

const host = 'localhost';
const port = 5001;

const server = http.createServer((request, response) => {
    if (request.url === '/fetch') {
        fs.readFile('fetch.html', (error, data) => {
            if (error) {
                console.error(error);
                response.writeHead(500);
                response.end('Server error');
            } else {
                response.writeHead(200, {'Content-Type': 'text/html'});
                response.end(data);
            }
        });
    } else {
        response.writeHead(404);
        response.end('Not found');
    }
});

server.listen(port, host, () => {
    console.log(`Server is running on http://${host}:${port}`);
});