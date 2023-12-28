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
        response.writeHead(200, {'Content-Type': 'text/html', 'Access-Control-Allow-Origin' : '*'});
        response.end(`<html lang="en"><head><title>Factorial</title></head><body><h3 id="header"></h3></body>
            <script>
                let header = document.querySelector('#header');
                header.innerHTML = 'k: ' + ${k} + '; fact: ' + ${Factorial(k)};
            </script></html>`);
    } else {
        response.writeHead(404);
        response.end('Not found');
    }
});

server.listen(port, host, () => {
    console.log(`Server is running on http://${host}:${port}`);
});