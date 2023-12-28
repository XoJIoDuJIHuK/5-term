const url = require('url');
const error = require('./server_error_handler')();
const DB = require('../database/db_module')();


function Delete_Handler(req, res) {

    let pathName = decodeURI(url.parse(req.url).pathname);
    let pathParameters = pathName.split('/');
    console.log('DELETE:\t' + pathName);
    res.writeHead(200, { 'Content-Type': 'application/json; charset=utf-8' });


    switch ('/api/' + pathParameters[2]) {

        case '/api/faculties': {
            DB.findFaculty(pathParameters[3])
                .then(result => {
                    if (result.recordset.length == 0) throw 'Faculty not found';
                    res.write(JSON.stringify(result.recordset, null, 4));
                })
                .catch(err => { error.handler(res, 431, err); });

            DB.deleteFaculty(pathParameters[3])
                .then(() => { res.end(); })
                .catch(err => { error.handler(res, 432, err.message); });
            break;
        }


        case '/api/pulpits': {
            DB.findPulpit(pathParameters[3])
                .then(result => {
                    if (result.recordset.length == 0) throw 'Pulpit not found';
                    res.write(JSON.stringify(result.recordset, null, 4));
                })
                .catch(err => { error.handler(res, 433, err); });

            DB.deletePulpit(pathParameters[3])
                .then(() => { res.end(); })
                .catch(err => { error.handler(res, 434, err.message); });

            break;
        }


        case '/api/subjects': {
            DB.findSubject(pathParameters[3])
                .then(result => {
                    if (result.recordset.length == 0) throw 'Subject not found';
                    res.write(JSON.stringify(result.recordset, null, 4));
                })
                .catch(err => { error.handler(res, 435, err); });

            DB.deleteSubject(pathParameters[3])
                .then(() => { res.end(); })
                .catch(err => { error.handler(res, 436, err.message); });

            break;
        }


        case '/api/auditoriumtypes': {
            DB.findAuditoriumType(pathParameters[3])
                .then(result => {
                    if (result.recordset.length == 0) throw 'Auditorium type not found';
                    res.write(JSON.stringify(result.recordset, null, 4));
                })
                .catch(err => { error.handler(res, 437, err); });

            DB.deleteAuditoriumType(pathParameters[3])
                .then(() => { res.end(); })
                .catch(err => { error.handler(res, 438, err.message); });

            break;
        }


        case '/api/auditoriums': {
            DB.findAuditorium(pathParameters[3])
                .then(result => {
                    if (result.recordset.length == 0) throw 'Auditorium not found';
                    res.write(JSON.stringify(result.recordset));
                })
                .catch(err => { error.handler(res, 439, err); });

            DB.deleteAuditorium(pathParameters[3])
                .then(() => { res.end(); })
                .catch(err => { error.handler(res, 440, err.message); });

            break;
        }


        default: error.handler(res, 410, 'Incorrect URL'); break;
    }

}


module.exports = (req, res) => new Delete_Handler(req, res);
