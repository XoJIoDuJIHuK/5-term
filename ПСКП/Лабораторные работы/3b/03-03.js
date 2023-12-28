function thirdJob(data) {
    return new Promise((resolve, reject) => {
        if (isNaN(data)) {
            reject("error, NaN")
        }
        else {
            if (data % 2) {
                setTimeout(() => {
                    resolve("odd");
                }, 1000);
            }
            else {
                setTimeout(() => {
                    reject("even");
                }, 2000);
            }
        }
    })
}

const param = "2"

thirdJob(param)
    .then(result => console.log("result:", result))
    .catch(error => console.log("error:", error))

async function asyncThirdJob(data) {
    try {
        const result = await thirdJob(data)
        console.log("result:", result)
    }
    catch (error) {
        console.log("error:", error)
    }
}

asyncThirdJob(param)