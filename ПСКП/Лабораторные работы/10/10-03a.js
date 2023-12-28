const dgram = require('dgram');

const client = dgram.createSocket('udp4');

const message = 'xd 3';

client.send(message, 3000, 'localhost', (err) => {
    if (err) {
        console.error(err);
    } else {
        console.log(`sent message "${message}" to the server`);
    }
});

client.on('message', (msg, rinfo) => {
    console.log(`got response from server: ${msg}`);
    client.close();
});