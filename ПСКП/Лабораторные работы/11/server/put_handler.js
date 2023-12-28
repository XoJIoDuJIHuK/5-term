const url = require('url');
const error = require('./server_error_handler')();
const DB = require('../database/db_module')();


function Put_Handler(req, res) {

    let json = '';
    let pathName = url.parse(req.url, true).pathname;
    console.log('PUT:\t' + pathName);
    res.writeHead(200, { 'Content-Type': 'application/json; charset=utf-8' });


    switch (pathName) {

        case '/api/faculties': {
            req.on('data', chunk => { json += chunk; });
            req.on('end', () => {
                json = JSON.parse(json);
                console.log('UPDATE:\t' + JSON.stringify(json) + '\n');

                DB.findFaculty(json.FACULTY)
                    .then(result => {
                        if (result.recordset.length == 0) throw 'Faculty not found';
                        res.write(JSON.stringify(result.recordset, null, 4));
                    })
                    .catch(err => { error.handler(res, 421, err); });

                DB.updateFaculties(json.FACULTY, json.FACULTY_NAME)
                    .then(() => { res.end(); })
                    .catch(err => { error.handler(res, 422, err.message); });
            });
            break;
        }


        case '/api/pulpits': {
            req.on('data', chunk => { json += chunk; });
            req.on('end', () => {
                json = JSON.parse(json);
                console.log('UPDATE:\t' + JSON.stringify(json) + '\n');

                DB.findPulpit(json.PULPIT)
                    .then(result => {
                        if (result.recordset.length == 0) throw 'Pulpit not found';
                        res.write(JSON.stringify(result.recordset, null, 4));
                    })
                    .catch(err => { error.handler(res, 423, err); });

                DB.updatePulpits(json.PULPIT, json.PULPIT_NAME, json.FACULTY)
                    .then(() => { res.end(); })
                    .catch(err => { error.handler(res, 424, err.message); });
            });
            break;
        }


        case '/api/subjects': {
            req.on('data', chunk => { json += chunk; });
            req.on('end', () => {
                json = JSON.parse(json);
                console.log('UPDATE:\t' + JSON.stringify(json) + '\n');

                DB.findSubject(json.SUBJECT)
                    .then(result => {
                        if (result.recordset.length == 0) throw 'Subject not found';
                        res.write(JSON.stringify(result.recordset, null, 4));
                    })
                    .catch(err => { error.handler(res, 425, err); });

                DB.updateSubjects(json.SUBJECT, json.SUBJECT_NAME, json.PULPIT)
                    .then(() => { res.end(); })
                    .catch(err => { error.handler(res, 426, err.message); });
            });
            break;
        }


        case '/api/auditoriumtypes': {
            req.on('data', chunk => { json += chunk; });
            req.on('end', () => {
                json = JSON.parse(json);
                console.log('UPDATE:\t' + JSON.stringify(json) + '\n');

                DB.findAuditoriumType(json.AUDITORIUM_TYPE)
                    .then(result => {
                        if (result.recordset.length == 0) throw 'Auditorium type not found';
                        res.write(JSON.stringify(result.recordset, null, 4));
                    })
                    .catch(err => { error.handler(res, 427, err); });

                DB.updateAuditoriumTypes(json.AUDITORIUM_TYPE, json.AUDITORIUM_TYPENAME)
                    .then(() => { res.end(); })
                    .catch(err => { error.handler(res, 428, err.message); });
            });
            break;
        }


        case '/api/auditoriums': {
            req.on('data', chunk => { json += chunk; });
            req.on('end', () => {
                json = JSON.parse(json);
                console.log('UPDATE:\t' + JSON.stringify(json) + '\n');

                DB.findAuditorium(json.AUDITORIUM)
                    .then(result => {
                        if (result.recordset.length == 0) throw 'Auditorium not found';
                        res.write(JSON.stringify(result.recordset, null, 4));
                    })
                    .catch(err => { error.handler(res, 429, err); });

                DB.updateAuditoriums(json.AUDITORIUM, json.AUDITORIUM_NAME, json.AUDITORIUM_CAPACITY, json.AUDITORIUM_TYPE)
                    .then(() => { res.end(); })
                    .catch(err => { error.handler(res, 430, err.message); });
            });
            break;
        }


        default: error.handler(res, 410, 'Incorrect URL'); break;
    }
}


module.exports = (req, res) => new Put_Handler(req, res);
