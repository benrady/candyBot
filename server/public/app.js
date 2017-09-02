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

function buttonPress() {
  play();
  setTimeout(fanfare, 45000);
  setTimeout(splash, 60000);
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
};
