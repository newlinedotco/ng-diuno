'use strict';

(function() {

  var scriptTag = document.getElementsByTagName('script')[1];
  var matches = scriptTag.src.match(/^http[s]?\:\/\/([^\/?#]+)(?:[\/?#]|$)/i);
  var host = matches[0];
  var body  = document.getElementsByTagName('body')[0];
  var head  = document.getElementsByTagName('head')[0];

  var createLinkTag = function(src) {
    var link = document.createElement('link');
    link.setAttribute('rel', 'stylesheet');
    link.setAttribute('type', 'text/css');
    link.setAttribute('href', host + '/' + src);
    head.appendChild(link);
  };

  var createScriptTag = function(src, async) {
    var script   = document.createElement('script');
    script.setAttribute('src', host + '/' + src);
    script.async = async || false;

    body.appendChild(script);
  };

  createLinkTag('bower_components/sass-bootstrap/dist/css/bootstrap.css');
  createLinkTag('bower_components/ionic/dist/css/ionic.min.css');
  createLinkTag('styles/main.css');

  var arr = [
    'scripts/modules/arduino.js',
    'scripts/app.js',
    'scripts/controllers/main.js',
    'scripts/services/d3.js',
    'scripts/directives/mainview.js',
    'scripts/directives/temperatureGauge.js'
  ];

  createScriptTag('bower_components/angular/angular.js');
  createScriptTag('bower_components/angular-route/angular-route.js');
  createScriptTag('bower_components/d3/d3.min.js');

  arr.forEach(function(src) {
    createScriptTag(src);
  });

  // Finally, bootstrap
  body.setAttribute('ng-app', 'myApp');
  var app   = document.createElement('div');
  var main = document.createElement('div');
  main.setAttribute('main-view', '');
  app.appendChild(main);
  body.appendChild(app);
})();