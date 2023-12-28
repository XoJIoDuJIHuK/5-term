const WebSocket = require("ws");
const fs = require("fs");
const WS_PORT = 4000;

const serverWS = new WebSocket.Server({
    port: WS_PORT, host: 'localhost'}, 
    () => {console.log(`WebSocket-server started http://localhost:${WS_PORT}`)
});

let count = 1;

serverWS.on('connection', ws => {
    ws.on('message', message => {
        const messageClient = JSON.parse(message);
        console.log(`{client:${messageClient.name}, timestamp:${messageClient.timestamp}}`);

        const messageFromServer = {n: count++, name: messageClient.name, timestamp: new Date().toISOString()};
        ws.send(`${JSON.stringify(messageFromServer)}\n`);
    })
})