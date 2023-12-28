const http = require("http");
const url = require("url");

const host = 'localhost';
const port = 5000;

function Factorial(k) {
    if (k < 3) return k;
    else return k * Factorial(k - 1);
}

const server = http.createServer((request,
                                  response) => {
    if (request.url.startsWith('/fact')) {
        const myUrl = new URL(`http://${host}:${port}` + request.url);
        let k = myUrl.searchParams.get('k');
        response.writeHead(200, {'Content-Type': 'text/json', 'Access-Control-Allow-Origin' : '*'});
        response.end(JSON.stringify({k: k, fact: Factorial(k)}));
    } else {
        response.writeHead(404);
        response.end('Not found');
    }
});

server.listen(port, host, () => {
    console.log(`Server is running on http://${host}:${port}`);
});