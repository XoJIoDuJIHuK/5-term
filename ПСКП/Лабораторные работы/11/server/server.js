const http = require('http');
const get_handler = require('./get_handler');
const post_handler = require('./post_handler');
const put_handler = require('./put_handler');
const delete_handler = require('./delete_handler');
const error = require('./server_error_handler')();
let server = http.createServer();


let http_handler = (req, res) => {
    switch (req.method) {
        case 'GET':     get_handler(req, res);      break;
        case 'POST':    post_handler(req, res);     break;
        case 'PUT':     put_handler(req, res);      break;
        case 'DELETE':  delete_handler(req, res);   break;
        default:        error.handler(res, 409, 'Incorrect request method.'); break;
    }
}



server.listen(3000, () => { console.log('\n[INFO] Server running at localhost:3000/\n'); })
    .on('error', error => { console.log('\n[ERROR] ', error.message); })
    .on('request', http_handler);