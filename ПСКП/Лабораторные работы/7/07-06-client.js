const axios = require('axios');
const fs = require('fs');

const fileData = fs.readFileSync('./render.png');

const customHeaders = {
    'task': '6',
};

axios.post('http://localhost:8000/', {
    fileData,
    fileName: 'render.png',
}, {
    headers: customHeaders,
})
    .then(response => {
        console.log('Server response:', response.data);
    })
    .catch(error => {
        console.error('Error uploading file:', error.message);
    });
