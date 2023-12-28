const net = require('net');

const serverPort = process.argv[2];
const numberX = +process.argv[3];

const client = new net.Socket();

client.connect(serverPort, 'localhost', () => {
    console.log(`connected to port ${serverPort}`);

    setInterval(() => {
        const buffer = Buffer.alloc(4)
        buffer.writeInt32BE(numberX)
        client.write(buffer);
        console.log(`sent number ${buffer.readInt32BE()} to the server`);
    }, 1000);
});

client.on('data', (data) => {
    const sum = data.toString();
    console.log(`get sum: ${sum}`);
});

client.on('close', () => {
    console.log('connection closed');
});