const http = require('http');
const fs = require('fs');
const axios=require('axios')


getImage = async ()=> {
    try {
        let response=await axios.get('http://localhost:8000', {
            responseType: "stream",
            headers: {
                'task': '7'
            }
        });
        response.data.pipe(fs.createWriteStream("image.png"))
    }
    catch(e){
        console.log(e);
    }
}
getImage().then(() => {});