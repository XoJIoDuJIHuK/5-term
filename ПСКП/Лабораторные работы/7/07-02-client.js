const x = 1;
const y = 2;

fetch(`http://localhost:8000?x=${x}&y=${y}`, {
    method: 'GET'
})
    .then(response => {
        console.log(`Статус ответа: ${response.status}`);
        return response.json()
    })
    .then(data => {
        console.log(`Данные: ${JSON.stringify(data)}`);
    });
