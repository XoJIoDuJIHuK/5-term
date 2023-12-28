const http = require("http");
const fs = require("fs");
const readline = require('node:readline');
const { stdin: input, stdout: output } = require('node:process');
const rl = readline.createInterface({ input, output });

const host = 'localhost';
const port = 5000;

let state = "norm";
let acceptableStates = ['norm', 'idle', 'stop', 'test'];

const server = http.createServer((request,
                                  response) => {
});

server.listen(port, host, () => {
    console.log(`Server is running on http://${host}:${port}`);
    console.log(`Current state is ${state}`);
    rl.setPrompt("new state>")
    rl.prompt(true);
    rl.on('line', (newState) => {
        if (newState === 'exit') {
            process.exit();
        }
        if (acceptableStates.indexOf(newState) === -1) {
            console.log('Unacceptable state');
        }
        else {
            state = newState;
            console.log(`Current state is ${state}`);
        }
    });
});