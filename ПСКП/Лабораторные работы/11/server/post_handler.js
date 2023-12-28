const url = require('url');
const error = require('./server_error_handler')();
const DB = require('../database/db_module')();


function Post_Handler(req, res) {

    let json = '';
    let pathName = url.parse(req.url, true).pathname;
    console.log('POST:\t' + pathName);
    res.writeHead(200, { 'Content-Type': 'application/json; charset=utf-8' });


    switch (pathName) {

        case '/api/faculties': {
            req.on('data', chunk => { json += chunk; });
            req.on('end', () => {
                json = JSON.parse(json);
                console.log('INSERT:\t' + JSON.stringify(json) + '\n');

                DB.findFaculty(json.FACULTY)
                    .then(result => { if (result.recordset.length != 0) throw 'There is already such faculty' })
                    .catch(err => { error.handler(res, 441, err.message); });

                DB.insertFaculties(json.FACULTY, json.FACULTY_NAME)
                    .then(() => { res.end(JSON.stringify(json, null, 4)); })
                    .catch(err => { error.handler(res, 416, err.message); });
            });
            break;
        }


        case '/api/pulpits': {
            req.on('data', chunk => { json += chunk; });
            req.on('end', () => {
                json = JSON.parse(json);
                console.log('INSERT:\t' + JSON.stringify(json) + '\n');

                DB.findPulpit(json.PULPIT)
                    .then(result => { if (result.recordset.length != 0) throw 'There is already such pulpit'; })
                    .catch(err => { error.handler(res, 442, err); });

                DB.findFaculty(json.FACULTY)
                    .then(result => { if (result.recordset.length == 0) throw 'Faculty not found'; })
                    .catch(err => { error.handler(res, 421, err); });

                DB.insertPulpits(json.PULPIT, json.PULPIT_NAME, json.FACULTY)
                    .then(() => { res.end(JSON.stringify(json)); })
                    .catch(err => { error.handler(res, 417, err.message); });
            });
            break;
        }


        case '/api/subjects': {
            req.on('data', chunk => { json += chunk; });
            req.on('end', () => {
                json = JSON.parse(json);
                console.log('INSERT:\t' + JSON.stringify(json) + '\n');

                DB.findSubject(json.SUBJECT)
                    .then(result => { if (result.recordset.length != 0) throw 'There is already such subject' })
                    .catch(err => { error.handler(res, 443, err); });

                DB.findPulpit(json.PULPIT)
                    .then(result => { if (result.recordset.length == 0) throw 'Pulpit not found'; })
                    .catch(err => { error.handler(res, 423, err); });

                DB.insertSubjects(json.SUBJECT, json.SUBJECT_NAME, json.PULPIT)
                    .then(() => { res.end(JSON.stringify(json, null, 4)); })
                    .catch(err => { error.handler(res, 418, err.message); });
            });
            break;
        }


        case '/api/auditoriumtypes': {
            req.on('data', chunk => { json += chunk; });
            req.on('end', () => {
                json = JSON.parse(json);
                console.log('INSERT:\t' + JSON.stringify(json) + '\n');

                DB.findAuditoriumType(json.AUDITORIUM_TYPE)
                    .then(result => { if (result.recordset.length != 0) throw 'There is already such auditorium type' })
                    .catch(err => { error.handler(res, 444, err); });

                DB.insertAuditoriumTypes(json.AUDITORIUM_TYPE, json.AUDITORIUM_TYPENAME)
                    .then(() => { res.end(JSON.stringify(json, null, 4)); })
                    .catch(err => { error.handler(res, 419, err.message); });
            });
            break;
        }


        case '/api/auditoriums': {
            req.on('data', chunk => { json += chunk; });
            req.on('end', () => {
                json = JSON.parse(json);
                console.log('INSERT:\t' + JSON.stringify(json) + '\n');

                DB.findAuditorium(json.AUDITORIUM)
                    .then(result => { if (result.recordset.length != 0) throw 'There is already such auditorium'; })
                    .catch(err => { error.handler(res, 445, err); });

                DB.findAuditoriumType(json.AUDITORIUM_TYPE)
                    .then(result => { if (result.recordset.length == 0) throw 'Auditorium type not found'; })
                    .catch(err => { error.handler(res, 427, err); });

                DB.insertAuditoriums(json.AUDITORIUM, json.AUDITORIUM_NAME, json.AUDITORIUM_CAPACITY, json.AUDITORIUM_TYPE)
                    .then(() => { res.end(JSON.stringify(json, null, 4)); })
                    .catch(err => { error.handler(res, 420, err.message); });
            });
            break;
        }


        default: error.handler(res, 410, 'Incorrect URL'); break;
    }
}



module.exports = (req, res) => new Post_Handler(req, res);
