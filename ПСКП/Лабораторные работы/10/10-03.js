const dgram = require('dgram');

const server = dgram.createSocket('udp4');

server.on('message', (msg, rinfo) => {
    const response = `ECHO:${msg}`;
    server.send(response, rinfo.port, rinfo.address, (err) => {
        if (err) {
            console.error(err);
        } else {
            console.log(`sent "${response}" back to ${rinfo.address}:${rinfo.port}`);
        }
    });
});

server.on('listening', () => {
    const address = server.address();
    console.log(`server running at localhost:${address.port}`);
});

server.bind(3000);