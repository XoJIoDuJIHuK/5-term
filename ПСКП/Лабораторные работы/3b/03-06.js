function square(x) {
    return new Promise((resolve, reject) => {
        setTimeout(() => {
            if (isNaN(x)) reject(`${x} is nan`)
            else resolve(x * x)
        }, Math.random() * 1000)
    })
}

function cube(x) {
    return new Promise((resolve, reject) => {
        setTimeout(() => {
            if (isNaN(x)) reject(`${x} is nan`)
            else resolve(x * x * x)
        }, Math.random() * 2000)
    })
}

function fourthPow(x) {
    return new Promise((resolve, reject) => {
        setTimeout(() => {
            if (isNaN(x)) reject(`${x} is nan`)
            else resolve(x * x * x * x)
        }, Math.random() * 3000)
    })
}

let params = [1, 2, 10]
let promises = [square(params[0]), cube(params[1]), fourthPow(params[2])]

Promise.any(promises)
    .then(result => console.log("result:", result))
    .catch(error => console.log("error:", error))

async function asyncPromiseAll(promises) {
    try {
        const result = await Promise.any(promises)
        console.log("result:", result)
    }
    catch (error) {
        console.log("error:", error)
    }
}

asyncPromiseAll(promises)