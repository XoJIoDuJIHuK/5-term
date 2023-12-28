const http = require('http')
const fs = require('fs')
const WebSocket = require('ws')

const httpServer = http.createServer((req, res) => {
    if (req.method === 'GET' && req.url === '/start') {
        res.writeHead(200, {'Content-Type': 'text/html'})
        const html = fs.readFileSync('01-index.html', 'utf8')
        res.end(html)
    } else {
        res.writeHead(400);
        res.end('Bad Request')
    }
})

httpServer.listen(3000, () => {
    console.log('HTTP server listening at http://localhost:3000/start');
})

const wsServer = new WebSocket.Server({ port: 4000 })

httpServer.on('upgrade', (request, socket, head) => {
    wsServer.handleUpgrade(request, socket, head, (ws) => {
        wsServer.emit('connection', ws, request)
    })
})

wsServer.on('connection', (ws) => {
    console.log('WebSocket connection established')

    let lastClientMessageNumber = -1
    let serverMessageNumber = 0
    setInterval(() => {
        ws.send(`08-01-server: ${lastClientMessageNumber}->${serverMessageNumber++}`)
    }, 5000)

    ws.on('message', (message) => {
        console.log(`Received message from client: ${message}`)
        lastClientMessageNumber = message.slice(14)
    })
})
