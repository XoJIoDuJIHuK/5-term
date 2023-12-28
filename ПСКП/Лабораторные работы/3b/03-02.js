function secondJob() {
    return new Promise((resolve, reject) => {
        setTimeout(() => {
            reject("Custom error")
        }, 3000);
    });
}

secondJob()
    .then(result => console.log("resolve", result))
    .catch(error => console.log("error", error));

async function asyncSecondJob() {
    try {
        const result = await secondJob();
        console.log(result);
    } catch (error) {
        console.log(error);
    }
}

asyncSecondJob();
