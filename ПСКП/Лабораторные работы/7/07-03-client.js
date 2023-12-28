fetch(`http://localhost:8000`, {
    method: 'POST',
    headers: {
        'Content-Type': 'application/x-www-form-urlencoded'
    },
    body: 'x=1&y=2&s=3'
})
    .then(response => {
        console.log(`Статус ответа: ${response.status}`);
        return response.json()
    })
    .then(data => {
        console.log(`Данные: ${JSON.stringify(data)}`);
    });
