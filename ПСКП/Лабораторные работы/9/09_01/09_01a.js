const WebSocket = require('ws');
const fs = require('fs');

const ws = new WebSocket('ws://localhost:4000/fileUpload');

const duplex = WebSocket.createWebSocketStream(ws, { encoding: 'utf8' });

const file = fs.createReadStream('./file.txt');
file.pipe(duplex);