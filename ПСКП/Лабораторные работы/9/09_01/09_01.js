const WebSocket = require("ws");
const fs = require("fs");
const WS_PORT = 4000;

const ws_server = new WebSocket.Server({port: WS_PORT, host: 'localhost', path: '/fileUpload'}, () => {console.log(`WebSocket-server started http://localhost:${WS_PORT}`)});

ws_server.on("connection", ws => {
    const duplex = WebSocket.createWebSocketStream(ws);

    const fileName = `./upload/file_${Math.ceil(Math.random()*100)}.txt`;

    let wfile = fs.createWriteStream(fileName);
    duplex.pipe(wfile);

    console.log(`\nFile downloaded to ${fileName}`);

    ws.onclose = event => console.log(event.code, event.reason);
});
