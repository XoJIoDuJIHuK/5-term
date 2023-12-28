function ServerErrorHandler() {
    this.handler = (res, errorCode, errorMessage) => {
        res.writeHead(errorCode, errorMessage, { 'Content-Type': 'application/json; charset=utf-8' });
        res.end(JSON.stringify({ errorCode: errorCode, errorMessage: errorMessage }, null, 4));
    }
}

module.exports = () => new ServerErrorHandler();