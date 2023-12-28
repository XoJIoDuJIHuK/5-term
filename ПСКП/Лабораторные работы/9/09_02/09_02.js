const WebSocket = require("ws");
const fs = require("fs");
const WS_PORT = 4000;

const ws_server = new WebSocket.Server({port: WS_PORT, host: 'localhost', path: '/fileDownload'}, () => {console.log(`WebSocket-server started http://localhost:${WS_PORT}`)});

ws_server.on("connection", ws => {
    const duplex = WebSocket.createWebSocketStream(ws);

    const fileName = `./download/file.txt`;

    let sendedFile = fs.createReadStream(fileName);
    sendedFile.pipe(duplex);

    console.log(`\nFile sended`);

    ws.onclose = event => console.log(event.code, event.reason);
});
