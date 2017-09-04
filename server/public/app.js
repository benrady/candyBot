function showView(name) {
  ["splash", "game", "fanfare"].forEach(function(id) {  
    d3.select("#" + id).classed('hidden', id != name);
  })
}
function splash() {
  d3.select('#splash').classed('hidden', false);
  d3.select('#game').classed('hidden', true);
  d3.select('#fanfare').classed('hidden', true);
}

function play() {
  d3.select('#splash').classed('hidden', true);
  d3.select('#game').classed('hidden', false);
  d3.select('#fanfare').classed('hidden', true);
}

function fanfare() {
  d3.select('#splash').classed('hidden', true);
  d3.select('#game').classed('hidden', true);
  d3.select('#fanfare').classed('hidden', false);
}

function decreasingSpin() {
  showView('game');
  d3.select('#pointer').attr('class', 'decreasing-spin')
}

function candyPoint() {
  showView('game');
  d3.select('#pointer').attr('class', 'candy-point')
}

function spinStart() {
  d3.select('#pointer').attr('class', 'spin-start');
}

function pointerSpin() {
  d3.select('#pointer').attr('class', 'spin-point');
}

function buttonPress() {
  play();
  // Play Sound
  spinStart(); 
  setTimeout(pointerSpin, 3000);
  setTimeout(decreasingSpin, 20000);
  setTimeout(candyPoint, 23000);
  setTimeout(fanfare, 25000);
  setTimeout(splash, 45000);
}

window.onload = function() {  
  var ws = new WebSocket('ws://' + window.location.host + '/ws');
  ws.onclose = function() { 
    setTimeout(function() {  
      window.location.reload();  
    }, 5000);
  }
  ws.onmessage = buttonPress;
  splash();
  if (window.location.hash) {
    var step = window.location.hash.replace('#', '');
    window[step]();
  }
};
