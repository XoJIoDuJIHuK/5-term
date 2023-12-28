const WebSocket = require('ws');
const fs = require('fs');

const ws = new WebSocket('ws://localhost:4000');

const name = process.argv[2] || Math.ceil(Math.random()*100);
console.log(process.argv[2])
const messageToServer = {name: name, timestamp: new Date().toISOString()};

const socket = WebSocket.createWebSocketStream(ws, { encoding: 'utf8' });

ws.onopen = () => {
    ws.send(JSON.stringify(messageToServer));

    ws.on('message', message => {
        const parsedMessage = JSON.parse(message);

        console.log(`{server:${parsedMessage.n} client:${parsedMessage.name}, timestamp:${parsedMessage.timestamp}}`);
    });
};