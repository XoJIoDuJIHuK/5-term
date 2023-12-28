const net = require('net');

const PORT1 = 40000;
const PORT2 = 50000;
const INTERVAL = 5000;

let sum1 = 0;
let sum2 = 0;

const server1 = net.createServer((socket) => {
    console.log('client connected to port 40000');

    socket.on('data', (data) => {
        const number = data.readInt32BE()
        sum1 += number;
        console.log(`got number ${number} at port 40000`);
    });

    setInterval(() => {
        socket.write(sum1.toString());
        console.log(`sum ${sum1} sent to port 40000`);
    }, INTERVAL);
});

server1.listen(PORT1, () => {
    console.log(`server launched at localhost:${PORT1}`);
});

const server2 = net.createServer((socket) => {
    console.log('client connected at port 50000');

    socket.on('data', (data) => {
        const number = data.readInt32BE();
        sum2 += number;
        console.log(`got number ${number} at port 50000`);
    });

    setInterval(() => {
        socket.write(sum2.toString());
        console.log(`sum ${sum2} sent to port 50000`);
    }, INTERVAL);
});

server2.listen(PORT2, () => {
    console.log(`server launched at localhost:${PORT2}`);
});