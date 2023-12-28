function square(x) {
    return new Promise((resolve, reject) => {
        if (isNaN(x)) reject(`${x} is nan`)
        else resolve(x * x)
    })
}

function cube(x) {
    return new Promise((resolve, reject) => {
        if (isNaN(x)) reject(`${x} is nan`)
        else resolve(x * x * x)
    })
}

function fourthPow(x) {
    return new Promise((resolve, reject) => {
        if (isNaN(x)) reject(`${x} is nan`)
        else resolve(x * x * x * x)
    })
}

let params = [1, 2, 10]
let promises = [square(params[0]), cube(params[1]), fourthPow(params[2])]

Promise.all(promises)
    .then(result => console.log("result:", result))
    .catch(error => console.log("error:", error))

async function asyncPromiseAll(promises) {
    try {
        const result = await Promise.all(promises)
        console.log("result:", result)
    }
    catch (error) {
        console.log("error:", error)
    }
}

asyncPromiseAll(promises)