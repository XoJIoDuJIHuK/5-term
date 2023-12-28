function signIn() {
    const login = document.querySelector("#login").value;
    const password = document.querySelector("#password").value;
    const error = document.querySelector("#error");

    error.innerHTML = "";

    fetch("http://localhost:8080/api/v1/auth/signin", {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
            "Accept-Type": "application/json"
        },
        body: JSON.stringify({
            login:login,
            password:password
        })
    })
        .then(response => {
            if (!response.ok) {
                throw new Error(response.status + " " + response.statusText)
            }
            return response.text()
        })
        .then(data => {
            data = JSON.parse(data)
            const userId = data['userId']
            const token = data['token']
            if (!userId || !token) {
                throw new Error("wrong response")
            }
            localStorage.setItem("userId", userId);
            localStorage.setItem("token", token);
            location.assign('/')
        })
        .catch(e => {
            console.error(e)
        })
}