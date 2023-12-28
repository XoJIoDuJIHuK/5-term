const DB = require('./DB')
const url = require('url')
const http = require('http')

const DBObject = new DB([
    {id: 0, name: "Aleh", bday: "12.03.1965"},
    {id: 1, name: "Lexa", bday: "31.07.1987"},
    {id: 2, name: "Ilya", bday: "06.02.1992"}
])

const port = 5000
const host = 'localhost'

const server = http.createServer((request, response) => {
    const queryURL = new URL(`http://localhost${request.url}`)

    let requestBody = ""
    request.on('data', (chunk) => {
        requestBody += chunk;
    })

    if (queryURL.pathname === '/api/db') {
        const method = request.method
        response.writeHead(200, {'Content-Type': 'text/json'});
        switch (method.toLowerCase()) {
            case 'get': {
                DBObject.select().then(result => {
                    response.end(result)
                })
                break
            }
            case 'put': {
                request.on('end', () => {
                    DBObject.insert(JSON.parse(requestBody)).then(result => {
                        response.end(result)
                    })
                })
                break
            }
            case 'post': {
                request.on('end', () => {
                    let row = JSON.parse(requestBody)
                    DBObject.update(JSON.parse(requestBody)).then(result => {
                        response.end(JSON.stringify(result))
                    })
                })
                break
            }
            case 'delete': {
                let row = queryURL.searchParams.get('id')
                let errorMessage
                if (row) {
                    DBObject.delete(row).then(result => {
                        response.end(result)
                    }).catch(errorMessage => {
                        response.end(JSON.stringify({error: errorMessage}))
                    })
                }
                break
            }
        }
    }
    else {
        response.writeHead(404)
        response.end('Not found')
    }
});

server.listen(port, host, () => {
    console.log(`Server is running on http://${host}:${port}`);
});