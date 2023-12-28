// noinspection DuplicatedCode

const DB = require('./DB')
const http = require('http')
const fs = require("fs");

const DBObject = new DB([
    {id: 0, name: "Aleh", bday: "12-03-1965"},
    {id: 1, name: "Lexa", bday: "31-07-1987"},
    {id: 2, name: "Ilya", bday: "06-02-1992"}
])

const port = 5000
const host = 'localhost'

const server = http.createServer((request, response) => {
    const queryURL = new URL(`http://localhost${request.url}`)

    let requestBody = ""
    request.on('data', (chunk) => {
        requestBody += chunk.toString();
    })

    if (queryURL.pathname === '/api/db') {

        // const method = request.headers['access-control-request-method']
        const method = request.method
        response.writeHead(200, {'Content-Type': 'text/json', 'Access-Control-Allow-Origin': '*'});

        switch (method.toLowerCase()) {
            case 'get': {
                DBObject.select().then(result => {
                    console.log("get result:", result)
                    response.end(result)
                })
                break
            }
            case 'put': {
                request.on('end', () => {
                    DBObject.insert(JSON.parse(requestBody)).then(result => {
                        console.log("put result:", result)
                        response.end(result)
                    })
                })
                break
            }
            case 'post': {
                request.on('end', () => {
                    let row
                    try {
                        row = JSON.parse(requestBody)
                        DBObject.update(row).then(result => {
                            console.log("post result:", result)
                            response.end(JSON.stringify(result))
                        })
                    } catch (error) {
                        console.log(error)
                    }
                })
                break
            }
            case 'delete': {
                let row = queryURL.searchParams.get('id')
                if (row) {
                    DBObject.delete(row).then(result => {
                        console.log("delete result:", result)
                        response.end(result)
                    }).catch(errorMessage => {
                        console.log("delete error:", errorMessage)
                        response.end(JSON.stringify({error: errorMessage}))
                    })
                }
                break
            }
        }
    }
    else if (queryURL.pathname === '/') {
        response.writeHead(200, {'Content-Type': 'text/html', 'Access-Control-Allow-Origin' : '*'});
        fs.readFile('04-02.html', (error, data) => {
            if (error) {
                console.error(error);
                response.writeHead(500);
                response.end('Server error');
            } else {
                response.writeHead(200, {'Content-Type': 'text/html'});
                response.end(data);
            }
        });
    }
    else {
        response.writeHead(404)
        response.end('Not found')
    }
});


server.listen(port, host, () => {
    console.log(`Server is running on http://${host}:${port}`);
});