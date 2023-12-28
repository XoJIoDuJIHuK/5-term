const http = require("http");
const url = require("url");

const host = 'localhost';
const port = 5000;

function factorial(n, callback) {
    if (n === 0) {
        process.nextTick(() => callback(null, 1));
    } else {
        process.nextTick(() => {
            factorial(n - 1, (err, result) => {
                if (err) {
                    callback(err);
                } else {
                    callback(null, n * result);
                }
            });
        });
    }
}

const server = http.createServer(async (request,
                                        response) => {
    if (request.url.startsWith('/fact')) {
        const myUrl = new URL(`http://${host}:${port}` + request.url);
        let k = myUrl.searchParams.get('k');

        factorial(k, (err, result) => {
            if (err) {
                response.statusCode = 500;
                response.end(`Error: ${err.message}`);
            } else {
                response.writeHead(200, {'Content-Type': 'text/json', 'Access-Control-Allow-Origin': '*'});
                response.end(JSON.stringify({k: k, fact: result}));
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