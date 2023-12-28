const http = require('http');
const url = require('url');
const querystring = require('querystring');
const xml = require('xml2js');
const fs = require("fs");
const multiParty = require('multiparty');
const fs_extra = require("fs-extra");
const axios = require("axios");

const server = http.createServer((request, response) => {
    response.writeHead(200, '{"Content-Type":"application/json"}')
    if (request.method === 'GET') {
        if (request.headers["task"] === '7') {
            response.end(fs.readFileSync('./render1.png'))
        }
        else {
            response.end(JSON.stringify(Object.fromEntries(new URL('http://localhost:8000' + request.url)
                .searchParams)));
        }
    }
    else if (request.method === 'POST') {
        let body = ""
        request.on('data', chunk => {
            body += chunk
        })
        request.on('end', () => {
            if (request.headers["content-type"] === 'application/json') {
                console.log(JSON.parse(body))
                response.end(JSON.stringify(JSON.parse(body)))
            }
            else if (request.headers["content-type"] === 'application/x-www-form-urlencoded') {
                response.end(JSON.stringify(querystring.parse(body)))
            }
            else if (request.headers["content-type"] === 'application/xml') {
                response.writeHead(200, '{ Content-Type: application/xml }')
                let requestId, sumOfX, concatenation
                xml.parseString(body, (err, result) => {
                    requestId = result.request.$.id
                    sumOfX = result.request.x.map(x => parseInt(x.$.value)).reduce((sum, x) => sum + x, 0);
                    concatenation = result.request.m.map(m => m.$.value).join('');
                })
                const builder = new xml.Builder()
                response.end(builder.buildObject({
                    response: {
                        $: {
                            id: 1,
                            response: 1
                        },
                        sum: {
                            $: {
                                element: 'x',
                                result: sumOfX
                            }
                        },
                        concat: {
                            $: {
                                element: 'm',
                                result: concatenation
                            }
                        }
                    }
                }))
            }
            else if (request.headers["task"] === '6') {
                request.on('end', () => {
                    try {
                        const { fileData, fileName } = JSON.parse(body);

                        if (!fileData || !fileName) {
                            response.writeHead(400, { 'Content-Type': 'application/json' });
                            response.end(JSON.stringify({ error: 'Invalid request data' }));
                            return;
                        }
                        fs.writeFileSync(`./static/${fileName}`, Buffer.from(fileData, 'base64'));

                        const fileInfo = {
                            name: fileName,
                            size: Buffer.from(fileData, 'base64').length,
                        };

                        response.writeHead(200, { 'Content-Type': 'application/json' });
                        response.end(JSON.stringify({ fileInfo }));
                    } catch (error) {
                        console.error('Error handling file upload:', error.message);
                        response.writeHead(500, { 'Content-Type': 'application/json' });
                        response.end(JSON.stringify({ error: 'Internal Server Error' }));
                    }
                });
            }
        })
    }
});

server.listen(8000, 'localhost', () => {
    console.log('Server running at port 8000');
});
