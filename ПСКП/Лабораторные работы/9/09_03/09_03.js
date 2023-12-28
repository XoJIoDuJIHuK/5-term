const WebSocket = require("ws");
const WS_PORT = 4000;

const serverWS = new WebSocket.Server({port: WS_PORT}, () => {
    console.log(`WS started on port ${WS_PORT}...`);
});

serverWS.on("connection", ws => {
    ws.on('pong', data => {});

    ws.onclose = event => console.log(event.code, event.reason);
});

let count = 0;
const fifteenSeconds = 15 * 1000;
const fiveSeconds = 5 * 1000;

setInterval(() => {
    serverWS.clients.forEach(client => {
        client.send(`09_03-server: ${++count}\n`);
    });
}, fifteenSeconds);


setInterval(() => {
    serverWS.clients.forEach(client => {
        client.ping('server: ping');
    });
    console.log(`server: ping, ${serverWS.clients.size} connected clients`)
}, fiveSeconds);
