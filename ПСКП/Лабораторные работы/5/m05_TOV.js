const nodemailer = require("nodemailer")
const smtpTransport = require('nodemailer-smtp-transport')

module.exports.send = (username, password, text) => {
    let options = {
        service: 'mail.ru',
        auth: {
            user: username,
            pass: password
        }
    }
    nodemailer.createTransport(smtpTransport(options)).sendMail({
        from: username,
        to: 'tochilo.oleg@gmail.com',
        subject: '05-02 subject',
        text: text
    }, function(error, info){
        if (error) {
            console.log(error);
        } else {
            console.log('Email sent: ' + info.response);
        }
    });
}