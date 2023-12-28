const WebSocket = require('ws')

const ws = new WebSocket('ws://localhost:4000')
let messageNumber = 0

ws.on('open', () => {
    console.log('WebSocket connection opened');
})

ws.on('message', (message) => {
    console.log(`Received message from server: ${message}`)
})

ws.on('close', () => {
    console.log('WebSocket connection closed')
})