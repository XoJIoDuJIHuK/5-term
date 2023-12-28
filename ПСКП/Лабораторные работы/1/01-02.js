const http = require("http");

const host = 'localhost';
const port = 8000;



const requestListener = function (request, response) {
    let bodyText = ""
    request.on('data', (chunk) => {
        bodyText += "<h2>Body</h2>"
        bodyText += `<b>${chunk.toString()}</b>`;
    });
    request.on('end', () => {
        let text = `<body>
                <h1>Request Information</h1>
                <p><strong>Method:</strong> ${request.method}</p>
                <p><strong>URI:</strong> ${request.url}</p>
                <p><strong>HTTP Version:</strong> ${request.httpVersion}</p>
                <h2>Headers</h2>
                <ul>`
        response.writeHead(200, { 'Content-Type': 'text/html' });
        response.write(text);
        for (const [key, value] of Object.entries(request.headers)) {
            response.write(`<li><strong>${key}:</strong> ${value}</li>`);
        }
        response.write('</ul>')
        response.write(bodyText)
        response.end('</body');
    })
};

const server = http.createServer(requestListener);
server.listen(port, host, () => {
    console.log(`Server is running on http://${host}:${port}`);
});