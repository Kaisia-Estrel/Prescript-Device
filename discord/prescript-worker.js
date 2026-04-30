const { Worker, parentPort } = require('worker_threads');

const fs = require('fs');

const fd = fs.openSync('/dev/rfcomm0', 'rs+');

async function handleJob({content, author, jobId}) {
  fs.writeSync(fd, content + '\0' + author + '\0');
  let response = Buffer.alloc(1);
  fs.readSync(fd, response);
  parentPort.postMessage({
    jobId,
    result: response[0] === 0,
  });
}

let queue = [];
let running = false;
function processQueue() {
  if (running) return;
  running = true;

  while (queue.length > 0) {
    const job = queue.shift();
    if (job === undefined) break;
    handleJob(job);
  }

  running = false;
}

parentPort.on('message', (job) => {
  queue.push(job);
  processQueue();
});

parentPort.on('close', () => {
  fs.closeSync(fd);
});
