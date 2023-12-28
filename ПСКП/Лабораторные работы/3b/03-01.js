function firstJob() {
    return new Promise((resolve) => {
        setTimeout(() => {
            resolve("Hello World");
        }, 2000);
    });
}

firstJob()
    .then(result => console.log(result))
    .catch(error => console.log(error));

async function asyncFirstJob() {
    try {
        const result = await firstJob();
        console.log(result);
    } catch (error) {
        console.log(error);
    }
}

asyncFirstJob();
