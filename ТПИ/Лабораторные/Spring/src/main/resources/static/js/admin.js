const offerIdInput = document.querySelector('#change-offer-id-input')
const subjectInput = document.querySelector('#change-offer-subject-input')
const costInput = document.querySelector('#change-offer-cost-input')
const tutorIdInput = document.querySelector('#change-offer-tutor-id-input')

document.addEventListener("DOMContentLoaded", clearIfNotAdmin)

function clearIfNotAdmin() {
    fetch(`/api/v1/user/isAdmin?userId=${userId}`, {
        headers: {'Authorization': `Bearer ${token}`}
    }).then(async response => processResponse(response))
        .then(data => {
            const isAdmin = data['message'] === "xd"
            if (!isAdmin) {
                document.querySelector('.container').innerHTML = "You are not admin, get out of here"
            }
        })
        .catch(e => {
            console.error(e)
        })
}

function makeRequest(method) {
    let urlString = `http://localhost:8080/api/v1/admin?`
    switch (method) {
        case "PUT": {
            urlString += `subject=${subjectInput.value}&cost=${costInput.value}&tutorId=${tutorIdInput.value}`
            break
        }
        case "POST": {
            urlString += `subject=${subjectInput.value}&cost=${costInput.value}` +
                `&tutorId=${tutorIdInput.value}&offerId=${offerIdInput.value}`
            break
        }
        case "DELETE": {
            urlString += `offerId=${offerIdInput.value}`
            break
        }
    }
    fetch(urlString, {
        method: method,
        headers: defaultHeaders
    })
        .then(response => processResponse(response))
        .then(data => {
            location.assign('/')
        })
        .catch(e => {
            console.error(e)
        })
}