const http = require('http');
const fs = require('fs');
const url = require('url');
const { parse } = require('querystring');
//const { send } = require('./m05_TOV');
const sender = require('m05_tov');

let http_handler = (req, resp)=>{
    resp.writeHead(200, {'Content-Type': 'text/html; charset=utf-8'});

    if (url.parse(req.url).pathname ==='/' && req.method === 'GET') {
        resp.end(fs.readFileSync('./05.html'));
    }
    else if (url.parse(req.url).pathname === '/' && req.method === 'POST') {
        let body = '';
        req.on('data', chunk => {body += chunk.toString();});

        req.on('end', ()=>{
            let a = parse(body);
            sender.send(a.username, a.password, a.text);
            resp.end(`<h1>OK: ${a.username} ${a.password} ${a.text}</h1>`);
        });
    }
    else resp.end('<h1>Not support</h1>');
}

let server = http.createServer(http_handler);
server.listen(5000);
console.log('Server running at http://localhost:5000/');