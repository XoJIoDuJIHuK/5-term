const WebSocket = require('ws')

const ws = new WebSocket('ws://localhost:4000')
let messageNumber = 0

ws.on('open', () => {
    console.log('WebSocket connection opened');
    const messageInterval = setInterval(() => {
        ws.send(`08-01-client: ${messageNumber++}`)
    }, 3000)
    setTimeout(() => {
        ws.close()
        clearInterval(messageInterval)
    }, 25000)
})

ws.on('message', (message) => {
    console.log(`Received message from server: ${message}`)
})

ws.on('close', () => {
    console.log('WebSocket connection closed')
})