const token = window.localStorage.getItem("token");
const userId = window.localStorage.getItem("userId");
const defaultHeaders = {
    'Content-Type': 'application/json',
    'Accept-Type': 'application/json',
    'Authorization': `Bearer ${token}`
}

function returnToSignPage() {
    localStorage.clear()
    location.assign('/signin')
}

function processResponse(response) {
    if (!response.ok) {
        if (response.status === 403) {
            returnToSignPage()
        }
        throw new Error(response.status)
    }
    return response.json()
}