const addOfferForm = document.getElementById("add-offer-form");
const offersContent = document.getElementById("offers-content");
const offerWrapper = document.getElementById("offer-wrapper");


document.addEventListener('DOMContentLoaded', fetchOffers)

function fetchOffers(toFilter = false) {
    if (!userId) {
        return
    }
    let urlString = `http://localhost:8080/api/v1/offer/list?id=${userId}`
    if (toFilter) {
        const subjectToFilter = document.querySelector('#filter-offers-form input[type=text][name=subject]').value
        if (subjectToFilter) {
            urlString += `&subject=${subjectToFilter}`
        }
    }
    fetch(urlString, {
        method: 'GET',
        headers: defaultHeaders
    })
        .then(response => processResponse(response))
        .then(data => {
            data.forEach(x => {
                offersContent.appendChild(offerComponent(x))
            })
        })
        .catch(e => {
            alert(e)
        })
}

function offerComponent(offer) {
    console.log(offer)
    const offerElement = document.createElement('div')

    const subjectElement = document.createElement("p")
    subjectElement.classList.add('offer-subject')
    subjectElement.textContent = offer.subject

    const costElement = document.createElement("p")
    costElement.classList.add('offer-cost')
    costElement.textContent = offer.cost

    const tutorElement = document.createElement("p")
    tutorElement.classList.add('offer-tutor-name')
    tutorElement.textContent = offer.tutorName

    const showButton = document.createElement("button")
    showButton.classList.add('btn')
    showButton.classList.add('btn-link')
    showButton.innerText = "Show"
    showButton.onclick = () => {
        showOffer(showButton.parentNode)
    }

    const alreadyOrderedRadioButton = document.createElement('input')
    alreadyOrderedRadioButton.type = "radio"
    alreadyOrderedRadioButton.disabled = true
    alreadyOrderedRadioButton.checked = offer.alreadyOrdered

    const offerIdElement = document.createElement('input')
    offerIdElement.type = "hidden"
    offerIdElement.value = offer.id

    offerElement.appendChild(subjectElement);
    offerElement.appendChild(costElement);
    offerElement.appendChild(tutorElement);
    offerElement.appendChild(alreadyOrderedRadioButton);
    offerElement.appendChild(offerIdElement);
    offerElement.appendChild(showButton);

    return offerElement
}

function showOffer(offerCard) {
    if (!offersContent.classList.contains('hidden')) {
        offersContent.classList.toggle('hidden')
    }
    if (offerWrapper.classList.contains('hidden')) {
        offerWrapper.querySelector('input[type=hidden]').value = offerCard.querySelector('input[type=hidden]').value
        offerWrapper.querySelector('.subject-value').innerText = offerCard.querySelector('.offer-subject').innerText
        offerWrapper.querySelector('.tutor-name-value').innerText = offerCard.querySelector('.offer-tutor-name').innerText
        offerWrapper.querySelector('.cost-value').innerText = offerCard.querySelector('.offer-cost').innerText
        const deleteOrderElement = offerWrapper.querySelector('.delete-order')
        const makeOrderElement = offerWrapper.querySelector('.make-order')
        if (offerCard.querySelector('input[type=radio]').checked) {
            if (deleteOrderElement.classList.contains('hidden')) {
                deleteOrderElement.classList.toggle('hidden')
            }
            if (!makeOrderElement.classList.contains('hidden')) {
                makeOrderElement.classList.toggle('hidden')
            }
        } else {
            if (!deleteOrderElement.classList.contains('hidden')) {
                deleteOrderElement.classList.toggle('hidden')
            }
            if (makeOrderElement.classList.contains('hidden')) {
                makeOrderElement.classList.toggle('hidden')
            }
        }
        fetch(`http://localhost:8080/api/v1/user/getmyname?id=${userId}`, {
            method: "GET",
            headers: defaultHeaders
        })
            .then(response => processResponse(response))
            .then(data => {
                const userName = data['message']
                console.log(userName)
                const deleteOfferWrapper = offerWrapper.querySelector('.delete-offer-wrapper')
                if (userName === offerCard.querySelector('.offer-tutor-name').innerText) {
                    if (deleteOfferWrapper.classList.contains('hidden')) {
                        deleteOfferWrapper.classList.toggle('hidden')
                    }
                    if (!makeOrderElement.classList.contains('hidden')) {
                        makeOrderElement.classList.toggle('hidden')
                    }
                    if (!deleteOrderElement.classList.contains('hidden')) {
                        deleteOrderElement.classList.toggle('hidden')
                    }
                } else {
                    if (!deleteOfferWrapper.classList.contains('hidden')) {
                        deleteOfferWrapper.classList.toggle('hidden')
                    }
                }
            })
            .catch(e => {
                console.error(e)
            })
        offerWrapper.classList.toggle('hidden')
    }
}

