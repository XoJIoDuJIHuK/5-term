const WebSocket = require('ws');
const fs = require('fs');

const ws = new WebSocket('ws://localhost:4000');

const connectionServer = WebSocket.createWebSocketStream(ws, { encoding: 'utf8' });

ws.on('ping', data => {
});

connectionServer.pipe(process.stdout);