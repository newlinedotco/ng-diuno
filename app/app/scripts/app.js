'use strict';

angular.module('myApp', [
  'fsArduino'
])
.constant('HOSTED_URL', function() {
  try {
    var scriptTag = document.getElementById('appscript');
    var matches = scriptTag.src.match(/^http[s]?\:\/\/([^\/?#]+)(?:[#]?|$)/i);
    return matches[0];
  } catch(e) {
    return 'http://localhost:9000';
  }
})
.config(function($sceDelegateProvider, HOSTED_URL) {
  $sceDelegateProvider.resourceUrlWhitelist([
    'self',
    HOSTED_URL() + '**'
  ]);
});