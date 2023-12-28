const sql = require('mssql');

let config = {
    user: 'xd',
    password: 'password',
    server: 'XoJIoDuJIHuK\\SQLEXPRESS',
    database: 'TOV',
    pool: { max: 10, min: 4 },
    options: {
        trustServerCertificate: true,
        encrypt: false,
        IntegratedSecurity: false
    }
};



function DB() {

    this.connPool = new sql.ConnectionPool(config)
        .connect()
        .then(pool => {
            console.log('[OK] Connected to database.\n');
            return pool;
        })
        .catch(err => console.log('[ERROR] ', err.message));




    // =============================================   SELECT   =============================================

    this.getFaculties = () => { return this.connPool.then(pool => pool.request().query('select * from FACULTY')) }

    this.getPulpits = () => { return this.connPool.then(pool => pool.request().query('select * from PULPIT')) }

    this.getSubjects = () => { return this.connPool.then(pool => pool.request().query('select * from SUBJECT')) }

    this.getAuditoriumTypes = () => { return this.connPool.then(pool => pool.request().query('select * from AUDITORIUM_TYPE')) }

    this.getAuditoriums = () => { return this.connPool.then(pool => pool.request().query('select * from AUDITORIUM')) }

    this.getFacultyAuditoriums = (faculty) => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('faculty', sql.NVarChar, faculty)
                .query('select * from PULPIT where FACULTY = @faculty');
        })
    }

    this.getAuditoriumsWithAuditoriumType = (audType) => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('audType', sql.NVarChar, audType)
                .query('select * from AUDITORIUM where AUDITORIUM_TYPE = @audType');
        })
    }





    // =============================================   INSERT   =============================================

    this.insertFaculties = (faculty, facultyName) => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('faculty', sql.NVarChar, faculty)
                .input('facultyName', sql.NVarChar, facultyName)
                .query('insert FACULTY(FACULTY, FACULTY_NAME) values (@faculty, @facultyName)');
        })
    }

    this.insertPulpits = (pulpit, pulpitName, faculty) => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('pulpit', sql.NVarChar, pulpit)
                .input('pulpitName', sql.NVarChar, pulpitName)
                .input('faculty', sql.NVarChar, faculty)
                .query('insert PULPIT(PULPIT, PULPIT_NAME, FACULTY) values (@pulpit, @pulpitName, @faculty)');
        });
    }

    this.insertSubjects = (subject, subjectName, pulpit) => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('subject', sql.NVarChar, subject)
                .input('subjectName', sql.NVarChar, subjectName)
                .input('pulpit', sql.NVarChar, pulpit)
                .query('insert SUBJECT(SUBJECT, SUBJECT_NAME, PULPIT) values (@subject, @subjectName, @pulpit)');
        });
    }

    this.insertAuditoriumTypes = (audType, audTypeName) => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('audType', sql.NVarChar, audType)
                .input('audTypeName', sql.NVarChar, audTypeName)
                .query('insert AUDITORIUM_TYPE(AUDITORIUM_TYPE, AUDITORIUM_TYPENAME) values (@audType, @audTypeName)');
        });
    }

    this.insertAuditoriums = (auditorium, audName, audCapacity, audType) => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('auditorium', sql.NVarChar, auditorium)
                .input('audName', sql.NVarChar, audName)
                .input('audCapacity', sql.Int, audCapacity)
                .input('audType', sql.NVarChar, audType)
                .query('INSERT AUDITORIUM(AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY, AUDITORIUM_TYPE)' +
                    ' values(@auditorium, @audName, @audCapacity, @audType)');
        });
    }





    // =============================================   UPDATE   =============================================

    this.updateFaculties = (faculty, facultyName) => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('faculty', sql.NVarChar, faculty)
                .input('facultyName', sql.NVarChar, facultyName)
                .query('update FACULTY set FACULTY_NAME = @facultyName where FACULTY = @faculty');
        });
    }

    this.updatePulpits = (pulpit, pulpitName, faculty) => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('pulpit', sql.NVarChar, pulpit)
                .input('pulpitName', sql.NVarChar, pulpitName)
                .input('faculty', sql.NVarChar, faculty)
                .query('update PULPIT set PULPIT_NAME = @pulpitName, FACULTY = @faculty where PULPIT = @pulpit');
        });
    }

    this.updateSubjects = (subject, subjectName, pulpit) => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('subject', sql.NVarChar, subject)
                .input('subjectName', sql.NVarChar, subjectName)
                .input('pulpit', sql.NVarChar, pulpit)
                .query('update SUBJECT set SUBJECT_NAME = @subjectName, PULPIT = @pulpit where SUBJECT = @subject');
        });
    }

    this.updateAuditoriumTypes = (audType, audTypeName) => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('audType', sql.NVarChar, audType)
                .input('audTypeName', sql.NVarChar, audTypeName)
                .query('update AUDITORIUM_TYPE set AUDITORIUM_TYPENAME = @audTypeName where AUDITORIUM_TYPE = @audType');
        });
    }

    this.updateAuditoriums = (auditorium, audName, audCapacity, audType) => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('auditorium', sql.NVarChar, auditorium)
                .input('audName', sql.NVarChar, audName)
                .input('audCapacity', sql.Int, audCapacity)
                .input('audType', sql.NVarChar, audType)
                .query('update AUDITORIUM set AUDITORIUM_NAME = @audName, AUDITORIUM_CAPACITY = ' +
                    '@audCapacity, AUDITORIUM_TYPE = @audType where AUDITORIUM = @auditorium');
        });
    }





    // =============================================   DELETE   =============================================

    this.deleteFaculty = faculty => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('faculty', sql.NVarChar, faculty)
                .query('delete from FACULTY where faculty = @faculty')
        });
    }

    this.deletePulpit = pulpit => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('pulpit', sql.NVarChar, pulpit)
                .query('delete from PULPIT where pulpit = @pulpit')
        });
    }

    this.deleteSubject = subject => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('subject', sql.NVarChar, subject)
                .query('delete from SUBJECT where subject = @subject')
        });
    }

    this.deleteAuditoriumType = type => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('type', sql.NVarChar, type)
                .query('delete from AUDITORIUM_TYPE where AUDITORIUM_TYPE = @type')
        });
    }

    this.deleteAuditorium = auditorium => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('auditorium', sql.NVarChar, auditorium)
                .query('delete from AUDITORIUM where AUDITORIUM = @auditorium')
        });
    }





    // ===========================================   FIND UTILS   ===========================================

    this.findFaculty = faculty => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('faculty', sql.NVarChar, faculty)
                .query('select * from FACULTY where faculty = @faculty')
        });
    }

    this.findPulpit = pulpit => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('pulpit', sql.NVarChar, pulpit)
                .query('select * from PULPIT where pulpit = @pulpit')
        });
    }

    this.findSubject = subject => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('subject', sql.NVarChar, subject)
                .query('select * from SUBJECT where subject = @subject')
        });
    }

    this.findAuditorium = auditorium => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('auditorium', sql.NVarChar, auditorium)
                .query('select * from AUDITORIUM where AUDITORIUM = @auditorium')
        });
    }

    this.findAuditoriumType = audType => {
        return this.connPool.then(pool => {
            return pool.request()
                .input('audType', sql.NVarChar, audType)
                .query('select * from AUDITORIUM_TYPE where AUDITORIUM_TYPE = @audType')
        });
    }
}



module.exports = () => new DB();