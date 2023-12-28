const http = require("http");
const fs = require("fs");

const host = 'localhost';
const port = 5000;

const server = http.createServer((request, response) => {
    if (request.url === '/jpg') {
        fs.readFile('image.jpg', (error, data) => {
            if (error) {
                console.error(error);
                response.writeHead(500);
                response.end('Server error');
            } else {
                response.writeHead(200, {'Content-Type': 'image/jpg'});
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