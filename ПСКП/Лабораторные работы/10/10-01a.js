const net = require('net');

const client = new net.Socket();

const serverHost = '127.0.0.1';
const serverPort = 3000;

client.connect(serverPort, serverHost, () => {
    console.log(`connected to ${serverHost}:${serverPort}`);
    const message = 'xd';
    client.write(message);
});

client.on('data', (data) => {
    const response = data.toString().trim();
    console.log(`server response|${response}`);
    client.destroy();
});

client.on('close', () => {
    console.log('connection closed');
});