const { Worker, parentPort } = require('worker_threads');

const fs = require('fs');

const fd = fs.openSync('/dev/rfcomm0', 'r+');

function sleep(ms) {
  Atomics.wait(
    new Int32Array(new SharedArrayBuffer(4)),
    0,
    0,
    ms
  );
}

function handleJob({content, author, jobId}) {
  const content_payload = Buffer.from(
    content.replace(/[^\x20-\x7E]/g, '')
  );
  const author_payload = Buffer.from(
    author.replace(/[^\x20-\x7E]/g, '')
  );


  const content_packet = Buffer.alloc(content_payload.length + 3);     
  let content_checksum = 0;
  for (const b of content_payload) {
    content_checksum ^= b;
  }
  content_packet[0] = 0xAA;
  content_packet[1] = content_payload.length;
  content_payload.copy(content_packet, 2);
  content_packet[content_packet.length - 1] = content_checksum;

  const author_packet = Buffer.alloc(author_payload.length + 3);     
  let author_checksum = 0;
  for (const b of author_payload) {
    author_checksum ^= b;
  }
  author_packet[0] = 0xAA;
  author_packet[1] = author_payload.length;
  author_payload.copy(author_packet, 2);
  author_packet[author_packet.length - 1] = author_checksum;

  sleep(50);
  fs.writeSync(fd, content_packet);
  sleep(50);
  fs.writeSync(fd, author_packet);
  sleep(50);
  let response = Buffer.alloc(1);
  fs.readSync(fd, response);

  return response[0] === 0xEE;
}

let queue = [];
let running = false;
function processQueue() {
  if (running) return;
  running = true;

  while (queue.length > 0) {
    const job = queue.shift();
    if (job === undefined) break;
    const result = handleJob(job);
    parentPort.postMessage({jobId: job.jobId, result});
    sleep(2000);
  }

  running = false;
}

parentPort.on('message', (job) => {
  queue.push({
    content: job.content, 
    author: job.author, 
    jobId: job.jobId
  });
  processQueue();
});

parentPort.on('close', () => {
  fs.closeSync(fd);
});
