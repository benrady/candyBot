function showView(name) {
  ["splash", "game", "fanfare"].forEach(function(id) {  
    d3.select("#" + id).classed('hidden', id != name);
  })
}

function splash() {
  showView('splash');
}

function showGame() {
  d3.select('#splash').classed('hidden', true);
  d3.select('#game').classed('hidden', false);
  d3.select('#fanfare').classed('hidden', true);
}

function fanfare() {
  showView('fanfare')
}

function decreasingSpin() {
  showView('game');
  d3.select('#pointer').attr('class', 'decreasing-spin')
}

function candyPoint() {
  d3.select('#pointer').attr('class', 'candy-point')
}

function spinStart() {
  d3.select('#pointer').attr('class', 'spin-start');
  spin.play();
}

function pointerSpin() {
  d3.select('#pointer').attr('class', 'spin-point');
}

function playGame() {
  showView('game');
  spinStart(); 
  setTimeout(pointerSpin, 6000);
  setTimeout(decreasingSpin, 18000);
  setTimeout(candyPoint, 21000);
}

function buttonPress() {
  playGame();
  setTimeout(fanfare, 23000);
  setTimeout(splash, 43000);
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
