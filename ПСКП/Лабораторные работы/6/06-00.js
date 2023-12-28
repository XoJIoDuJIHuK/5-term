const http = require('http');
const querystring = require('querystring');
const fs = require("fs");
const fs_extra = require("fs-extra");
const xml = require('xml2js');
const multiParty = require('multiparty');
const util = require('util');

const port = 5000
const host = 'localhost'
let xmlResponseId = 0
let fileId = 0

const server = http.createServer((request, response) => {
    const queryURL = new URL(`http://localhost${request.url}`)

    let requestBody = ''
    request.on('data', (chunk) => {
        requestBody += chunk;
    });
    const paths = queryURL.pathname.split('/').slice(1);
    if (paths.length === 0) {
        response.writeHead(404, '{ Content-Type: text/plain }');
        response.end('Lmao, wrong pathname');
        return;
    }

    switch (paths[0]) {
        case 'connection': {
            response.writeHead(200, '{ Content-Type: text/plain }');
            if (queryURL.searchParams && queryURL.searchParams.has('set')) {
                server.keepAliveTimeout = +queryURL.searchParams.get('set');
                response.end(`New keepAliveTimeout is set to ${server.keepAliveTimeout}`);
            } else {
                response.end(`Current keepAliveTimeout equals ${server.keepAliveTimeout}`);
            }
            break;
        }
        case 'headers': {
            response.writeHead(200, '{ Content-Type: text/plain }');
            response.write('Headers:\n');
            for (let header in request.headers) {
                response.write(`${header}: ${request.headers[header]}\n`);
            }
            response.write('Response headers:\n')
            for (let header in response.headers) {
                response.write(`${header}: ${response.headers[header]}\n`);
            }
            response.end()
            break;
        }
        case 'parameter': {
            if (paths.length === 1) {       //task 3
                const parameters = queryURL.searchParams;
                if (parameters && parameters.has('x') && parameters.has('y')) {
                    const x = +parameters.get('x');
                    const y = +parameters.get('y');
                    if (isNaN(x) || isNaN(y)) {
                        response.writeHead(403, '{ Content-Type: text/plain }');
                        response.end('X and Y must be numbers');
                    } else {
                        response.writeHead(200, '{ Content-Type: text/plain }');
                        response.end(`x + y = ${x + y}\nx - y = ${x - y}\nx * y = ${x * y}\nx / y = ${x / y}\n`);
                    }
                } else {
                    response.writeHead(413, '{ Content-Type: text/plain }');
                    response.end('Wrong parameters');
                }
            } else if (paths.length === 3) { //task 4
                const x = +paths[1];
                const y = +paths[2];
                response.writeHead(200, '{ Content-Type: text/plain }');
                if (isNaN(x) || isNaN(y)) {
                    response.end(queryURL.href);
                } else {
                    response.end(`x + y = ${x + y}\nx - y = ${x - y}\nx * y = ${x * y}\nx / y = ${x / y}\n`);
                }
            } else {
                response.writeHead(404, '{ Content-Type: text/plain }');
                response.end('Wrong pathname');
            }
            break;
        }
        case 'socket': {
            response.writeHead(200, '{ Content-Type: text/plain }');
            response.write(`host: ${request.socket.localAddress}:${request.socket.localPort}\n`);
            response.end(`server: ${response.socket.localAddress}:${response.socket.localPort}`);
            break;
        }
        case 'resp-status': {
            const parameters = queryURL.searchParams;
            if (parameters && parameters.has('code') && parameters.has('mess')) {
                response.writeHead(+parameters.get('code'), parameters.get('mess'),
                    '{ Content-Type: text/plain }');
                response.end();
            } else {
                response.writeHead(406, '{ Content-Type: text/plain }');
                response.end('Wrong parameters');
            }
            break;
        }
        case 'form': {
            fs.readFile('FormPage.html', (error, data) => {
                if (error) {
                    console.error(error);
                    response.writeHead(500);
                    response.end('Server error');
                } else {
                    response.writeHead(200, {'Content-Type': 'text/html'});
                    response.end(data);
                }
            });
            break;
        }
        case 'formparameter': {
            const parameterNames = ['text-input', 'number-input', 'date-input', 'checkbox-input',
                'radio-input', 'textarea-input', 'submit', 'submit-but-better']
            request.on('end', () => {
                response.writeHead(200, '{ Content-Type: text/html }');
                response.write('<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"><title>Task 7</title></head><body>');
                const parameters = querystring.parse(requestBody)
                console.log(parameters)
                for (name of parameterNames) {
                    if (name in parameters) {
                        response.write(`<h3>${name}: ${parameters[name]}</h3><br>`)
                    }
                }
                response.end('</body></html>');
            })
            break;
        }
        case 'json': {
            console.log(request.headers['content-type'])
            if (request.headers['content-type'] === "application/json") {
                request.on('end', () => {
                    try {
                        const json = JSON.parse(requestBody)
                        const responseJson = {
                            __comment: json['__comment'],
                            x_plus_y: +json['x'] + +json['y'],
                            Concatenation_s_o: json['s'] + ": " + json['o']['surname'] + ", " + json['o']['name'],
                            Length_m: json['m'].length
                        }
                        response.writeHead(200, '{ Content-Type: application/json }')
                        response.end(JSON.stringify(responseJson))
                    } catch (e) {
                        console.error(e)
                    }
                })
            }
            else {
                response.writeHead(404)
                response.end()
            }
            break
        }
        case 'xml': {
            if (request.headers['content-type'] === 'application/xml') {
                request.on('end', () => {
                    response.writeHead(200, '{ Content-Type: application/xml }')
                    let requestId, sumOfX, concatenation
                    xml.parseString(requestBody, (err, result) => {
                        requestId = result.request.$.id
                        sumOfX = result.request.x.map(x => parseInt(x.$.value)).reduce((sum, x) => sum + x, 0);
                        concatenation = result.request.m.map(m => m.$.value).join('');
                    })
                    const builder = new xml.Builder()
                    response.end(builder.buildObject({
                        response: {
                            $: {
                                id: xmlResponseId++,
                                response: requestId
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
                })
            } else {
                response.writeHead(404)
                response.end()
            }
            break
        }
        case 'files': {
            if (paths.length === 1) {//task 10
                fs.readdir('./static', (err, result) => {
                    if (err) console.error(err)
                    else {
                        response.writeHead(200,
                            `{ Content-Type: text/plain, X-static-files-count: "${result.length}"`)
                        response.end()
                    }
                })
            } else if (paths.length === 2) {//task 11
                fs.access(`./static/${paths[1]}`, fs.constants.F_OK, err => {
                    if (err) {
                        response.writeHead(404,
                            '{ Content-Type: text/plain,' +
                            `File-Name: ${paths[1]}` +
                            '}')
                        response.end()
                    } else {
                        response.writeHead(200,
                            '{ Content-Type: text/plain,' +
                            `File-Name: ${paths[1]}` +
                            '}')
                        fs.createReadStream(`./static/${paths[1]}`)
                            .on('data', chunk => {
                                response.write(chunk)
                            })
                            .on('end', () => {
                                response.end()
                            })
                            .on('error', err => {
                                console.error(err)
                            })
                    }
                })
            } else {
                response.writeHead(404)
                response.end()
            }
            break
        }
        case 'upload': {
            if (request.method==='GET') {
                response.end(fs.readFileSync('./upload form.html'))
            }
            else if (request.method==='POST') {
                const form = new multiParty.Form();

                form.parse(request, function(err, fields, files) {
                    if (err) console.error(err)
                    response.writeHead(200, { 'content-type': 'text/plain' });
                    response.write('received upload:\n\n');
                    console.log(fs.existsSync(files.upload[0].path))
                    console.log(files.upload[0].originalFilename)
                    let message = 'File uploaded successfully'
                    fs_extra.move(files.upload[0].path, `D:\\_УЧЁБА_\\5сем\\ПСКП\\Лабораторные работы\\6\\static\\${files.upload[0].originalFilename}`, (err) => {
                        if (err) message = err
                        console.log('file moved')
                    })
                    response.end(message)
                });
            }
            break
        }
        default: {
            response.end('Lmao, wrong pathname')
        }
    }
});

server.listen(port, host, () => {
    console.log(`Server is running on http://${host}:${port}`);
});