function showAllOffers() {
    if (offersContent.classList.contains('hidden')) {
        offersContent.classList.toggle('hidden')
    }
    if (!offerWrapper.classList.contains('hidden')) {
        offerWrapper.classList.toggle('hidden')
    }
}

function refreshOffers(toFilter = false) {
    while (offersContent.firstChild) {
        offersContent.removeChild(offersContent.firstChild);
    }
    fetchOffers(toFilter)
}

function makeOrder() {
    const offerId = offerWrapper.querySelector('input[type=hidden]').value
    const amountOfHours = offerWrapper.querySelector('input[type=number]').value
    fetch('http://localhost:8080/api/v1/offer/makeorder', {
        method: 'POST',
        headers: defaultHeaders,
        body: JSON.stringify({
            userId,
            offerId,
            amountOfHours
        })
    })
        .then(response => processResponse(response))
        .then(() => {
            showAllOffers()
            refreshOffers()
        })
        .catch(e => {
            console.error(e)
        })
}

function deleteOrder(offer) {
    const offerId = offerWrapper.querySelector('input[type=hidden]').value
    fetch(`http://localhost:8080/api/v1/offer/deleteorder?userId=${userId}&offerId=${offerId}`, {
        method: "DELETE",
        headers: defaultHeaders
    })
        .then(response => processResponse(response))
        .then(data => {
            showAllOffers()
            refreshOffers()
        })
        .catch(e => {
            console.error(e)
        })
}

function deleteOffer(offer) {
    const offerId = offerWrapper.querySelector('input[type=hidden]').value
    fetch(`http://localhost:8080/api/v1/offer/deleteoffer?userId=${userId}&offerId=${offerId}`, {
        method: "DELETE",
        headers: defaultHeaders
    })
        .then(response => processResponse(response))
        .then(data => {
            showAllOffers()
            refreshOffers()
        })
        .catch(e => {
            console.error(e)
        })
}

function createOffer() {
    const subject = addOfferForm.querySelector('input[name=subject]').value
    const cost = addOfferForm.querySelector('input[name=cost]').value
    fetch(`http://localhost:8080/api/v1/offer/create?userId=${userId}&subject=${subject}&cost=${cost}`, {
        method: "PUT",
        headers: defaultHeaders
    })
        .then(response => processResponse(response))
        .then(data => {
            addOfferForm.classList.toggle('hidden')
            showAllOffers()
            refreshOffers()
        })
        .catch(e => {
            console.error(e)
        })
}

function rateOffer() {
    const offerId = offerWrapper.querySelector('input[type=hidden]').value
    let rating = offerWrapper.querySelector('input[type=number][name=offer-rating]').value
    rating = rating < 1 ? 1 : rating > 5 ? 5 : rating
    fetch(`http://localhost:8080/api/v1/offer/rate?userId=${userId}&offerId=${offerId}&rating=${rating}`, {
        method: "POST",
        headers: defaultHeaders
    })
        .then(response => processResponse(response))
        .then(data => {
            if (data['message'] === "ok") {
                alert ("rating successfull")
            }
        })
        .catch(e => {
            console.error(e)
        })
}