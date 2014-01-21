'use strict';

angular.module('fsArduino', [])
.constant('ARDUINO_URL', 'http://' + window.ip)
.factory('Arduino', function(ARDUINO_URL, $http) {

  var actions = {
    'getTemp': 0
  };

  var actionifyPins = function(pins) {
    var str = '';
    for (var i = 0; i < pins.length; i++) {
      var p = pins[i];
      str += 'p' + p.pin;
      if (typeof(p.mode) !== 'undefined') {str += 'm' + p.mode;}
      if (typeof(p.value) !== 'undefined') {str += 'v' + p.value;}
      if (typeof(p.action) !== 'undefined') {str += 'a' + actions[p.action];}
    }
    return str;
  };
  var service = {
    getPins: function() {
      return $http({
        method: 'GET',
        url: ARDUINO_URL + '/pins'
      }).then(function(data) {
        return data.data;
      });
    },
    setPins: function(pins) {
      var strAction = actionifyPins(pins);
      return $http({
        method: 'POST',
        url: ARDUINO_URL + '/pins/update',
        data: strAction,
        headers: {'X-Action-Len': strAction.length}
      }).then(function(data) {
        return data.data;
      });
    }
  };

  return service;
});