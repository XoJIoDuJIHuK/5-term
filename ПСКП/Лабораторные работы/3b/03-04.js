const { v4: uuidv4 } = require("uuid")

function validateCard(cardNumber) {
    console.log("card number:", cardNumber)
    return Boolean(Math.round(Math.random()))
}

function createOrder(cardNumber) {
    return new Promise((resolve, reject) => {
        if (validateCard(cardNumber)) {
            reject("Card is not valid")
        }
        else {
            setTimeout(() => {
                resolve(uuidv4(undefined, undefined, undefined))
            }, 5000)
        }
    })
}

function proceedToPayment(orderId) {
    console.log("order id:", orderId)
    return new Promise((resolve, reject) => {
        if (Math.round(Math.random())) {
            resolve("Payment successful")
        }
        else {
            reject("Payment failed")
        }
    })
}

const cardNumberParameter = 123

createOrder(cardNumberParameter)
    .then(result => {
        proceedToPayment(result)
            .then(proceedResult => console.log("proceed result:", proceedResult))
            .catch(proceedError => console.log("proceed error:", proceedError))
    })
    .catch(error => console.log("error:", error))

async function asyncCreateOrder(cardNumber) {
    try {
        const result = await createOrder(cardNumber)
        try {
            const proceedResult = await proceedToPayment(result)
            console.log("proceed result:", proceedResult)
        }
        catch (proceedError) {
            console.log("proceed error:", proceedError)
        }
    }
    catch (error) {
        console.log("error:", error)
    }
}

asyncCreateOrder(cardNumberParameter)