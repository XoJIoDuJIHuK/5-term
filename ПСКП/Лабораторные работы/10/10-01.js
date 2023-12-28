const net = require('net');

const server = net.createServer((socket) => {
    socket.on('data', (data) => {
        const message = data.toString().trim();
        const response = `ECHO:${message}`;
        socket.write(response);
    });
});

const port = 3000;
server.listen(port, () => {
    console.log(`server started at localhost:${port}`);
});