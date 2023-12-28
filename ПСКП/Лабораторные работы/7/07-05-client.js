const xml = require('xml2js');

const xmlObject = (new xml.Builder()).buildObject({
    request: {
        $: {
            id: 28
        },
        x: [
            {$: {value: 2}},
            {$: {value: 4}},
            {$: {value: 0}},
        ],
        m: [
            {$: {value: 'a'}},
            {$: {value: 'b'}},
            {$: {value: 'c'}},
        ]
    }
})
console.log(xmlObject)

fetch(`http://localhost:8000`, {
    method: 'POST',
    headers: {
        'Content-Type': 'application/xml'
    },
    body: xmlObject
})
    .then(response => {
        console.log(`Статус ответа: ${response.status}`);
        return response.text()
    })
    .then(data => {
        console.log(`Данные: ${data}`);
    });
